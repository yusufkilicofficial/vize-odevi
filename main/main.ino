#include "deney1_lib.h"
#include "deney2_lib.h"
#include "deney3_lib.h"

void setup() {
  deney1_setup();
  deney2_setup();
  deney3_setup();
}

void loop() {
  deney1_loop();
  deney2_loop();
  deney3_loop();
  _delay_ms(1);
}

int main(void) {
  setup();
  while(1) {
    loop();
  }
  return 0;
}
