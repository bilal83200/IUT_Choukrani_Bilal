#ifndef UART_H
#define UART_H
void CB_RX1_Add(unsigned char value);
unsigned char CB_RX1_GET(void);
unsigned char CB_RX1_IsDataAvailable(void);
int CB_RX1_GetDataSize(void);
int CB_RX1_GetRemainingSize(void);

#endif /* UART_H */

