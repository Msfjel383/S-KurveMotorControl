# SCurveMotion Library

The SCurveMotion library provides an easy way to control motion with an S-curve acceleration profile. This is particularly useful for applications like robotic motion, CNC machines, or other projects where smooth acceleration and deceleration are desired. It implements a motion profile that transitions smoothly between acceleration, constant speed (cruise), and deceleration phases.

## Features

- **Smooth motion**: The library generates smooth motion profiles with adjustable acceleration, deceleration, and jerk time.
- **Configurable**: You can customize the acceleration, deceleration, jerk time, and maximum speed to suit your needs.
- **Simple API**: Provides an easy-to-use interface for controlling motion.

## Installation

You can install the SCurveMotion library directly into the Arduino IDE by following these steps:

1. Download the library `.zip` file from the [GitHub repository](https://github.com/yourusername/SCurveMotion) (replace with your actual link).
2. Open the Arduino IDE.
3. Go to **Sketch** → **Include Library** → **Add .ZIP Library...**.
4. Select the downloaded `.zip` file.
5. The library is now installed and ready to use.

Alternatively, you can clone the repository using Git and place the folder in your Arduino libraries directory.

## Usage

### 1. Include the Library

In your Arduino sketch, include the SCurveMotion library:

```cpp
#include <SCurveMotion.h>

SCurveMotion motion;

void setup() {
  Serial.begin(9600);
  motion.init(2.0, 2.0, 0.5, 100.0);  // Initialize motion parameters
  motion.start(1000.0);               // Start motion towards position 1000
}

void loop() {
  motion.update();  // Update motion state

  // Print motion parameters
  Serial.print("Position: ");
  Serial.println(motion.getPos());
  Serial.print("Velocity: ");
  Serial.println(motion.getVel());
  Serial.print("Acceleration: ");
  Serial.println(motion.getAcc());
  Serial.print("Jerk: ");
  Serial.println(motion.getJerk());

  delay(100);  // Delay for readability
}

