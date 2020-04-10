/*
 * Display_lcd.c
 *
 * Created: 10/04/2020 16:10:08
 * Author : Silas
 *
 * Interface de dados de 4 bits para um display de caracter 20x4 e equivalentes
 *Endereços
 *0x80 -> primeira linha
 *0xC0 -> segunda  linha
 *0x94 -> terceira linha
 *0xD4 -> quarta   linha
 */ 


#define  F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

//Definicoes de macros
#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))    // seta o bit da porta
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT)) //zera o bit da porta
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))   //complemeta valor do bit
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))   //leitura (PINx,pxx)

#define DADOS_lCD	PORTD            //defina portx como porta de barramentos
#define RS          PD2              //pino de instrucoes ou dados para lcd
#define E           PD3              //pino enable lcd

unsigned char i=1;                   //variavel para inicializar lcd

//--------------------------------------------------------------------------------------------
//Função para enviar comandos ao lcd com dados de 4 bits
//-------------------------------------------------------------------------------------------
void cmd_lcd(unsigned char c, char cd)
{
	DADOS_lCD =c; //Primeiro os 4 bits MSB depois os LSB
	
	for (;i!=0;i--)
	{
		if (cd==0)
				CLEARBIT(PORTD,RS);
		else	
				SETBIT(PORTD,RS);
		
		SETBIT(PORTD,E);
		CLEARBIT(PORTD,E);
		_delay_us(45);
		
		if((cd==0)&&(c<127))  //se for instrução espera tempo de resposta do display
				_delay_ms(2);
		DADOS_lCD=c<<4;
	}
	i=2;	
}
//---------------------------------------------------------------------------------------------
//Função de inicialização no lcd
//---------------------------------------------------------------------------------------------------
void inicializa_lcd(void)
{
	cmd_lcd(0x28,0);   //interface de 4 bits
	cmd_lcd(0x28,0);   //interface de 4 bits 2 ou mais linhas 
	cmd_lcd(0x0C,0);   //mensagem aparente cursor ativo piscando
	cmd_lcd(0x01,0);   //limpa display
	cmd_lcd(0x80,0);   // curso na primeira linha primeiro caracter
}

//-------------------------------------------------------------------------------------------------------
//Função para escrever no lcd
//----------------------------------------------------------------------------------------------
void escreve_lcd(char *c)
{
	for (;*c!=0;c++) cmd_lcd(*c,1);
}
//--------------------------------------------------------------------------------------------------------

int main(void)
{
    DDRD = 0xFF; 
	inicializa_lcd();
	escreve_lcd("Interface de");
	cmd_lcd(0xC0,0);
	escreve_lcd("dados de 4 bits");
	cmd_lcd(0x94,0);
	escreve_lcd("funciona");
	cmd_lcd(0xD4,0);
	escreve_lcd("ok");
	
	
    while (1) 
    {
		
		//cmd_lcd(0x01,0);   //limpa display
		
		
    }
}

