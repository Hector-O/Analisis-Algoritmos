#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned char** asignaMemMatriz(unsigned char** matriz, int ancho, int alto);

int
main()
{
	/*Declaración de variables*/
	int r=0;//dias que dura el fertilizante
	int dias_abiertos=0;
	int* conjunto_C;
	int* conjunto_S;
	int tam_S=0;
	int i=0;//variable para iterar
	int f=0;//dia en que se acabara el fertilizante
	int aux=0;
	int bandera=1;
	int contador=0;
	
	
	
	
	
	//Adquisición de datos desde consola
	while( r == 0 )
	{
		printf("introuduzca (r) el no. de dias en que el granjero tiene fertilizante\n");
		scanf("%d", &r);
		fflush(stdin);	
	}
	
	while( dias_abiertos == 0 )
	{
		printf("introuduzca el no. de dias en que abre la tienda\n");
		scanf("%d", &dias_abiertos);
		fflush(stdin);
	}
	
	conjunto_S = (int *)malloc(sizeof(int)*dias_abiertos);//sera igual o menos al conjunto C
	conjunto_C = (int *)malloc(sizeof(int)*dias_abiertos);
	
	printf("introuduzca cada uno de los dias en que abre la tienda en orden ascendente (Incluya el 0)\n");
	for(i=0; i<dias_abiertos; i++)
	{
		printf("Dia no. %d: ", i+1);
		scanf("%d", &conjunto_C[i]);
		if( i != 0 )
		{
			while( conjunto_C[i] == 0 )
			{
				printf("Error, intruduza un valor distinto de 0: ");
				scanf("%d", &conjunto_C[i]);
				fflush(stdin);
			}
			if( conjunto_C[i-1] >= conjunto_C[i] )
			{
				printf("Error, intruduza un valor mayor al dia anterior: ");
				scanf("%d", &conjunto_C[i]);
				fflush(stdin);
			}
		}
		
		fflush(stdin);
		printf("\n");
	}
	
	printf("\nLa entrada: \n");
	for(i=0; i<dias_abiertos; i++)
	{
		printf("[%d] ", conjunto_C[i]);
	}
	
	/*Algoritmo Greedy*/
	f = 0 + r;	
	contador=0;
	i=0;
	while( i < dias_abiertos )
	{	
		if( conjunto_C[i] < f )//si el dia que abre es menor al dia que se acaba guarda ese dia
		{
			aux = conjunto_C[i];
			if( i == 0)
			{
				conjunto_S[contador] = aux ;
				contador++;
			}
		}
		else{
			conjunto_S[contador]=aux;
			contador++;
			
			f=0;
			f = aux + r;
		}
		i++;
	};
	conjunto_S[contador]=aux;
	contador++;
	
	printf("\nEl conjunto salida es: \n");
	for(i=0; i<contador; i++)
	{
		printf("[%d] ", conjunto_S[i]);
	}
	
	return 0;
}





unsigned char**
asignaMemMatriz(unsigned char** matriz, int ancho, int alto)
{
	int i = 0;
	matriz = (unsigned char **)malloc(alto*sizeof(unsigned char*));
	if( matriz == NULL )
	{
		perror("Error al asignar memoria de filas");
		exit(0);
	}
	for(i=0 ; i<alto ;i++)
	{
		matriz[i]=(unsigned char*)malloc(ancho*sizeof(unsigned char));
		if( matriz == NULL)
		{
			perror("Error al asignar memoria de columnas");
		}
	}
	return matriz;
}
