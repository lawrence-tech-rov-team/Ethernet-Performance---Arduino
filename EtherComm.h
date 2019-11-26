#ifndef ETHER_COMM_H
#define ETHER_COMM_H

/*
 * NOTES: 
 * UDP max packet size is <512 (tcpip.cpp line 413)
 */

#include <EtherCard.h>
#include <IPAddress.h>

class EtherComm{
public:

  EtherComm(uint16_t recvPort, uint16_t destPort) : _recvPort(recvPort), _destPort(destPort){  
  }

  bool begin(uint8_t csPin);

  friend void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len);

  void Loop(){
    ether.packetLoop(ether.packetReceive());
  }

private:
  static byte _myip[]; //What to set the Arduino ethernet ip address to
  static byte _gwip[]; //Gateway IP address. Not really used, but required.
  static byte _mymac[]; //What to set the Arduino's ethernet mac address to - must be unique on your network
  
  uint16_t _recvPort;
  uint16_t _destPort;
  static byte _destIP[]; //Broadcast to all

  void sendUdp(const char *data, uint16_t len);

};

#endif //ETHER_COMM_H
