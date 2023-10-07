#include<avr/wdt.h>

uint8_t countISR = 0;
bool run_once = false;

bool flag_btn_0 = false;
bool flag_btn_1 = false;

uint8_t delay_btn_0 = 0;
uint8_t delay_btn_1 = 0;

uint8_t count_push_0 = 0;
uint8_t count_push_1 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(13, OUTPUT);

  cli();
  set_timer_10KHz();
  sei();

  wdt_enable(WDTO_15MS);
}

void loop() {
  // put your main code here, to run repeatedly:
  wdt_reset();

  if(run_once == true){
    run_once = false;

    digitalWrite(13, HIGH);

    check_btn_0();
    check_btn_1();

    digitalWrite(13, LOW);
  }
}

ISR(TIMER1_COMPA_vect) {
  if(countISR++ >= 20){
    countISR = 1;

    run_once = true;
  }
}
void set_timer_10KHz() {
  //set timer1 interrupt at 1KHz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   //initialize counter value to 0
  // set compare match register for 1KHz increments
  OCR1A = 199;  // = (16*10^6) / (1000*64) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 and CS10 bits for 64 prescaler
  TCCR1B |= (1 << CS11); 
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void check_btn_0(void){
  if(digitalRead(2) == HIGH){
    if(delay_btn_0++ >= 10){
      if(flag_btn_0 == false){
        flag_btn_0 = true;

        count_push_0++;
        Serial.println("count_push_0: " + String(count_push_0));
      }
    }
  } else{
    delay_btn_0 = 0;
    flag_btn_0 = false;
  }
}

void check_btn_1(void){
  if(digitalRead(3) == HIGH){
    if(delay_btn_1++ >= 20){
      if(flag_btn_1 == false){
        flag_btn_1 = true;

        count_push_1++;
        Serial.println("count_push_1: " + String(count_push_1));
      }
    }
  } else{
    delay_btn_1 = 0;
    flag_btn_1 = false;
  }
}