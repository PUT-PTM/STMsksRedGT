#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hcsr04.h"
#include "tm_stm32f4_gpio.h"

#define zielona GPIO_Pin_12
#define pomaranczowa GPIO_Pin_13
#define czerwona GPIO_Pin_14
#define niebieska GPIO_Pin_15
#define wszystkie GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 //dla wygody



TM_HCSR04_t czujnik;

void czujnik_init(void)		//inicjalizacja czujnika, z referencja do obiektu, wywolanie wszystkich poprzednich funkcji!
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	diody_init();
	tim3_init();
	przerwania_tim3_init();
	TM_DELAY_Init(); // funkcje opozniajace
	TM_HCSR04_Init(&czujnik, GPIOD, GPIO_PIN_0, GPIOC, GPIO_PIN_1);
}

void tim3_init(void)		//konfiguracja timera3; przerwania co sekunde
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 8399;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE);
}
void przerwania_tim3_init(void) //konfiguracja przerwan dla timera3; priority group 2(wazne)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// numer przerwania
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	// priorytet glowny
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// subpriorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	// uruchom dany kanal
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// zapisz wypleniona strukture do rejestru
	NVIC_Init(&NVIC_InitStructure);

	// wyczyszczenie przerwania od timera 3
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	// zezwolenie na przerwania od przepelnienia dla timera 3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

}
void TIM3_IRQHandler(void) // obsluga przerwania timera 3
{
	GPIO_ResetBits(GPIOD, wszystkie);
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TM_HCSR04_Read(&czujnik); //odczyt stanu

		if (czujnik.Distance < 0) //blad
		{
			GPIO_SetBits(GPIOD, czerwona);
		}
		else if (czujnik.Distance > 50) //ponad 50cm
		{
			GPIO_SetBits(GPIOD, pomaranczowa);
		}
		else //miedzy 0 a 50 cm
		{
			GPIO_SetBits(GPIOD, niebieska);
		}
		// wyzerowanie flagi wyzwolonego przerwania
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
void diody_init(void) //inicjalizacja 4 diod kolejno: zielona pomaranczowa czerwona niebieska
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

