/*
 * NOTES: 
 * UDP max packet size is 400 (tcpip.cpp line 413)
 */
#define ETHER_CARD_BUFFER_SIZE 400

#include <EtherCard.h>
#include <IPAddress.h>

//What to set the Arduino ethernet ip address to
static byte myip[] = { 169,254,240,157 };

//Gateway IP address. Not really used, but required.
static byte gwip[] = { 192,168,178,1 }; 

//What to set the Arduino's ethernet mac address to - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

unsigned int recvPort = 6001;
unsigned int destPort = 6002;
static byte destIP[] = { 255,255,255,255 }; //Broadcast to all

#define CS 8 //CS pin



//callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
/*  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);

  Serial.print("dest_port: ");
  Serial.println(dest_port);
  Serial.print("src_port: ");
  Serial.println(src_port);


  Serial.print("src_port: ");
  ether.printIp(src_ip);
  Serial.println("data: ");*/
  //Serial.println(data);
  ether.sendUdp(data, len, recvPort, destIP, destPort);
  //sendCountMsg(len);
}

void setup(){
  Serial.begin(9600);
  Serial.println(F("\nInitializing..."));

  if (ether.begin(mymac, CS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  Serial.println("Connected to controller.");

  Serial.println("Static IP Setup...");
  ether.staticSetup(myip, gwip);

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  //Register the udpSerialPrint() method to listen to port the receive port.
  ether.udpServerListenOnPort(&udpSerialPrint, recvPort);
}

uint16_t count = 0;
uint32_t timer = 0;
uint32_t currentTime;

void loop(){
  //this must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
/*
  currentTime = millis();
  if((currentTime - timer) >= 1000){
    timer += 1000;
    Serial.print("--> ");
    Serial.println(count);
    sendCountMsg(count++);
  }
  */
}

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

