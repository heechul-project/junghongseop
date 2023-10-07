// 2-2 브레이크 시스템 타이머 인터럽트 사용

#define break_switch (2)
#define accel_switch (3)
#define hazard_led (9)
#define break_led (10)

#define ON HIGH
#define OFF LOW

bool run_once = false;
bool toggle1 = false;
uint16_t count1 = 0;


void setup() {
  //set pins as outputs
  Serial.begin(9600);
  Serial.println("reset");

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);  // built in led

  cli();  //stop interrupts

  set_timer_1KHz();

  sei();  //allow interrupts

}  //end setup

void loop() {
  //do other things here
  if (run_once == true) {
    run_once = false;

    digitalWrite(13, HIGH);

    if (digitalRead(break_switch) == ON) {
      digitalWrite(break_led, HIGH);

      if (count1 == 0) {
        if (toggle1 == false) {
          digitalWrite(hazard_led, HIGH);
        } else {
          digitalWrite(hazard_led, LOW);
        }
      }

      if (++count1 >= 2000) {  // 1ms X 2000 = 2초
        count1 = 0;
        toggle1 = !toggle1;
      }
    }

    if ((digitalRead(accel_switch) == ON) && (digitalRead(break_switch) == OFF)) {
      count1 = 0;
      toggle1 = false;

      digitalWrite(break_led, LOW);
      digitalWrite(hazard_led, LOW);
    }

    digitalWrite(13, LOW);
  }
}

ISR(TIMER1_COMPA_vect) {
  run_once = true;
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
