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
    
    
    LED_BLANCHE = 1 ;
    LED_BLEUE = 1;
    LED_ORANGE = 1;
    
    //PWMSetSpeed(20,MOTEUR_DROITE);
    
    while(1){
        
        //rien
} 
}
