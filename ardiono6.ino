// 3-2-2 소프트웨어 PWM으로 LED 밝기 조절
// 100Hz PWM

#include <avr/wdt.h>

bool run_once = false;

uint8_t count1to100 = 0;
uint8_t duty = 99;


void setup() {
  //set pins as outputs
  Serial.begin(9600);
  Serial.println("reset");

  pinMode(13, OUTPUT);  // built in led
  pinMode(10, OUTPUT);

  cli();  //stop interrupts
  set_timer_10KHz();
  sei();  //allow interrupts

  wdt_enable(WDTO_15MS);

}  //end setup

void loop() {
  //do other things here
  wdt_reset();

  if (run_once == true) {
    run_once = false;

    digitalWrite(13, HIGH);
    // start

    if (count1to100 > duty) digitalWrite(10, LOW);
    else digitalWrite(10, HIGH);

    // end
    digitalWrite(13, LOW);
  }
}

ISR(TIMER1_COMPA_vect) {
  run_once = true;
  if (count1to100++ == 100) count1to100 = 1;
}

void serialEvent() {
  duty = Serial.read();
}

void set_timer_1KHz() {
  //set timer1 interrupt at 1KHz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   //initialize counter value to 0
  // set compare match register for 1KHz increments
  OCR1A = 249;  // = (16*10^6) / (1000*64) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 and CS10 bits for 64 prescaler
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void set_timer_10KHz() {
  //set timer1 interrupt at 10KHz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   //initialize counter value to 0
  // set compare match register for 10KHz increments
  OCR1A = 199;  // = (16*10^6) / (10000*8) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}