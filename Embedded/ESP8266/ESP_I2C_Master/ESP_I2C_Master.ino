#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Wifi variable
const char* ssid = "MEOW";
const char* password = "098765432";


//GET Request variable
int const ROOM_NUMBER = 402;
//Your Domain name with URL path or IP address with path
String serverName = "https://DisastrousRipeDemo.suitedicez.repl.co/";
//Your Domain name without starting \https:// and ending /
String host = "DisastrousRipeDemo.suitedicez.repl.co";


//I2C variable
int STM32_address = 9;


void setup() {
 
 //Base setup
 Serial.begin(9600); /* begin serial for debug */
  Serial.println("Starting setup");
 
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
 
  

  //Request setup

  Serial.println("Setup complete!");
}

void loop() {
// Wire.beginTransmission(STM32_address); /* begin with device address STM32_address */
// Wire.write("Hello Arduino");  /* sends hello string */
// Wire.endTransmission();    /* stop transmitting */

  //Receive data from STM32
 Wire.requestFrom(STM32_address, 1); /* request & read data of size 13 from slave */
 while(Wire.available()){
    // c command : 1 ask is full , 2 add , 3 minus
    char c = Wire.read();
    if(c == '1'){ //Turn on
      digitalWrite(LED_BUILTIN, LOW);
    }
    else{ //Turn off
      digitalWrite(LED_BUILTIN, HIGH);
    }
  Serial.println("Received data from STM32 : " + (String)c);
  String GET_output = GET_request("isfull",ROOM_NUMBER);
  if(GET_output == "false"){
    Serial.println("Room " + String(ROOM_NUMBER) + " is not full");
    
    //Adding people
    if(GET_request("add",ROOM_NUMBER) == "true"){
      Serial.println("Room " + String(ROOM_NUMBER) + " adding complete!");
    }
    else{
      Serial.println("Room " + String(ROOM_NUMBER) + " adding fail.");
    }
    
  }
  else if(GET_output == "true"){
    Serial.println("Room " + String(ROOM_NUMBER) + " is full");
  }
  else{
    Serial.println("GET Request [isfull] with room " + String(ROOM_NUMBER) + " error : " + GET_output);
  }

 }
 Serial.println();
 delay(1000);
}


String GET_request(String request,int room_number){
  if(WiFi.status()== WL_CONNECTED){
      WiFiClientSecure client;
        HTTPClient http;

        const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
          client.setInsecure(); // this is the magical line that makes everything work
          if (!client.connect(host, httpPort)) { //works!
            Serial.println("connection failed");
            return "Connection failed" ;
          }
    
      String serverPath = serverName + request + "?room=" + String(room_number);
      Serial.println("GET from server");
      
  // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();

      String output;
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println("payload : " + payload);
        output = payload;
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        output = "Error code: " + (String)httpResponseCode;
      }
      // Free resources
      http.end();
      return output;
  }
  else{
    return "Wifi not connected";
  }
}
