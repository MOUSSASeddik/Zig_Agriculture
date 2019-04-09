#ifndef XbeeMlab_h
#define XbeeMlab_h

#include "Arduino.h"

class frame10{ // Transmit Request
public:
	frame10();
	word length;
	byte type; // 0x10
	byte id; // 1 if response needed else -usually- 0
	unsigned long adrsH; // destination 64bit adress
	unsigned long adrsL;
	word da; // 16bit dest address usually FFFE
	byte raduis; // set to 0 for maximum raduis
	byte option; // set to 0 usually
	char data[35]; 
	byte checksum;
	byte frameToPck(byte pck[]);
	byte checksumify();
	byte lengthify(); // to use if sending string
	byte datalen(byte len); // to use if sending bytes (incase of 0x00 prob for lengthify)
	byte send();
	byte strIn(String str);
};

class frame90{ // Receive Packet
public:
	frame90();
	word length;
	byte type; // 0x90
	unsigned long adrsH; // source 64bit address
	unsigned long adrsL;
	word sa; // 16bit dest address usually FFFE
	byte option;
	char data[35];
	byte checksum;
	int pckToFrame(byte pck[]);
};

class frame92{ // IO Data Sample Rx Indicator (only valid for one analog/ many digital input)
public:
	frame92();
	word length;
	byte type; //0x92
	unsigned long adrsH; // source high 64bit address
	unsigned long adrsL;
	word sa; // 16bit dest address usually FFFE
	byte option;
	byte num; // number of samples included in the paylod, always 01
	word dig_ch; // digital mask channel 
	byte analog_ch; // analog mask channel
	word value; // analog or digi value
	byte checksum;
	int pckToFrame(byte pck[]);
};

word RxPacket(byte packet[], int wait);	//receive from xbee via rx 
word TxPacket(byte packet[]);	//send to xbee via tx
int sendOK();
int sendATID();


#endif