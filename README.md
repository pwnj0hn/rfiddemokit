# rfiddemokit
This repository contains the source code for an Arduino-based access control system demo kit. This demonstration project uses a Wiegand interface to read data from access cards and controls LEDs to indicate access status.

## Components Required
* Arduino Pro Micro (or any other compatible Arduino board)
* Wiegand RFID reader (For example this: https://www.aliexpress.com/item/1005005048178825.html?spm=a2g0o.productlist.main.1.7f20MWoNMWoNbi)
* LEDs (1 green, 1 red)
* Resistors for LED (typically 220 ohms)
* LM7805 or similar voltage regulator

## System Features
* Wiegand Interface: Compatible with Wiegand protocol for reading RFID tags.
* Access Indication: Uses green and red LEDs to indicate access granted or denied respectively.
* Error Handling: Communicates errors and system status via the serial monitor.

## Wiring Instructions
1. Wiegand Reader to Arduino:
* PIN_D0 -> Arduino Pin 2
* PIN_D1 -> Arduino Pin 3
2. LEDs:
* Green LED -> Arduino Pin 6 through a resistor
* Red LED -> Arduino Pin 7 through a resistor
3.
* Connect power in to circuit to reader and voltage regulator (9-16V)
* Connect power from voltage regulator to Arduino Vcc
4. Common Ground: Connect the ground of the LEDs and the Wiegand reader to the Arduino ground.

## Setup and Configuration
1. Clone this repository to your local machine.
2. Open the project in the Arduino IDE.
3. Upload the code to your Arduino board.

## Usage
Once the system is powered and the program is running:
* Present an RFID card to the reader.
The system will read the card and determine if access is granted or denied based on the predefined valid card number (115C505B).
The green LED lights up if access is granted; the red LED lights up if access is denied.
* Serial Monitor Output
The Arduino's serial monitor outputs the status of card reads including any errors and the state of the reader connection.

## Modifying the Code
You can adjust the card numbers that are granted access by modifying the userData01 variable in the receivedData function.

## Troubleshooting
Ensure all connections are secure if the reader is not recognizing cards or if LEDs are not responding as expected.

## Contributions
Contributions to this project are welcome. Please fork this repository and submit a pull request for any enhancements.

