
#include <PixyI2C.h>

//PixyI2C pixy;
PixyI2C pixy(0x55); // You can set the I2C address through PixyI2C object 

//Constructs a blank Pixy object for use. 
//Pixy pixy; 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  //pinMode(led_green, OUTPUT); 
  //pinMode(led_red, OUTPUT); 
  pixy.init(); 
}

void loop() {
  int blocks = pixy.getBlocks(); 
  static int i = 0; 

  //We need a way to access the recently detected block object 
  if (blocks) {
    //The following is the key for the color signatures: 
    //1 = Green
    //2 = Pink
    //3 = Blue
    //4 = Orange
    if(pixy.blocks[i].signature == 1) { 
      //Insert code for stopping
      Serial.print("Green detected \n"); 
      //i++;
    }
    else if (pixy.blocks[i].signature == 2) {
      Serial.print("Pink detected \n"); 
      //i++; 
      //Go forward
    }
    else if (pixy.blocks[i].signature == 3) { 
      //Insert code for turning right 
      Serial.print("Blue detected \n"); 
      //i++;
    }
    else if (pixy.blocks[i].signature == 4) { 
      //Insert code for turning left
      Serial.print("Orange Detected \n");
      //i++;
    }
    else { 
      Serial.print("No Object Detected");
    }
  }
}
