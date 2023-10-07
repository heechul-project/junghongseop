#include <avr/wdt.h>

uint8_t countISR = 0;
bool run_once = false;

bool flag_btn_0 = false;
bool flag_btn_1 = false;

unit8_t delay_btn_0 = 0;
unit8_t delay_btn_1 = 0;

unit8_t count_push_0 = 0;
unit8_t count_push_1 = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin
}

void loop() {
  // put your main code here, to run repeatedly:
  wdt_reset();

  if(run_once == true) {
    run_once = false;

    digitalWrite(13, HIGH);

    check_btn_0();
    check_btn_1();

    digitalWrite(13, LOW);
  }

  ISR(TIMER1_COMPA_vect) {
    if(countISR++ >= 20){
      countISR = 1;

      run_once = true;
    }
  }
}
