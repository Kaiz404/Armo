// Create variables for G-code parsing
String M; String A; String B; String C; String D; String E;
int ind1; int ind2; int ind3; int ind4; int ind5; int ind6;
int indM; int indA; int indB; int indC; int indD; int indE;

String inputStr;

// Include necessary libraries
#include <AccelStepper.h>
#include <Wire.h>

// Create AccelStepper instances
AccelStepper Xaxis(1, 2, 5); // pin 2 = step, pin 5 = direction
AccelStepper Yaxis(1, 3, 6); // pin 3 = step, pin 6 = direction
AccelStepper Zaxis(1, 4, 7); // pin 4 = step, pin 7 = direction

void setup() {
  // Initialize communication protocols
  Serial.begin(115200);
  Serial.setTimeout(5);
  Wire.begin();

  // Set AccelStepper library configurations for max speed and acceleration
  Xaxis.setMaxSpeed(1000);
  Yaxis.setMaxSpeed(1000);
  Zaxis.setMaxSpeed(1000);
  
  Xaxis.setAcceleration(100);
  Yaxis.setAcceleration(100);
  Zaxis.setAcceleration(100);
}


void loop() {
  if (Serial.available() > 0) { // If serial G-code data is detected from the computer

    // Read G-code string and parse angles
    inputStr = Serial.readString();
    parseStr(inputStr);


    // Set angles as stepper motor goals
    Xaxis.moveTo(map(A.toInt(), -180, 180, -500, 500));
    Yaxis.moveTo(map(B.toInt(), -180, 180, -1900, 1900));
    Zaxis.moveTo(map(C.toInt(), -180, 180, -2000, 2000)); // Axis C or 3 is set to 1/16 steps to reduce noise

    // Use l2C communication to commute angles to second Arduino board on address 9
    Wire.beginTransmission(9);
    Wire.write("D");
    Wire.write(D.toInt());
    Wire.endTransmission();
    
    Wire.beginTransmission(9);
    Wire.write("E");
    Wire.write(E.toInt());
    Wire.endTransmission();
    
    Wire.beginTransmission(9);
    Wire.write("M");
    Wire.write(M.toInt());
    Wire.endTransmission();
    
    resetVal();
  }

  // Run
   Xaxis.run();
   Yaxis.run();
   Zaxis.run();
}


/* Parses serial G-code into individual angle degrees for stepper motor to execute
 * Example G-code: "M90 A10 B20 C30 D40 E50"
 * Where
 * M = Servo Angle
 * A = Angle for Axis 1
 * B = Angle for Axis 2
 * C = Angle for Axis 3
 * D = Angle for Axis 4
 * E = Angle for Axis 5
 */
void parseStr(String str){
  Serial.println("parsing....");

  // Find indexs of all angles
  indM = str.indexOf('M');
  indA = str.indexOf('A');
  indB = str.indexOf('B');
  indC = str.indexOf('C');
  indD = str.indexOf('D');
  indE = str.indexOf('E');

  // Check if index numbers are "-1", if is "-1" then the character doesn't exist
  // If character exists read the angle
  // Repeat for all angles in the string until finish
  if (indM != -1){
    indM = str.indexOf('M');
    ind1 = str.indexOf(' ');
    M = str.substring(indM+1, ind1);
    
    ind2 = str.indexOf(' ');
    str = str.substring(ind2+1);
  }
  if (indA != -1){
    indA = str.indexOf('A');
    ind2 = str.indexOf(' ');
    A = str.substring(indA+1, ind2);
    
    ind3 = str.indexOf(' ');
    str = str.substring(ind3+1);
  }
  if (indB != -1){
    indB = str.indexOf('B');
    ind3 = str.indexOf(' ');
    B = str.substring(indB+1, ind3);
    
    ind4 = str.indexOf(' ');
    str = str.substring(ind4+1);
  }
  if (indC != -1){
    indC = str.indexOf('C');
    ind4 = str.indexOf(' ');
    C = str.substring(indC+1, ind4);
    
    ind5 = str.indexOf(' ');
    str = str.substring(ind5+1);
  }
  if (indD != -1){
    indD = str.indexOf('D');
    ind5 = str.indexOf(' ');
    D = str.substring(indD+1, ind5);
    
    ind6 = str.indexOf(' ');
    str = str.substring(ind6+1);
  }
  if (indE != -1){
    indE = str.indexOf('E');
    E = str.substring(indE+1);
  }

  // Print all angles
  Serial.print("Servo Angle:");
  Serial.println(M);
  Serial.print("Axis 1 Angle:");
  Serial.println(A);
  Serial.print("Axis 2 Angle:");
  Serial.println(B);
  Serial.print("Axis 3 Angle:");
  Serial.println(C);
  Serial.print("Axis 4 Angle:");
  Serial.println(D);
  Serial.print("Axis 5 Angle:");
  Serial.println(E);
}

void resetVal(){
  // Resets all variables to default after parsing
  String M = "";
  String A = "";
  String B = "";
  String C = "";
  String D = "";
  String E = "";

  int ind0 = 0;
  int ind1 = 0;
  int ind2 = 0;
  int ind3 = 0;
  int ind4 = 0;
  int ind5 = 0;
  int ind6 = 0;
  
  int indM = -1;
  int indA = -1;
  int indB = -1;
  int indC = -1;
  int indD = -1;
  int indE = -1;
}
