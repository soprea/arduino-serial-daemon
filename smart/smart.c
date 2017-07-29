#include "../utils/config.h"
#include "../utils/header.h"
#include "../utils/utils.h"

struct conf parms;
/* configuration file */
//static char keypressed[10];

void smart() {
  //Doing the smart moves;
  //Send them with sendKey(keypressed);
}

int main(void) {
  init_parameters(&parms);
  parse_config(&parms);
  daemonize();
  while (1) {
    /* smart(); */
  }
  return 0;
}
