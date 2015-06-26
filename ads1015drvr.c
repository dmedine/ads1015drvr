#include "ads1015drvr.h"
#include "string.h"
#include "pthread.h"

//pthread_mutex_t device_lock = PTHREAD_MUTEX_INITIALIZER;

t_ads1015 *ads1015_create(void){

  t_ads1015 *x = malloc(sizeof(t_ads1015));
 
  x->write_buf[0] = 0;
  
  // flip the bits to the factory defaults on the chip
  // bits in write_buf[1]
  x->input_sel = A0_A1;
  x->gains = TWOPT048V;
  x->op_mode = SNGL_SHOT;

  // bits in write_buf[2]
  x->dr = DR1600;
  x->comp_mode = TRAD;
  x->comp_pol = ACT_LO;
  x->comp_latch = NON_LATCHING;
  x->comp_queue = NO_COMP;

  x->addr = 0x48;

  ads1015_set_configuration(x);

  x->read_buf[0] = 0;
  x->read_buf[1] = 0;

  strcpy(x->filename, "/dev/i2c-1");
  ads1015_open(x);

  x->val = 0;
  x->verbose = 0;

  x->quit = 0;
  x->sample_rate = 100;

  return (x);

}

void ads1015_open(t_ads1015 *x){

  // open the i2c device
  if((x->file = open(x->filename, O_RDWR))<0)
    {
      perror("Failed to open the i2c bus");
      exit(1);
    }

  // initialize the device
  if(ioctl(x->file, I2C_SLAVE, x->addr) < 0)
    {
      printf("Failed to acquire bus access and/or talk to slave.\n");
      exit(1);
    
    }

}

void ads1015_set_configuration(t_ads1015 *x){

  x->write_buf[1] = OS_ON+x->input_sel+x->gains+x->op_mode;
  if(x->verbose==1)printf("write_buf[1] : %#x\n", x->write_buf[1]);
  x->write_buf[2] = x->dr+x->comp_mode+x->comp_pol+x->comp_latch+x->comp_queue;
  if(x->verbose==1)printf("write_buf[2] : %#x\n", x->write_buf[2]);
}

void ads1015_read_once(t_ads1015 *x){

  // set the pointer to the config register
  x->write_buf[0] = CONFIG_REG;
  write(x->file, x->write_buf, 3);
 if(x->verbose==1)printf("writing to configuration register...\n");

  // wait for it...
  while ((x->read_buf[0] & 0x80) == 0)
    read(x->file, x->read_buf, 2); 
  if(x->verbose==1)printf("filling conversion buffer...\n");
   

  x->write_buf[0] = CONV_REG; // indicate that we are ready to read the conversion register    
  write(x->file, x->write_buf, 1); 
  if(x->verbose==1)printf("querying conversion buffer...\n");

  read(x->file, x->read_buf, 2); // grab the conversion
  if(x->verbose==1)printf("reading conversion buffer...\n");

  x->val = x->read_buf[0]<<8 | x->read_buf[1];
  if(x->verbose==1)printf("val : %d\n", x->val);
}

void *ads1015_read_continuous(void *y){


  t_ads1015 *x = (t_ads1015 *)y;

  int usleep_interval = (int)(1.0/(float)x->sample_rate) * 1000000;
  if(x->verbose==1)printf("usleep_interval : %d\n", usleep_interval);
  x->quit = 1; // this has to be reset outside the loop

  while(x->quit)
    {
      // I don't beleive locking is necessary because we only read from x->val, never write
      //pthread_mutex_lock(&device_lock);
      ads1015_read_once(x);

      //pthread_mutex_unlock(&device_lock);
      usleep(usleep_interval);
    }

  pthread_exit(NULL);
}

void ads2015_stop_continuous(t_ads1015 *x){

  x->quit = 0;

}

void ads2015_launch_continuous(t_ads1015 *x){

  pthread_t thread;
  int foo, rc;

  rc = pthread_create(&thread, NULL, ads1015_read_continuous, (void *)x);

}



void ads1015_kill(t_ads1015 *x){

  if(x!=NULL)
    {
      free(x);
      x = NULL;
    }

}
