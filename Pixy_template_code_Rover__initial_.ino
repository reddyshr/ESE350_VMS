
#include <SPI.h>
#include <Pixy.h>

//Constructs a blank Pixy object for use. 
Pixy pixy; 
void setup() {
  // put your setup code here, to run once:
  pixy.init(); 
}

void loop() {
  blocks = pixy.getBlocks(); 
  static int i = 0; 

  //We need a way to access the recently detected block object 
  if (blocks) {
    //The following is the key for the color signatures: 
    //1 = Red 
    //2 = Blue 
    //3 = Green 
    if(pixy.blocks[i].signature == 1) { 
      //Insert code for stopping
      i++;
    }
    else if (pixy.blocks[i].signature == 2) { 
      //Insert code for turning right 
      i++;
    }
    else if (pixy.blocks[i].signature == 3) { 
      //Insert code for turning left
      i++;
    }
    else { 
      i++; 
      //Go forward
    }
  }
}
