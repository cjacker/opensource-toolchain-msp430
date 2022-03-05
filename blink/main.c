#include <msp430fr6989.h>

int main(void)
{
    unsigned int i;

    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    P1DIR |= BIT0;
    P1OUT |= BIT0;
    P9DIR |= BIT7;
    P9OUT |= BIT7;
  
    PM5CTL0 &= ~LOCKLPM5;

    while(1)
    {
      P9OUT |= BIT7;
      for(i = 0; i < 30000; i++);
      P9OUT &= ~BIT7;
      for(i = 0; i < 30000; i++);
    }

    return 0;
}

