esCicloHamilton(m_adyacencia, ruta, total_nodos)
	if( total_nodos es diferente de longitud_ruta-1 )
		imprimir("No es un ciclo Hamiltoniano")
        regresa -1
	if( nodo_inicial_ruta es diferente a nodo_final_ruta )
		imprimir("No es un ciclo Hamiltoniano")
		regresa -1

	//inicia arreglo_visitados con 0
	for(desde 0 hasta total_nodos)
		nodos_visitados[i]=0

	while( desde 0 hasta total_nodos )
		aux1=ruta[i]
		aux2=ruta[i+1]
		if ( existe vertice en m_adyacencia entre axu1 y aux2 )
			if( no se ha visitado el nodo_aux1 )//pregunta si nodos visitados ha sido visitado
				nodos_visitados[regresaIndice(nodos, aux1, total_nodos)]=1;
			else
				imprimir("No es un ciclo Hamiltoniano")
				regresa -1
		else
				imprimir("No es un ciclo Hamiltoniano: no existe una vertice de la ruta\n");
				regresa -1
	
	for( desde 0 hasta total_nodos )
		if ( no se ha visitado el nodo )
			imprimir("No es un ciclo Hamiltoniano")
			regresa -1

	imprimir("Es un ciclo Hamiltoniano")
	regresa 1;