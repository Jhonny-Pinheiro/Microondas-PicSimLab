#include "config.h"
#include "pic18f4520.h"
#include "funcoes.h"

char display = 0;
char contando = 0;
int valor_contagem;
const char valores[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C};

void atualizaDisplay() {
    switch (display) {
        case 0:
            PORTA = 0;
            PORTD = valores[(valor_contagem / 1) % 10];
            PORTA |= (1 << 5);
            display = 1;
            break;
        case 1:
            PORTA = 0;
            PORTD = (valores[(valor_contagem / 10) % 10]);
            PORTA |= (1 << 4);
            display = 0;
            break;
    }
}
void iniciarContagem(int stop) {
    int k, t, aux = 0;
    int unsigned delay = 3000; //valor arbitrario (ajusta o tempo)
    long int unsigned cont = 0; //conta de 00 até stop<60 
    valor_contagem = stop;
    t = 0;
    BitSet(PORTC, 2);
    while (valor_contagem) { //conta até o valor de stop
        atualizaDisplay();
        if(t%40 == 0 && valor_contagem > 0) {
            valor_contagem--;
        }
        t++;
        for (k = 0; k < delay; k++);
    }
    BitClr(PORTC, 2);
    BitClr(PORTA, 5);
    BitClr(PORTA, 4); 
    PORTD = 0X00;
}