#include "Arduino.h"
#include "XbeeMlab.h"


frame10::frame10() {
	type = 0x10;	
	id = 0;
	adrsH = 0;
	adrsL = 0xffff; // broadcast
	da = 0xfffe; // broadcast
	raduis = 0;
	option = 0;
};
frame90::frame90() {};
frame92::frame92() {};

byte frame10::frameToPck(byte pck[]){
	pck[0] = 0x7e;
	pck[1] = (length >> 8) & 0xff;
	pck[2] = length & 0xff;
	pck[3] = type;
	pck[4] = id;
	for(int i=3; i>=0; i--)
		pck[8-i] = (adrsH >> i*8) & 0xff;
	for(int i=3; i>=0; i--)
		pck[12-i] = (adrsL >> i*8) & 0xff;
	pck[13] = (da >> 8) & 0xff;
	pck[14] = da & 0xff;
	pck[15] = raduis;
	pck[16] = option;
	for(int i=0; i<length-14; i++)
		pck[17+i] = data[i];
	pck[length + 3] = checksum;
	return length;
};

byte frame10::checksumify(){
	int sum = type + id + ((da >> 8) & 0xff) + (da & 0xff) + raduis + option;
	for(int i=3; i>=0; i--)
		sum += (adrsH >> i*8) & 0xff;
	for(int i=3; i>=0; i--)
		sum += (adrsL >> i*8) & 0xff;
	for(int i=0; i<length-14; i++)
		sum += data[i];
	sum = sum & 0xff;
	checksum = 0xff - sum;
	return checksum;
};

byte frame10::strIn(String str){
	for(int i=0; i<=str.length(); i++)
		data[i] = str[i];
	return str.length();
};

byte frame10::lengthify(){
	length = strlen(data) +  14; 
	return length;
};

byte frame10::datalen(byte len){
	length = len + 14;
	return length;
};

byte frame10::send(){
	Serial.write(0x7e); delay(10);
	Serial.write((length >> 8) & 0xff); delay(10);
	Serial.write(length & 0xff); delay(10);
	Serial.write(type); delay(10);
	Serial.write(id); delay(10);
	for(int i=3; i>=0; i--){
		Serial.write((adrsH >> i*8) & 0xff);
		delay(10);
	}
	for(int i=3; i>=0; i--){
		Serial.write((adrsL >> i*8) & 0xff);
		delay(10);
	}
	Serial.write((da >> 8) & 0xff); delay(10);
	Serial.write(da& 0xff); delay(10);
	Serial.write(raduis); delay(10);
	Serial.write(option); delay(10);
	for(int i=0; i<length-14; i++){
		Serial.write(data[i]);
		delay(10);
	}
	Serial.write(checksum); delay(10);
}

int frame90::pckToFrame(byte pck[]){
	length = pck[2] + (pck[1] << 8);
	type = pck[3];
	adrsH = 0;
	adrsL = 0;
	for(int i=0; i<4; i++)
		adrsH += ((unsigned long)pck[7-i] << (i*8));		
	for(int i=0; i<4; i++)
		adrsL += ((unsigned long)pck[11-i] << (i*8));
	sa = pck[12] + (pck[13] << 8);
	option = pck[14];
	for(int i=15; i<= length+2; i++)
		data[i-15] = pck[i];
	data[length-12] = '\0'; //finish the string
	checksum = pck[length+3];
	return 0;
};

int frame92::pckToFrame(byte pck[]){
	length = pck[2] + (pck[1] << 8);
	type = pck[3];
	adrsH = 0;
	adrsL = 0;
	for(int i=0; i<4; i++)
		adrsH += ((unsigned long)pck[7-i] << (i*8));		
	for(int i=0; i<4; i++)
		adrsL += ((unsigned long)pck[11-i] << (i*8));
	sa = pck[12] + (pck[13] << 8);
	option = pck[14];
	num = pck[15];
	dig_ch = pck[17] + (pck[16] << 8);
	analog_ch = pck[18];
	value = pck[20] + (pck[19] << 8);
	checksum = pck[length+3];
	return 0;
};

word RxPacket(byte packet[], int wait){
	long unsigned time = millis();
	word len = 0; // must be <=46
	while(((packet[0]=Serial.read())!=0x7E) && ((millis() - time) < wait)); // wait 'til 0x7E or 'til timeout
	if(int(millis() - time) < wait){
		delay(10);
		packet[1] = Serial.read(); 
		delay(10);
		packet[2] = Serial.read(); 
		delay(10);
		len = packet[2] + (packet[1] << 8); // get length
		if (len>46) return 0xffff; //must be <=46
		for(int i=3; i<= len+3; i++){
	    	packet[i] = Serial.read(); 
	    	delay(10);
		};
	};
	return len;
};

word TxPacket(byte packet[]){
	if(packet[0] == 0x7e){
		word len = packet[2] + (packet[1] << 8);
		for(int i=0; i <= len+3; i++){
			Serial.write(packet[i]);
			delay(10);
		};
		return len;
	};
	return 0;
};

int sendOK(){ //send "OK" to coordinator
	Serial.write(0x7E);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x10);	delay(10);
	Serial.write(0x10);	delay(10);
	Serial.write(0x01);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0xFF);	delay(10);
	Serial.write(0xFE);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x00);	delay(10);
	Serial.write(0x4F);	delay(10);
	Serial.write(0x4B);	delay(10);
	Serial.write(0x57);	delay(10);
	return 0;
};

int sendATID(){
	Serial.write(0x7E);
	delay(10);
	Serial.write(0x00);
	delay(10);
	Serial.write(0x04);
	delay(10);
	Serial.write(0x08);
	delay(10);
	Serial.write(0x01);
	delay(10);
	Serial.write(0x49);
	delay(10);
	Serial.write(0x44);
	delay(10);
	Serial.write(0x69);
	delay(10);
	return 0;
};

