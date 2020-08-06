#include "infrared.h"


Infrared::Infrared(){
}
Infrared::~Infrared(){
};

bool Infrared::begin(){
    if (!IRLremote.begin(PIN_IR)){
        Serial.println(F("You did not choose a valid pin."));
        return false;
    }else{
        Serial.println(F("IR initialized"));
        return true;
    }
}  // begin()

bool Infrared::receiving(){
    return IRLremote.receiving();
}  // receiving()

void Infrared::change_mode(int newmode) {
  
  Serial.print("New mode: ");
  Serial.println(newmode);
  currentmode = newmode;
  
} // change_mode()

void Infrared::irlcall() {        
    // This is the built-in IR function that just selects a mode.

  if (IRLremote.available()) {
    
    auto data = IRLremote.read();  // Get the new data from the remote
    
    // Serial.print(F("Address: "));  // Print the protocol data. Note that there's also 65535, which we don't use.
    // Serial.println(data.address);

    // Serial.print(F("Command: "));
    // Serial.println(data.command);
    // Serial.println();

    if (data.address == IR_ADD) {     
           
      switch(data.command) {
        case IR_POWER:      change_mode(1);  break; 
        case IR_PLAY_PAUSE: change_mode(2);  break; 
        case IR_VOL_PLUS:   change_mode(3);  break; 
        case IR_VOL_MINUS:  change_mode(4);  break; 
        case IR_NEXT:       change_mode(5);  break; 
        case IR_PREV:       change_mode(6);  break; 
        case IR_3D:         change_mode(7);  break; 

      // case IR_A1:  change_mode(1);  break;          //a1 - 
      // case IR_A2:  change_mode(2);  break;          //a2 - 
      // case IR_A3:  change_mode(3);  break;          //a3 - 
      // case IR_A4:  change_mode(4);  break;          //a4 - 

      // case IR_B1:  change_mode(5);   break;          //b1 - 
      // case IR_B2:  change_mode(6);   break;          //b2 - 
      // case IR_B3:  change_mode(7);   break;          //b3 - 
      // case IR_B4:  change_mode(8);   break;          //b4 - 

      // case IR_C1:  change_mode(9);   break;          //c1 - 
      // case IR_C2:  change_mode(10);  break;          //c2 - 
      // case IR_C3:  change_mode(11);  break;          //c3 - 
      // case IR_C4:  change_mode(12);  break;          //c4 -

      // case IR_D1:  change_mode(13);  break;          //d1 -
      // case IR_D2:  change_mode(14);  break;          //d2 - 
      // case IR_D3:  change_mode(15);  break;          //d3 - 
      // case IR_D4:  change_mode(16);  break;          //d4 -

      // case IR_E1:  change_mode(17);  break;          //e1 -
      // case IR_E2:  change_mode(18);  break;          //e2 - 
      // case IR_E3:  change_mode(19);  break;          //e3 - 
      // case IR_E4:  change_mode(20);  break;          //e4 -

      // case IR_F1:  change_mode(21);  break;          //f1 -
      //case IR_F2:  change_mode(22);  break;          //f2 - 
      //case IR_F3:  change_mode(98);  break;          //f3 - 
      //case IR_F4:  change_mode(99);  break;          //f4 -

      default: break;                                // We could do something by default
      
      } // switch IRCommand

    } // if IR_ADD
  } // if IRLRemote


} // irlcall()