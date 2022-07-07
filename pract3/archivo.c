#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

unsigned char ** asignaMemMatrizCuadrada(unsigned char**, int);
void muestraMatriz(unsigned char**, int);
void llenaMatriz(unsigned char**, int);
void cerosMatriz(unsigned char**, int);

int
main()
{   
    FILE * archvLec;
    FILE* archvEsc;
    int tam, tam2;//ancho y alto de la imagen
    int direccion;//direccion donde se guardan los datos de las matrices
    int leidos = 0;
    int i=0, j=0;;
    
	unsigned char *ap;
    unsigned char buffer[30];
	unsigned char temporal;

	//abre el archivo
    archvLec = fopen("ejemplo2.bmp", "rb");
    if( archvLec == NULL )
    {
        perror("Error: no se abrio el archivo\n");
        exit (0);
    }

    //lee el header del archivo bmp
    for(;i<30;i++)
    {
    	buffer[i]= fgetc(archvLec);//lee byte por byte
    	//guarda la direccion de la matriz
    	if( i>=10 && i<=13 )
    	{
    		ap=(unsigned char *)&direccion;
    		ap[i-10]=buffer[i];	
		}
    	
    	//guarda el tam (ancho) de la matriz
    	if( i>=18 && i<=21 )
    	{
    		ap=(unsigned char *)&tam;		
    		ap[i-18]=buffer[i];	
		}
		//guarda el tam (alto) de la matriz
    	if( i>=22 && i<=25 )
    	{
    		ap=(unsigned char *)&tam2;		
    		ap[i-22]=buffer[i];	
		}
	}
	printf("La anchura es:%d Y la altura es:%d", tam, tam2);
    
    //comprobaciones de tamanio de la imagen
    if( tam != tam2)//comprueba que es cuadrada
    {
    	printf("Error ingrese una imagen de resolucion cuadrada");
    	exit(0);
	}
	for(i=0; i<34;i++)//comprueba que sea una potencia de 2, menor a 2^32
	{
		if( (double)tam == pow((double)2, (double)i) )
		{
			break;
		}
		if( i == 33 )
		{
			printf("Tu archivo no es potencia de 2");
			exit(0);
		}
	}
	printf("Archivo aceptado. Prosiguiendo\n");
	

	//creacion de la matriz
	unsigned char **matrizB;//B
	unsigned char **matrizG;//G
	unsigned char **matrizR;//R
	matrizB = asignaMemMatrizCuadrada(matrizB, tam);
	matrizG = asignaMemMatrizCuadrada(matrizG, tam);
	matrizR = asignaMemMatrizCuadrada(matrizR, tam);

    
	rewind(archvLec);//regresa al inicio del archivo
    
    //for que mueve al comienzo de los datos de las matrices para despues leer de las matrices
    for(i=0; i<direccion; i++)
	{
    	temporal=fgetc(archvLec);
	}
	
	//empieza a guardar los datos en las matrices en memoria
	for(i=0; i<tam; i++)//filas
	{
		for(j=0; j<tam; j++)//columnas
		{
			matrizB[i][j]=fgetc(archvLec);
			matrizG[i][j]=fgetc(archvLec);
			matrizR[i][j]=fgetc(archvLec);
			temporal=fgetc(archvLec);//lee basura (0xff)
		}
	}
	
	/* Evitar usar si el tamanio es de 512,1024,2048,... se empieza a tardar bastante
	//muestra las matrices obtenidas
    puts("la matriz B:");
    muestraMatriz(matrizB, tam);
	puts("la matriz G:");
    muestraMatriz(matrizG, tam);
    puts("la matriz R:");
    muestraMatriz(matrizR, tam);
    */
    
    //hace ceros la matrices no deseada para dejar un colo primario
    cerosMatriz(matrizB, tam);//elimina color blue
    //cerosMatriz(matrizG, tam);//elimina color green
    cerosMatriz(matrizR, tam);//elimina color red
    
    
//PSEUDOCÓDIGO funcion que gira la matriz....................................................................    
    func(matriz, ancho){
		if( ancho == 2 ){
    		aux = matriz[0][0];
    		matriz[0][0] = matriz[0][1];
			matriz[0][1] = matriz[1][1];
			matriz[1][1] = matriz[1][0];
			matriz[1][0] = aux;
			return 
		}
		else
		{
			func(matriz[0][0], ancho/2)
			func(matriz[ancho/2][0], ancho/2)
			func(matriz[0][alto/2], alto/2)
			func(matriz[ancho/2] [alto/2], alto/2)
		
			matriz1=matriz[0][0]
			matriz2=matriz[ancho/2][0]
			matriz3=matriz[0][alto/2]
			matriz4=matriz[ancho/2][alto/2]
			
			aux = creaMatriz(ancho)
			aux = guarda(matriz1, ancho)
			mueve(matriz1, matriz2, ancho)
			mueve(matriz2, matriz4, ancho)
			mueve(matriz3, aux, ancho)
			free (aux)	
		}
	}
}

mueve(matrizX, matrizY, ancho)
{
	for(i=0; i<ancho; i++)
	{
		for(j=0; j<ancho; j++)
		{
				matrizX[i][j]=matrizY[i][j]
		}
	}
}

    /*generacion el nuevo archivo*/
    rewind(archvLec);//regresa al inicio del archivo
    archvEsc = fopen("nuevo.bmp", "wb");
	
	//lee la cabecera vieja y escribe la nueva
	int escritos;
    for(i=0; i<direccion; i++)
	{
    	temporal=fgetc(archvLec);//lee del archivo de entrada byte a byte
    	escritos = fwrite(&temporal, sizeof(unsigned char), 1, archvEsc);//escribe en el archivo byte por byte conforme lee
	}
	temporal=0xff;//variable 0xff que separa los datos de las matrices del archivo bmp
	for(i=0; i<tam; i++)//filas
	{
		for(j=0; j<tam; j++)//columnas
		{
			escritos=fwrite(&matrizB[i][j], sizeof(char), 1, archvEsc);//escribe el dato de la matriz Blue
			escritos=fwrite(&matrizG[i][j], sizeof(char), 1, archvEsc);//escribe el dato de la matriz Green
			escritos=fwrite(&matrizR[i][j], sizeof(char), 1, archvEsc);//escribe el dato de la matriz Red
			escritos=fwrite(&temporal, sizeof(char), 1, archvEsc);//escribe la constante 0xff
		}
	}
    


	//libera la memoria de la matrices usadas
	for(i=0; i<tam; i++)
	{
		free(matrizB[i]);
		free(matrizG[i]);
		free(matrizR[i]);
	}
	free(matrizB);
	free(matrizG);
	free(matrizR);	
	
	//termina y cierra los archivos
	fclose(archvEsc);
	fclose(archvLec);
 	return 0;
}

unsigned char** asignaMemMatrizCuadrada(unsigned char** matriz, int tam)
{
	int i = 0;
	matriz = (unsigned char **)malloc(tam*sizeof(unsigned char*));
	if( matriz == NULL )
	{
		perror("Error al asignar memoria de filas");
		exit(0);
	}
	for(i=0 ; i<tam ;i++)
	{
		matriz[i]=(unsigned char*)malloc(tam*sizeof(unsigned char));
		if( matriz == NULL)
		{
			perror("Error al asignar memoria de columnas");
		}
	}
	return matriz;
}
void llenaMatriz(unsigned char**matriz, int tam)
{
	int i,j;
	unsigned char cont=0;
	for(i=0; i<tam;i++)
	{
		for(j=0; j<tam;j++)
		{
			cont++;
			matriz[i][j]=cont++;
		}
	}
}

void muestraMatriz(unsigned char**matriz, int tam)
{
	int i,j;
	for(i=0; i<tam;i++)
	{
		for(j=0; j<tam;j++)
		{
			printf("[%.2x] ", matriz[i][j]);
		}
		puts("");
	}
}

void cerosMatriz(unsigned char**matriz, int tam)
{
	int i,j;
	for(i=0; i<tam;i++)
	{
		for(j=0; j<tam;j++)
		{
			matriz[i][j]=0;
		}
	}
}

int potencia(int base, int potencia)
{
	int aux=base;
	int i;
	for(i=0; i<potencia; i++)
	{
		aux=aux*base;
	}
	return aux;
}
