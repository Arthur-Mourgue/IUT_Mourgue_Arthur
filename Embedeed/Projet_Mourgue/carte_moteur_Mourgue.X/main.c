#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"


int main(void){
    
    InitOscillator();
    InitIO();
    
    InitTimer1();
    InitTimer23();
    
    InitPWM();
    PWMSetSpeed(20,1);
    
    
    LED_BLANCHE = 1 ;
    LED_BLEUE = 1;
    LED_ORANGE = 1;
    
    while(1){
        
        //rien
} 
}
