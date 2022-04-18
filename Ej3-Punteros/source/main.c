#include <stdio.h>

int main()
{
    long int valor1 = 200000, valor2;
    //a
    long *ptrL;
    //b
    ptrL = &valor1;
    //c
    printf ("El valor al cual apunta ptrL es: %d\n", *ptrL);
    //d
    valor2 = *ptrL;
    //e
    printf ("Valor 2 es: %d\n", valor2);
    //f y g
    printf("La dirección de valor1 es %X que debería ser igual al valor de ptrL: %X\n",&valor1,ptrL);

    return 0;
}