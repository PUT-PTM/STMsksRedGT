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
volatile uint8_t znak = 69;
void odczytaj()
{
    const int16_t dlugosc = 1;
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
            }
}
void interpretuj(uint8_t komenda[])
{
    if (komenda[0] == 's') {mrugnij(); mrugnij();mrugnij(); mrugnij();mrugnij(); mrugnij();mrugnij(); mrugnij();mrugnij(); mrugnij();}
    if (komenda[0] == 'q') {mrugnijPomaranczowa(); mrugnijZielona(); }
    if (komenda[0] == 'w') {mrugnijPomaranczowa();}
    if (komenda[0] == 'e') {mrugnijPomaranczowa(); mrugnijCzerwona(); }
    if (komenda[0] == 'a') {mrugnijZielona();}
    if (komenda[0] == 'd') {mrugnijCzerwona();}
    if (komenda[0] == 'z') {mrugnijNiebieska(); mrugnijZielona();}
    if (komenda[0] == 'x') {mrugnijNiebieska();}
    if (komenda[0] == 'c') {mrugnijNiebieska(); mrugnijCzerwona();}

}
void mrugnij() {
    int i = 0;
    GPIO_ToggleBits(GPIOD, zielona | pomaranczowa | czerwona | niebieska);
    for(i; i<500000; i++);
}
void szybciej() {
    //zrób szyciej serwo o 10%
}
void wolniej() {
    //zrob wolniej serwo o 10%
}

void mrugnijCzerwona(){
    int i=0;
    GPIO_ToggleBits(GPIOD, czerwona);
    for(i; i<500000; i++);
    GPIO_ToggleBits(GPIOD, czerwona);
}
void mrugnijPomaranczowa(){
    int i=0;
    GPIO_ToggleBits(GPIOD, pomaranczowa);
    for(i; i<500000; i++);
    GPIO_ToggleBits(GPIOD, pomaranczowa);
}
void mrugnijZielona(){
    int i=0;
    GPIO_ToggleBits(GPIOD, zielona);
    for(i; i<500000; i++);
    GPIO_ToggleBits(GPIOD, zielona);
}
void mrugnijNiebieska(){
    int i=0;
    GPIO_ToggleBits(GPIOD, niebieska);
    for(i; i<500000; i++);
    GPIO_ToggleBits(GPIOD, niebieska);
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

void send_char(uint16_t c) {
    char table[6] = {'R', 'E', 'A', 'D', 'Y', '\n'};
    for(int i=0; i<6; i++){
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(USART3, table[i]);
    //mrugnij();
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

        //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
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
	TIM_TimeBaseStructure.TIM_Period = 60000;
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

void serwo_w_prawo(){ TIM4->CCR3 = 1500; }
void serwo_w_lewo(){ TIM4->CCR3 = 900; }
void serwo_standard(){ TIM4->CCR3 = 1200; }

void silnik_przod(volatile uint16_t speed){
	//GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
	TIM4->CCR1 = speed;
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	GPIO_ResetBits(GPIOA, GPIO_Pin_9);

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////CZUJNIK /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TM_HCSR04_t czujnik;
void tim3_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 //Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 8399;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// zapisz wype³nion¹ strukturê do rejestrów
	NVIC_Init(&NVIC_InitStructure);


	// wyczyszczenie przerwania od+ timera 3 (wyst¹pi³o przy konfiguracji timera)
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	// zezwolenie na przerwania od przepe³nienia dla timera 3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

}
void TIM3_IRQHandler(void)
{
	GPIO_ResetBits(GPIOD, wszystkie);
	czujnik.Distance=-10;
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{
         		TM_HCSR04_Read(&czujnik);

         				        if (czujnik.Distance < 0) //blad
         				        {
         				        	GPIO_SetBits(GPIOD, czerwona);
         				        }
         				        else if (czujnik.Distance > 50) //ponad 50cm
         				        {
         				        	GPIO_SetBits(GPIOD, pomaranczowa);
         				        	//silnik_przod(20000);
         				        }
         				        else //miedzy 0 a 50 cm
         				        {
         				        	GPIO_SetBits(GPIOD, niebieska);
         				        	//silnik_przod(0);
         				        }
                	// wyzerowanie flagi wyzwolonego przerwania
                	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
         	}
}
void diody_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


int main(void)
{
	SystemInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	Conf_GPIO(); // inicjalizacja portow GPIO
	Conf_Timer4(); // konfiguracja zegara TIM4
	Conf_PWM(); // inicjalizacja i konfiguracja PWM

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//tim3_init();
	//przerwania_tim3_init();
	diody_init();


	TM_DELAY_Init(); // funkcje opozniajace

	int i=0;
	inicjalizacja();

	if (!TM_HCSR04_Init(&czujnik, GPIOD, GPIO_PIN_0, GPIOC, GPIO_PIN_1)) //
		  	  {																   //bylo ECHO PD0, TRIGGER PC1
		        while(1)
		        {
		            GPIO_SetBits(GPIOD, czerwona);
		            Delayms(100);
		        }
		  	  }

	while(1)
	{


		//silnik_przod(50000);
		serwo_standard();
		for (int i = 0; i < 1000000; i++){}
		serwo_w_prawo();
		for (int i = 0; i < 1000000; i++){}
		serwo_standard();
		for (int i = 0; i < 1000000; i++){}
		serwo_w_lewo();
		for (int i = 0; i < 1000000; i++){}
		//silnik_przod(10000);
		//for (int i = 0; i < 100000000; i++){}

	}
}

