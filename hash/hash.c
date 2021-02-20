#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// ********** Definiciones **********

#define REDIMENSION 2
#define FACTOR_DE_CARGA_MAX 70
#define TAM_DEFAULT 1019
#define NO_ENCONTRADO -1

typedef enum estado{
    OCUPADO,
    VACIO,
    BORRADO,
}estado_t;


typedef struct nodo_hash{
	char* clave;
	void* dato;
    estado_t estado;
} nodo_hash_t;

struct hash{
	size_t tam;
	size_t cant;
	hash_destruir_dato_t destruir_dato;
    nodo_hash_t* tabla;
};

struct hash_iter{
	size_t actual;
	const hash_t* hash;
};

// ********** Auxiliares **********

// Funcion de hasinh
// Se usa la funcion sdbm, la segunda listada en: www.cse.yorku.ca/~oz/hash.html
size_t f_hash(const char *str){
	int hash = 0;
        int c;
        while ((c = *str++))
            hash = c + (hash << 6) + (hash << 16) - hash;
	return (size_t)hash;
}

// Reserva memoria para la tabla de hash, y declaro a cada nodo contenido como vacio.
nodo_hash_t* iniciar_tabla(size_t tam){
    nodo_hash_t* tabla = malloc(sizeof(nodo_hash_t) * tam);
    if (!tabla){
        return false;
    }
    
    for (size_t i = 0; i < tam; i++){
        tabla[i].estado = VACIO;
        tabla[i].clave = NULL;
	    tabla[i].dato = NULL;
    }
    return tabla;
}

void setear_nodo(hash_t* hash, const char* clave, void* dato, size_t pos){
    hash->tabla[pos].clave = strdup(clave);
	hash->tabla[pos].dato = dato;
	hash->tabla[pos].estado = OCUPADO;
	hash->cant++;
}

bool iter_buscar_ocupado(hash_iter_t *iter){	
    while (iter->actual < iter->hash->tam){
		if (iter->hash->tabla[iter->actual].estado == OCUPADO){        
			return true;
		}
		iter->actual++;
	}
    return false;
}

bool redimensionar(hash_t* hash, size_t tam_nuevo){
    nodo_hash_t* tabla_aux = hash->tabla;
    size_t tam_aux = hash->tam;
    nodo_hash_t* tabla = iniciar_tabla(tam_nuevo);
    hash->tabla = tabla;
    hash->cant = 0;
    hash->tam = tam_nuevo;
    if (!hash->tabla){ 
        return false;
    }
    for (size_t i = 0; i < tam_aux; i++){
        if (tabla_aux[i].estado == OCUPADO){
            hash_guardar(hash, tabla_aux[i].clave, tabla_aux[i].dato);
            free(tabla_aux[i].clave);
        }
    }
    free(tabla_aux);
    return true;
}

// Si mas del 60% de la tabla esta ocupada, tam*2
bool comprobar_redimension(hash_t* hash){
    size_t porcentaje = (hash->cant*100/hash->tam);
    if (porcentaje > FACTOR_DE_CARGA_MAX){        
        size_t nuevo_tam = hash->tam * REDIMENSION;
        return redimensionar(hash, nuevo_tam);
    }
    return true;
}

size_t buscar_pos_nodo(const hash_t* hash, const char* clave){
    size_t pos = f_hash(clave) % hash->tam;
    //La mayoria de las veces va a estar delante.
    for (size_t i = pos; i < hash->tam; i++){ 
        if (hash->tabla[i].estado == OCUPADO && strcmp(clave, hash->tabla[i].clave) == 0) {
            return i;
        } 
    }
    //Pero podrìa estar detras.
    for (size_t i = 0; i < pos; i++){ 
        if (hash->tabla[i].estado == OCUPADO  && strcmp(clave, hash->tabla[i].clave) == 0) {
            return i;
        } 
    }
    return NO_ENCONTRADO; 
}

// ********** Primitivas **********

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
    if(!hash){
		return NULL;
	}
    hash->destruir_dato = destruir_dato;
	hash->tam = TAM_DEFAULT;
	hash->cant = 0; 
    hash->tabla = iniciar_tabla(hash->tam);
    if (!hash->tabla){ 
        free(hash);
		return NULL;
    }
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){

    if (!comprobar_redimension(hash)){
        return false;
    }

    size_t pos = f_hash(clave) % hash->tam;
    while (pos < hash->tam){ //hacer esto menos feo.
		if (!hash->tabla[pos].estado == OCUPADO){ 
			setear_nodo(hash,clave,dato,pos);
            return true;
        }
		else{ 
			if((strcmp(hash->tabla[pos].clave,clave)) == 0){ 
				if (hash->destruir_dato){
                    hash->destruir_dato(hash->tabla[pos].dato);
                }
                hash->tabla[pos].dato = dato;
                return true;
				}
			else{ 
				pos++;
                if (pos == hash->tam){
                    pos = 0;
                }
            }
        }
    }
    return false;

}

void *hash_borrar(hash_t *hash, const char *clave) {

	size_t pos = buscar_pos_nodo(hash,clave);
    if (pos == NO_ENCONTRADO){
        return NULL;
    }

	free(hash->tabla[pos].clave);
    void* dato = hash->tabla[pos].dato;
    hash->tabla[pos].dato = NULL;
	hash->tabla[pos].estado = VACIO;
	hash->cant--;
	return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	size_t pos = buscar_pos_nodo(hash,clave);
    if (pos == NO_ENCONTRADO){
        return NULL;
    }
	return hash->tabla[pos].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	return (buscar_pos_nodo(hash,clave) != NO_ENCONTRADO);
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant;
}

void hash_destruir(hash_t *hash){
	for (int i = 0; i < hash->tam;i++){
        if (hash->tabla[i].estado == OCUPADO){
            free(hash->tabla[i].clave);
            if (hash->destruir_dato){
                hash->destruir_dato(hash->tabla[i].dato);
            }
        }
    }
    free(hash->tabla);
	free(hash);
}

/* ******************** Iterador ******************** */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter){
		return NULL;
	}
	
	iter->hash = hash;
    iter->actual = 0;
	if (!iter_buscar_ocupado(iter)){
		iter->actual = iter->hash->tam;
	}
	return iter;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)){
		return false;
	} 
    iter->actual++;
	return iter_buscar_ocupado(iter);
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)){
		return NULL;
	} 
	return iter->hash->tabla[iter->actual].clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter){
	return iter->actual == iter->hash->tam;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t *iter){
	free(iter);
}
