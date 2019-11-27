
#include "EtherComm.h"

#define Ethernet_CS 8


void setup(){//Ethernet_CS
  Serial.begin(9600);
  Serial.println(F("\nInitializing..."));

  
  if (!EtherComm::begin(6001, 6002, Ethernet_CS)){ //recv port, dest port, cs pin
    Serial.println(F("Failed to access Ethernet controller"));
    while(1);
  }
  Serial.println(F("Connected to controller."));
}

void loop(){
  //this must be called for comm functions to work.
  EtherComm::Loop();
}
/*
//Size of temp buffer to send message
#define MSG_SIZE 6 

//Sends the number 'num' over ethernet using UDP
void sendCountMsg(uint16_t num){
  static char msg[MSG_SIZE]; //Message buffer

  char* msgStart = &msg[MSG_SIZE]; //The start of the message. Moves as more digits are added.
  uint8_t len = 0; //The current length of the message, or the number of digits.

  do{
    *(--msgStart) = '0' + (num % 10); //Move the message pointer and set the new character to the right most digit in the number
    len++; //Increase the length since a new digit was added
    num /= 10; //Shift the number to the right by one, popping the right most digit from the number.
  }while((num > 0) && (len < MSG_SIZE)); //Repeat until no digits are remaining or the buffer is full.

  //Send the message using our handy-dandy library
  ether.sendUdp(msgStart, len, recvPort, destIP, destPort);
}
*/
