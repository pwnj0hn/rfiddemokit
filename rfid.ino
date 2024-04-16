#include <Wiegand.h>

//Wiegand pins
#define PIN_D0 2
#define PIN_D1 3

//LED pins
#define LED_GREEN 6
#define LED_RED 7

Wiegand wiegand;

uint8_t receivedDataStorage[32]; // Adjust size as needed
uint8_t receivedDataBits = 0;


void setup() {
  //configure serialport
  Serial.begin(9600);
  //setting wiegand messages
  wiegand.onReceive(receivedData, "Card read: ");
  wiegand.onReceiveError(receivedDataError, "Card read error: ");
  wiegand.onStateChange(stateChanged, "State changed: ");
  wiegand.begin(Wiegand::LENGTH_ANY, true);

  //configure Wiegand pins and interrupt
  pinMode(PIN_D0, INPUT);
  pinMode(PIN_D1, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_D0), pinStateChanged, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_D1), pinStateChanged, CHANGE);
  pinStateChanged();

  //configure LED pins as output
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  //welcome blink
  for(int i=0;i<3;i++){
    digitalWrite(LED_GREEN, HIGH); 
    digitalWrite(LED_RED, HIGH); 
    delay(500);
    digitalWrite(LED_GREEN, LOW); 
    digitalWrite(LED_RED, LOW); 
    delay(500);
  }
  Serial.println("Ready...");

}

//global variable used for LEDs after swipe is done
unsigned long lastSwipeTime = 0; // Store the last swipe time
bool isLEDLight = false; // Track if the message is displayed


//loop
void loop() {
  //interrupt for badge swipe
  noInterrupts();
  wiegand.flush();
  interrupts();
  
  // Check if it's time to clear the LEDs
  if (isLEDLight && millis() - lastSwipeTime >= 5000) {
    digitalWrite(LED_GREEN, LOW); 
    digitalWrite(LED_RED, LOW); 
    isLEDLight = false; // Reset the flag
  }
  delay(100);
}


void pinStateChanged() {
  wiegand.setPin0State(digitalRead(PIN_D0));
  wiegand.setPin1State(digitalRead(PIN_D1));
}

//reader connected or disconnected message on serial
void stateChanged(bool plugged, const char* message) {
    Serial.print(message);
    Serial.println(plugged ? "Reader Connected" : "Reader Disconnected");
}

//converts data to a string with number in hex
String dataToHexString(uint8_t* data, uint8_t bits) {
  String hexString = "";
  uint8_t bytes = (bits + 7) / 8;
  for (int i = 0; i < bytes; i++) {
      if (data[i] < 0x10) hexString += '0'; // Add leading zero for values less than 0x10
      hexString += String(data[i], HEX);
  }
  hexString.toUpperCase(); // Convert to uppercase if desired
  return hexString;
}


void displayMessageAndsetLED(String status, String hexData){
      Serial.println(status);
      Serial.println(hexData);
      if(status=="Granted"){
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
      }
      else{
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH); 
      }
}


//receiving data from card
void receivedData(uint8_t* data, uint8_t bits, const char* message) {
    // Store received data
    uint8_t bytes = (bits + 7) / 8;
    for (int i = 0; i < bytes; i++) {
        receivedDataStorage[i] = data[i];
    }
    receivedDataBits = bits;

    // Convert received data to hex string and print
    String hexData = dataToHexString(receivedDataStorage, receivedDataBits);
    // Sets a user card
    String userData01 = "115C505B";

    if(hexData==userData01){
      displayMessageAndsetLED("Granted",hexData);
    }
    else{
      displayMessageAndsetLED("Denied",hexData);
    }

    lastSwipeTime = millis(); // Record the current time
    isLEDLight = true; // Set the flag indicating a LED is on
}



void receivedDataError(Wiegand::DataError error, uint8_t* rawData, uint8_t rawBits, const char* message) {
    Serial.print(message);
    Serial.print(Wiegand::DataErrorStr(error));
    Serial.print(" - Raw data: ");
    Serial.print(rawBits);
    Serial.print(" bits / ");
    String hexData = dataToHexString(rawData, rawBits);
    Serial.println(hexData);
    displayMessageAndsetLED("Error!",hexData);
}
