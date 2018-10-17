#include <Arduino.h>
#include <Wire.h>

/* Define pins */
#define latchPin 12               //RCK shift register
#define dataPin 11                //SER shift register
#define clockPin 9                //SCK shift register
#define cageArrivalIndicatorPin 8 //LED that indicates the elevator cage is at the floor
#define topFloorDetectionPin 7    //IR sensor mounted at the top of the floor
#define bottomFloorDetectionPin 6 //IR sensor mounted at the bottom of the floor
#define buttonDown 4              //Button down
#define buttonUp 5                //Button up
#define ledDown 2                 //LED in button down
#define ledUp 3                   //LED in button up

int floorNumber = 0;               // Defaults to zero which means the lift always starts at the bottom
int doorOpen = 0;
int cageArrivalStatus = 0;

// array of bytes to display 1 to 5 on the 7 segment display
const int segmentDisplayNumbers[6] = {3, 159, 37, 13, 153, 73};

void setup () {
  /* Configure all pins */
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(ledUp, OUTPUT);
  pinMode(ledDown, OUTPUT);
  pinMode(cageArrivalIndicatorPin, OUTPUT);

  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);

  pinMode(topFloorDetectionPin, INPUT);
  pinMode(bottomFloorDetectionPin, INPUT);

  //boot in 'save mode'
  digitalWrite(cageArrivalIndicatorPin, LOW);
  digitalWrite(ledDown, LOW);
  digitalWrite(ledUp, LOW);

  setFloorIndicatorDisplay(159);
  //writes the floor number (1) to the display. Byte's are inverted since the 7
  //segment display is a common anode.
  // Number 0 :  00000011     3
  // Number 1 :  10101111     159
  // Number 2 :  00100101     37
  // Number 3 :  00001101     13
  // Number 4 :  10011001     153
  // Number 5 :  01001001     73

  Serial.begin(9600);

  // For communication
  Wire.begin(8);                  // Start I2C communication // https://www.arduino.cc/en/Reference/Wire inside NOTE: addresses should start from 8
  Wire.onRequest(requestEvent);         // Set the event handler to hande data sending to the master
  Wire.onReceive(receiveFromMaster);
}

void loop() {
  //Check status of ir sensors
  if(isCageAtFloor()) {
    cageArrivalStatus = 1;

    digitalWrite(cageArrivalIndicatorPin, HIGH); //Set indicator led HIGH

    /*
      Turn off button lights ones the cage is at the floor. doorOpen is one if
      the cage is going to this floor and arrived at this floor.
    */
    if(doorOpen == 1) {
      digitalWrite(ledDown, LOW);
      digitalWrite(ledUp, LOW);
    }
  } else {
    digitalWrite(cageArrivalIndicatorPin, LOW); //Disable LED if cage is not at the floor
  }

  // Check status of buttons.
  if(digitalRead(buttonDown) == LOW) {
    digitalWrite(ledDown, HIGH); // Enable button LED
    Serial.println("luuk wilt omhoog");
  }

  if(digitalRead(buttonUp) == LOW) {
    digitalWrite(ledUp, HIGH); // Enable button LED
    Serial.println("luuk wilt omlaag");
  }
}

/* Assigns received data from the master to variables. */
void receiveFromMaster() {
  floorNumber = Wire.read();
  doorOpen = Wire.read();

  setFloorIndicatorDisplay(floorNumber);
}

/* Writes data to master  */
void requestEvent() {
  Wire.write(!buttonDown); // inverted state because of input pullup
  Wire.write(!buttonUp);
  Wire.write(cageArrivalStatus);
  // as expected by master
}

/*
  Determines if the cage arrival indicator LED should light up. It does this by
  reading the analog input of both the ir sensors which are mounted to the floor.
*/
int isCageAtFloor() {
  int topDetectionOutput = 1 - digitalRead(topFloorDetectionPin);
  int bottomDetectionOutput = 1 - digitalRead(bottomFloorDetectionPin);

  if (topDetectionOutput == 1 && bottomDetectionOutput == 1) {
    return 1;
  }

  return 0;
}

/*
  Pushes the given byte to the shift register and sets the latchpin high.
*/
void setFloorIndicatorDisplay(int byte) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin,clockPin,MSBFIRST, byte);
  digitalWrite(latchPin, HIGH);
}
