//**************************************************************************************************************
/**
 * main.c
 * Laboratorio 9 - Tivaware
 * Juan Daniel Cortez - 17161
 */
//**************************************************************************************************************
// Librerias
//**************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
//**************************************************************************************************************
// Variables Globales
//**************************************************************************************************************

uint32_t ui32Period;
uint32_t ui32Period2;

int Push;
int bandera;

//**************************************************************************************************************
// Código Principal
//**************************************************************************************************************
int main(void)
{
    // Configuración del oscilador externo, usando PLL y teniendo una frecuencia de 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Se habilita el reloj para el puerto F y B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Se establecen como salidas el puerto F y B
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //Se configura el boton PUSH 2 o PF_0
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R = 0x0f;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Se habilita el reloj para los temporizadores
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    // Configuración del Timer 0 y 1 como temporizador períodico
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    // Se calcula el período para el temporizador (1 seg)
    ui32Period = (SysCtlClockGet()) / 2;
    // Se calcula el período para el temporizador (0.5 seg)
    ui32Period2 = (ui32Period / 2);
    // Establecer el periodo de los temporizador
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);
    TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period2 - 1);

    // Se habilita la interrupción por el TIMER0A y TIMER1A
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER1A);
    // Se establece que exista la interrupción por Timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    // Se habilitan las interrupciones Globales
    IntMasterEnable();
    // Se habilitan los Timer
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER1_BASE, TIMER_A);
    //**********************************************************************************************************
    // Loop Principal
    //**********************************************************************************************************
    while (1)
    {
        Push = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
        if(Push == 0)
        {
            bandera = 1;
        }
        if(bandera == 1)
        {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
        SysCtlDelay(30000000);
        /**VerdeParpadeante**/
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
        SysCtlDelay(10000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
        SysCtlDelay(10000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
        SysCtlDelay(10000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
        SysCtlDelay(10000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
        SysCtlDelay(10000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
        SysCtlDelay(10000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
        SysCtlDelay(10000000);
        /*****Amarillo******/
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x04);
        SysCtlDelay(20000000);
        /*****Rojo*******/
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02);
        SysCtlDelay(10000000);
        bandera = 0;
        }
    }
}
//**************************************************************************************************************
// Handler de la interrupción del TIMER 0 - Recordar modificar el archivo tm4c123ght6pm_startup_css.c
//**************************************************************************************************************
void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    if (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0))
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 1);
    }
}
//**************************************************************************************************************
// Handler de la interrupción del TIMER 1 - Recordar modificar el archivo tm4c123ght6pm_startup_css.c
//**************************************************************************************************************
void Timer1IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    if (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1))
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 2);
    }
}
