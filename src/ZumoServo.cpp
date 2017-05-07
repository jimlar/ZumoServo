#include <Wire.h>
#include <Zumo32U4.h>


#define SERVO_PIN 6

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached


uint16_t volatile servoTime = 0;
uint16_t volatile servoHighTime = 2 * DEFAULT_PULSE_WIDTH;
boolean volatile servoHigh = false;

void servoInit()
{
  pinMode(SERVO_PIN, OUTPUT);

  // 10-bit operation
  TC4H = 0x03; OCR4C = 0xFF;
  //Configuration of Timer 4 Registers, OC4A (D13) + 0C4B (D10)
  // TCCR4A = 0b10100011;

  TCCR4C = 0b10100011; // ?? bits right
  //Prescaler
  TCCR4B = 0b00000011;
}

void servoWriteMicroSeconds(int highTimeMicroseconds) {
  TC4H = highTimeMicroseconds >> 8;
  OCR4C = 0xFF & highTimeMicroseconds;
}

void servoWrite(int value)
{
  if(value < 0) value = 0;
  if(value > 180) value = 180;
  value = map(value, 0, 180, MIN_PULSE_WIDTH,  MAX_PULSE_WIDTH);
  servoWriteMicroSeconds(value);
}
