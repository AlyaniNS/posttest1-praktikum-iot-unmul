#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Universitas Mulawarman";
const char* password = "";

ESP8266WebServer server(80); // Port untuk web server

const int BuzzerPin = D4; //Pin buzzer
const int DecreaseBuzzerFrequency = D1; //Button untuk mengurangi frekuensi buzzer
const int IncreaseBuzzerFrequency = D2; //Button untuk meningkatkan frekuensi buzzer
const int PowerBuzzer = D3; //Button untuk on dan off buzzer
const int LEDPin = D8; //Pin LED
const int LEDStatusBrightness = D5; //Button untuk mengurangi kecerahan LED
const int LEDOnBrightness = D6; //Button untuk meningkatkan kecerahan LED
const int PowerLED = D7; // Button untuk on dan off LED

//Inisialisasi value awal
int LEDValue;
int BuzzValue = 100;

void setup() {
  Serial.begin(115200);      
  pinMode(BuzzerPin, OUTPUT); 
  pinMode(DecreaseBuzzerFrequency, INPUT_PULLUP);                                                                                                                                    
  pinMode(IncreaseBuzzerFrequency, INPUT_PULLUP);                                                                                                                                    
  pinMode(PowerBuzzer, INPUT_PULLUP);                                                                                                                                    
  pinMode(LEDPin, OUTPUT); 
  pinMode(LEDStatusBrightness, INPUT_PULLUP);                                                                                                                                    
  pinMode(LEDOnBrightness, INPUT_PULLUP);                                                                                                                                    
  pinMode(PowerLED, INPUT_PULLUP);     

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menyambungkan WiFi...");
  }
  Serial.println("Menyambungkan WiFi.");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot); // Halaman utama

  server.begin(); // Memulai web server                                                                                                                               
}

void loop() {
  server.handleClient(); // Menangani permintaan dari klien

  int BuzzerOn = analogRead(DecreaseBuzzerFrequency);
  int BuzzerFreqValue = analogRead(IncreaseBuzzerFrequency);
  int BuzzerStatus = analogRead(PowerBuzzer);
  if (BuzzerOn == LOW) {
    if (BuzzerFreqValue == 0){ //Buzzer pertama kali ketika dinyalakan
      BuzzValue = 100;
      delay(500);
    }else{
      BuzzValue = 0;
      delay(500);
    }
  }
  if(BuzzerFreqValue == LOW){ //Meningkatkan frekuensi buzzer interval 5
    if (BuzzValue >= 100 && BuzzValue <=2495){
      BuzzValue += 5;
    }
  }
  if(BuzzerStatus == LOW){ //Mengurangi frekuensi buzzer interval 5
    if (BuzzValue >= 105 && BuzzValue <=2500){
      BuzzValue -= 5;
    }
  }


  int LEDOn = analogRead(LEDStatusBrightness);
  int LEDBrightnessValue = analogRead(LEDBrightnessValue);
  int LEDStatus = analogRead(PowerLED);
  if (LEDOn == LOW) { //LED pertama kali dinyalakan
    if (LEDValue == 0){
      LEDValue = 5;
      delay(500);
    }else{
      LEDValue = 0;
      delay(500);
    }
  }
  if(LEDBrightnessValue == LOW){ //Meningkatkan kecerahan LED interval 5
    if (LEDValue >= 5 && LEDValue <=250){
      LEDValue += 5;
    }
  }
  if(LEDStatus == LOW){ //Mengurangi kecerahan LED interval 5
    if (LEDValue >= 10 && LEDValue <=255){
      LEDValue -= 5;
    }
  }

  //Variasi frekuensi tone buzzer
  analogWrite(BuzzerPin, BuzzerFreqValue);
  //Variasi tingkat kecerahan LED
  analogWrite(LEDPin, LEDValue);
}

//Monitoring via web server
void handleRoot() {
  String isi = "<html><title>Monitoring LED & Buzzer</title><body><p>Tingkat Kecerahan LED : " + String(LEDValue) + "</p> <br> <p>Frekuensi Tone Buzzer : " + String(BuzzValue) + "</p></body></html>";
  server.send(200, "text/html",isi); 
}
