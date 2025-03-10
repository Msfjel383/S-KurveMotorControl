#include "SKurveMotorControl.h"
#include <math.h>

SKurveMotorControl motorControl(200.0, 600.0, 1000.0);

void setup() {
    Serial.begin(115200);
    delay(3000);
    motorControl.setSpeed(200.0);
}
float targetSpeed=200.0;
void loop() {
    motorControl.update();
    float currentSpeed = motorControl.getCurrentSpeed();
    float currentAcceleration = motorControl.getCurrentAcceleration();

    Serial.print(targetSpeed);
    Serial.print("\t");
    Serial.print(currentSpeed);
    Serial.print("\t");
    Serial.println(currentAcceleration);

    // Prüfen, ob serielle Daten vorhanden sind
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n'); // Lese die Eingabe bis zum Zeilenumbruch
        input.trim(); // Entfernt Leerzeichen und Steuerzeichen

        if (input.length() > 0) {
            targetSpeed = input.toFloat(); // Konvertiere in eine Zahl

            // Falls die Konvertierung erfolgreich war (auch 0 ist erlaubt)
            if (targetSpeed != 0 || input == "0") {
                motorControl.setSpeed(targetSpeed);
            }
        }
    }
    delay(20);
}
