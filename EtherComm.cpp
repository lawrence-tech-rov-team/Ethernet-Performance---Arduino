#include "EtherComm.h"

byte EtherComm::_myip[] = { 169,254,240,157 }; //What to set the Arduino ethernet ip address to
byte EtherComm::_gwip[] = { 192,168,178,1 }; //Gateway IP address. Not really used, but required.
byte EtherComm::_mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 }; //What to set the Arduino's ethernet mac address to - must be unique on your network
byte EtherComm::_destIP[] = { 255,255,255,255 }; //Broadcast to all

static EtherComm* callback;

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
  callback->sendUdp(data, len);
  //sendCountMsg(len);
}

void EtherComm::sendUdp(const char *data, uint16_t len){
  ether.sendUdp(data, len, _recvPort, _destIP, _destPort);
}

bool EtherComm::begin(uint8_t csPin){
    if (ether.begin(_mymac, csPin) == 0) return false;
    ether.staticSetup(_myip, _gwip);

    //Serial.println(F("Static IP Setup..."));
    /*
    ether.printIp("IP:  ", ether.myip);
    ether.printIp("GW:  ", ether.gwip);
    ether.printIp("DNS: ", ether.dnsip);
    */

    //Register the udpSerialPrint() method to listen to port the receive port.
    callback = this;
    ether.udpServerListenOnPort(&udpSerialPrint, _recvPort);
    
    return true;
  }
