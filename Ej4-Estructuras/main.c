#include <stdio.h>

//Definicion de la estructura
struct complejo {
    float real;
    float imaginario;
};

int main ()
{
    //declaracion de la variable x del tpo complejo e inicialización de sus miembros
    struct complejo x = { 1.3, -2.2};
    
    //declaracion de la variable puntero px del tipo complejo
    struct complejo *px;

    //referencio el puntero a la variable x
    px = &x;

    //escribo expresiones en terminos del puntero
    px->real += 10;
    px->imaginario -=5;

    printf("El numero complejo es %.2f + (%.2f) i \n",px->real,px->imaginario);

    //declaracion de un arreglo de estructuras
    struct complejo cx[100];

    //escribo expresiones para el decimoctavo elemento
    cx[17].real = 5;
    cx[17].imaginario = 3;

    printf("El decimoctavo numero complejo es %.2f + (%.2f) i \n",cx[17].real,cx[17].imaginario);

    return 0;
}