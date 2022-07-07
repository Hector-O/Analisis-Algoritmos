#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int** asignaMemMatriz(int**, int, int);
void muestraMatriz(int**, int, int);
void llenaMatriz(int**, int, int);

int
main()
{
	FILE* arc1;
	FILE* arc2;
	unsigned char caract1;
	unsigned char caract2;
	unsigned char *buffer1;
	unsigned char *buffer2;
	unsigned char *buffer_final;
	int coincide = 0;
	int total = 0;
	int tam1=0;
	int tam2=0;
	int i=0, j=0;
	int final=0;
	int entero;
	int **tabla_cuenta;
	int **tabla_flecha;
	struct stat estado1;
	struct stat estado2;

	arc1 = fopen("codigo1.c", "r");
	arc2 = fopen("codigo2.c", "r");
	if ( arc1 == NULL )
	{
		perror("Error al abrir el archivo 1");
	}
	if ( arc2 == NULL )
	{
		perror("Error al abrir el archivo 2");
	}
	
	entero = stat("codigo1.c", &estado1);
	entero = stat("codigo2.c", &estado2);
	tam1 = estado1.st_size;
	tam2 = estado2.st_size;
	
	printf("tam1:%d\ttam2:%d\n\n",tam1,tam2);
	
	
	buffer1=(unsigned char*)malloc(sizeof(char)*tam1);
	buffer2=(unsigned char*)malloc(sizeof(char)*tam2);
	buffer_final=(unsigned char*)malloc(sizeof(char)*tam2);
	
	i=0;
	while( !feof(arc1))
	{	
		buffer1[i] = fgetc(arc1);
		//printf("%c", buffer1[i]);
		i++;
	}
	buffer1[i] = '\0';
	
	i=0;
	while( !feof(arc2) )
	{
		buffer2[i] = fgetc(arc2);
		i++;
	}
	buffer2[i] = '\0';
	
	tam1=strlen(buffer1)+1;//columnas
	tam2=strlen(buffer2)+1;//filas
	
	tabla_cuenta = asignaMemMatriz(tabla_cuenta,tam1,tam2);
	tabla_flecha = asignaMemMatriz(tabla_flecha,tam1,tam2);
	
	puts("");
	printf("La INICIAL de flechas, tam1:%d   tam2:%d\n", tam1, tam2);
	muestraMatriz(tabla_flecha, tam1, tam2);
	puts("");
	
	for(i=0; i<tam1; i++)
	{
		tabla_cuenta[0][i]=0;
		tabla_flecha[0][i]=0;
	}
	for(i=0; i<tam2; i++)
	{
		tabla_cuenta[i][0]=0;
		tabla_flecha[i][0]=0;
	}
	
	for(i=1; i<tam2; i++)
	{
		for(j=1; j<tam1; j++)
		{
			if( buffer1[i-1]==buffer2[j-1] )
			{
				tabla_cuenta[i][j] = tabla_cuenta[i-1][j-1]+1;
				tabla_flecha[i][j] = 2; //2 es usado como flecha diagonal
			}
			else if( tabla_cuenta[i-1][j] >= tabla_cuenta[i][j-1] )
			{
				tabla_cuenta[i][j] = tabla_cuenta[i-1][j];
				tabla_flecha[i][j] = 3; //3 es usado como flecha vertical
			}
			else
			{
				tabla_cuenta[i][j] = tabla_cuenta[i][j-1];
				tabla_flecha[i][j] = 1; //1 es usado como flecha horizontal
			}
		}
	}
	
	puts("");
	printf("La INTERMEDIA de flechas, tam1:%d   tam2:%d\n", tam1, tam2);
	muestraMatriz(tabla_flecha, tam1, tam2);
	puts("");
	
	
	int count=0;
	for(i=tam2-1; i>0; i--)
	{
		for(j=tam1-1; j>0; j--)
		{
			if( tabla_flecha[i][j] == 1 )//si la flecha es horizontal
			{
				j--;
			}
			if( tabla_flecha[i][j] == 2 )//si la flecha es diagonal
			{
				buffer_final[count]=buffer1[i];
				i--;
				j--;
				count++;
			}
			if( tabla_flecha[i][j] == 3 )//si la flecha es vertical
			{
				i--;
			}
		}
	}
	buffer_final[count]='\0';
	
	
	printf("Longitud:%d\n", strlen(buffer_final));
	for(i=0; i<count; i++)
	{
		printf("%c", buffer_final[i]);
	}
	
	puts("");
	puts("");
	printf("La FINAL de flechas, tam1:%d   tam2:%d\n", tam1, tam2);
	muestraMatriz(tabla_flecha, tam1, tam2);
	puts("");
	
	printf("\nEl primero tiene: \n");
	for(i=0; i<tam1; i++)
	{
		printf("%c", buffer1[i]);
	}
	printf("\n\n\n");
	
	printf("El segundo tiene: \n");
	for(i=0; i<tam2; i++)
	{
		printf("%c", buffer2[i]);
	}
	printf("\n");
	
	return 0;
}

int**
asignaMemMatriz(int** matriz, int columnas, int filas)
{
	int i = 0;
	matriz = (int**)malloc(filas*sizeof(int*));
	if( matriz == NULL )
	{
		perror("Error al asignar memoria de filas");
		exit(0);
	}
	for(i=0 ; i<filas ;i++)
	{
		matriz[i]=(int*)malloc(columnas*sizeof(int));
		if( matriz == NULL)
		{
			perror("Error al asignar memoria de columnas");
		}
	}
	return matriz;
}

void
muestraMatriz(int**matriz, int columnas, int filas)
{
	int i,j;
	for(i=0; i<filas;i++)		
	{
		for(j=0; j<columnas;j++)
		{
			printf("[%d] ", matriz[i][j]);
		}
		printf("\n");
	}
}

void
llenaMatriz(int**matriz, int columnas, int filas)
{
	int i,j;
	for(i=0; i<filas;i++)		
	{
		for(j=0; j<columnas;j++)
		{
			matriz[i][j]=-1;
		}
	}
}
