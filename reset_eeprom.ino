#include <EEPROM.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    for (int i = 0; i < 512; i++) {
       EEPROM.write(i, 0);
     }
     EEPROM.commit();

     Serial.println("EEPROM is resetted");
}

void loop() {
  // put your main code here, to run repeatedly:

}
