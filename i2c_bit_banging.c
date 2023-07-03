
#include<stdio.h>
// Boolean data type
#include <stdbool.h>
//bit banging i2c
// Hardware-specific support functions that MUST be customized:
#define I2CSPEED 100
void I2C_delay(void);
static int SDA,SCL;
void set_SCL(void){
    SCL = 1;
};   // Do not drive SCL (set pin high-impedance)
void clear_SCL(void){
   SCL = 0;
}; // Actively drive SCL signal low
void set_SDA(void){
    SDA = 1;
};   // Do not drive SDA (set pin high-impedance)
void clear_SDA(void){
   SDA = 0;
}; // Actively drive SDA signal low

bool started = false; // global data

void i2c_start_cond(void)
{
  if (started) { 
    // if started, do a restart condition
    // set SDA to 1
    printf("start condition: started = false\n");
    set_SDA();
    printf("fucntions: set sda");
    I2C_delay();
    printf("fucntions: delay");
    set_SCL();
    printf("fucntions: set scl");
    

    // Repeated start setup time, minimum 4.7us
    I2C_delay();
    printf("fucntions: delay");
  }

 
 printf("start condition: started = true\n");
  // SCL is high, set SDA from 1 to 0.
  clear_SDA();
  printf("fucntions: clear sda\n");
  I2C_delay();
  printf("fucntions: delay\n");
  clear_SCL();
  printf("fucntions: clear scl\n");
  started = true;
  printf("started: false -->true\n");
}

void i2c_stop_cond(void)
{
  // set SDA to 0
  clear_SDA();
  printf("fucntions: clear sda\n");
  I2C_delay();
  printf("fucntions: delay\n");
  set_SCL();
  printf("fucntions: set clock\n");
  // Clock stretching
  

  // Stop bit setup time, minimum 4us
  I2C_delay();
  printf("fucntions: delay\n");

  // SCL is high, set SDA from 0 to 1
  set_SDA();
  printf("fucntions: set sda\n");
  I2C_delay();
  printf("fucntions: delay\n");
  started = false;
  printf("stop condition: started == false\n");
}




// Write a byte to I2C bus. Return 0 if ack by the target.
int* i2c_write_byte(int *start, int *stop,unsigned char byte) {
  if (*start){
    i2c_start_cond();
  }
  printf("start write: SDA: %d,SCl: %d, global started: %d\n",SDA, SCL, started);
  static int arr[7];
  int i;
  for (i = 0; i < 8; i++) {
    if (byte & 0x80){
        arr[i]=1;
    }
    else arr[i]=0;
    byte <<= 1;
  }
  if(*stop){
    i2c_stop_cond();
  }
   printf("stop write: SDA: %d,SCl: %d, global started: %d",SDA, SCL, started);
  return arr;
}
void i2c_read_and_print( int* data){
   printf("\n data: %d%d%d%d%d%d%d%d \n ------------------------\n", data[0], data[1], data[2], data[3],data[4], data[5], data[6], data[7]);
}



void I2C_delay(void)
{ 
  volatile int v;
  int i;

  for (i = 0; i < I2CSPEED / 2; ++i) {
    v;
  }
}

// end 
int main()
{
    unsigned char data = 0x80;
    int start_state, stop_state;
    int *start, *stop;
    start = &start_state; stop = &stop_state;
    printf("start = true; stop = true\n");
     *start = 1 ; *stop = 1;
    i2c_read_and_print( i2c_write_byte(start, stop, data));
    data = 0x31;
     printf("start = true; stop = false\n");
    *start = 1 ; *stop = 0;
    i2c_read_and_print( i2c_write_byte(start, stop, data));
    data = 0x11;
     printf("start = false; stop = true\n");
    *start = 0 ; *stop = 1;
    i2c_read_and_print( i2c_write_byte(start, stop, data));
    data = 0x15;
     printf("start = false; stop = false\n");
    start = 0 ; stop = 0;
    i2c_read_and_print( i2c_write_byte(start, stop, data));
}
//update 1/7/2023
