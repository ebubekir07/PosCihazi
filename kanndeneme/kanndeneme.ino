#include <LiquidCrystal_I2C.h>
#include <Dictionary.h>
#include <NodeArray.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseJson.h>

#define SS_PIN D4 //SDA Pini
#define RST_PIN D3
#define FIREBASE_HOST "kantinsys.firebaseio.com"
#define FIREBASE_AUTH "q9bLTL4V6DnpMg4jdo739Z64abUd5zeTWet6zJfG"
#define WIFI_SSID "TurkTelekom"
#define WIFI_PASSWORD "pixvuxxh12"
uint8_t NewMac[] = {0x34,0x97, 0xF6, 0x33, 0xB1, 0x4D};
MFRC522 mfrc522(SS_PIN, RST_PIN);
String girilensifre="";
LiquidCrystal_I2C lcd(0x27, 16, 2);
String ypara="";
Dictionary &d = *(new Dictionary(6));
int manContData[9];
FirebaseData datarfid;
int buzzer=15;
int sayac=100;
int fonksiyon=0;
String sifre;
String admin="2985";
int basildi=0;
String paraData,cardnumber;
String buy;

byte verialindi[] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};
byte bosveri[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  wifi_set_macaddr(STATION_IF, &NewMac[0]);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting...");
  lcd.begin();
  lcd.backlight();
  pinMode(buzzer,OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("connecting...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(700);
  }
  delay(400);
  Serial.println();
  Serial.print("connected: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connected.");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  SPI.begin();      
  mfrc522.PCD_Init(); 
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Toplam Tutar: ");

 
}
String tus;
float mevcutpara;
void loop() {
  if (Serial.available()>0) {
    int tusdeger = Serial.read();
    switch(tusdeger){
      case 1:
      tus="1";
      break;
      case 2 :
      tus="2";
      break;
      case 3 :
      tus="3";
      break;
      case 4 :
      tus="4";
      break;
      case 5 :
      tus="5";
      break;
      case 6 :
      tus="6";
      break;
      case 7 :
      tus="7";
      break;
      case 8 :
      tus="8";
      break;
      case 9 :
      tus="9";
      break;
      case 42 :
      tus=".";
      break;
      case 35 :
      buy="";
      tus="";
      ypara="";
      lcd.clear();
      if(fonksiyon==0){
        lcd.print("Toplam Tutar: ");
      }
      else if(fonksiyon==2){
        lcd.print("Yuklenecek Tutar");
      }
      break;
      case 97 :///aaaaaa
      fonksiyon=1;
      buy="";
      tus="";
      break;
      case 98 ://bbbbb
      girilensifre="";
      basildi=0;
      tus="";
      lcd.clear();
      lcd.print("Toplam Tutar: ");
      fonksiyon=0;
      break;
      case 99 :
      tus="C";
      break;
      case 100 :
      tus="D";
      break;
      
      default :
      tus=tusdeger;
      break;
      
    }
    if(fonksiyon==0){
      buy=buy+tus;
      lcd.setCursor(0, 1);
      lcd.print(buy);
      Serial.println(buy);
    }
    else if(fonksiyon==1){
      paraYukle();
    }
    else if(fonksiyon==2){
      if(tus){
        ypara=ypara+tus;
        lcd.setCursor(0,1);
        lcd.print(ypara);
        
      }
    }
  }
  if (sayac==100){
    lcd.createChar(1, verialindi);
    lcd.setCursor(15,1);
    lcd.write(1);
    delay(10);
    Firebase.getString(datarfid, "/kartlar/password");
    sifre=datarfid.stringData();
    sayac=0;
    lcd.createChar(1, bosveri);
    lcd.setCursor(15,1);
    lcd.write(1);
  }
  sayac=sayac+1;
  rfidkontrol();
  if(cardnumber==""){
    cardnumber="bos";
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Baglaniyor...    ");
    Firebase.getString(datarfid, "/kartlar/"+cardnumber);
    mevcutpara=datarfid.stringData().toFloat();
    if(fonksiyon==0){
      if(mevcutpara>=buy.toFloat()){
        float kalan = mevcutpara-buy.toFloat();
        Firebase.setString(datarfid, "kartlar/"+cardnumber, String(kalan));
        digitalWrite(buzzer,HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Kalan Nakit : ");
        lcd.setCursor(0, 1);
        lcd.print("--->");
        lcd.setCursor(5,1);
        lcd.print(kalan);
        delay(700);
        digitalWrite(buzzer,LOW);
        delay(1300);
      }
      else{
        digitalWrite(buzzer,HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Yetersiz Nakit ");
        lcd.setCursor(0, 1);
        lcd.print("--->");
        lcd.setCursor(5,1);
        lcd.print(mevcutpara);
        delay(50);
        digitalWrite(buzzer,LOW);
        delay(50);
        digitalWrite(buzzer,HIGH);
        delay(50);
        digitalWrite(buzzer,LOW);
        delay(50);
        digitalWrite(buzzer,HIGH);
        delay(50);
        digitalWrite(buzzer,LOW);
        delay(50);
        digitalWrite(buzzer,HIGH);
        delay(50);
        digitalWrite(buzzer,LOW);
        delay(50);
        digitalWrite(buzzer,HIGH);
        delay(50);
        digitalWrite(buzzer,LOW);
        delay(50);
        digitalWrite(buzzer,HIGH);
        delay(50);
        digitalWrite(buzzer,LOW);
        delay(1500);
      }
      buy="";
      tus="";
      lcd.clear();
      lcd.print("Toplam Tutar: ");
    }
    else if(fonksiyon==2){
      
      float newPara = mevcutpara+ypara.toFloat();
      Firebase.setString(datarfid, "kartlar/"+cardnumber, String(newPara));
      lcd.setCursor(0, 0);
      lcd.print("Yuklendi-Nakit: ");
      lcd.setCursor(0, 1);
      lcd.print("--->");
      lcd.setCursor(5,1);
      lcd.print(newPara);
      delay(2000);
      ypara="";
      tus="";
      lcd.clear();
      lcd.print("Yuklenecek Tutar");
    }
  }
  
}
void rfidkontrol(){
  cardnumber="";
  if ( ! mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // Kart okundu ise
  if ( ! mfrc522.PICC_ReadCardSerial()){
    return;
  }
  cardnumber=String(mfrc522.uid.uidByte[0]) + "," + String(mfrc522.uid.uidByte[1])+"," +String(mfrc522.uid.uidByte[2])+","+String(mfrc522.uid.uidByte[3]);
  
}


void paraYukle(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("  < Sifre >");
  if(tus){
    
    basildi++;
    yildiz();
    girilensifre=girilensifre+tus;
    Serial.println(girilensifre);
    if(girilensifre.length()==4){
      if(girilensifre==sifre or girilensifre==admin){ 
        lcd.clear();
        lcd.print("Yuklenecek Tutar:");
        girilensifre="";
        tus="";
        
        basildi=0;
        fonksiyon=2;
        
        } 
      else if(girilensifre!=sifre){
        lcd.clear();
        lcd.print("Yanlis sifre");
        delay(200);
        girilensifre="";
        basildi=0;
        tus="";
        paraYukle();
      }
    }
  }
}
void yildiz(){
  switch(basildi){
    case 1:
    girilensifre="";
    tus="";
    break;
    case 2 :
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  < Sifre >");
    //kare();
    lcd.setCursor(0,1);
    lcd.print("*");

    break;

    case 3 :
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  < Sifre >");
    //kare();
    lcd.setCursor(0,1);
    lcd.print("**");

    break;

    case 4 :
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  < Sifre >");
    //kare();
    lcd.setCursor(0,1);
    lcd.print("***");
 
    break;

    case 5 :
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  < Sifre >");
    //kare();
    lcd.setCursor(0,1);
    lcd.print("****");
    delay(200);
    basildi=0;
    break;
  }
}

  
