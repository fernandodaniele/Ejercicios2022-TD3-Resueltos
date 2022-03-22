#include <stdio.h>

int main()
{
    int i=0;
    int valor;
    float promedio;
    int suma = 0;
    printf("Ingrese los valores para calcular el promedio. Para finalizar ingrese 9999\n");
    while(1)
    {
        scanf("%d",&valor);
        if(valor== 9999)
        {
            promedio = suma / i;
            printf("%.2f",promedio);
            break;
        }
        suma = suma + valor;
        i++;
    }
    return 0;
}