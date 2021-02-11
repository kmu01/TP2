#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo{
	void *dato;
	struct nodo *siguiente;
} nodo_t;

struct cola{
	nodo_t *primero;
	nodo_t *ultimo;
};

cola_t *cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if (cola == NULL){
		return NULL;
	}
	cola->primero = NULL;
	cola->ultimo = NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){

	while (!cola_esta_vacia(cola)){
		void *dato = cola_desencolar(cola);
		if (destruir_dato != NULL){
			destruir_dato(dato);
		}
	}
	free(cola);	
}

bool cola_esta_vacia(const cola_t *cola){
	return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){
	nodo_t* nuevo = malloc(sizeof(nodo_t));
	if (nuevo == NULL){
		return false;
	}
	nuevo->dato = valor;
	nuevo->siguiente = NULL;

	if (cola_esta_vacia(cola))
		cola->primero = nuevo;
	else
		cola->ultimo->siguiente = nuevo;
	cola->ultimo = nuevo;
	return true;
}

void *cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	return cola->primero->dato;
}

void *cola_desencolar(cola_t *cola){

	if (cola_esta_vacia(cola)){
		return NULL;
	}
	void *dato = cola->primero->dato;
	nodo_t *nodo_aux = cola->primero;
	cola->primero = cola->primero->siguiente;
	free(nodo_aux);
	if (cola_esta_vacia(cola)){
		cola->ultimo = NULL;
	}
	return dato;
}
