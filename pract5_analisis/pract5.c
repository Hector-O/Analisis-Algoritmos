#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct datos
{
	int color;
	int cantidad;
};

struct arbol
{
  struct datos data;
  struct arbol *izq;
  struct arbol *der;
};

struct cola
{
  struct arbol nodo;
  struct cola *siguiente;
  struct cola *anterior;
};

struct arbol *insertarArbol (struct arbol *raiz, struct arbol*, struct arbol*);
void mostrarArbolPRE (struct arbol *);
void mostrarArbolIN (struct arbol *);
void mostrarArbolPOST (struct arbol *);
struct arbol *buscarArbol (struct arbol *, struct datos);
int esHoja (struct arbol *);
struct arbol *padreDe (struct arbol *, struct datos);
struct arbol *mayorDe (struct arbol *);
struct arbol *menorDe (struct arbol *);
struct arbol *eliminarRaizDeArbol (struct arbol *);
struct arbol *eliminarDeArbol (struct arbol *, struct datos);

void copiaNodoArbol(struct arbol*, struct arbol*);
struct cola* huffman(struct cola*);
struct cola* creaPadreDeDos(struct cola*, struct cola*);


void quickSort(struct datos *, int, int);

unsigned char ** asignaMemMatriz(unsigned char**, int, int);
void muestraMatriz(unsigned char**, int, int);
void llenaMatriz(unsigned char**, int);

struct cola *insertarFila (struct cola *Fila, int cantidad, int color);
struct cola *insertaPrioridad(struct cola *, struct cola *);
void mostrarFila (struct cola *);
struct cola *quitarFila (struct cola *);
struct cola* eliminaPrimeroCola(struct cola*);

int
main()
{   
    FILE * archvLec;
    FILE* archvEsc;
    int ancho, alto;//ancho y alto de la imagen
    int direccion;//direccion donde se guardan los datos de las matrices
    int leidos = 0;
    int i=0, j=0;;
    
	unsigned char *ap;
    unsigned char buffer[30];
	unsigned char temporal;

	//abre el archivo
    archvLec = fopen("gris.bmp", "rb");
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
    	
    	//guarda el ancho de la matriz
    	if( i>=18 && i<=21 )
    	{
    		ap=(unsigned char *)&ancho;		
    		ap[i-18]=buffer[i];	
		}
		//guarda el alto de la matriz
    	if( i>=22 && i<=25 )
    	{
    		ap=(unsigned char *)&alto;		
    		ap[i-22]=buffer[i];	
		}
	}
	printf("La anchura es:%d Y la altura es:%d", ancho, alto);

	//creacion de la matriz
	unsigned char **matrizB;//B
	unsigned char **matrizG;//G
	unsigned char **matrizR;//R
	matrizB = asignaMemMatriz(matrizB, ancho, alto);
	matrizG = asignaMemMatriz(matrizG, ancho, alto);
	matrizR = asignaMemMatriz(matrizR, ancho, alto);
    
    
	rewind(archvLec);//regresa al inicio del archivo
    
    //for que mueve al comienzo de los datos de las matrices para despues leer de las matrices
    for(i=0; i<direccion; i++)
	{
    	temporal=fgetc(archvLec);
	}
	
	//empieza a guardar los datos en las matrices en memoria
	for(i=0; i<alto; i++)//filas
	{
		for(j=0; j<ancho; j++)//columnas
		{
			matrizB[i][j]=fgetc(archvLec);
			matrizG[i][j]=fgetc(archvLec);
			matrizR[i][j]=fgetc(archvLec);
			temporal=fgetc(archvLec);//lee basura (0xff)
		}
	}
	
    //
    int frecuencia[256];
    int inv_frec[256];
    int maximo=0;
    
    /*Para el calculo de las frecuencias*/
    for(i=0; i<256; i++)
    {
    	frecuencia[i]=0;
    	inv_frec[i]=255;
	}
    
    /*Se obtiene la frecuencia y el valor maximo*/
    for(i=0; i<alto; i++)//filas
	{
		for(j=0; j<ancho; j++)//columnas
		{
			frecuencia[matrizB[i][j]]++;
			if ( frecuencia[matrizB[i][j]] > maximo )
			{
				maximo = frecuencia[matrizB[i][j]];
			}
		}
	}
	
	/*Impresi�n del histrograma en un archivo txt*/
	FILE * arcHisto;
	arcHisto = fopen("histograma.txt", "w+");
	
    for(i=0; i<256;i++)//for que pasa a porcentaje el histograma
	{
		inv_frec[i]=maximo-frecuencia[i];
		inv_frec[i]=(100*inv_frec[i])/maximo;
	}
	puts(" ");
	
	//Imprime el histograma en el archivo
	for(i=0; i<100; i++)//recorre cada filas
	{
		for(j=0; j<256; j++)//recorre entre filas
		{
			if( inv_frec[j] > 0 )//si hay valores en inv_frecuencia
			{
				fputc(' ', arcHisto);
				inv_frec[j]--;
			}//cuando no hay valores en inv_frecuencia
			else if( inv_frec[j] == 0 ){
				fputc('#', arcHisto);
			}
		}
		fputc('\n', arcHisto);//salto de linea
	}
	fclose(arcHisto);//cierra el archivo
	
	/*Para la codificacion de Huffman*/
	/*
		Se ordenan las fecuencias de menos a mayor
		Creamos el arbol segun el balanceo
	*/
	
	/*printf("\n\n\nImpresion del arreglo original");
	for(i=0; i<256; i++)
	{
		printf("Color: %d\t frecuencia:%d\n", i, frecuencia[i]);
	}
	printf("\n\n\n");*/
	
	/*Ordenamos las frecuencias con sus respectivos colores*/
	struct datos *arregloData;
	int n = 256;
	arregloData=(struct datos *)malloc(sizeof(struct datos)*n);
	
	for(i=0; i<n; i++)
	{
		arregloData[i].cantidad = frecuencia[i];
		arregloData[i].color = i;
	}
	quickSort(arregloData, 0, n-1);
	
	/*Creamos una cola de nodos del arbol*/
	struct cola* miFila;
	miFila=NULL;
	
	for(i=0; i<n; i++)
	{
		miFila = insertarFila (miFila, arregloData[i].cantidad, arregloData[i].color);
	}
	
	printf("\n\n ****************** Mi FILA es ****************** \n\n");
	mostrarFila (miFila);
	
	
	struct cola* prueba;
	prueba=(struct cola*)malloc(sizeof(struct cola));
	struct cola* prueba2;
	prueba2=(struct cola*)malloc(sizeof(struct cola));
	struct cola* prueba3;
	prueba3=(struct cola*)malloc(sizeof(struct cola));
	
	prueba->nodo.data.cantidad = 1;
	prueba->nodo.data.color = 1;
	prueba->nodo.der = NULL;
	prueba->nodo.izq = NULL;
	prueba->anterior = prueba3;
	prueba->siguiente = prueba2;
	
	prueba2->nodo.data.cantidad = 2;
	prueba2->nodo.data.color = 2;
	prueba2->nodo.der = NULL;
	prueba2->nodo.izq = NULL;
	prueba2->anterior = prueba;
	prueba2->siguiente = prueba3;
	
	prueba3->nodo.data.cantidad = 3;
	prueba3->nodo.data.color = 3;
	prueba3->nodo.der = NULL;
	prueba3->nodo.izq = NULL;
	prueba3->anterior = prueba2;
	prueba3->siguiente = prueba;
	mostrarFila (prueba);
	
	
	printf("antes de huffman");
	prueba = huffman(prueba);
	puts("termina huffman");
	
	
	mostrarArbolPRE (&prueba->nodo);
	puts("termina mostrar");
	
	
	/*
	printf("\n\n\nImpresion del arbol xddd \n\n\n");

	mostrarArbolPRE (miArbol);	*/
	
}

struct cola*
huffman(struct cola* fila)
{
	printf("Entro a huffman\n");
	if( fila==NULL )
	{
		printf("Entro al 0\n");
		return NULL;
	}
	if ( fila->siguiente == fila )
	{
		printf("Entro al 1\n");
		return fila;
	}
	if( fila->siguiente->siguiente == fila )////POSIBLE PROBLEMA DE USO DE MUCHA MEMORIA
	{
		printf("Entro al 2\n");
		return creaPadreDeDos(fila, fila->siguiente);
	}
	else//Entonces son 3 o mas nodos en la cola////POSIBLE PROBLEMA DE USO DE MUCHA MEMORIA
	{
		printf("Entro al 3 o mas\n");
		do{
			struct cola * nuevo;
			nuevo = creaPadreDeDos(fila, fila->siguiente);
			fila = eliminaPrimeroCola(fila);
			fila = eliminaPrimeroCola(fila);
			fila = insertaPrioridad(fila, nuevo);
			printf("Pa ver si se cicla\n");
		}while( !(fila->siguiente->siguiente == fila) );
		//se cumple que al fina solo quedan dos
		if( fila->siguiente->siguiente == fila )////POSIBLE PROBLEMA DE USO DE MUCHA MEMORIA
		{
			printf("Entro al 2\n");
			printf("salio del while exitosamente con solo dos nodos\n");
			return creaPadreDeDos(fila, fila->siguiente);
		}
	}
	printf("\ndentro de HUFFMAN\n");
}

struct cola*
creaPadreDeDos(struct cola* nodo1, struct cola* nodo2)
{
	struct cola* nuevo;
	nuevo = (struct cola*)malloc(sizeof(struct cola));
	
	nuevo->anterior=NULL;
	nuevo->siguiente=NULL;
	
	nuevo->nodo.data.cantidad = nodo1->nodo.data.cantidad + nodo2->nodo.data.cantidad;
	nuevo->nodo.data.color=-1;//como no es hoja entonces no es valido

	nuevo->nodo.izq = &nodo1->nodo;
	nuevo->nodo.der = &nodo2->nodo;
	return nuevo;
}

struct cola*
insertaPrioridad(struct cola* Fila, struct cola* nuevo)
{
	struct colo* inicio;
	inicio = Fila;
  if (Fila == NULL)
    {
    	printf("Hiciste algo mal\nError en insertaPrioridad, fila\n");
    }
  if (nuevo == NULL)
    {
    	printf("Hiciste algo mal\nError en insertaPrioridad, nuevo\n");
    }
  if ( nuevo->nodo.data.cantidad == Fila->nodo.data.cantidad )
  {
  	printf("entro el if del insertaPrioridad IF IF IF IF IF IF IF IF \n");
  	//se mueve para que se conserve la prioridad
  	do{
  		Fila=Fila->siguiente;
  		printf("Sigue en ESE IF\n");
	}while( nuevo->nodo.data.cantidad == Fila->nodo.data.cantidad && Fila->siguiente != inicio);
  }
  else
  {
  	printf("entro el ELSE del insertaPrioridad ELSE ELSE ELSE ELSE ELSE ELSEELSE\n");
  	//inserta el nuevo nodo
  	struct cola* ultimo;
  	ultimo = Fila->anterior;
  	nuevo->siguiente = Fila;
  	nuevo->anterior = ultimo;
	ultimo->siguiente = nuevo;
  	Fila->anterior = nuevo;
  	return Fila;
  }
}

struct cola*
eliminaPrimeroCola(struct cola* Fila)
{
	struct cola* ultimo;
	struct cola* segundo;
	ultimo = Fila->anterior;
	segundo = Fila->siguiente;
	
	ultimo->siguiente = segundo;
	segundo->anterior = ultimo;
	//free(Fila);
	return segundo;
}

void
copiaNodoArbol(struct arbol* arbol1, struct arbol* arbol2)
{
	arbol1->data.cantidad = arbol2->data.cantidad;
	arbol1->data.color = arbol2->data.color;
	arbol1->der = arbol2->der;
	arbol1->izq = arbol2->izq;
}

/*Funciones Matriz*/
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

void
muestraMatriz(unsigned char**matriz, int ancho, int alto)
{
	int i,j;
	for(i=0; i<alto;i++)		
	{
		for(j=0; j<ancho;j++)
		{
			printf("[%.2x] ", matriz[i][j]);
		}
		printf("\n");
	}
}

int dividir(struct datos *arreglo, int inicio, int fin)
{
  int izq;
  int der;
  int pibote;
  int temp;
  int tempc;

  pibote = arreglo[inicio].cantidad;
  izq = inicio;
  der = fin;

  //Mientras no se cruzen los �ndices
  while (izq < der){
    while (arreglo[der].cantidad > pibote){
	  der--;
    }

	while ((izq < der) && (arreglo[izq].cantidad <= pibote)){
      izq++;
    }

    // Si todavia no se cruzan los indices seguimos intercambiando
	if(izq < der){
      temp= arreglo[izq].cantidad;
      tempc= arreglo[izq].color;
      
      arreglo[izq].cantidad = arreglo[der].cantidad;
      arreglo[izq].color = arreglo[der].color;
      
      arreglo[der].cantidad = temp;
      arreglo[der].color = tempc;
    }
  }

  //Los indices ya se han cruzado, ponemos el pivote en el lugar que le corresponde
  temp = arreglo[der].cantidad;
  tempc = arreglo[der].color;
  
  arreglo[der].cantidad = arreglo[inicio].cantidad;
  arreglo[der].color = arreglo[inicio].color;
  
  arreglo[inicio].cantidad = temp;
  arreglo[inicio].color = tempc;
  //La nueva posici�n del pivote
  return der;
}

void quickSort(struct datos *arreglo, int inicio, int fin)
{
  int pivote;
  if(inicio < fin)
  {
    pivote = dividir(arreglo, inicio, fin );
    quickSort( arreglo, inicio, pivote - 1 );//ordeno el arreglo de la izquierda
    quickSort( arreglo, pivote + 1, fin );//ordeno el arreglo de la derecha
  }
}


/*Funciones de un Arbol de busqueda binaria
struct arbol *
insertarArbol (struct arbol *raiz, struct arbol* nodoIzq, struct arbol* nodoDer)
{
  if (raiz == NULL)
    {
      struct arbol *nuevo = (struct arbol *) malloc (sizeof (struct arbol));
      
      nuevo->data.color = -1;//como no es nodo hoja entonces no es valido
      nuevo->data.cantidad = nodoIzq->data.cantidad + nodoDer->data.cantidad;
      
      nuevo->izq = nodoIzq;
      nuevo->der = nodoDer;
      return nuevo;
    }
  if ( estructura.cantidad < raiz->data.cantidad)
    {
      struct arbol *nuevo = insertarArbol (raiz->izq, estructura);
      raiz->izq = nuevo;
      return raiz;
    }
  if (estructura.cantidad > raiz->data.cantidad)
    {
      struct arbol *nuevo = insertarArbol (raiz->der, estructura);
      raiz->der = nuevo;
      return raiz;
    }
  if (estructura.cantidad == raiz->data.cantidad)
    {
      return raiz;
    }
}*/


void
mostrarArbolPRE (struct arbol *raiz)
{
  if (raiz == NULL)
    {
      return;
    }

  printf ("Cantidad:%d\t Color:%d\n", raiz->data.cantidad, raiz->data.color);
  mostrarArbolPRE (raiz->izq);
  mostrarArbolPRE (raiz->der);
}

void
mostrarArbolIN (struct arbol *raiz)
{
  if (raiz == NULL)
    {
      return;
    }

  mostrarArbolIN (raiz->izq);
  printf ("Cantidad:%d\t Color:%d\n", raiz->data.cantidad, raiz->data.color);
  mostrarArbolIN (raiz->der);
}

void
mostrarArbolPOST (struct arbol *raiz)
{
  if (raiz == NULL)
    {
      return;
    }

  mostrarArbolPOST (raiz->izq);
  mostrarArbolPOST (raiz->der);
  printf ("Cantidad:%d\t Color:%d\n", raiz->data.cantidad, raiz->data.color);
}

struct arbol *
buscarArbol (struct arbol *raiz, struct datos estructura)
{
  if (raiz == NULL)
    {
      return NULL;
    }
  if  (raiz->data.cantidad == estructura.cantidad )
    {
      return raiz;
    }

  struct arbol *buscado;

  if (raiz->data.cantidad > estructura.cantidad)
    {
      buscado = buscarArbol (raiz->izq, estructura);
    }
  else
    {
      buscado = buscarArbol (raiz->der, estructura);
    }
  if (buscado != NULL)
    {
      return buscado;
    }
  if (buscado == NULL)
    {
      return NULL;
    }
}


int
esHoja (struct arbol *raiz)
{
  if (raiz->der == NULL && raiz->izq == NULL)
    {
      return 1;
    }
  else
    {
      return 0;
    }

}

struct arbol *
padreDe (struct arbol *raiz, struct datos estructura)
{
  if (raiz == NULL)
    {
      return NULL;
    }
  struct arbol *buscado = NULL;
  buscado = buscarArbol (raiz, estructura);
  if (buscado == NULL)
    {
      return NULL;
    }

  if ((raiz->izq != NULL && raiz->izq->data.cantidad == estructura.cantidad)
      || (raiz->der != NULL && raiz->der->data.cantidad == estructura.cantidad))
    {
      return raiz;
    }

  struct arbol *padre = NULL;
  struct arbol *padre2 = NULL;

  if (estructura.cantidad < raiz->data.cantidad)
    {
      padre = padreDe (raiz->izq, estructura);
    }
  if (estructura.cantidad > raiz->data.cantidad)
    {
      padre2 = padreDe (raiz->der, estructura);
    }
  if (padre != NULL)
    {
      return padre;
    }
  if (padre2 != NULL)
    {
      return padre2;
    }
  return NULL;

}

struct arbol *
mayorDe (struct arbol *raiz)
{
  if (raiz == NULL)
    {
      return NULL;
    }

  struct arbol *mayor = NULL;

  if (raiz->der != NULL)
    {
      mayor = mayorDe (raiz->der);
    }
  else
    {
      return raiz;
    }
  return mayor;
}

struct arbol *
menorDe (struct arbol *raiz)
{
  if (raiz == NULL)
    {
      return NULL;
    }

  struct arbol *menor = NULL;

  if (raiz->izq != NULL)
    {
      menor = menorDe (raiz->izq);
    }
  else
    {
      return raiz;
    }
  return menor;
}

struct arbol *
eliminarRaizDeArbol (struct arbol *raiz)
{
  if (raiz == NULL)
    {
      return NULL;
    }

  if (esHoja (raiz))
    {
      free (raiz);
      return NULL;
    }

  struct arbol *mayor = NULL;
  mayor = mayorDe (raiz->izq);

  if (mayor != NULL)
    {
      struct arbol *padreDeMayor = NULL;
      padreDeMayor = padreDe (raiz, mayor->data);

      if (esHoja (mayor))
	{
	  if (raiz->izq->data.cantidad == mayor->data.cantidad)
	    {
	      mayor->der = raiz->der;
	      free (raiz);
	      return mayor;
	    }
	  else
	    {
	      padreDeMayor->der = mayor->izq;
	      mayor->izq = raiz->izq;
	      mayor->der = raiz->der;
	      free (raiz);
	      return mayor;
	    }
	}
      else
	{
	  if (raiz->izq->data.cantidad == mayor->data.cantidad)
	    {
	      mayor->der = raiz->der;
	      free (raiz);
	      return mayor;
	    }
	  else
	    {
	      padreDeMayor->der = mayor->izq;
	      mayor->der = raiz->der;
	      mayor->izq = raiz->izq;
	      free (raiz);
	      return mayor;
	    }
	}
    }

  struct arbol *menor = NULL;
  menor = menorDe (raiz->der);

  if (menor != NULL)
    {
      struct arbol *padreDeMenor = NULL;
      padreDeMenor = padreDe (raiz, menor->data);

      if (esHoja (menor))
	{
	  if (raiz->der->data.cantidad == menor->data.cantidad)
	    {
	      menor->izq = raiz->izq;
	      free (raiz);
	      return menor;
	    }
	  else
	    {
	      padreDeMenor->izq = menor->der;
	      menor->izq = raiz->izq;
	      menor->der = raiz->der;
	      free (raiz);
	      return menor;
	    }
	}
      else
	{
	  if (raiz->der->data.cantidad == menor->data.cantidad)
	    {
	      menor->izq = raiz->izq;
	      free (raiz);
	      return menor;
	    }
	  else
	    {
	      padreDeMenor->izq = menor->der;
	      menor->izq = raiz->izq;
	      menor->der = raiz->der;
	      free (raiz);
	      return menor;
	    }
	}
    }
}


struct arbol *
eliminarDeArbol (struct arbol *raiz, struct datos estructura)
{
  if (raiz == NULL)
    {
      return NULL;
    }

  if (raiz->data.cantidad == estructura.cantidad)
    {
      return eliminarRaizDeArbol (raiz);
    }

  struct arbol *buscado = NULL;
  buscado = buscarArbol (raiz, estructura);
  if (buscado == NULL)
    {
      return raiz;
    }


  struct arbol *padreDeBuscado = NULL;
  padreDeBuscado = padreDe (raiz, buscado->data);
  if (padreDeBuscado == NULL)
    {
      return NULL;
    }

  if (padreDeBuscado->data.cantidad > buscado->data.cantidad)
    {
      padreDeBuscado->izq = eliminarRaizDeArbol (buscado);
      return raiz;
    }

  if (padreDeBuscado->data.cantidad < buscado->data.cantidad)
    {
      padreDeBuscado->der = eliminarRaizDeArbol (buscado);
      return raiz;
    }

}



struct cola *
insertarFila (struct cola *Fila, int cantidad, int color)
{
  if (Fila == NULL)
    {
      struct cola *nuevo = NULL;
      nuevo = (struct cola *) malloc (sizeof (struct cola));	//crea nodo inicial
      
      nuevo->nodo.data.cantidad = cantidad;
      nuevo->nodo.data.color = color;
      nuevo->nodo.der=NULL;
      nuevo->nodo.izq=NULL;
      
      nuevo->siguiente = nuevo;
      nuevo->anterior = nuevo;
      return nuevo;
    }

  struct cola *nuevo = NULL;
  nuevo = (struct cola *) malloc (sizeof (struct cola));	//crea nodo
  struct cola *ultimo = NULL;
  ultimo = Fila->anterior;

  nuevo->nodo.data.cantidad=cantidad;
  nuevo->nodo.data.color=color;
  nuevo->nodo.der = NULL;
  nuevo->nodo.izq = NULL;
  
  nuevo->siguiente = Fila;
  nuevo->anterior = ultimo;

  ultimo->siguiente = nuevo;
  Fila->anterior = nuevo;

  return Fila;
}

void
mostrarFila (struct cola *Fila)
{
  if (Fila == NULL)
    {
      printf ("La fila esta vacia \n");
      return;
    }
  else
    {
      puts ("en la fila hay: ");
      struct cola *aux = Fila;
      do
	{
	  printf ("Cantidad: %d\tColor:%d\n", aux->nodo.data.cantidad, aux->nodo.data.color);
	  aux = aux->siguiente;
	}
      while (aux != Fila);

    }
  puts ("");
}


struct cola *
quitarFila (struct cola *Fila)
{
  if (Fila == NULL)
    {
      return NULL;
    }
  if (Fila->siguiente == Fila)
    {
      free (Fila);
      return NULL;
    }

  struct cola *segundo = Fila->siguiente;
  struct cola *ultimo = Fila->anterior;

  segundo->anterior = ultimo;
  ultimo->siguiente = segundo;

  free (Fila);
  return segundo;
}
