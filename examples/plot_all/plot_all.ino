#include <SCurveMotion.h>

SCurveMotion sCurve;

float ziel = 10000.0;
void setup() {
  Serial.begin(115200);
  delay(3000);
  sCurve.init(2, 4, 1, 1000.0); // AccTime, DecTime, JerkTime, maxSpeed
  sCurve.start(ziel); // Zielposition
}
bool flag=true;
void loop() {
  sCurve.update();
  
  float pos = sCurve.getPos();
  float speed = sCurve.getVel();
  float acc = sCurve.getAcc();
  float jerk = sCurve.getJerk();
  Serial.print(pos);
  Serial.print("\t");
  Serial.print(speed);
  Serial.print("\t");
  Serial.print(acc);
  Serial.print("\t");
  Serial.print(jerk);
  Serial.println("\t");
  // Verwendung der Werte
  delay(10);
  if(sCurve.getPos()==ziel&&flag){
    flag=false;
    delay(2000);
    sCurve.start(0.0); // Zielposition
  }
}