  /*

Run this sketch on two Zigduinos, open the serial monitor at 9600 baud, and type in stuff
Watch the Rx Zigduino output what you've input into the serial port of the Tx Zigduino

*/
#define LED 2
#define PIR 4
#define SIZE 2

#include <ZigduinoRadio.h>

char idcopain[7]={'a','a','a','a','a','a','a'};
String myid = "baaaaaa";

int ledState = LOW; 
int pirState = LOW;
unsigned long previousMillis = 0; 
const long interval = 5000;
int flag =0;
 
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(PIR, INPUT);
  ZigduinoRadio.begin(11,(uint8_t*)idcopain);
  Serial.begin(9600);
  
  ZigduinoRadio.attachError(errHandle);
  ZigduinoRadio.attachTxDone(onXmitDone);
  digitalWrite(LED, HIGH);
  delay(3000);
  digitalWrite(LED, LOW);
  Serial.print("Coucou ! ");
}

void loop()
{
  if (flag == 0)
  {
    int tmp = pirState;
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;  
      
      if (digitalRead(PIR) == HIGH) {
        ledState = HIGH;
        pirState = HIGH;
        if(tmp != pirState)
          SendMessage("l1"); 
      }
      else {
        ledState = LOW;
        pirState = LOW;
        if(tmp != pirState)
          SendMessage("l0"); 
      }
    }
    digitalWrite(LED, ledState);
  }  
  
  if (Serial.available())
  {
    ZigduinoRadio.beginTransmission();
    
    Serial.println();
    Serial.print("Tx: ");

    
    while(Serial.available())
    {
      char c = Serial.read();
      Serial.write(c);
      ZigduinoRadio.write(c);
    }
    
    Serial.println(); 
    
    ZigduinoRadio.endTransmission();
  }

  ReadMessage();
  
  delay(100);
}

void errHandle(radio_error_t err)
{
  Serial.println();
  Serial.print("Error: ");
  Serial.print((uint8_t)err, 10);
  Serial.println();
}

void onXmitDone(radio_tx_done_t x)
{
  Serial.println();
  Serial.print("TxDone: ");
  Serial.print((uint8_t)x, 10);
  Serial.println();
}

void SendMessage(uint8_t message[2]) {
  ZigduinoRadio.beginTransmission();
  ZigduinoRadio.write(message, (uint8_t)SIZE);
  //Serial.print((char*)message);
  ZigduinoRadio.endTransmission();
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
        digitalWrite(LED, ledState);
        flag = 1;
        Serial.println();
        Serial.print("Rx: ALLUME ");
        SendMessage("l1"); 
      }
      else if(cmd.compareTo("l0") == 0)
      {
        ledState = LOW;
        digitalWrite(LED, ledState);
        flag = 0;
        Serial.println();
        Serial.print("Rx: ETEINT ");
        SendMessage("l0"); 
      }
      else if(cmd.compareTo("ls") == 0)
      {
        Serial.println();
        Serial.print("Rx LED State : "); 
        Serial.print(ledState);
        SendMessage("ls"); 
      }
      else //on affiche les infos par defaut
      {
        Serial.println();
        Serial.print("Rx: ");
        Serial.print(cmd);
        Serial.println("Commande inconnue ");
  
      }
      
      Serial.println();
      Serial.print("LQI: ");
      Serial.print(ZigduinoRadio.getLqi(), 10);
      Serial.print(", RSSI: ");
      Serial.print(ZigduinoRadio.getLastRssi(), 10);
      Serial.print(" dBm, ED: ");
      Serial.print(ZigduinoRadio.getLastEd(), 10);
      Serial.println("dBm");
    }
    else
      Serial.println("Ce message ne m'est pas destiné");
  }
}

