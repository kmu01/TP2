#include "abb.h"
#include "pila.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Defino el struct abb */
typedef struct abb_nodo abb_nodo_t;

struct abb{
    abb_nodo_t* raiz;
    size_t cantidad;
    abb_destruir_dato_t destruir_dato;
    abb_comparar_clave_t comparar_clave;
};

/* Defino el struct abb-nodo */
struct abb_nodo{
    char* clave;
    void* dato;
    abb_nodo_t* izquierdo;
    abb_nodo_t* derecho;
    abb_nodo_t* padre;
};

/* Primitivas del abb-nodo */
abb_nodo_t* abb_nodo_crear (){
    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
    nodo->dato = NULL;
    nodo->clave = NULL;
    nodo->izquierdo = NULL;
    nodo->derecho = NULL;
    nodo->padre = NULL;
    return nodo;
}

void* abb_nodo_destruir (abb_nodo_t* nodo){
    if (nodo == NULL){
        return NULL;
    }
    if (nodo->clave == NULL){
        return NULL;
    }
    void* dato = nodo->dato;
    free (nodo->clave);
    free (nodo);
    return dato;
}


/* Funciones auxiliares que trabajan con nodos en vez de arboles */


abb_nodo_t *buscar_nodo(abb_nodo_t* nodo,const char *clave,abb_comparar_clave_t comparar){
    if(nodo==NULL){
        return NULL;
    }
    if (nodo->clave == NULL){
        return NULL;
    }
    int valor_comparativo = (int)comparar(clave,nodo->clave);
    if(valor_comparativo==0){
        return nodo;
    }
    else if(valor_comparativo<0){
        return buscar_nodo (nodo->izquierdo,clave,comparar);
    }
    return buscar_nodo (nodo->derecho , clave,comparar);
}

abb_nodo_t *busco_lugar(abb_nodo_t *nodo, void* clave,abb_comparar_clave_t comparar){
    if(nodo==NULL){
        return NULL;
    }
    if (nodo->clave == NULL){
        return nodo;
    }
    int valor_comparativo = (int)comparar (clave , nodo->clave);
    if(valor_comparativo>0){
        if (nodo->derecho == NULL){
            abb_nodo_t* hijo = abb_nodo_crear ();
            if (hijo==NULL){
                return NULL;
            }
            hijo->padre = nodo;
            nodo->derecho = hijo;
            return hijo;
        }
        return busco_lugar (nodo->derecho , clave,comparar);
    }
    else {
        if (nodo->izquierdo == NULL){
            abb_nodo_t* hijo = abb_nodo_crear ();
            if (hijo == NULL){
                return NULL;
            }
            hijo->padre = nodo;
            nodo->izquierdo = hijo;
            return hijo;
        }
        return busco_lugar (nodo->izquierdo , clave,comparar);
    }
}

void _abb_destruir (abb_nodo_t* nodo,abb_destruir_dato_t destruir_dato){
    if(nodo->izquierdo!=NULL){
        _abb_destruir(nodo->izquierdo,destruir_dato);
    }
    if(nodo->derecho!=NULL){
        _abb_destruir(nodo->derecho,destruir_dato);
    }
    if (destruir_dato != NULL){
        destruir_dato (nodo->dato);
    }
    free (nodo->clave);
    free (nodo);
}

bool _abb_in_order (abb_nodo_t* nodo ,bool visitar(const char *, void *, void *),void* extra){
    if (nodo == NULL){
        return true;
    }
    if (! _abb_in_order (nodo->izquierdo , visitar , extra)){
        return false;
    }
    if (!visitar(nodo->clave, nodo->dato, extra)){
        return false;
    }
    if (! _abb_in_order (nodo->derecho , visitar , extra)){
        return false;
    }
    return true;
}


/* PRIMITIVAS ARBOL */

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t *arbol = malloc(sizeof(abb_t));
    if (arbol==NULL){
        return NULL;
    }
    abb_nodo_t* raiz = abb_nodo_crear ();
    arbol->raiz = raiz;
    arbol->destruir_dato = destruir_dato;
    arbol->comparar_clave = cmp;
    arbol->cantidad = 0;
    return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    char* copia_clave = malloc ((strlen(clave)+1) * sizeof (char));
    strcpy (copia_clave , clave);
    if(abb_pertenece(arbol,clave)){
        abb_nodo_t* nodo_encontrado = buscar_nodo(arbol->raiz , clave,arbol->comparar_clave);
        void* dato_viejo = nodo_encontrado->dato;
        if (arbol->destruir_dato != NULL){
            arbol->destruir_dato (dato_viejo);
        }
        nodo_encontrado->dato = dato;
        free(copia_clave);
        return true;
    }
    abb_nodo_t *lugar = busco_lugar(arbol->raiz,copia_clave,arbol->comparar_clave);
    if (lugar == NULL){
        return false;
    }
    lugar->clave = copia_clave;
    lugar->dato = dato;
    arbol->cantidad ++;
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    abb_nodo_t* nodo_a_borrar = buscar_nodo (arbol->raiz,clave,arbol->comparar_clave);
    if (nodo_a_borrar == NULL){
        return NULL;
    }
    if (nodo_a_borrar->izquierdo == NULL && nodo_a_borrar->derecho == NULL){
        abb_nodo_t* padre = nodo_a_borrar->padre;
        if (padre == NULL){
            void* dato = nodo_a_borrar->dato;
            free (nodo_a_borrar->clave);
            nodo_a_borrar->dato = NULL;
            nodo_a_borrar->clave= NULL;
            arbol->cantidad --;
            return dato;
        }
        if (padre->izquierdo == nodo_a_borrar){
            padre->izquierdo = NULL;
        }
        else if (padre->derecho == nodo_a_borrar){
            padre->derecho = NULL;
        }
        void* dato = abb_nodo_destruir(nodo_a_borrar);
        arbol->cantidad --;
        return dato;
    }
    if (nodo_a_borrar->izquierdo == NULL || nodo_a_borrar->derecho == NULL){
        abb_nodo_t* padre = nodo_a_borrar->padre;
        if (padre == NULL){
            if (nodo_a_borrar->izquierdo != NULL){
                arbol->raiz = nodo_a_borrar->izquierdo;
                nodo_a_borrar->izquierdo->padre = NULL;
            }
            else{
                arbol->raiz = nodo_a_borrar->derecho;
                nodo_a_borrar->derecho->padre = NULL;
            }
            void* dato = abb_nodo_destruir (nodo_a_borrar);
            arbol->cantidad --;
            return dato;
        }
        if (padre->izquierdo == nodo_a_borrar){
            if (nodo_a_borrar->izquierdo != NULL){
                padre->izquierdo = nodo_a_borrar->izquierdo;
                nodo_a_borrar->izquierdo->padre = padre;
            }
            else{
                padre->izquierdo = nodo_a_borrar->derecho;
                nodo_a_borrar->derecho->padre = padre;
            }
        }
        else{
            if (nodo_a_borrar->izquierdo != NULL){
                padre->derecho = nodo_a_borrar->izquierdo;
                nodo_a_borrar->izquierdo->padre = padre;
            }
            else{
                padre->derecho = nodo_a_borrar->derecho;
                nodo_a_borrar->derecho->padre = padre;
            }
        }
        void* dato = abb_nodo_destruir (nodo_a_borrar);
        arbol->cantidad --;
        return dato;
    }
    else{
        abb_nodo_t* reemplazante = nodo_a_borrar->izquierdo;
        while (reemplazante->derecho != NULL){
            reemplazante = reemplazante->derecho;
        }
        char* clave_reemplazante = reemplazante->clave;
        char* copia_clave_reemplazante = malloc ((strlen(clave_reemplazante)+1)*sizeof(char));
        strcpy (copia_clave_reemplazante , clave_reemplazante);
        void* dato_reemplazo = abb_borrar (arbol,clave_reemplazante);
        char* clave_borrado = nodo_a_borrar->clave;
        void* dato_borrado = nodo_a_borrar->dato;
        nodo_a_borrar->clave = copia_clave_reemplazante;
        nodo_a_borrar->dato = dato_reemplazo;
        free (clave_borrado);
        return dato_borrado;
    }
}


void *abb_obtener(const abb_t *arbol, const char *clave){
    abb_nodo_t* nodo = buscar_nodo (arbol->raiz,clave,arbol->comparar_clave);
    if (nodo == NULL){
        return NULL;
    }
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){ 
    abb_nodo_t* nodo = buscar_nodo (arbol->raiz,clave,arbol->comparar_clave);
    if (nodo == NULL){
        return false;
    }
    return true;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
    if(arbol==NULL){
        return;
    }
    _abb_destruir (arbol->raiz , arbol->destruir_dato);
    free (arbol);
}

/* Iterador interno - Utiliza una funcion wrapper
 para poder trabjar recursivamente con nodos */

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    if (!arbol || !arbol->raiz->clave){
        return;
    }
    _abb_in_order(arbol->raiz,visitar,extra);
}

/* Iterador externo */

struct abb_iter{
    const abb_t *arbol;
    abb_nodo_t* actual;
    pila_t* pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;
    pila_t* pila = pila_crear();
    if (!pila){
        free(iter);
        return NULL;
    }
    iter->pila = pila;
    iter->arbol = arbol;
    iter->actual = arbol->raiz;
    if (arbol->raiz->clave == NULL){
        iter->actual = NULL;
        return iter;
    }
    pila_apilar (iter->pila , iter->actual);
    while (iter->actual->izquierdo != NULL){
        iter->actual = iter->actual->izquierdo;
        pila_apilar (iter->pila , iter->actual);
    }
    iter->actual = pila_ver_tope(iter->pila);
    return iter;
}
bool abb_iter_in_avanzar(abb_iter_t *iter){
    if (iter->actual == NULL){
        return false;
    }
    if (pila_esta_vacia(iter->pila)){
        iter->actual = NULL;
        return true;
    }
    abb_nodo_t* aux = pila_desapilar (iter->pila);
    if (aux->derecho != NULL){
        aux = iter->actual->derecho;
        pila_apilar (iter->pila , aux);
        while (aux->izquierdo != NULL){
            aux = aux->izquierdo;
            pila_apilar (iter->pila, aux);
        }
    }
    iter->actual = pila_ver_tope (iter->pila);
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if (!abb_iter_in_al_final(iter)){
        return iter->actual->clave;
    }
    return NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    if (iter->actual == NULL && pila_esta_vacia(iter->pila)){
        return true;
    }
    return false;
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free (iter);
}
