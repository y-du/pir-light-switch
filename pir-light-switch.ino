#include <JeeLib.h>

const byte pir_pin{2};
const byte relais_pin{3};

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup() {
  pinMode(pir_pin, INPUT);
  pinMode(relais_pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Sleepy::loseSomeTime(30000);
  attachInterrupt(0, pir_state_ISR, CHANGE);
  digitalWrite(LED_BUILTIN, LOW);
}


long last_motion{0};
byte pir_state{0};
byte relais_state{0};
long sleep_duration{30000};
long off_delay{60000};


void pir_state_ISR() {
    pir_state = digitalRead(pir_pin);
}


void loop() {
  if (pir_state == HIGH) {
    if (relais_state == LOW) {
      relais_state = HIGH;
      digitalWrite(relais_pin, relais_state);
    }
    last_motion = millis();
  }
  if (millis() - last_motion >= off_delay) {
    relais_state = LOW;
    digitalWrite(relais_pin, relais_state);
  }
  Sleepy::loseSomeTime(sleep_duration);
}
