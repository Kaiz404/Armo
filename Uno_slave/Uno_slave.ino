// Include necessary libraries
#include <Wire.h>
#include <AccelStepper.h>
#include <Servo.h>


#define FULLSTEP 4

// Creates an instance
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper Stepper1(FULLSTEP, 4, 6, 5, 7);
AccelStepper Stepper2(FULLSTEP, 8, 10, 9, 11);
Servo servoM;

// Create necessary variables
char mode;
int posY;
int posZ;
int posM;
int posY1;
int posY2;
int posZ1;
int posZ2;

void setup() {

  Serial.begin(115200);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9);

  Wire.onReceive(receiveEvent); // Attach a function to trigger when something is received.

  servoM.attach(3);

  // Set AccelStepper library configurations for max speed and acceleration
  Stepper1.setMaxSpeed(1000.0);
  Stepper2.setMaxSpeed(1000.0);

  Stepper1.setAcceleration(400);
  Stepper2.setAcceleration(400);
}

void loop() {
  if (mode == 'Z') {
    // Update angles for Stepper motor 4 and 5
    Stepper1.moveTo(posZ1);
    Stepper2.moveTo(posZ2);
  }
  if (mode == 'M') {
    // Update servo angle
    servoM.write(posM);
  }

  Stepper1.run();
  Stepper2.run();
}

// This function executes everytime G-code is sent from the Arduino Master
void receiveEvent() {

  mode = Wire.read(); // Reads first character to find which axis does the angle belong to

  if (mode == 'Y') {
    // Read angle value
    int val = Wire.read();

    // Since we are sending bytes declared as ints, the byte polarity bit is b7.
    // When we want to receive negative values we just substract them by 256
    // Refer to this forum for more info: https://forum.arduino.cc/t/sending-negative-integers-over-i2c/431397
    if (val < 128) {
      val = val;
    } else {
      val = val - 256;
    }

    // Map angle values to number of steps
    // Since axis 4&5 are differential gear boxs we need to set the two 28byj-48 stepper motors to move together
    posY =  map(val, -60, 60, -340, 340);
    posY1 = posY;
    posY2 = -posY;
  }

  if (mode == 'Z') {
    // axis Z/5 is the rotation of the end effector
    
    // Read angle value
    int val = Wire.read();

    if (val < 128) {
      val = val;
    } else {
      val = val - 256;
    }

    // Calculate steps
    posZ = map(val, -90, 90, -510, 510);
    posZ1 = posY1 + posZ * 2;
    posZ2 = posY2 + posZ * 2;
  }

  if (mode == 'M') {
    // Angle M is for the servo motor to open and close the end gripper
    posM = Wire.read();
  }
}
