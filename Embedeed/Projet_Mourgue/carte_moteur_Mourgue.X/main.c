#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"

unsigned int ADCValue0, ADCValue1, ADCValue2;
unsigned int *result ;

int main(void) {

    InitOscillator();
    InitIO();

    InitTimer1();
    InitTimer23();

    InitPWM();
    InitADC1();


    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    //PWMSetSpeed(20,MOTEUR_DROITE);

    while (1) {

        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();

            *result = ADCGetResult();
            ADCValue0 = result[0];
            ADCValue1 = result[1];
            ADCValue2 = result[2];
        }
    }
}
