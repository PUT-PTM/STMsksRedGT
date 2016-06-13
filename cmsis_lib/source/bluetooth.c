#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "tm_stm32f4_delay.h"

#include <stdio.h>


volatile uint16_t poprawnoscDanych = 0;
volatile uint16_t odebraneDane = 0;
volatile uint16_t licznik = 0;
volatile uint8_t znak = 69;
volatile uint16_t dane_serwo = 1250;
volatile uint16_t dane_silnik = 0;
const int16_t dlugosc = 1;
volatile uint16_t serwo_max = 1800;
volatile uint16_t serwo_medium = 1250;
volatile uint16_t serwo_min = 700;
volatile uint16_t silnik_max = 60000;
volatile uint16_t silnik_min = 0;

void odczytaj() {
	uint8_t bufforDanych[dlugosc];
	odebraneDane = 0;
	poprawnoscDanych = 0;
	licznik = 0;
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
	if (odebraneDane == dlugosc) {
		interpretuj(bufforDanych);
	}
}
void interpretuj(uint8_t komenda[])
{
	if (komenda[0] == 's') {
		dane_serwo = serwo_medium;
		dane_silnik = silnik_min;
		GPIO_ResetBits(GPIOE, GPIO_Pin_9);
		GPIO_ResetBits(GPIOE, GPIO_Pin_7);
		Delayms(500);
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
		GPIO_ResetBits(GPIOA, GPIO_Pin_9);
	}
	if (komenda[0] == 'q') {
		if(dane_serwo>= serwo_min+50)
		dane_serwo -= 50;
		if(dane_silnik<=silnik_max-500)
		dane_silnik += 500;
	}
	if (komenda[0] == 'w') {
		if(dane_silnik<=silnik_max-1000)
		dane_silnik += 1000;
	}
	if (komenda[0] == 'e') {
		if(dane_serwo<= serwo_max-50)
		dane_serwo += 50;
		if(dane_silnik<=silnik_max-500)
		dane_silnik += 500;
	}
	if (komenda[0] == 'a') {
		if(dane_serwo>= serwo_min+100)
		dane_serwo -= 100;
	}
	if (komenda[0] == 'd') {
		if(dane_serwo<= serwo_max-100)
		dane_serwo += 100;
	}
	if (komenda[0] == 'z') {
		if(dane_serwo>= serwo_min+50)
		dane_serwo -= 50;
		if(dane_silnik>=silnik_min+500)
		dane_silnik -= 500;
	}
	if (komenda[0] == 'x') {
		if(dane_silnik>=silnik_min+1000)
		dane_silnik -= 1000;
	}
	if (komenda[0] == 'c') {
		if(dane_serwo<= serwo_max-50)
		dane_serwo += 50;
		if(dane_silnik>=silnik_min+500)
		dane_silnik -= 500;
	}

}

void send_char(char c) {
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(USART3, c);
}

void send_string(const char* s) {
	while (*s) {
		send_char(*s++);
	}
}

void USART3_IRQHandler(void) {
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		odczytaj();
		send_string("READY\n");
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {
		}
	}
}
void inicjalizacja() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef DIODA;
	DIODA.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	DIODA.GPIO_Mode = GPIO_Mode_OUT;
	DIODA.GPIO_OType = GPIO_OType_PP;
	DIODA.GPIO_Speed = GPIO_Speed_100MHz;
	DIODA.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &DIODA);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_InitTypeDef TX;
	TX.GPIO_OType = GPIO_OType_PP;
	TX.GPIO_PuPd = GPIO_PuPd_UP;
	TX.GPIO_Mode = GPIO_Mode_AF;
	TX.GPIO_Pin = GPIO_Pin_10;
	TX.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &TX);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	TX.GPIO_Mode = GPIO_Mode_AF;
	TX.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &TX);

	USART_InitTypeDef USART;
	USART.USART_BaudRate = 9600;
	USART.USART_WordLength = USART_WordLength_8b;
	USART.USART_StopBits = USART_StopBits_1;
	USART.USART_Parity = USART_Parity_No;
	USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART);

	USART_Cmd(USART3, ENABLE);

	NVIC_InitTypeDef NVIC_USART;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_USART.NVIC_IRQChannel = USART3_IRQn;
	NVIC_USART.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_USART.NVIC_IRQChannelSubPriority = 0;
	NVIC_USART.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_USART);
	NVIC_EnableIRQ(USART3_IRQn);

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
