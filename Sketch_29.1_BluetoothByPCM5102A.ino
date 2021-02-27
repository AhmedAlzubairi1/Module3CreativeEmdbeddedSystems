/**********************************************************************
  Filename    : Bluetooth Music By PCM5102
  Description : Use ESP32 to play the audio received via Bluetooth.
                Use your phone's Bluetooth connection to ESP32, 
                then open the music APP and play the music, 
                and you'll hear ESP32 play the phone's music
  Auther      : www.freenove.com
  Modification: 2020/07/11
**********************************************************************/
#include "BluetoothSerial.h"
#include "driver/i2s.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "bt_app_core.h"
#include "bt_app_av.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"

#define CONFIG_I2S_LRCK_PIN 25
#define CONFIG_I2S_BCK_PIN  26
#define CONFIG_I2S_DATA_PIN 22
BluetoothSerial SerialBT;

//Edge LED set ups
int leds [4] = { 33,32,15,0}; 
int ledsLimits[4] = {60, 120, 180, 240};
bool leftLedState=false;
bool rightLedState=false;
bool offState=false;
// LED Pixel set up
#include "Freenove_WS2812_Lib_for_ESP32.h"

#define LEDS_COUNT  8
#define LEDS_PIN  2
#define CHANNEL   0
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);
bool rainbowState=false;



// These are the timers
  unsigned long leftLedTime=0;
  unsigned long rightLedTime=0;
  unsigned long rainbowTime=0;
  unsigned long offTime=0;
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32");
  Serial.println("Init seccess!");

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  
  i2s_config_t i2s_config;
  i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  
  i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
  i2s_config.sample_rate = 44100;
  i2s_config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
  i2s_config.communication_format = I2S_COMM_FORMAT_I2S_MSB;
  i2s_config.intr_alloc_flags = 0;
  i2s_config.dma_buf_count = 6;
  i2s_config.dma_buf_len = 60;
  i2s_config.tx_desc_auto_clear = true;
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  
  i2s_pin_config_t pin_config;
  pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
  pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
  pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
  pin_config.data_in_num = -1;
  i2s_set_pin(I2S_NUM_0, &pin_config);

  bt_app_task_start_up();
  
  esp_a2d_register_callback(&bt_app_a2d_cb);
  esp_a2d_sink_register_data_callback(bt_app_a2d_data_cb);
  esp_a2d_sink_init();
  esp_avrc_ct_init();
  esp_avrc_ct_register_callback(bt_app_rc_ct_cb);
  esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
  Serial.println("ok");
  
  //This is the set up for the leds
  pinMode(leds[0],OUTPUT);
  pinMode(leds[1],OUTPUT);
  pinMode(leds[2],OUTPUT);
  pinMode(leds[3],OUTPUT);
  //This is for the led strip
  strip.begin();
  strip.setBrightness(10);
  strip.setAllLedsColor(0,0,0);
  //turnOffLeds(0,0);

}

void loop(){
  Serial.println(offState);
  Serial.println("-----------------------------------------------------------------------------------");
  Serial.println(millis() -offTime);
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  bool leftLed=false;
bool rightLed=false;
  //If I am on rainbow state and 5 seconds past, turn on rainbow
  if(rainbowState && (millis() -rainbowTime)>5000){
    rainbow();
    Serial.println("ON RIANBOW");
  }
  // If the left state is on, wait 5 seconds and turn it on
  if (leftLedState && (millis() -leftLedTime)>5000){
      turnOffLeds(0,0);
    if (rightLedState){
      // I should give a slight delay to adjust for current active lights
      delay(900);
    }
    delay(100);
     rightLedState=false;
      turnOnLeds(0,255); 
    Serial.println("ON LEFT");
  }
    // If the right state is on, wait 5 seconds and turn it on
  if (rightLedState && (millis() -rightLedTime)>5000){
      turnOffLeds(0,0);
    if (leftLedState){
            // I should give a slight delay to adjust for current active lights
      delay(900);      
    }
    delay(100);
      leftLedState=false;

      turnOnLeds(1,255); 
      Serial.println("ON RIGHT");
    
  }
  // If I am at the off state, turn off everything after half a second
   if (offState && (millis() -offTime)>500){
    Serial.println("ON OFF---------------------------------------------------------------------------------------------");
      // Turn off leds
      strip.setAllLedsColor(0,0,0);
      turnOffLeds(0,0);
      //Set all states to false
      rainbowState=false;
      leftLedState=false;
      rightLedState=false;
      offState=false;
      //Give delay for time so everything goes through
      delay(10000);
   
  }
  if (SerialBT.available()) {
    
    String inputValue= SerialBT.readString();
    // Now get rid of \n and ETX values at end of line
    inputValue = inputValue.substring(0,inputValue.length()-2);

    // Change the state depending on if the user wants red leds on, rainbow lights on, or everything off
    if (inputValue.equals("rainbow")){
      Serial.print("on\n");
      rainbowState=true;
        
      
    }
    else if (inputValue.equals("off")){
    offState=true;
    offTime=millis();
    //Turn off all the lights
      Serial.print("off\n");
      //strip.setBrightness(1);
      
    }
    else if (inputValue.equals("left")){
      //Turn on edge leds in clockwise direction
      Serial.print("left\n");
      leftLedState=true;
      leftLedTime=millis();
    }
    else if (inputValue.equals("right")){
      //Turn on edge leds in counter clockwise direction
      Serial.print("right\n");
      rightLedState=true;
      rightLedTime=millis();
    }
  }

  delay(20);  
}
void turnOnLeds(int dir, int value){

  if (dir ==0){
    Serial.println("Clockwise");
    for (int i=0; i<=3;i++){
      //check if I passed that location.
      if ( ledsLimits[i]<=value){
        
        Serial.print(ledsLimits[i]);
        Serial.print(" is <= ");
        Serial.print(value);
        Serial.print(" turning on led:");
        Serial.println(leds[i]);
        digitalWrite(leds[i],HIGH);
        delay(100);
      }
    }
    // clock wise direction
  }
  else{
    Serial.println("Counter Clockwise");
    // Counter clockwise direction
    // here I need to go backwrods
    int start=0;
    for (int i=3; i>=0;i--){
      //check if I passed that location.
      if (ledsLimits[start]<=value){
        Serial.print(ledsLimits[i]);
        Serial.print(" is <= ");
        Serial.print(value);
        Serial.print(" turning on led:");
        Serial.println(leds[i]);
        digitalWrite(leds[i],HIGH);
        delay(100);      }
      start++;
      
    }
  }
  
}


void turnOffLeds(int dir, int value){
// turn off all values greater than it.
  if (dir ==0){
    Serial.println("Clockwise off");
    for (int i=0; i<=3;i++){
      //check if I didn't pass that location.
      if ( ledsLimits[i]>value){
        
        Serial.print(ledsLimits[i]);
        Serial.print(" is > ");
        Serial.print(value);
        Serial.print(" turning OFF led:");
        Serial.println(leds[i]);
        digitalWrite(leds[i],LOW);
      }
    }
    // clock wise direction
  }
  else{
    Serial.println("Counter Clockwise");
    // Counter clockwise direction
    // here I need to go backwrods
    int start=0;
    for (int i=3; i>=0;i--){
      //check if I passed that location.
      if (ledsLimits[start]>value){
        digitalWrite(leds[i],LOW);
        
      }
      start++;
      
    }
  }


}

void rainbow(){
  // Code found from C_tutorial pdf 
    strip.setBrightness(10);

    for (int j = 0; j < 255; j += 2) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i, strip.Wheel((i * 256 / LEDS_COUNT + j) & 255));
    }
    strip.show();
    delay(5);
  }  
}
