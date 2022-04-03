#include "../inc/led.h"

static uint16_t ledRojo, ledAmarillo, ledVerde; //estas variables solamente son accesibles en este archivo

void configurarLed(uint16_t ledR, uint16_t ledA, uint16_t ledV)
{
    ledRojo = ledR;
    ledAmarillo = ledA;
    ledVerde = ledV;
}

void prenderLed(char led)
{
    switch (led)
    {
    case 'R':
    	HAL_GPIO_WritePin(GPIOD, ledRojo, 1);
    break;
    case 'A':
    	HAL_GPIO_WritePin(GPIOD, ledAmarillo, 1);
    break;
    case 'V':
    	HAL_GPIO_WritePin(GPIOD, ledVerde, 1);
    break;
    default:
        break;
    }
}

void apagarLed(char led)
{
    switch (led)
    {
    case 'R':
    	HAL_GPIO_WritePin(GPIOD, ledRojo, 0);
    break;
    case 'A':
    	HAL_GPIO_WritePin(GPIOD, ledAmarillo, 0);
    break;
    case 'V':
    	HAL_GPIO_WritePin(GPIOD, ledVerde, 0);
        break;
    default:
        break;
    }
}
