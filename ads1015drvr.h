// this driver is for the TI ADS1015 I2C device

#include "stdio.h"
#include "errno.h"
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

// defines for which bits we need //
//-----------------------------------

// pointer register
#define CONV_REG 0
#define CONFIG_REG 1
#define LO_THRESH_REG 2
#define HI_THRESH_REG 3

// configuration register
// these will have to be added to complete the instructions

// bit 15
#define OS_OFF 0
#define OS_ON 0x80

// bits 14-12, input selectors
// differential modes
// input_sel:
#define A0_A1 0
#define A0_A3 0x10
#define A1_A3 0x20
#define A2_A3 0x30
// mono modes
#define A0 0x40
#define A1 0x50
#define A2 0x60
#define A3 0x70

// bits 11:9, programmable gains
// gains
#define SIXPT144V 0
#define FOURPT096V 0x2
#define TWOPT048V 0x4
#define ONEPT024V 0x6
#define PT512V 0x8
#define PT256V 0xA

// bit 8, operating mode
// op_mode
#define CONT_MODE 0
#define SNGL_SHOT 1

// bits 7:5, data rate
// dr
#define DR128 0
#define DR250 0x20
#define DR490 0x40
#define DR920 0x60
#define DR1600 0x80
#define DR2400 0xA0
#define DR3300 0xC0

// bit 4, comparator mode
// comp_mode
#define TRAD 0
#define WINDOW 0x10

// bit 3, comp polarity
// comp_pol
#define ACT_LO 0
#define ACT_HI 0x08

// bit 2, comp latching
// comp_latch
#define NON_LATCHING 0
#define LATCHING 0x04

// bits 1-0, comparator queue
// comp_queue
#define ASSERT1 0
#define ASSERT2 0x01
#define ASSERT4 0x02
#define NO_COMP 0x03

typedef struct _ads1015{

  // containers for r/w operations
  uint8_t write_buf[3];
  uint8_t read_buf[2];

  // write_buf[1] sums these
  uint8_t input_sel; // default is A0_A1
  uint8_t gains; // defualt is TWOPT048V
  uint8_t op_mode; // default is SNGL_SHOT
  
  // write_buf[2] sums these
  uint8_t dr; // default is DR1600
  uint8_t comp_mode; // default is TRAD
  uint8_t comp_pol; // default is ACT_LO
  uint8_t comp_latch; // default is  NON_LATCHING
  uint8_t comp_queue; // dfault is NO_COMP

  uint8_t addr; // the address of the chip, use the i2cdetect utility to find this

  int file;
  char filename[256]; // defaulst to  /dev/i2c-1

  int val; // the actual value
  int verbose; // flag for printing to terminal - defualt is 0;

  int quit; // flag for going in/out of the r/w loop
  int sample_rate; // in Hz, defualt is 100

}t_ads1015; 

t_ads1015 *ads1015_create(void); // simply creates a t_ads1015 object with defualt values
void ads1015_set_configuration(t_ads1015 *x); // sets the write configuration, doesn't need to happen every time...
void ads1015_open_device(t_ads1015 *x);
void ads1015_read_once(t_ads1015 *x); // performs the read/write procedure once
void *ads1015_read_continuous(void *y); // does the r/w in an infinite loop on a separate thread
void ads1015_launch_continuous(t_ads1015 *x); // launches the r/w loop
void ads1015_kill(t_ads1015 *x); // destroy the t_ads1015 object
