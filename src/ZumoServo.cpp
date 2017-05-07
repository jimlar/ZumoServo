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

  // See http://wiki.dsvf.net/index.php?title=Fast_PWM_on_the_Atmega_32U4

  //Prescaler (1/32)
  TCCR4B = 0b00000110;

  // Enable PWM pin
  TCCR4C = 0b00001001;

  //Ignore fault protection and use fast PWM
  TCCR4D = 0b00000000;

  // Enhanced pwm -> 11bits
  TCCR4E = 0b01000000;
  TC4H = 7; OCR4C = 0xFF;

  // PLL Postcaler for High Speed Timer, disconnect and use system clock
  PLLFRQ = PLLFRQ & 0b11001111;
}

int set_pwm(int value){
  value &= 0x7FF;  // restrict to 11 bits
  TC4H = (value >> 8);
  OCR4D = value & 0x00FF;
  return value;
}

void servoWriteMicroSeconds(int highTimeMicroseconds) {
  highTimeMicroseconds = constrain(highTimeMicroseconds, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  set_pwm(highTimeMicroseconds / 2);
}

void servoWrite(int value)
{
  value = constrain(value, 0, 180);
  value = map(value, 0, 180, MIN_PULSE_WIDTH,  MAX_PULSE_WIDTH);
  servoWriteMicroSeconds(value);
}
