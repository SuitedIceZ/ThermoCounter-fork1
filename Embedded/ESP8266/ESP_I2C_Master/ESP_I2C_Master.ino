#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Wifi variable
const char* ssid = "oppo f11 pro";
 //การประกาศสร้างตัวแปรเก็บ Username เครื่อข่าย WiFi ชื่อว่า ssid 
const char* password = "fainrada";
 //การประกาศสร้างตัวแปรเก็บ Password ของเครื่อข่าย WiFi ชื่อว่า pass

//GET POST Request variable
//Your Domain name with URL path or IP address with path
String serverName = "https://disastrousripedemo.cdgstudent.repl.co/";
String host = "disastrousripedemo.cdgstudent.repl.co";
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

//I2C variable
int STM32_address = 9;
void setup() {
 //Base setup
 Serial.begin(9600); /* begin serial for debug */
 Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
 pinMode(LED_BUILTIN, OUTPUT);
 digitalWrite(LED_BUILTIN, HIGH);

 //Wifi setup
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
// Wire.beginTransmission(STM32_address); /* begin with device address STM32_address */
// Wire.write("Hello Arduino");  /* sends hello string */
// Wire.endTransmission();    /* stop transmitting */

  //Receive data from STM32
 Wire.requestFrom(STM32_address, 1); /* request & read data of size 13 from slave */
 while(Wire.available()){
    char c = Wire.read();
    if(c == '1'){ //Turn on
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if(c == '0'){ //Turn on
      digitalWrite(LED_BUILTIN, HIGH);
    }
  Serial.print(c);


  if(WiFi.status()== WL_CONNECTED){
      WiFiClientSecure client;
        HTTPClient http;

        const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
          client.setInsecure(); // this is the magical line that makes everything work
          if (!client.connect(host, httpPort)) { //works!
            Serial.println("connection failed");
            return;
          }
    
      String serverPath = serverName;// + "?temperature=24.37";
      Serial.println("GET from server");
      
  // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
  }
 }
 Serial.println();
 delay(1000);
}
