#include <XbeeMlab.h>

int k ;
 byte tab[25];
 int frameLength; 
 void setup() {
  Serial.begin(9600);
  
 }
void loop ()
{
  
  if (Serial.available())  {k= readFrame(); identification(tab);}
  if((millis()%2000)==0)    displayFrame(k);

}




void displayFrame(int k)
{
  if(k)
        {
          for (int j=0;j<k;j++)
            {
              Serial.print(tab[j],HEX);
              Serial.print(",");
              delay(2);
            } 
            Serial.print("\t");
            Serial.print(k);
            Serial.print("\n");
  }
}

  
int readFrame()
  {
    byte startingByte =Serial.read(); 

    if (startingByte==0x7E)
    {
      delay(1000); 
      frameLength = Serial.available();

      for(int i=0; i<frameLength;i++)
      {
        tab[i]=Serial.read();
        delay(2);    
      }
    } return frameLength;
  }
void identification (byte tab[]){
    if (tab[3] == 0x92 ) { frame92 O; O.pckToFrame(tab);}
    // formatage des données + gprs code 
    else 
     if (tab[3]==0x90){ frame90 O2; O2.pckToFrame(tab);
     // formatage des données + gprs code 
     }
    else if (tab[3] == 0x10 ){frame10 O3; O3.frameToPck(tab);
    // formatage des données + gprs code 
    }
}

