//KeyFoot Stomp switch keyboard emulator for computers, for programs with repetative key presses.
//Jim Matheson 2021 (Neutron Sound)
//BOM: teensy LC, stomp buttons, wire, enclosure. (USB bulkhead extension if you want to be fancy)


#define SW_PIN1 0 //what pins are the buttons connected to? the other leg of the button goes to ground, also connected to teensy ground
#define SW_PIN2 1
#define SW_PIN3 2
#define SW_PIN4 3
#define SW_PIN5 4
#define SW_PIN6 5

#include "Keyboard.h"
//this was made on teensy LC, will probably work on any micro with USB keyboard emulation
//with teensy, set the USB type to serial+keyboard+mouse+joystick 

const uint8_t bu[] = {SW_PIN1, SW_PIN2, SW_PIN3, SW_PIN4, SW_PIN5, SW_PIN6};//array of button pins
const uint8_t Nbut = 6;//how many buttons did you use?
char buttonMessage[Nbut] = {'1', '2', '3', '4', '5', '6'};//this is just for messages sent to serial for testing
uint8_t output;//signal from button check
bool buttonState[Nbut];//last checked state of the buttons
bool Toggle[Nbut];//toggle for optional dual output
//char ctrlKey = KEY_LEFT_CTRL; //this did not work (it's from an arduino example)

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < Nbut; i++) {
    pinMode(bu[i], INPUT_PULLUP);
  }
  Serial.begin(9600);
  delay(100);
  Serial.println("Neutron Sound");
  Serial.println("KeyFoot");
  Serial.println("Configurable keyboard shortcuts");
  Keyboard.begin();
}

void loop() {
  
  check_buttons();
  if (output > 0) {//see if there was an output from the button check
    Serial.print(buttonMessage[output - 1]);
    Serial.println(" was pressed");
    switch (output) {//put keyboard commands here i use these for dipTrace
      case 1:
      if (Toggle[0]){
      Keyboard.print('1');//diptrace change board side
      } else {
      Keyboard.print('2');  
      }
      break;
      case 2:
      //Keyboard.print('2');
      break;
      case 3:
      Keyboard.print('[');//in dipTrace i have mapped these to pour/unpour
      break;
      case 4:
      Keyboard.print(']');
      break;
      case 5:
      Keyboard.press(KEY_LEFT_CTRL);//copy works in all non stupid programs.
     Keyboard.press('c');
    delay(100);
     Keyboard.releaseAll();
      break;
      case 6:
      Keyboard.press(KEY_LEFT_CTRL);//paste 
     Keyboard.press('v');
    delay(100);
     Keyboard.releaseAll();
      break;
    }
    output = 0;
    
  }
  delay(10);//super advanced "debounce", tested with smallbear stomp switches (increase if buttons still bounce).
}

void check_buttons() {
  for (int i = 0; i < Nbut; i ++) {//step through the button
    bool buh = digitalRead(bu[i]);//read the button
    if (buh != buttonState[i]) {//see if button state has changed since last time
      if (buh == 1) {//1 means the button is high, the buttons i have are normally closed, if you have normally open buttons, change this to 0
        Toggle[i] = !Toggle[i];
        output = i + 1;
        //you could put "break;" here if you want it to report the first button, or leave it out to report the last one
      }
      
    }
    buttonState[i] = buh;
  }
}
