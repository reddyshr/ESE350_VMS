#include <PixyI2C.h>
#include <Wire.h>

PixyI2C pixy(0x55); // You can set the I2C address through PixyI2C object  
int colorSignature;
int i;

int getColor() {
  int maxBlock = -1;
  int maxArea = 0;
  int blocks = pixy.getBlocks();
  if (blocks != 0) {
    for (int i = 0; i < blocks; i++) {
      int area = pixy.blocks[i].width * pixy.blocks[i].height;
      if (area >= maxArea) {
        maxArea = area;
        maxBlock = i;
      }
    }
  }
  return maxBlock;
}

/* Send the color signature to the master device */
void sendColor() {
  Wire.write(colorSignature);
}

/* Determine what color was detected */
void decode_color(int signature) {
  if(signature == 1) { 
      Serial.println("Orange"); 
    }
    else if (signature == 2) {
      Serial.println("Green"); 
    }
    else if (signature == 3) { 
      Serial.println("Pink"); 
    }
    else if (signature == 4) {
      Serial.println("Yellow");
    }
    else { 
      Serial.println("No Object Detected");
    }
}


//Constructs a blank Pixy object for use. 
void setup() {
  i = 0;
  Wire.begin(6);
  Wire.onRequest(sendColor);
  Serial.begin(9600); 
  pixy.init(); 
}

void loop() {
  i++;
  colorSignature = -1;
  int val = getColor();  
  while (val == -1) {
    val = getColor(); 
  }
  colorSignature = pixy.blocks[val].signature;
    decode_color(pixy.blocks[val].signature);
  delay(100);
}
