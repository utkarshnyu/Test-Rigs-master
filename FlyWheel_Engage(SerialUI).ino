#define stp 12
#define dir 13
#define MS1 9
#define MS2 10
#define MS3 11
#define EN  8
#define MOTORCONST .000125 //inches per step 
 
// Set values here (vel = in/s) (distance = in)
float SPEED = 0;
float DISTANCE = 0;

//Step and timing calculation variables
long STEPNUMBER = 0;
long MICROSPERSTEP = 0;
float DISTTRAVELED = 0;
int CONFIRM = 0;
int DISENGAGE = 0;
 
void setup() {  
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  Serial.begin(9600);
  resetBEDPins();

  Serial.println("Please enter the engage speed.");
}
 
void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
    SPEED = Serial.parseFloat(); //Read user input and trigger appropriate function
    Serial.print("  "); Serial.println(SPEED);
    Serial.println("Please enter the engage distance.");
    while(DISTANCE == 0){
      DISTANCE = Serial.parseFloat(); //Read user input and trigger appropriate function
    }
    Serial.print("  "); Serial.println(DISTANCE);
    Serial.println();
    stepCalc();
    displayCurrentSettings();
    Serial.println("Enter 1 to confirm. Enter 2 to reset.");
    while((CONFIRM == 0)){
      CONFIRM = Serial.parseInt(); //Read user input and trigger appropriate function
    }
    if (CONFIRM == 1){
      executeTest();
      Serial.println();
      Serial.println("Enter 1 to disengage");
      while(DISENGAGE != 1){
        DISENGAGE = Serial.parseInt(); //Read user input and trigger appropriate function
      }
      executeReverseTest();
      CONFIRM = 2;
    }
    if (CONFIRM == 2){
      Serial.println();
      Serial.println("==== Reseting =====");
      DISTANCE = 0;
      SPEED = 0;
      CONFIRM = 0;
      DISENGAGE = 0;
      Serial.println("Please enter the engage speed.");
    }
    resetBEDPins();
  }
}

void executeTest(){
  float x = millis();
  digitalWrite(EN, LOW);
  digitalWrite(dir, HIGH);
  digitalWrite(MS1, HIGH); //Pull MS1,MS2, and MS3 high to set logic to 1/16th microstep resolution
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
  for (int i = 0; i < STEPNUMBER; i++) {
    moveMotorForward();
  }
  float y = millis();
  Serial.println();
  Serial.println("==== Stopped ====");
  Serial.print("Moved: ");
  Serial.print(DISTTRAVELED); Serial.println(" in");
  Serial.print("Time Taken: ");
  Serial.print((y-x)/1000.0); Serial.println(" s");
  resetBEDPins();
}

void executeReverseTest(){
  digitalWrite(EN, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, HIGH); //Pull MS1,MS2, and MS3 high to set logic to 1/16th microstep resolution
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
  for (int i = 0; i < STEPNUMBER; i++) {
    moveMotorForward();
  }
  float y = millis();
  resetBEDPins();
}
 
void moveMotorForward() {
  digitalWrite(stp, HIGH);
  delayMicroseconds(MICROSPERSTEP/2); // Delay each step for changing speed
  digitalWrite(stp, LOW);
  delayMicroseconds(MICROSPERSTEP/2); // short delay to make sure we are done moving
  DISTTRAVELED += MOTORCONST;
}

void stepCalc(){
  MICROSPERSTEP = (MOTORCONST/SPEED) * 1000000;
  STEPNUMBER = DISTANCE / MOTORCONST; // calculate number of steps
}

void displayCurrentSettings(){
  Serial.println("==== Settings ====");
  Serial.print("Speed: ");
  Serial.print(SPEED); Serial.println(" in/s");
  Serial.print("Travel distance:  ");
  Serial.print(DISTANCE); Serial.println(" in");
  Serial.println();
  Serial.println("==== Calculations ====");
  Serial.print("Step time:  ");
  Serial.print(MICROSPERSTEP); Serial.println(" usec");
  Serial.print("Number of steps: ");
  Serial.println(STEPNUMBER);
}

void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, HIGH);
}
