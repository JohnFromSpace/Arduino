#include <PID_v1.h>

// Define sensor pins
#define LEFT_SENSOR A0
#define CENTER_SENSOR A1
#define RIGHT_SENSOR A2
#define TRIGGER_PIN 7
#define ECHO_PIN 8

// Define motor pins
#define LEFT_MOTOR_1 9
#define LEFT_MOTOR_2 10
#define RIGHT_MOTOR_1 5
#define RIGHT_MOTOR_2 6

// Define sensor threshold values
#define THRESHOLD 500

// Define speed control parameters
#define BASE_SPEED 150
#define TURN_SPEED 100

// PID constants
double Kp = 2.0, Ki = 0.5, Kd = 1.0;
double targetPosition = 512; // Middle of sensor range
double input, output, setpoint;
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

// Define obstacle avoidance parameters
#define MIN_DISTANCE 20 // Minimum distance to avoid obstacle (in cm)

void setup() {
    // Initialize motor pins
    pinMode(LEFT_MOTOR_1, OUTPUT);
    pinMode(LEFT_MOTOR_2, OUTPUT);
    pinMode(RIGHT_MOTOR_1, OUTPUT);
    pinMode(RIGHT_MOTOR_2, OUTPUT);
    
    // Initialize ultrasonic sensor pins
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    // Initialize serial communication
    Serial.begin(9600);
    
    // Set PID parameters
    pid.SetMode(AUTOMATIC);
    pid.SetSampleTime(10); // 10ms sample time
    pid.SetOutputLimits(-255, 255); // Output limits for motor control
    
    // Set initial setpoint
    setpoint = targetPosition;    
}

void loop() {
    // Read sensor values
    int leftSensorValue = analogRead(LEFT_SENSOR);
    int centerSensorValue = analogRead(CENTER_SENSOR);
    int rightSensorValue = analogRead(RIGHT_SENSOR);
    
    // Calculate error for PID
    input = (leftSensorValue + rightSensorValue) / 2.0;
    
    // Compute PID output
    pid.Compute();
    
    // Determine the line position
    bool onLeft = leftSensorValue > THRESHOLD;
    bool onCenter = centerSensorValue > THRESHOLD;
    bool onRight = rightSensorValue > THRESHOLD;
    
    // Check for obstacles
    long duration, distance;
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration / 2) / 29.1;
    
    // Make decisions based on sensor readings and obstacle detection
    if (distance < MIN_DISTANCE) {
    // Stop and avoid obstacle
    avoidObstacle();
    } else {
        // Continue line following
        if (onLeft && !onCenter && !onRight) {
          // Turn left
          turnLeft();
        } else if (!onLeft && onCenter && !onRight) {
          // Go straight
          goStraight();
        } else if (!onLeft && !onCenter && onRight) {
          // Turn right
          turnRight();
        } else {
          // Move backward
          moveBackwards();
        }    
    }
}

// Function to make the robot turn left
void turnLeft() {
    int leftSpeed = constrain(BASE_SPEED - output, -255, 255);
    int rightSpeed = constrain(BASE_SPEED + output, -255, 255);

    analogWrite(LEFT_MOTOR_1, abs(leftSpeed));
    analogWrite(LEFT_MOTOR_2, 0);
    analogWrite(RIGHT_MOTOR_1, abs(rightSpeed));
    analogWrite(RIGHT_MOTOR_2, 0); 
}

// Function to make the robot turn right
void turnRight() {
    int leftSpeed = constrain(BASE_SPEED - output, -255, 255);
    int rightSpeed = constrain(BASE_SPEED + output, -255, 255);

    analogWrite(LEFT_MOTOR_1, abs(leftSpeed));
    analogWrite(LEFT_MOTOR_2, 0);
    analogWrite(RIGHT_MOTOR_1, abs(rightSpeed));
    analogWrite(RIGHT_MOTOR_2, 0);
}

// Function to make the robot go straight
void goStraight() {
    int leftSpeed = constrain(BASE_SPEED - output, -255, 255);
    int rightSpeed = constrain(BASE_SPEED + output, -255, 255);

    analogWrite(LEFT_MOTOR_1, abs(leftSpeed));
    analogWrite(LEFT_MOTOR_2, 0);
    analogWrite(RIGHT_MOTOR_1, abs(rightSpeed));
    analogWrite(RIGHT_MOTOR_2, 0);
}

// Function to make the robot move backwards
void moveBackwards() {
    analogWrite(LEFT_MOTOR_1, 0);
    analogWrite(LEFT_MOTOR_2, BASE_SPEED);
    analogWrite(RIGHT_MOTOR_1, 0);
    analogWrite(RIGHT_MOTOR_2, BASE_SPEED);    
}

// Function to avoid obstacle
void avoidObstacle() {
    
}
