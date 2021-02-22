#include "pila.h"

#include <stdlib.h>

#define CAPACIDAD 5 //Capacidad inicial de la pila

#define FACTOR_REDIMENSION 2 //Factor por el que multiplico para agrandar o divido para achicar

#define RATIO_ACHICAR 1/4

bool _pila_redimensionar (pila_t* pila , size_t tam);


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){
    pila_t *pila = malloc (sizeof(pila_t));
    if (pila == NULL){
        return NULL;
    }
    pila->datos = malloc (CAPACIDAD * sizeof (void*));
    if (pila->datos == NULL){
        free (pila);
        return NULL;
    }
    pila -> cantidad = 0;
    pila -> capacidad = CAPACIDAD;
    return pila;
}

void pila_destruir(pila_t *pila){
    free (pila->datos);
    free (pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return (pila->cantidad == 0);
}

bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad){
        if (!_pila_redimensionar(pila , pila->capacidad * FACTOR_REDIMENSION)){
            return false;
        }
    }
    pila->datos [pila->cantidad] = valor;
    pila->cantidad += 1;
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    if (pila->cantidad == 0){
        return NULL;
    }return pila-> datos [pila->cantidad-1];
}

void *pila_desapilar(pila_t *pila){
    if (pila->cantidad == 0){
        return NULL;    
    }
    pila->cantidad -=1; 
    if (pila->cantidad <= pila ->capacidad * RATIO_ACHICAR && pila->cantidad > CAPACIDAD){
        _pila_redimensionar (pila , pila->capacidad / FACTOR_REDIMENSION);
    }
    return pila->datos[pila -> cantidad];
}

//Se encarga de redimensionar la pila
bool _pila_redimensionar (pila_t* pila , size_t tam){
    pila->capacidad = tam;
    void* aux = realloc (pila -> datos ,pila ->capacidad * sizeof (void*));
    if (aux == NULL){
        return false;
    }
    pila->datos = aux;
    return true;
}