#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* archivo();
int obten_no_nodos(FILE*);
char* obten_nodos(FILE*, int);
void lee_matriz(FILE*, int**, int);

void pregunta_ruta(char*, int);
int esCicloHamilton(int**, char*, char*, int);
int regresaIndice(char*, char, int);

int** asignaMemMatriz(int ancho, int alto);
void muestraMatriz(int**, int, int);
void llenaMatriz(int**, int, int, int);

int contador=0;

int
main()
{
    FILE *fd;
    int no_nodos;
    int **m_adyacencia;
	char *nodos;
	char* ruta;
	ruta = (char*)malloc(sizeof(char)*100);
	
    
    fd=archivo();
    no_nodos = obten_no_nodos(fd);
	nodos =obten_nodos(fd, no_nodos);
	
	printf("\n\nMatriz\n");
	m_adyacencia = asignaMemMatriz(no_nodos, no_nodos);
	lee_matriz(fd, m_adyacencia, no_nodos);
	muestraMatriz(m_adyacencia, no_nodos, no_nodos);
	
	pregunta_ruta(ruta, no_nodos);
	
	int comprueba = esCicloHamilton(m_adyacencia, ruta, nodos, no_nodos);
	
    return 0;
}

FILE* 
archivo()
{
	FILE* fd;
	char archivo[100], num;
	printf("¿Que ejemplo desea usar?\n1) Ejemplo 1\n2) Ejemplo 2\n3) Otro ejemplo\n");

	scanf("%c", &num);
	printf("\n");

	switch(num){
		case'1':fd = fopen("file1.txt", "r");
				break;
		case'2':fd = fopen("file2.txt", "r");
				break;
		case'3':printf("Teclee el nombre del archivo en formato .txt y con la estructura adecuada\n"); 
				scanf("%s", archivo);
				printf("\n");
				fd = fopen(archivo, "r");
				break;
	}
	if(fd==NULL)
	{
		perror("Hubo un error al abrir el archivo");
		exit (0);
	}
	return fd;
}

int
obten_no_nodos(FILE* fd)
{
    unsigned char cantidad;
    cantidad = fgetc(fd);
    return atoi(&cantidad);
}

char*
obten_nodos(FILE* fd, int total)
{
	char* nodos;
	char temp;
	int cuenta=0;
	
	nodos = (char*)malloc(sizeof(char)*total*2);
	do
	{
		temp=fgetc(fd);
		if ( temp != ' ' && temp != ',' )
		{
			nodos[cuenta]=temp;
			cuenta++;
		}
	} while ( temp != '\n' );
	nodos[cuenta]='\0';
	return nodos;
}

void
lee_matriz(FILE* fd, int** matriz, int tam)
{
	unsigned char temp=' ';
	int i=0;
	int j=0;
	
	while( temp != '-' )
	{
		temp=fgetc(fd);
		
		if( temp == '1' || temp == '0' )
		{
			if ( j == tam )
			{
				j=0;
				i++;
			}
			if ( temp == '\n' )
			{
				j++;
			}
			matriz[i][j]=atoi(&temp);	
			j++;
		}
	}
}

void
pregunta_ruta(char * ruta, int tam)
{
	printf("Introduzca la ruta a comprobar, sin comas ni espacio:\n");
	scanf("%s", ruta);
	return;
}

int
esCicloHamilton(int** m_adyacencia, char* ruta, char* nodos, int total_nodos)
{
	int a=strlen(ruta);
	if( total_nodos != a-1 )
	{
		printf("No es un ciclo Hamiltoniano: La longitud de la ruta no permite que sea Hamiltoniano\n");
		exit(0);
	}
	if( ruta[0] != ruta[total_nodos] )
	{
		printf("No es un ciclo Hamiltoniano: no es un ruta ciclica\n");
		return -1;
	}

	int i;
	char aux1, aux2;
	char* nodos_visitados;
	nodos_visitados = (char*)malloc(sizeof(char)*total_nodos);
	for(i=0; i< total_nodos; i++)//llena con 0 el arreglo de nodos visitados
	{
		nodos_visitados[i]=0;
	}
	i=0;
	while( i<total_nodos )
	{
		aux1=ruta[i];
		aux2=ruta[i+1];
		
		if ( m_adyacencia[regresaIndice(nodos, aux1, total_nodos)][regresaIndice(nodos, aux2, total_nodos)] == 1 )//pregunta si existe la vertice
		{
			if( nodos_visitados[regresaIndice(nodos, aux1, total_nodos)] == 0 )//pregunta si nodos visitados ha sido visitado
			{
				nodos_visitados[regresaIndice(nodos, aux1, total_nodos)]=1;
			}
			else
			{
				printf("No es un ciclo Hamiltoniano: pasa por un nodo mas de una vez\n");
				return -1;
			}
		}
		else
			{
				printf("No es un ciclo Hamiltoniano: no existe una vertice de la ruta\n");
				return -1;
			}	
		i++;
	};
	
	for(i=0; i< total_nodos; i++)//revisa el arreglo de nodos visitados
	{
		if (nodos_visitados[i])
		{
			continue;
		}
		else
		{
			printf("No es un ciclo Hamiltoniano: no se han visitado todos los nodos\n");
			return -1;
		}
	}
	printf("Es un ciclo Hamiltoniano\n");
	return 1;
}

int
regresaIndice(char* nodos, char caracter, int total_nodos)
{
	int i;
	for(i=0; i<total_nodos; i++)
	{
		if(nodos[i]==caracter)
		{
			return i;
		}
	}
	printf("No existe ese nodo(%c) en el grafo\n", caracter);
	return -1;
}

int**
asignaMemMatriz(int columnas, int filas)
{
	int **matriz;
	int i = 0;
	matriz = (int**)malloc(filas*sizeof(int*));
	if( matriz == NULL )
	{
		perror("Error al asignar memoria de las filas");
		exit(0);
	}
	for(i=0 ; i<filas ;i++)
	{
		matriz[i]=(int*)malloc(columnas*sizeof(int));
		if( matriz == NULL)
		{
			perror("Error al asignar memoria de las columnas");
			exit(0);
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
llenaMatriz(int**matriz, int columnas, int filas, int valor)
{
	int i,j;
	for(i=0; i<filas;i++)		
	{
		for(j=0; j<columnas;j++)
		{
			matriz[i][j]=valor;
		}
	}
}
