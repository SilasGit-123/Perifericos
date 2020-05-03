/*
 * serial.c
 *
 * Created: 25/04/2020 13:48:49
 * Author : Silas
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

char rec_u;                        //variavel global

#define  CLR_BIT(address, bit)          (address&=~(1<<bit)) //zera bit

//********************************************************************************
// Inicializa a USART - 9600,n,8,1 a 8000000MHz
//********************************************************************************
void init_usart(void){

     UCSRB = 0x00;                                              //desabilita TX, RX para setar baud rate
     UCSRA = 0x00;                                              //limpa flag TXC, seleciona veloc. normal e desab. Multi-Proc. Com.
     UCSRC = ((1<<URSEL) |  (1<<UCSZ1) | (1<<UCSZ0));           //modo assincrono, 8 bits, sem paridade e 1 stop bit
     UBRRH = 0x00;                                              //seta baud rate a 9600 bps (8000000MHz) e U2X=0 (veloc. normal)
     UBRRL = 0x33;                                              //valor 51 decimal, conforme tabela no datasheet
     UCSRB = ((1<<RXCIE) | (1<<RXEN) | (1<<TXEN));              //transmissão e recepção habilitados, int de recepção habilitada
}
//********************************************************************************
// Envia um byte através da USART
//********************************************************************************
void uart_envia(unsigned char caracter){
     while (! (UCSRA & (1<<UDRE)));                   //verifica se está desocupado, senão espera
     UDR = caracter;                                 //transmite caracter
}
//********************************************************************************
// Recebe um dado quando vector de interrupção UART_RX_vect é disparado
//********************************************************************************
void uart_recebe(void){ 
    unsigned char c;
    c = UDR;                                //le byte
    if (CLR_BIT(UCSRA, RXC)){              //finalizou recepção
        rec_u = c;                         //guarda na variável global, declarada anteriormente
        uart_envia(c);                     //envia eco do caracter recebido
    }
}


//********************************************************************************
// Interrupção UART - RX
//********************************************************************************
ISR(USART_RXC_vect){
       uart_recebe();                        //recebe dado da UART do microcontrolador
}


int main(void)
{
	
	init_usart();
	sei();
		
    while (1) 
    {
		
    }
}

