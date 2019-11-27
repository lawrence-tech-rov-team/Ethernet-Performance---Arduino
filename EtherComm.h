#ifndef ETHER_COMM_H
#define ETHER_COMM_H

/*
 * NOTES: 
 * UDP max packet size is <512 (tcpip.cpp line 413)
 */

#include <EtherCard.h>
#include <IPAddress.h>

#define BUFFER_SIZE 100

#define CHECKSUM_MASK 0x7F

#define START_BYTE 0xFF

#define CMD_PING 0

class EtherComm{
public:

  static bool begin(uint16_t recvPort, uint16_t destPort, uint8_t csPin);

  friend void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len);

  static void Loop(){
    ether.packetLoop(ether.packetReceive());
  }

  static void SendCommand(uint8_t cmd, uint16_t len);
  static char buffer[];

private:
  static byte _myip[]; //What to set the Arduino ethernet ip address to
  static byte _gwip[]; //Gateway IP address. Not really used, but required.
  static byte _mymac[]; //What to set the Arduino's ethernet mac address to - must be unique on your network
  
  static uint16_t _recvPort;
  static uint16_t _destPort;
  static byte _destIP[]; //Broadcast to all

  //static bool foundStart;

  //static void sendUdp(const char *data, uint16_t len);
  
  EtherComm(/*uint16_t recvPort, uint16_t destPort*/)/* : _recvPort(recvPort), _destPort(destPort)*/{  
  }
};

#endif //ETHER_COMM_H
