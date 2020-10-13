#include <Keypad.h>

const byte satir = 4; //four rows
const byte sutun = 4; //four columns
char tus;
char tus_takimi[satir][sutun] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte satir_pinleri[satir] = {9, 8, 7, 6}; //connect to the Rows of the keypad pin 8, 7, 6, 5 respectively
byte sutun_pinleri[sutun] = {5, 4, 3, 2};
Keypad pres = Keypad(makeKeymap(tus_takimi) , satir_pinleri , sutun_pinleri , satir , sutun);

void setup() {
 Serial.begin(9600);
 
}
 
void loop() {
  
 tus = pres.getKey();
 if (tus != NO_KEY)
    {
      switch(tus){        
        case '1' :
        Serial.write(1);
        break;
      
        case '2' :
        Serial.write(2);
        break; 

        case '3' :
        Serial.write(3);
        break; 

        case '4' :
        Serial.write(4);
        break; 

        case '5' :
        Serial.write(5);
        break; 

        case '6' :
        Serial.write(6);
        break; 

        case '7' :
        Serial.write(7);
        break; 

        case '8' :
        Serial.write(8);
        break; 

        case '9' :
        Serial.write(9);
        break; 

        case '0' :
        Serial.write(0);
        break; 

        case 'A' :
        Serial.write('a');
        break; 

        case 'B' :
        Serial.write('b');
        break; 

        case 'C' :
        Serial.write('c');
        break; 

        case 'D' :
        Serial.write('d');
        break; 

        case '*' :
        Serial.write('*');
        break; 

        case '#' :
        Serial.write('#');
        break; 
  
        default : 
        Serial.write(31);
        break;
          }
          }
    
 
}
