
// Online IDE - Code Editor, Compiler, Interpreter

#include<stdio.h>
 int chip_select,data_in, clock_line; 
int *SD_CS,*SD_DI,*SD_CLK;

void output_low(int *pin){
    *pin = 0;
}
void output_high(int *pin){
    *pin = 1;
}
void delay(int timedelay){
    for (int i = 0; i < timedelay ; i++){
        printf("-|");
    }
}
// transmit byte serially, MSB first
void send_8bit_serial_data(unsigned char data)
{
   int i;

   // select device (active low)
   output_low(SD_CS);
   printf("chip select - active low: %d\n",chip_select);

   // send bits 7..0
   for (i = 0; i < 8; i++)
   {
       // consider leftmost bit
       // set line high if bit is 1, low if bit is 0
       if (data & 0x80)
           output_high(SD_DI);
       else
           output_low(SD_DI);
        printf("bit %d, data_in: %d \n",i,data_in);
       // pulse the clock state to indicate that bit value should be read
       output_low(SD_CLK);
       printf("clock : %d -(delay)-",clock_line);
       delay(6);
       output_high(SD_CLK);
        printf(" -->clock : %d\n",clock_line);

       // shift byte left so next bit will be leftmost
       data <<= 1;
   }

   // deselect device
   output_high(SD_CS);
   printf("deselect - chip_select: %d\n",chip_select);
   
}
// MSB hex to binary
void print_byte_binary(unsigned char data){
    for (int i = 0 ; i < 8 ; i++){
        if (data & 0x80){
            printf("1");
        }
        else{
            printf("0");
        }
        data <<= 1;
    }
    printf("\n");
}



int main()
{
SD_CS = &chip_select;
SD_DI = &data_in;
SD_CLK = &clock_line;
    unsigned char data = 0x1f;
    print_byte_binary(data);
    send_8bit_serial_data(data);
}
