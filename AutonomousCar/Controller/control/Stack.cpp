/*
 * Stack.cpp
 *
 * Created: 4/23/2017 10:13:50 PM
 *  Author: Shreetej
 */ 


#include <avr/io.h>
#include <stdio.h>
#include "Stack.h"
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
using namespace std;

  Stack::Stack() {
      head = -1;
  }
  
  void Stack::pushVal(int val) {
     if (head == 19) {
         //Serial.println("Stack is full");
         return;
     } else {
         arr[head + 1] = val;
         head = head + 1;
     }
     
  }
  
  int Stack::popVal() {
    if (head < 0) {
      return -1;
    } else {
      int val = arr[head];
      head = head - 1;
      return val;
    }
  }
  



