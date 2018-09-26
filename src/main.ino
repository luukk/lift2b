#include <Arduino.h>

/* Define pins */
#define latchPin 12
#define dataPin 11
#define clockPin 9
#define cageArrivalIndicatorPin 8
#define topFloorDetectionPin 7
#define bottomFloorDetectionPin 6
#define buttonDown 5
#define buttonUp 4
#define ledDown 3
#define ledUp 2


const int datArray[9] = {3, 159, 37, 13, 153, 73, 65, 31, 1}; // 10 digits for shift register

void setup () {
  /* Configure all pins */
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(ledUp, OUTPUT);
  pinMode(ledDown, OUTPUT);
  pinMode(cageArrivalIndicatorPin, OUTPUT);

  pinMode(buttonDown, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(topFloorDetectionPin, INPUT);
  pinMode(bottomFloorDetectionPin, INPUT);

  digitalWrite(cageArrivalIndicatorPin, LOW);

  setFloorIndicatorDisplay(159);
  //writes the floor number (1) to the display. Byte's are inverted since the 7
  //segment display is a common anode.
  // Number 0 :  00000011     3
  // Number 1 :  10101111     159
  // Number 2 :  00100101     37
  // Number 3 :  00001101     13
  // Number 4 :  10011001     153
  // Number 5 :  01001001     73
  // Number 6 :  01000001     65
  // Number 7 :  00011111     31
  // Number 8 :  00000001     1

  Serial.begin(9600);
}

void loop() {
  //Check status of ir sensors
  if(isCageAtFloor()) {
    digitalWrite(cageArrivalIndicatorPin, HIGH);

    //Turn off button lights ones the cage is at the floor.
    digitalWrite(ledDown, LOW);
    digitalWrite(ledUp, LOW);
  } else {
    digitalWrite(cageArrivalIndicatorPin, LOW); //Disable LED if cage is not at the floor
  }

  // Check status of buttons.
  if(digitalRead(buttonDown) == 1) {
    digitalWrite(ledDown, HIGH); // Enable button LED
    Serial.println("luuk wilt omhoog");
  }

  if(digitalRead(buttonUp) == 1) {
    digitalWrite(ledUp, HIGH); // Enable button LED
    Serial.println("luuk wilt omlaag");
  }
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
