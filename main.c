#include "main.h"

//Variables
static __IO uint32_t TimingDelay;
void UARTSend(const unsigned char * bBuffer, unsigned long bufCount);
volatile char received_string[MAX_STRLEN+1]; // przetrzymuje otrzymanego stringa
uint8_t Buffer[6];

int main(void) {
  
  unsigned char hello_str[] = "HELLO";
  u8 loop = 1;
  
  initPA15();
  init_USART1(BT_BAUD);


  while(loop){

    //Wyślij
    UARTSend(hello_str, sizeof(hello_str));
    
    Delay(10000000);
  }
	//wylaczenie usarta
    USART_Cmd(USART1, DISABLE);
}

//metody do wysylania stringow do usarta
//
void UARTSend(const unsigned char *bBuffer, unsigned long bufCount)
{
  while(bufCount--)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }
    USART_SendData(USART1, (uint8_t) *bBuffer++);
  }
}

void setPA15On(){
    GPIOA->BSRRL = GPIO_Pin_15;
}

void togglePA15(){
    GPIOA->ODR = GPIO_Pin_15;
}

void init_USART1(uint32_t baudrate){
  

  GPIO_InitTypeDef GPIO_InitStruct; 
  USART_InitTypeDef USART_InitStruct; 
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;			
  
  GPIO_Init(GPIOB, &GPIO_InitStruct);					
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
  
  USART_InitStruct.USART_BaudRate = baudrate;			
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;		
  USART_InitStruct.USART_Parity = USART_Parity_No;		
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
  USART_Init(USART1, &USART_InitStruct);					
  
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
  NVIC_Init(&NVIC_InitStructure);						
  
  USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void){
  if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
    
    static uint8_t cnt = 0; // string lenght
    char t = USART1->DR; // sended char in var t
    
    if( (t != '$') && (cnt < MAX_STRLEN) )
	{
      received_string[cnt] = t;
      cnt++;
    }
    else
	{
      cnt = 0;
    }
  }
}

void initPA15(){
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}
