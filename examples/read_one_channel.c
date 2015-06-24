#include "ads1015drvr.h"
#include "unistd.h"

int main(void){

  int i;

  // get a device object
  t_ads1015 *x = ads1015_create();

  // choose the correct configuration:
  // channel A0
  x->input_sel = A0;
  // .0256V gain
  x->gains = PT256V;
  // data rate of 3300 samples per second on the chip
  x->dr = DR128;

  x->verbose = 1;
  // set the configuration in verbose mode to check the bits
  ads1015_set_configuration(x);

  //x->verbose = 0; // no longer necessary


  for(i=0;i<20; i++)
    {
      // read the value once;
      ads1015_read_once(x);
      // print it
      printf("val : %d\n", x->val);
      // sleep for one second
      sleep(1);
    }
  return 0;

}
