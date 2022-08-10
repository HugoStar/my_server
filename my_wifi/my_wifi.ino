
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <GParser.h>

const char* ssid = "Home";
const char* password = "Kk32297044Vv";

const char* serverName = "http://192.168.1.67:5000/item/data";

unsigned long lastTime = 0;
unsigned long timerDelay = 50000;

//СОСТОЯНИЯ

int light_is_on = 0;
int humidity_is_up = 0;
int humidity_is_down = 0;

double humidity = 0.0;
double temperature = 0.0;
double current_time = 0.0;
double tds_data = 0.0;

//ОГРАНИЧЕНИЯ

double light_on_time = 0.0;
double light_off_time = 0.0;

double prepare_max_humidity = 0.0;
double prepare_min_humidity = 0.0;


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
    if (Serial.available()) {
    char str[30];
    int amount = Serial.readBytesUntil(';', str, 30);
    str[amount] = NULL;
    
    GParser data(str, ',');
    int am = data.split();
    String result = "";
    for (int i = 0; i < am; i++) {
      switch (i) {
        case 0:
          light_is_on = i;
          break;
        case 1:
          humidity_is_up = i;
          break;
        case 2:
          humidity_is_down = i;
          break;
        case 3:
          humidity = i;
          break;
        case 4:
          temperature = i;
          break;
        case 5:
          current_time = i;
          break;
        case 6:
          tds_data = i;
          break;
        case 7:
          light_on_time = i;
          break;
        case 8:
          light_off_time = i;
          break;
        case 9:
          prepare_max_humidity = i;
          break;
        case 10:
          prepare_min_humidity = i;
          break;
          
      }
    }
    if(WiFi.status()== WL_CONNECTED){
      
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"is_light\":\""+(String)light_is_on+"\"}");
     
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
 
        
      // Free resources
      http.end();
    }

    
  }
}
