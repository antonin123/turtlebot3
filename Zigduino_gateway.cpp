/*

Run this sketch on two Zigduinos, open the serial monitor at 9600 baud, and type in stuff
Watch the Rx Zigduino output what you've input into the serial port of the Tx Zigduino

*/

#include <ZigduinoRadio.h>

char idcopain[7]={'b','a','a','a','a','a','a'};
String myid = "aaaaaaa";
int ledState;

void setup()
{
  ZigduinoRadio.begin(11,(uint8_t*)idcopain);
  Serial.begin(9600);
  
  ZigduinoRadio.attachError(errHandle);
  ZigduinoRadio.attachTxDone(onXmitDone);
}

void loop()
{
  if (Serial.available())
  {
    ZigduinoRadio.beginTransmission();
    

    while(Serial.available())
    {
      char c = Serial.read();
      Serial.write(c);
      ZigduinoRadio.write(c);
    }
    

    ZigduinoRadio.endTransmission();
  }
  
  ReadMessage();
  delay(100);
}

void errHandle(radio_error_t err)
{
  Serial.print("Error: ");
  Serial.print((uint8_t)err, 10);
}

void onXmitDone(radio_tx_done_t x)
{
  Serial.print("");
}


void ReadMessage() {
  if (ZigduinoRadio.available())
  {
  
    int indice =0;
    char buf2[3];
    char entete[8];
    String cmd = String("");
    String hdr = String("");
  
    while(ZigduinoRadio.available() && indice <7 ) {
      entete[indice]=(char)ZigduinoRadio.read();   
      indice++;
    }
    indice =0;
    while(ZigduinoRadio.available() && indice < 2) {
      buf2[indice]=(char)ZigduinoRadio.read();  
      indice++;
    }
    while(ZigduinoRadio.available()){
     ZigduinoRadio.read();
    // Si indice >= 9, Format trop long, ignoré
    }
    
    entete[7] = '\0';
    hdr = String(entete);
//    Serial.print("hdr ");
//    Serial.println(hdr);
    
    buf2[2] = '\0';   // On fixe la fin de chaine
    cmd = String(buf2);
//    Serial.print("cmd: ");
//    Serial.println(cmd);

     
    if(hdr.compareTo(myid) == 0) {
      if(cmd.compareTo("l1") == 0)
      {
        ledState = HIGH;
        Serial.print("l1");
      }
      else if(cmd.compareTo("l0") == 0)
      {
        ledState = LOW;
        Serial.print("l0");
      }
      else if(cmd.compareTo("ls") == 0)
      {
        Serial.print(ledState);
      }
      else //on affiche les infos par defaut
      {
        Serial.print("ex");
      }
      
    }
    else
      Serial.println("Ce message ne m'est pas destiné");
  }
}

