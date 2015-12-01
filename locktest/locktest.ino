//lockuse
int outputpin = 8;
int inputpin = 7   ;
int switchon = 0;
//toilet use
int toiletinputpin = 19;
int toiletswitchon = 0;
//rfid use
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10  
 #define RST_PIN 9  
 MFRC522 mfrc522(SS_PIN, RST_PIN);  
 MFRC522::MIFARE_Key key;  
 int block=0;  
 byte blockcontent[16] = {"Hello world"};  
 byte readbackblock[18];  
String rfidUid = "";
int sameRfidCount = 0;
int sameLockCount = 0;
int sameIRCount = 0;

void setup() {
  Serial.begin(9600);
  // lockuse
  pinMode(outputpin,OUTPUT);
  pinMode(inputpin,INPUT);
  digitalWrite(outputpin,HIGH);
  // toiletuse
  pinMode(toiletinputpin,INPUT);
  // rfid use
  SPI.begin();  
   mfrc522.PCD_Init();  
   //Serial.println("Scan a MIFARE Classic card");  
   for (byte i = 0; i < 6; i++) {  
     key.keyByte[i] = 0xFF;  
   }  
}

void loop() {
  // testlock
  testLock();
  // test rfid
  testRfid();
  //next
  testToilet();
  delay(100);
}

void testRfid(){
  mfrc522.PCD_Init();  
     if ( ! mfrc522.PICC_IsNewCardPresent()) {  
       return;  
     }  
     if ( !mfrc522.PICC_ReadCardSerial()) {  
       return;  
     }  
   
  String rfidUid2 = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      rfidUid2 += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      rfidUid2 += String(mfrc522.uid.uidByte[i], HEX);
    }
  if(rfidUid != rfidUid2 || sameRfidCount == 3){
      Serial.println("beep:"+rfidUid2);    
      rfidUid = rfidUid2;
      sameRfidCount = 0;
    }
  else{
    sameRfidCount++;
    }
  
  }


void testLock(){
  
  if(switchon != digitalRead(inputpin) && sameLockCount>=3){
      if (digitalRead(inputpin) == 0){
         Serial.println("Lock:open");
        }
      else{
         Serial.println("Lock:close");
        }
       sameLockCount =0;
       switchon = !switchon;
    }
  else{
    sameLockCount ++;
    if(sameLockCount >= 300) sameLockCount =3;
    }
  
  
  }

void testToilet(){
   
  if(toiletswitchon != digitalRead(toiletinputpin) && sameIRCount>=3){
      if (digitalRead(toiletinputpin) == 0){
         Serial.println("Toilet:in");
        }
      else{
         Serial.println("Toilet:out");
        }
       sameIRCount = 0;
       toiletswitchon = !toiletswitchon;
    }
  else{
    sameIRCount ++;
    if(sameIRCount >= 300) sameIRCount =3;
    }
  
  }
