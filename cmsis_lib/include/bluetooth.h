#ifndef bluetooth
#define bluetooth

void odczytaj();
void interpretuj(uint8_t);
void send_char(char);
void send_string(const char*);
void USART3_IRQHandler(void);
void inicjalizacja();


#endif
