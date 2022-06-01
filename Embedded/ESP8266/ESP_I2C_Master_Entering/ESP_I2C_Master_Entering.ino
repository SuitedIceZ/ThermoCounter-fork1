#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Wifi variable
const char* ssid = "MEOW";
const char* password = "098765432";


//GET Request variable
int const ROOM_NUMBER = 402;
//Your Domain name with URL path or IP address with path without ending /
String serverName = "http://192.168.43.106:8000";
//Your Domain name without starting \https:// and ending /
String host = "192.168.43.106:8000";


//I2C variable
int STM32_address = 9;

//Debug
int LD = 0;

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
  while (WiFi.status() != WL_CONNECTED) {
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
  //LD ON indicate ready for scaning
  digitalWrite(LED_BUILTIN, LOW);

  //Receive data from STM32
  Wire.requestFrom(STM32_address, 1); /* request & read data of size 1 from slave */
  while (Wire.available()) {
    //LD OFF indicate not ready for scaning
    digitalWrite(LED_BUILTIN, HIGH);

    // c command : 1 ask is full then add if possible , 2 minus.
    char c = Wire.read();

    Serial.println("Received data from STM32 : " + (String)c);
    if (c == '1') { //ask is full then add if possible
      String GET_output = GET_request("add", ROOM_NUMBER);
      if (GET_output == "true") {
        Serial.println("Room " + String(ROOM_NUMBER) + " is not full adding complete");
        Wire.beginTransmission(STM32_address); /* begin with device address STM32_address */
        Wire.write("3");
        Wire.endTransmission();
      }
      else if (GET_output == "false") { //room is full adding fail
        Wire.beginTransmission(STM32_address); /* begin with device address STM32_address */
        Wire.write("4");
        Wire.endTransmission();
        Serial.println("Room " + String(ROOM_NUMBER) + " is full adding fail");
      }
      else {
        Serial.println("GET Request [isfull] with room " + String(ROOM_NUMBER) + " error : " + GET_output);
      }

    }

  }
  Serial.println();
  delay(1000);
}


String GET_request(String request, int room_number) {
  if (WiFi.status() == WL_CONNECTED) {
    //WiFiClientSecure client;
    HTTPClient http;
     WiFiClient client;
    //client.setInsecure(); // this is the magical line that makes everything work
     /* const int httpPort = 8000; // 80 is for HTTP / 443 is for HTTPS!
    if (!client.connect(host, httpPort)) { //works!
      Serial.println("connection failed");
      return "Connection failed" ;
    }
*/
    String serverPath = serverName + "/" + request + "?room=" + String(room_number);
    Serial.println("GET from server");

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    String output;
    if (httpResponseCode > 0) {
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
  else {
    return "Wifi not connected";
  }
}
