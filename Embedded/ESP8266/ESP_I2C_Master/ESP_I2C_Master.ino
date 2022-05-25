#include <Wire.h>
int STM32_address = 9;
int LED_is_on = 0;
void setup() {
 Serial.begin(9600); /* begin serial for debug */
 Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
 pinMode(LED_BUILTIN, OUTPUT);
 digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
// Wire.beginTransmission(STM32_address); /* begin with device address STM32_address */
// Wire.write("Hello Arduino");  /* sends hello string */
// Wire.endTransmission();    /* stop transmitting */

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
 }
 Serial.println();
 delay(1000);
}
