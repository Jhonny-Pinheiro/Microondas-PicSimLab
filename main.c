#include "config.h"
#include "pic18f4520.h"
#include "delay.h"
#include "lcd.h"
#include "teclado.h"
#include "funcoes.h"

unsigned int atrasoMin = 20;
unsigned int atrasoMed = 500;
unsigned int atrasoMax = 1000;

void main() {
    for(;;){
    //---------------------------------------------------------------
    //Configurações iniciais do programa:
    unsigned char i;
    int k;
    unsigned char tmp;
    char indice = 0; 
    int stop = 0;

    ADCON1 = 0x06;
    TRISB = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;

    lcd_init();

    //Teclado numérico
    TRISB = 0xF8;
    
    
    //---------------------------------------------------------------
    //Tela inical: quando o usuario entra com o tempo desejado, esse
    //             tempo deve ser entre 1 e 59 segundos.
    //Após digitar o tempo, deve apertar '#' para inicar o processo.
    //Caso o usuário apertar '*', executa a função descongelar, com 
    //             tempo programado de 30 segundos
    
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Tempo de Preparo");
    lcd_cmd(L_L2);
    
    
    while (indice < 3) {
        TRISD = 0x0F;
        tmp = tc_tecla(0);
        TRISD = 0x00;
        if(tmp == 0x0A)
        {
            stop = 30;
            lcd_dat('3');
            lcd_dat('0');
            indice = 2;
        }
        if(indice == 0)
        {
            if(tmp >= 0 && tmp < 6)
            {
                lcd_dat(tmp+0x30);
                stop += tmp*10;
                indice = 1;
            }
        }else if(indice == 1)
        {
            if(tmp >=0 && tmp <= 9)
            {
                if(tmp == 0 && stop < 10) {
                    stop = stop;
                }else {
                    stop += tmp;
                    lcd_dat(tmp+0x30);
                    indice = 2;
                }
            }
        }else if(indice==2) {
            if (tmp == 0x0C) {
                indice = 3;
            }
        }
    }
    
    //-----------------------------------------------------------------    
    //Limpa a tela do LCD e escreve "Em funcionamento"
    
    lcd_cmd(L_CLR); //apaga 1a linha
    lcd_cmd(L_L1); 
    lcd_cmd(L_CLR); //apaga 2a linha
    lcd_cmd(L_L2);
    lcd_cmd(L_L1);
    lcd_str("Em funcionamento"); //aviso de funcionamento
    TRISA = 0x00; //config da porta A 

    BitClr(INTCON2, 7); //liga pull up 
    ADCON1 = 0x0E; //config AD 
    TRISD = 0x00; //config. a porta D 
    PORTD = 0x00;
    TRISC = 0x00;
    iniciarContagem(stop); 
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("    Pronto");
    BitSet(PORTC,1);
    for(i = 0; i < 9; i++)
    {
        PORTD = 0xFF;
        for(k = 0; k < 20000; k++);
        PORTD = 0x00;
        for(k = 0; k < 20000; k++);
    }
    BitClr(PORTC,1);
    for(k = 0; k < 25000; k++);
}
}