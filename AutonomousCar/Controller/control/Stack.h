/*
 * Stack.h
 *
 * Created: 4/23/2017 10:09:42 PM
 *  Author: Shreetej
 */ 


#ifndef STACK_H_
#define STACK_H_

class Stack {

  struct Node {
    int decision;
  };
  
  public:
    Stack();
    void pushVal(int val);
    int popVal();
    int getSize();
    
  private:
      int head;
      int arr[20];
  
  
  
    
};



#endif /* STACK_H_ */


