/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */




    
#ifdef NO_LIB_INSTALLED


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial
#include <OneWire.h>
#include <DallasTemperature.h>
ESP8266WiFiMulti WiFiMulti;


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 14
Adafruit_NeoPixel strip = Adafruit_NeoPixel(49, PIN, NEO_GRB + NEO_KHZ800);


#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


const float kelvin_subtract = 273.15;
const String owm_city_id = "2934681"; //DUISBURG
const String owm_api_key = "25d86d23507280aa2bc0ce79d269ebb8"; //FREE API KEY


float outside_temp = 0.0f; //OUSIDE TEMP VAR
float inside_temp = 0.0f;

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 10000;           // interval at which to blink (milliseconds)


int find_text(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}

void get_inside_temp(){
  sensors.requestTemperatures();
  inside_temp = sensors.getTempCByIndex(0);
  USE_SERIAL.print("inside_temp : ");USE_SERIAL.println(inside_temp);
  }

void get_ouside_temp(){
      // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
        //http.begin("https://api.openweathermap.org", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://api.openweathermap.org/data/2.5/weather?id=" + owm_city_id + "&APPID=" + owm_api_key); //HTTP

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                //USE_SERIAL.println(payload);
                if(httpCode == 200){
                //search for temp
                int pos_start_temp  = payload.indexOf("\"temp\":");
                int pos_end_temp = payload.indexOf(",", pos_start_temp);
                String temp_string = payload.substring(pos_start_temp + 7, pos_end_temp); //7 sizeof temp:
                 outside_temp = temp_string.toFloat();
                outside_temp -= kelvin_subtract;
                USE_SERIAL.print("outside_temp : ");USE_SERIAL.println(outside_temp);
                }
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
}


void setup() {
digitalWrite(ONE_WIRE_BUS, HIGH);
    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP WIFI] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("FRITZBox7362SL", "6226054527192856");
    WiFiMulti.addAP("Keunecke", "blablabla");


      // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  
}





void loop() {
//TIMER FOR GET OUTSIDE TEMP
unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    get_ouside_temp();
  }
//get_inside_temp();
  
 delay(500);
}


#endif


