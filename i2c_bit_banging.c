
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
    set_SDA();
    I2C_delay();
    set_SCL();
    

    // Repeated start setup time, minimum 4.7us
    I2C_delay();
  }

 

  // SCL is high, set SDA from 1 to 0.
  clear_SDA();
  I2C_delay();
  clear_SCL();
  started = true;
}

void i2c_stop_cond(void)
{
  // set SDA to 0
  clear_SDA();
  I2C_delay();

  set_SCL();
  // Clock stretching
  

  // Stop bit setup time, minimum 4us
  I2C_delay();

  // SCL is high, set SDA from 0 to 1
  set_SDA();
  I2C_delay();

  

  started = false;
}




// Write a byte to I2C bus. Return 0 if ack by the target.
int* i2c_write_byte(bool start, bool stop,unsigned char byte) {
  if (start){
    i2c_start_cond();
  }
  static int arr[7];
  int i;
  for (i = 0; i < 8; i++) {
    if (byte & 0x80){
        arr[i]=1;
    }
    else arr[i]=0;
    byte <<= 1;
  }
  if(stop){
    i2c_stop_cond();
  }
  return arr;
}
void i2c_read_and_print( int* data){
   printf(" data: %d%d%d%d%d%d%d%d \n", data[0], data[1], data[2], data[3],data[4], data[5], data[6], data[7]);
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
    bool start,stop;
    start = true ; stop = true;
    i2c_read_and_print( i2c_write_byte(start, stop, data));
    printf("SDA: %d, SCL: %d line0\n",SDA,SCL);
    data = 0x31;
    start = true ; stop = false;
    i2c_read_and_print( i2c_write_byte(start, stop, data));
    printf("SDA: %d, SCL: %d line1\n",SDA,SCL);
    data = 0x11;
    start = false ; stop = true;
    i2c_read_and_print( i2c_write_byte(start, stop, data));
    printf("SDA: %d, SCL: %d line2\n",SDA,SCL);
    data = 0x15;
    start = false ; stop = false;
    i2c_read_and_print( i2c_write_byte(start, stop, data));
    printf("SDA: %d, SCL: %d line3\n",SDA,SCL);
}
//update 1/7/2023