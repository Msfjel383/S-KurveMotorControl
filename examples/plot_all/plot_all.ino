#include <SCurveMotion.h>

SCurveMotion sCurve;

float targetPosition = 1000.0;

void setup() {
  Serial.begin(115200);
  delay(3000);
  
  // Initialize the motion profile with desired parameters
  sCurve.init(2.0, 2.0, 0.5, 100.0); // AccTime, DecTime, JerkTime, MaxSpeed
  
  // Start motion towards the target position
  sCurve.start(targetPosition);
}

bool motionComplete = false;

void loop() {
  sCurve.update();
  
  // Retrieve motion parameters
  float position = sCurve.getPos();
  float velocity = sCurve.getVel();
  float acceleration = sCurve.getAcc();
  float jerk = sCurve.getJerk();
  
  // Output motion parameters to the serial monitor
  Serial.print(position);
  Serial.print("\t");
  Serial.print(velocity);
  Serial.print("\t");
  Serial.print(acceleration);
  Serial.print("\t");
  Serial.print(jerk);
  Serial.println();
  
  // Reverse direction upon reaching the target position
  if (position == targetPosition && !motionComplete) {
    motionComplete = true;
    delay(2000); // Pause at target position
    sCurve.start(0.0); // Start motion back to home position
  }
  delay(10); // Short delay for stability
}
