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
