#include <IRremote.h>
//toilet id
String toiletid = "2";

//lockuse
int outputpin = 8;
int inputpin = 7   ;
int switchon = 0;
//toilet use
int toiletinputpin = 19;
int toiletswitchon = 0;
//rf hot use
int rfhotinputpin = 2;
int rfhotoutputpin = 4;
int rfhotswitch=0;

//rf remote
int recv_pin = 6;
IRrecv irrecv(recv_pin);
decode_results results;
float sinVal;
int toneVal;



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

// b pin 
int bpin = 3;

void setup() {
  Serial.begin(9600);
  // lockuse
  pinMode(outputpin,OUTPUT);
  pinMode(inputpin,INPUT);
  digitalWrite(outputpin,HIGH);
  // toiletuse
  pinMode(toiletinputpin,INPUT);
  // rf hot sensor
  pinMode(rfhotinputpin,INPUT);
  pinMode(rfhotoutputpin, OUTPUT);
  // rfid use
  SPI.begin();  
   mfrc522.PCD_Init();  
   //Serial.println("Scan a MIFARE Classic card");  
   for (byte i = 0; i < 6; i++) {  
     key.keyByte[i] = 0xFF;  
   }  
  //bpin
  pinMode(bpin,OUTPUT);

  //rf remote
  irrecv.enableIRIn();
  pinMode(5,OUTPUT);
}
int incomingByte = 0;
void loop() {
  // testlock
  testLock();
  // test rfid
  testRfid();
  //next
  testToilet();
  //IF HOT LED
  testRFHot();
  //Serial.println(digitalRead(2));
  if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                // say what you got:
                if(incomingByte==98){
                   bbb();
                  }
               
        }


callb();

  
  delay(100);
}

void testRFHot(){
    if(digitalRead(rfhotinputpin) && rfhotswitch==0){
      rfhotswitch =1;
      digitalWrite(rfhotoutputpin,rfhotswitch);
      Serial.println("Bath:in:"+toiletid);
    }
   if(!digitalRead(rfhotinputpin)){
      rfhotswitch =0;
      digitalWrite(rfhotoutputpin,rfhotswitch);
    }
  
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
      Serial.println("beep:"+rfidUid2+":"+toiletid);    
      rfidUid = rfidUid2;
      sameRfidCount = 0;
    }
  else{
    sameRfidCount++;
    }
  
  }


void testLock(){
  
  if(switchon != digitalRead(inputpin) && sameLockCount>=5){
      if (digitalRead(inputpin) == 0){
         Serial.println("Lock:open:"+toiletid);
        }
      else{
         Serial.println("Lock:close:"+toiletid);
        }
       sameLockCount =0;
       switchon = !switchon;
    }
  else{
    sameLockCount ++;
    if(sameLockCount >= 300) sameLockCount =6;
    }
  
  
  }

void testToilet(){
   
  if(toiletswitchon != digitalRead(toiletinputpin) && sameIRCount>=5){
      if (digitalRead(toiletinputpin) == 0){
         Serial.println("Toilet:in:"+toiletid);
        }
      else{
         Serial.println("Toilet:out:"+toiletid);
        }
       sameIRCount = 0;
       toiletswitchon = !toiletswitchon;
    }
  else{
    sameIRCount ++;
    if(sameIRCount >= 300) sameIRCount =6;
    }
  
  }


void bbb(){
    for(int i = 0;i<=200;i++){
                      delay(10);
                      analogWrite(bpin,i);
                    }
                    for(int i = 200;i>=0;i--){
                      delay(10);
                      analogWrite(bpin,i);
                    }
  
  }


void callb(){

    if(irrecv.decode(&results)){
      if(results.value==0xFF18E7&&toiletid=="2"){
         Serial.println("Warning:on:"+toiletid);
  //2 0xFF18E7
  //1 0xFF30CF
          for(int x=0;x<180;x++){
            sinVal = (sin(x*(3.1412/180)));
            toneVal = 2000+(int(sinVal*1000));
           
            newtone(5,toneVal,10);
            delay(2);
            }
        }
      if(results.value==0xFF30CF&&toiletid=="1"){
        Serial.println("Warning:on:"+toiletid);
  //2 0xFF18E7
  //1 0xFF30CF
          for(int x=0;x<180;x++){
            sinVal = (sin(x*(3.1412/180)));
            toneVal = 2000+(int(sinVal*1000));
            
            newtone(5,toneVal,10);
            delay(2);
            }
        }
    irrecv.resume();
    }
  
  
  }

void newtone(byte tonePin, int frequency, int duration) {
int period = 1000000L / frequency;
int pulse = period / 2;
for (long i = 0; i < duration * 1000L; i += period) {
digitalWrite(tonePin, HIGH);
delayMicroseconds(pulse);
digitalWrite(tonePin, LOW);
delayMicroseconds(pulse);
}
}
