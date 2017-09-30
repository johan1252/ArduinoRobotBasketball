# ArduinoRobotBasketball
Arduino Robotics project used for Queen's University 2nd Year Autonomous Basketball Competition.
The hardware robot used for this project included two PWM motors, IR Receiver, 3 IR sensors, bumper touch sensors, force sensor, and a servo arm with gripper.

IR Beacon Sensing is used to identify where a "ball pick up location" is. In the competition the ball can start at any of 3 ball pickup locations. Each location is identified using an IR beacon transmitter.

3 IR sensors on the bottom of the robot were used for line following to allow the robot to move across the board accurately and efficiently.

Servo arm with forcer sensor was used to allow the robot to identify when a ball was succesfully picked up by it's gripper.

Bumper touch sensor was used to indicate to the robot whenever the basketball net was located.

Map of court below:
![alt text](https://github.com/johan1252/ArduinoRobotBasketball/blob/master/299_court.png?raw=true)

<pre>
Where:
* = ball pickup locations (East, West, South)
"-" = black line for line following
"====" = backetball net
"+" = border
</pre>
## Documentation

Additional documentation can be found in technical report "ELEC299-Technical-Report-Group-20.docx".

## Getting Started

### Prerequisites

```
Arduino IDE
Robot with 2 PWM motors, IR Receiver, 3 IR sensors, bumper touch sensors, force sensor, and a servo arm with gripper.
```

## Images

Robot used:

![alt text](https://github.com/johan1252/ArduinoRobotBasketball/blob/master/299_robot.png?raw=true)

## Authors

* Maytha Nassor
* Muhammad Usman Majeed
* Johan Cornelissen
