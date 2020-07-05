/*
 * adc.c
 *
 * Created: 25/05/2020 12:39:31
 * Author : Silas
 
  O AVR ADC deve ter um clock na frequência entre 50 e 200kHz.
  Portanto, precisamos definir os bits de prescaller 
  para que o clock do ADC caiba nesse intervalo
  divida o clock pelo prescaller e obtenha o resultado
  EX: 16000000/128=125khz de clock do conversor ADC,que esta dentro da faixa.
 */ 

#define F_CPU 800000UL

#include <avr/io.h>
#include <util/delay.h>

//---------------------------------------------------------------------------------------------
//Função de inicialização do ADC
//---------------------------------------------------------------------------------------------------
void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	// ADC Enable and prescaler of 128
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
}
//---------------------------------------------------------------------------------------------
//Função de leitura do ADC em 10 bits
//---------------------------------------------------------------------------------------------------
uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	// wait for conversion to complete
	// ADSC becomes '0' again
	while(ADCSRA & (1<<ADSC));
	return (ADC);
}


int main(void)
{
	 
	/*
	 Conversor ADC Um pequeno exemplo usando o sensor LM35 , como inicializar,  ler 
	 e posteriomente  enviar esses dados via serial.
	 */
   char buffer[10];
   int temp1;

   
   
    //initialize adc
   adc_init();
   _delay_ms(20);

    while (1) 
    {
	     
	     temp1=((adc_read(0)*5.0)/1024)*100;   // le o valor de PA0 e calcula a temperatura /exemplo lm35 
		 snprintf(buffer,10,"temp1.%d",temp1); //converte em string e concatena e coloca o resultado no buffer, a saida sera temp1.valor  podendo ser enviado via serial.	
		 
	 _delay_ms(1000);
		
    }
}

