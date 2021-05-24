

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <BluetoothSerial.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 433E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

boolean btConnected = false;
char key, previousKey;
long previousMillis = 0;
BluetoothSerial ESP_BT;

//packet counter
int readingID = 0;

String incoming;

int counter = 0;
String LoRaMessage = "";



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

//Initialize OLED display
void startOLED(){
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER");
}

//Initialize LoRa module
void startLoRA(){
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  while (!LoRa.begin(BAND) && counter < 10) {
    Serial.print(".");
    counter++;
    delay(500);
  }
  if (counter == 10) {
    // Increment readingID on every new reading
    readingID++;
    Serial.println("Starting LoRa failed!"); 
  }
  Serial.println("LoRa Initialization OK!");
  display.setCursor(0,10);
  display.clearDisplay();
  display.print(" Lets GO!");
  display.display();
  delay(2000);
}





void sendReadings(String txt) {
//  LoRaMessage = ( vale );
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(txt);
  LoRa.endPacket();
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("LoRa packet sent!");
  display.setCursor(0,20);
  display.print("TEXT:");
  display.setCursor(72,20);
  display.print(LoRaMessage);
  Serial.print("Sending packet: ");
  Serial.println(readingID);
  readingID++;
}

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  ESP_BT.begin("Amber"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");
  startOLED();
  startLoRA();
}
void loop() {
  String bt;

  if (ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
    incoming = ESP_BT.readString(); //Read what we recevive 
    Serial.print("Received:"); Serial.println(incoming);

    if (incoming != "hi")
        {
          sendReadings(incoming);
        }   
  }
  delay(20);
//}

//if (Serial.available()) 
//  {
//    while (Serial.available()) 
//    {
//      bt = Serial.readString();
//      sendReadings(bt);
//    }
//  }
//delay(500);

}
