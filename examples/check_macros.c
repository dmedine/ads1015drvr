#include "ads1015drvr.h"

int main(void){

  // create a ads1015 "object"
  t_ads1015 *x = ads1015_create();
  
  x->verbose = 1; //set it to verbose mode
  printf("we should see 0x85 and 0x83\n");
  ads1015_set_configuration(x); 

  // choose a random, different value for each setting to check the macros are correct
  x->input_sel = A0;
  x->gains = ONEPT024V;
  x->op_mode = CONT_MODE;

  x->dr = DR490;
  x->comp_mode = WINDOW;
  x->comp_pol = ACT_HI;
  x->comp_latch = LATCHING;
  x->comp_queue = ASSERT4;
  printf("we should see 0xC7 and 0x5E\n");
  ads1015_set_configuration(x); // call the set_write_buf function and inspect the hex values
  
  ads1015_kill(x);

  return 0;
}

