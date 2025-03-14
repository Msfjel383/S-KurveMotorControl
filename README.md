# SCurveMotion Library

The SCurveMotion library provides an easy way to control motion with an S-curve acceleration profile. This is particularly useful for applications like robotic motion, CNC machines, or other projects where smooth acceleration and deceleration are desired. It implements a motion profile that transitions smoothly between acceleration, constant speed (cruise), and deceleration phases.

## Features

- **Smooth motion**: The library generates smooth motion profiles with adjustable acceleration, deceleration, and jerk time.
- **Configurable**: You can customize the acceleration, deceleration, jerk time, and maximum speed to suit your needs.
- **Simple API**: Provides an easy-to-use interface for controlling motion.

## Installation

You can install the SCurveMotion library directly into the Arduino IDE by following these steps:

1. Download the library `.zip` file.
2. Open the Arduino IDE.
3. Go to **Sketch** → **Include Library** → **Add .ZIP Library...**.
4. Select the downloaded `.zip` file.
5. The library is now installed and ready to use.

Alternatively, you can clone the repository using Git and place the folder in your Arduino libraries directory.

### Visualizing Data with Better Serial Plotter

To visualize the motion parameters in real-time using the Arduino Serial Plotter, follow these steps:

1. **Install Better Serial Plotter**:
   - Follow the instructions on [Better Serial Plotter GitHub](https://github.com/nathandunk/BetterSerialPlotter).

2. **Upload the Sketch**:
   - Upload the example code "plot_all" to your Arduino board.

3. **Open the Serial Plotter**:
   - The plotter will display graphs of position, velocity, acceleration, and jerk.

![image](https://github.com/user-attachments/assets/5e74c62c-5494-40d9-9ec0-77875b7c2f0f)



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

  // get motion parameters
   float pos = sCurve.getPos();
  float speed = sCurve.getVel();
  float acc = sCurve.getAcc();
  float jerk = sCurve.getJerk();
// your code
}

