#include "EtherComm.h"

byte EtherComm::_myip[] = { 169,254,240,157 }; //What to set the Arduino ethernet ip address to
byte EtherComm::_gwip[] = { 192,168,178,1 }; //Gateway IP address. Not really used, but required.
byte EtherComm::_mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 }; //What to set the Arduino's ethernet mac address to - must be unique on your network
byte EtherComm::_destIP[] = { 255,255,255,255 }; //Broadcast to all

uint16_t EtherComm::_recvPort = 0;
uint16_t EtherComm::_destPort = 0;

char EtherComm::buffer[BUFFER_SIZE];
//bool EtherComm::foundStart = false;

//===========================================================================================================================================
void RunPingCommand(const char* data, uint16_t len){
  if(len == 1){
    EtherComm::buffer[2] = data[0];
    EtherComm::SendCommand(CMD_PING, 1);
  }
}
//===========================================================================================================================================
bool CheckChecksum(const char *data, uint16_t len){
  len--;
  uint8_t checksum = 0;
  for(uint16_t index = 0; index < len; index++){
    checksum += (uint8_t)data[index];
  }

  return (checksum & CHECKSUM_MASK) == (uint8_t)data[len];
}

//callback that prints received packets to the serial port
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
/*  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);

  Serial.print("dest_port: ");
  Serial.println(dest_port);
  Serial.print("src_port: ");
  Serial.println(src_port);


  Serial.print("src_port: ");
  ether.printIp(src_ip);
  Serial.println("data: ");*/
  //Serial.println(data);
  //EtherComm::sendUdp(data, len);
  //sendCountMsg(len);
  //digitalWrite(13, LOW);
  if(len >= 3){ //At minimum, start byte, command, and checksum.
    //Serial.print("DatLen: ");
    //Serial.println(len);
    //digitalWrite(13, HIGH);
    if((uint8_t)data[0] == START_BYTE){
      if(CheckChecksum(data, len)){
        //Serial.println("Checksum");
        uint8_t cmd = (uint8_t)data[1];
        //Serial.println(cmd);
        data += 2;
        len -= 3;
  
        switch(cmd){
          case CMD_PING: 
            RunPingCommand(data, len);
            break;
          default: break;
        }
      }
    }
  }
}

/*
void EtherComm::sendUdp(const char *data, uint16_t len){
  ether.sendUdp(data, len, _recvPort, _destIP, _destPort);
}
*/
bool EtherComm::begin(uint16_t recvPort, uint16_t destPort, uint8_t csPin){
  EtherComm::_recvPort = recvPort;
  EtherComm::_destPort = destPort;
  
  if (ether.begin(EtherComm::_mymac, csPin) == 0) return false;
  ether.staticSetup(EtherComm::_myip, EtherComm::_gwip);
  
  //Serial.println(F("Static IP Setup..."));
  /*
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);
  */
  
  //Register the udpSerialPrint() method to listen to port the receive port.
  ether.udpServerListenOnPort(&udpReceive, recvPort);
  
  return true;
}

void EtherComm::SendCommand(uint8_t cmd, uint16_t len){
  EtherComm::buffer[0] = START_BYTE;
  EtherComm::buffer[1] = cmd;
  len += 2;
  
  uint8_t checksum = 0;
  uint16_t index = 0;
  while(index < len){
    checksum += (uint8_t)EtherComm::buffer[index++];  
  }
  EtherComm::buffer[index] = (uint8_t)(checksum & CHECKSUM_MASK);

  ether.sendUdp(EtherComm::buffer, len + 3, _recvPort, _destIP, _destPort);
}
