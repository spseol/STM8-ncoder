#include "stm8s.h"
#include "milis.h"

//#include "delay.h"
#include "uart1.h"
#include <stdio.h>

#define _ISOC99_SOURCE
#define _GNU_SOURCE

#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define LED_HIGH   GPIO_WriteHigh(LED_PORT, LED_PIN)
#define LED_LOW  GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN)

#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#define BTN_PUSH (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)==RESET) 

#define NCODER_CLK_PORT GPIOB
#define NCODER_CLK_PIN GPIO_PIN_5
#define NCODER_DATA_PORT GPIOB
#define NCODER_DATA_PIN GPIO_PIN_4
#define NCODER_SW_PORT GPIOB
#define NCODER_SW_PIN GPIO_PIN_3
#define NCODER_GET_CLK (GPIO_ReadInputPin(NCODER_CLK_PORT, NCODER_CLK_PIN) != RESET)
#define NCODER_GET_DATA ( GPIO_ReadInputPin(NCODER_DATA_PORT, NCODER_DATA_PIN)!=RESET)
#define NCODER_GET_SW ( GPIO_ReadInputPin(SW_PORT, NCODER_SW_PIN )==RESET)

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);

    GPIO_Init(NCODER_CLK_PORT, NCODER_CLK_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(NCODER_DATA_PORT, NCODER_DATA_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(NCODER_SW_PORT, NCODER_SW_PIN, GPIO_MODE_IN_PU_NO_IT);

    init_milis();
    init_uart1();
}

int minule = 0;
int8_t check_ncoder(void)
{

    if (minule == 0 && NCODER_GET_CLK == 1) {
        // vzestupná hrana 
        minule = 1;
        if (NCODER_GET_DATA == 0) {
            return -1;
        } else {
            return 1;
        }
    } 
    else if (minule == 1 && NCODER_GET_CLK == 0) {
        // sestupná hrana 
        minule = 0;
        if (NCODER_GET_DATA == 0) {
            return 1;
        } else {
            return -1;
        }
    }
    return 0;
}

int main(void)
{
    uint32_t time = 0;
    uint32_t time1 = 0;
    int16_t bagr = 0;
    setup();

    while (1) {

        if (milis() - time > 33) {
            LED_REVERSE;
            time = milis();
            printf("\r  %5d     ", bagr);
        }

        if (milis() - time1 > 1) {
            time1 = milis();
        }
        bagr += check_ncoder();

    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
