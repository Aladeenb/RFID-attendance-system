#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>  

#define SS_PIN 10 //RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

int Contrast=20;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);   
 
int speakerOut = 9;

byte card_ID[7]; //card UID size 7byte

byte Zebra[7]={0x04,0x34,0xA5,0xCA,0x06,0x5E,0x80};
byte Monkey[7]={0x04,0x39,0x06,0xB2,0x25,0x66,0x84};
byte Lion[7]={0x63,0x3B,0x42,0x16,0x00,0x00,0x00};//first UID card
byte Panda[7]={0x43,0x2E,0x04,0x15,0x00,0x00,0x00};
byte Shark[7]={0x3C,0x3E,0xA0,0x3D,0x00,0x00,0x00};
byte Walrus[7]={0xC2,0xC4,0x0A,0xC1,0x00,0x00,0x00};
byte Leopard[7]={0x5F,0xE7,0x56,0x42,0x00,0x00,0x00};
byte Wolf[7]={0x45,0x61,0xDA,0x85,0x00,0x00,0x00};

//if you want the arduino to detect the cards only once
int NumbCard[15];//this array content the number of cards. in my case i have just two cards.
int j=0;        

int const RedLed=6;
int const GreenLed=5;
int const Buzzer=8;

signed short secondes =10;
char timeline[16];

String Name;//user name
String Message; //user message
long Number;//user number
int n ;//The number of card you want to detect (optional)  

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  analogWrite(6,Contrast);
  lcd.begin(16, 2);
  
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,Date,Time,Name,Number");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)

  pinMode(RedLed,OUTPUT);
  pinMode(GreenLed,OUTPUT);
  pinMode(Buzzer,OUTPUT);

   }

void beep(){
  digitalWrite(speakerOut, HIGH);    
  delayMicroseconds(5000);
  digitalWrite(speakerOut, LOW);
  
  }

void screen()
 {  
     lcd.setCursor(0, 0);
     lcd.print("Hello Friends");
    
    lcd.setCursor(0, 1);
     lcd.print("KesPra");
}
    
void loop() {
  //look for new card
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;//got to start of loop if there is no card present
 }
 
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {

  return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }

 
 for (byte i = 0; i < mfrc522.uid.size; i++) {
     card_ID[i]=mfrc522.uid.uidByte[i];

       if(card_ID[i]==Zebra[i]){
       Name="Zebra";//user name
       Number=111110;//user number
       j=0;//first number in the NumbCard array : NumbCard[j]
      }
      else if(card_ID[i]==Monkey[i]){
       Name="Monkey";//user name
       Number=111111;//user number
       j=1;//Second number in the NumbCard array : NumbCard[j]
       Message="joined in time";
      }
      else if(card_ID[i]==Lion[i]){
       Name="Lion";//user name
       Number=100002;//user number
       j=2;//Second number in the NumbCard array : NumbCard[j]
       Message="is here! Lecture started.";
       lcd.setCursor(0, 1);
       sprintf(timeline,"%0.2d mins %0.2d secs", secondes);
       lcd.print(timeline);
  
       delay(1000);
       secondes--;
  
        if (secondes == 0)
        {
          lcd.print("You're late with 10 secondes.");
        }
      }
      else if(card_ID[i]==Panda[i]){
       Name="Panda";//user name
       Number=111113;//user number
       j=3;
       Message="joined";
      }
      else if(card_ID[i]==Shark[i]){
       Name="Shark";//user name
       Number=111114;//user number
       j=4;//Second number in the NumbCard array : NumbCard[j]
       Message="joined";
      }
      else if(card_ID[i]==Walrus[i]){
       Name="Walrus";//user name
       Number=111115;//user number
       j=5;//Second number in the NumbCard array : NumbCard[j]
       Message="joined";
      }
      else if(card_ID[i]==Leopard[i]){
       Name="Leopard";//user name
       Number=111116;//user number
       j=6;//Second number in the NumbCard array : NumbCard[j]
       Message="joined";
      }
      else if(card_ID[i]==Wolf[i]){
       Name="Wolf";//user name
       Number=111117;//user number
       j=7;//Second number in the NumbCard array : NumbCard[j]
       Message="joined";
      }
      else{
          digitalWrite(GreenLed,LOW);
          digitalWrite(RedLed,HIGH);
          delay(2000);
          digitalWrite(speakerOut, HIGH);    
          delayMicroseconds(100);
          delayMicroseconds(100);
          delayMicroseconds(100);
          delayMicroseconds(100);
          delayMicroseconds(100);
          digitalWrite(speakerOut, LOW);
          Serial.println("Access denied!");
          goto cont;//go directly to line 85
     }
}
      if(NumbCard[j] == 1){//to check if the card already detect
      //if you want to use LCD
      Serial.println("attendee already Exists");
      }
      else{
      NumbCard[j] = 1;//put 1 in the NumbCard array : NumbCard[j]={1,1} to let the arduino know if the card was detecting 
      n++;//(optional)
      Serial.print("DATA,DATE,TIME," + Name);//send the Name to excel
      Serial.println(Number); //send the Number to excel
      Serial.println(Name+" "+ Message);
      digitalWrite(GreenLed,HIGH);
      digitalWrite(RedLed,LOW);
      screen();
      beep();
      Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
      }
      delay(1000);
cont:
delay(2000);
digitalWrite(GreenLed,LOW);
digitalWrite(RedLed,LOW);

//if you want to close the Excel when all card had detected and save Excel file in Names Folder. in my case i have just 2 card (optional)
//if(n==2){
    
  //  Serial.println("FORCEEXCELQUIT");
 //   }
}