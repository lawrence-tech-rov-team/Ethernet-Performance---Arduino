#include <EtherCard.h>
#include <IPAddress.h>

//What to set the Arduino ethernet ip address to
static byte myip[] = { 169,254,240,157 };

//Gateway IP address. Not really used, but required.
static byte gwip[] = { 192,168,178,1 }; 

//What to set the Arduino's ethernet mac address to - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

#define CS 8 //CS pin

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

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
  Serial.println(data);
}

void setup(){
  Serial.begin(9600);
  Serial.println(F("\nInitializing..."));

  if (ether.begin(sizeof Ethernet::buffer, mymac, CS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  Serial.println("Connected to controller.");

  Serial.println("Static IP Setup...");
  ether.staticSetup(myip, gwip);

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  //Register the udpSerialPrint() method to listen to port 6003
  ether.udpServerListenOnPort(&udpSerialPrint, 6003);
}

void loop(){
  //this must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
}

