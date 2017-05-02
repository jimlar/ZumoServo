#include <Wire.h>
#include <Zumo32U4.h>


#define SERVO_PIN 6

uint16_t volatile servoTime = 0;
uint16_t volatile servoHighTime = 3000;
boolean volatile servoHigh = false;

ISR(TIMER4_COMPA_vect)
{
  servoTime += OCR4A + 1;

  static uint16_t highTimeCopy = 3000;
  static uint8_t interruptCount = 0;

  if(servoHigh)
  {
    if(++interruptCount == 2)
    {
      OCR4A = 255;
    }

    if(servoTime >= highTimeCopy)
    {
      digitalWrite(SERVO_PIN, LOW);
      servoHigh = false;
      interruptCount = 0;
    }
  }
  else
  {
    if(servoTime >= 40000)
    {
      highTimeCopy = servoHighTime;
      digitalWrite(SERVO_PIN, HIGH);
      servoHigh = true;
      servoTime = 0;
      interruptCount = 0;
      OCR4A = ((highTimeCopy % 256) + 256)/2 - 1;
    }
  }
}

void servoInit()
{
  digitalWrite(SERVO_PIN, LOW);
  pinMode(SERVO_PIN, OUTPUT);

  // Turn on CTC mode.  Timer 2 will count up to OCR2A, then
  // reset to 0 and cause an interrupt.
  TCCR4A = (1 << WGM41);
  // Set a 1:8 prescaler.  This gives us 0.5us resolution.
  TCCR4B = (1 << CS41);

  // Put the timer in a good default state.
  TCNT4 = 0;
  OCR4A = 255;

  TIMSK4 |= (1 << OCIE4A);  // Enable timer compare interrupt.
  sei();   // Enable interrupts.
}

void servoSetPosition(uint16_t highTimeMicroseconds)
{
  TIMSK4 &= ~(1 << OCIE4A); // disable timer compare interrupt
  servoHighTime = highTimeMicroseconds * 2;
  TIMSK4 |= (1 << OCIE4A); // enable timer compare interrupt
}
