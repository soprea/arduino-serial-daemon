#include "../utils/config.h"
#include "../utils/header.h"
#include "../utils/utils.h"

struct conf parms;
/* configuration file */

void smart() {
  //Doing the smart moves;
  //Send them with sendKey(keypressed);
  static char keypressed = 'T';
  sendKey(keypressed);
}

int main(void) {
  init_parameters(&parms);
  parse_config(&parms);
  daemonize();
  while (1) {
    smart();
    usleep(1000);
  }
  return 0;
}
