#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hcsr04.h"
#include "tm_stm32f4_gpio.h"
#include <stdio.h>

#define zielona GPIO_Pin_12
#define pomaranczowa GPIO_Pin_13
#define czerwona GPIO_Pin_14
#define niebieska GPIO_Pin_15
#define wszystkie GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 //dla wygody

/////////////////////////////BLUETOOTH////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
volatile uint16_t poprawnoscDanych = 0;
volatile uint16_t odebraneDane = 0;
volatile uint16_t licznik = 0;

void odczytaj()
{
    const int16_t dlugosc = 10;
    uint8_t bufforDanych[dlugosc];
    odebraneDane = 0;
    poprawnoscDanych = 0;
    licznik = 0;
    GPIO_SetBits(GPIOG, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
    for (odebraneDane = 0; odebraneDane < dlugosc;) {
        poprawnoscDanych++;
        while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET) {
            bufforDanych[odebraneDane] = USART_ReceiveData(USART3);
            odebraneDane++;
            poprawnoscDanych = 0;
        }
        if (poprawnoscDanych > 1000) {
            odebraneDane = 0;
            break;
        }
    }
            if (odebraneDane == dlugosc)
            {
            	interpretuj(bufforDanych);
                if(bufforDanych[0] == 'S' && bufforDanych[1] == 'T' && bufforDanych[2] == 'A' && bufforDanych[3] == 'R' && bufforDanych[4] == 'T')
                {
                    mrugnij(); mrugnij(); mrugnij(); mrugnij(); mrugnij();
                }
                if(bufforDanych[0] == 'L' && bufforDanych[1] == 'E' && bufforDanych[2] == 'D')
                {
                    if (bufforDanych[3] == 'O' && bufforDanych[4] == 'N') GPIO_SetBits(GPIOD, GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15);
                    else if (bufforDanych[3] == 'O' && bufforDanych[4] == 'F' && bufforDanych[5] == 'F') GPIO_ResetBits(GPIOD, GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15);
                }
                else if(bufforDanych[0] == 'L')
                {
                    if(bufforDanych[1] == '1') GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
                    else if(bufforDanych[1] == '2') GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
                    else if(bufforDanych[1] == '3') GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
                    else if(bufforDanych[1] == '4') GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
                }
            }
}
void interpretuj(uint8_t komenda[])
{
    if (komenda[0] == 's' && komenda[1] == 't' && komenda[2] == 'a' && komenda[3] == 'r' && komenda[4] == 't') mrugnij();
    if (komenda[0] == 's' && komenda[1] == 'z' && komenda[2] == 'y' && komenda[3] == 'b' && komenda[4] == 'c' && komenda[5] == 'i' && komenda[6] == 'e' && komenda[7] == 'j') szybciej();
    if (komenda[0] == 'w' && komenda[1] == 'o' && komenda[2] == 'l' && komenda[3] == 'n' && komenda[4] == 'i' && komenda[5] == 'e' && komenda[6] == 'j') wolniej();
    if (komenda[0] == 'l' && komenda[1] == 'e' && komenda[2] == 'd' && komenda[3] == '1') ledON(1);
    if (komenda[0] == 'l' && komenda[1] == 'e' && komenda[2] == 'd' && komenda[3] == '2') ledON(2);
    if (komenda[0] == 'l' && komenda[1] == 'e' && komenda[2] == 'd' && komenda[3] == '3') ledON(3);
    if (komenda[0] == 'l' && komenda[1] == 'e' && komenda[2] == 'd' && komenda[3] == '4') ledON(4);
    if (komenda[0] == 'l' && komenda[1] == 'e' && komenda[2] == 'd' && komenda[3] == 'o' && komenda[4] == '1') ledOFF(1);
    if (komenda[0] == 'l' && komenda[1] == 'e' && komenda[2] == 'd' && komenda[3] == 'o' && komenda[4] == '2') ledOFF(2);
    if (komenda[0] == 'l' && komenda[1] == 'e' && komenda[2] == 'd' && komenda[3] == 'o' && komenda[4] == '3') ledOFF(3);
    if (komenda[0] == 'l' && komenda[1] == 'e' && komenda[2] == 'd' && komenda[3] == 'o' && komenda[4] == '4') ledOFF(4);
    if (komenda[0] == 's' && komenda[1] == 'e' && komenda[2] == 'r' && komenda[3] == 'w' && komenda[4] == 'o' && komenda[5] == 'w') serwo_standard();
    if (komenda[0] == 's' && komenda[1] == 'e' && komenda[2] == 'r' && komenda[3] == 'w' && komenda[4] == 'o' && komenda[5] == 'a') serwo_w_lewo();
    if (komenda[0] == 's' && komenda[1] == 'e' && komenda[2] == 'r' && komenda[3] == 'w' && komenda[4] == 'o' && komenda[5] == 's') mrugnij(); mrugnij();mrugnij();mrugnij();mrugnij();mrugnij();
    if (komenda[0] == 's' && komenda[1] == 'e' && komenda[2] == 'r' && komenda[3] == 'w' && komenda[4] == 'o' && komenda[5] == 'd') serwo_w_prawo();
}
void mrugnij() {
    int i = 0;
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    for (i; i<1000000; i++) {}
}
void szybciej() {
    //zrób szyciej serwo o 10%
}
void wolniej() {
    //zrob wolniej serwo o 10%
}

void ledON(int x) {
    switch (x)
    {
    case 1:
        GPIO_SetBits(GPIOD, GPIO_Pin_12);
        break;
    case 2:
        GPIO_SetBits(GPIOD, GPIO_Pin_13);
        break;
    case 3:
        GPIO_SetBits(GPIOD, GPIO_Pin_14);
        break;
    case 4:
        GPIO_SetBits(GPIOD, GPIO_Pin_15);
        break;
    }
}

void ledOFF(int x) {
    switch (x)
    {
    case 1:
        GPIO_ResetBits(GPIOD, GPIO_Pin_12);
        break;
    case 2:
        GPIO_ResetBits(GPIOD, GPIO_Pin_13);
        break;
    case 3:
        GPIO_ResetBits(GPIOD, GPIO_Pin_14);
        break;
    case 4:
        GPIO_ResetBits(GPIOD, GPIO_Pin_15);
        break;
    }
}
void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
    {
        odczytaj();
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
    }
}
void inicjalizacja()
{
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        GPIO_InitTypeDef  DIODA;
        DIODA.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
        DIODA.GPIO_Mode = GPIO_Mode_OUT;
        DIODA.GPIO_OType = GPIO_OType_PP;
        DIODA.GPIO_Speed = GPIO_Speed_100MHz;
        DIODA.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOD, &DIODA);

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

        GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
        GPIO_InitTypeDef TX;
        TX.GPIO_OType=GPIO_OType_PP;
        TX.GPIO_PuPd=GPIO_PuPd_UP;
        TX.GPIO_Mode=GPIO_Mode_AF;
        TX.GPIO_Pin=GPIO_Pin_10;
        TX.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &TX);

        GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3);
        TX.GPIO_Mode=GPIO_Mode_AF;
        TX.GPIO_Pin=GPIO_Pin_11;
        GPIO_Init(GPIOC, &TX);

        USART_InitTypeDef USART;
        USART.USART_BaudRate=9600;
        USART.USART_WordLength=USART_WordLength_8b;
        USART.USART_StopBits=USART_StopBits_1;
        USART.USART_Parity=USART_Parity_No;
        USART.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
        USART.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
        USART_Init(USART3, &USART);

        USART_Cmd(USART3, ENABLE);

        NVIC_InitTypeDef NVIC_USART;
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
        NVIC_USART.NVIC_IRQChannel=USART3_IRQn;
        NVIC_USART.NVIC_IRQChannelPreemptionPriority=0;
        NVIC_USART.NVIC_IRQChannelSubPriority=0;
        NVIC_USART.NVIC_IRQChannelCmd=ENABLE;

        NVIC_Init(&NVIC_USART);
        NVIC_EnableIRQ(USART3_IRQn);
}
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////////////////////SILNIK I SERVO ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void Conf_GPIO() {
	 //piny sterujace kierunkiem silnika
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	 //piny PWM dla serwa i silnika
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void Conf_Timer4() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 65500;
	TIM_TimeBaseStructure.TIM_Prescaler = 83;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM4, ENABLE);
}

void Conf_PWM() {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// KONFIGURACJA 1 KANALU
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// KONFIGURACJA 2 KANALU
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
}

void serwo_w_prawo(){ TIM4->CCR3 = 1500; GPIO_ToggleBits(GPIOD, GPIO_Pin_14);}
void serwo_w_lewo(){ TIM4->CCR3 = 900; GPIO_ToggleBits(GPIOD, GPIO_Pin_13);}
void serwo_standard(){ TIM4->CCR3 = 1200; GPIO_ToggleBits(GPIOD, GPIO_Pin_12);}

void silnik_przod(volatile uint16_t speed){

	TIM4->CCR2 = speed;
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	GPIO_ResetBits(GPIOA, GPIO_Pin_9);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////CZUJNIK /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void tim3_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 //Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 7200;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE);
}
void przerwania_tim3_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// numer przerwania
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	// priorytet g³ówny
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	// subpriorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kana³
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// zapisz wype³nion¹ strukturê do rejestrów
	NVIC_Init(&NVIC_InitStructure);


	// wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	// zezwolenie na przerwania od przepe³nienia dla timera 3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

}
void TIM3_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{
         		GPIO_ResetBits(GPIOD, wszystkie);
                	// wyzerowanie flagi wyzwolonego przerwania
                	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
         	}
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


int main(void)
{
	//SystemInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	Conf_GPIO(); // inicjalizacja portow GPIO
	Conf_Timer4(); // konfiguracja zegara TIM4
	Conf_PWM(); // inicjalizacja i konfiguracja PWM

	przerwania_tim3_init();

	TM_HCSR04_t czujnik;
	TM_DELAY_Init(); // funkcje opozniajace

	int i=0;
	inicjalizacja();

	while(1)
	{
		silnik_przod(50000);
		serwo_standard();
		for (int i = 0; i < 1000000; i++){}
		serwo_w_prawo();
		for (int i = 0; i < 1000000; i++){}
		serwo_standard();
		for (int i = 0; i < 1000000; i++){}
		serwo_w_lewo();
		for (int i = 0; i < 1000000; i++){}
		silnik_przod(10000);

	}
}

