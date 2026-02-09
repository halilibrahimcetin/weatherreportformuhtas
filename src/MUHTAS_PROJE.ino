#include <DHT.h>
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPI.h>


DHT dht(2,DHT11);

//DEFINE KISIMLARI BASLANGICI
#define WIFI_SSID "ASUS"
#define WIFI_SIFRE "55201056622Sc@"
#define FIREBASE_AUTH "https://iot-weather-report-for-muhtas-default-rtdb.europe-west1.firebasedatabase.app/"
//#define FIREBASE_HOST "UxlDIxGD7dZN9oft22Lkl5VX6pMxOvy3ErQnZT50"
#define FIREBASE_HOST "AIzaSyDft3CF8JriwsZB8RX--hlNWaH3B0W98jM"
Adafruit_BMP280 bmp;
//DEFINE SONU


//SETTTING UP BASLANGICI
void setup() {
    pinMode(16,OUTPUT);
    pinMode(17,OUTPUT);
    dht.begin();
    delay(2000);
    Serial.begin(9600);

    /*unsigned durum;
    durum = bmp.begin(0x76);*/

    WiFi.begin(WIFI_SSID,WIFI_SIFRE);
    Serial.println("Bağlanılınıyor...");
    while(WiFi.status() != WL_CONNECTED)
    {
      Serial.println(".");
      delay(500);
      digitalWrite(17,HIGH);
      delay(500);
      digitalWrite(17,LOW);
    }
    Serial.println();
    Serial.println("Bağlandı");
    delay(500);
    digitalWrite(17,HIGH);
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_AUTH, FIREBASE_HOST);
    bmp.begin(0x76);
}
//SETTING UP SONU

//FLAG ATAMALARI
int n = 0;
//FLAG ATAMALARI SONU


//SERİ MONİTÖR KISMI
void loop() {
  double sicaklik=dht.readTemperature();
  double nem=dht.readHumidity();
  float yagmursensoru=analogRead(A0);
  yagmursensoru =map(yagmursensoru, 0, 1023 ,0.0 ,100.0);
  yagmursensoru =(yagmursensoru/float(4.0));
  double basinc = bmp.readPressure();
  double irtifa = bmp.readAltitude();
  basinc=basinc/100;
  basinc*=1.0377551;
  irtifa*=1.44540001;
  Serial.print("\nSicaklik: ");
  Serial.print(sicaklik);
  Serial.print(" Nem:");
  Serial.print(nem);
  Serial.print(" Sensor kuruluğu:%");
  Serial.print(yagmursensoru);
  Serial.print(" Basınç:");
  Serial.print(basinc);
  Serial.print("Pa");
  Serial.print(" Rakım:");
  Serial.print(irtifa);
  Serial.print("m");
  delay(2000);


  

//SERİ MONİTÖR SONU



//FIREBASE

Firebase.setFloat("Nem:",nem);
if(Firebase.failed()){
  Serial.println("!! (NEM) VERİLER SUNUCUYA İLETİLEMEDİ!!");
  Serial.println(Firebase.error());
  return;
}
Firebase.setFloat("Sicaklik:",sicaklik);
if(Firebase.failed()){
  Serial.println("!! (SICAKLIK) VERİLER SUNUCUYA İLETİLEMEDİ!!");
  Serial.println(Firebase.error());
  return;
}
Firebase.setFloat("Sensor Kurulugu:", yagmursensoru);
if(Firebase.failed()){
  Serial.println("!! (YAĞMUR SENSÖRÜ) VERİLER SUNUCUYA İLETİLEMEDİ!!");
  Serial.println(Firebase.error());
  return;
}
delay(200);

Firebase.setFloat("Basinc", basinc);
if(Firebase.failed()){
  Serial.println("!! (BASINÇ) VERİLER SUNUCUYA İLETİLEMEDİ!!");
  Serial.println(Firebase.error());
  return;
}
Firebase.setFloat("Rakim:", irtifa);
if(Firebase.failed()){
  Serial.println("!! (RAKIM) VERİLER SUNUCUYA İLETİLEMEDİ!!");
  Serial.println(Firebase.error());
  return;
}

Serial.println(" Tüm veriler başarıyla gönderildi.");

digitalWrite(16,HIGH);//tüm veriler başarıyla gönderildiğinde LED ile bildir komutu
delay(100);
digitalWrite(16,LOW);
delay(1000);
} 
  