#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/*
Crea un arbol
Pre: nada
Post: Devuelve una raiz de un arbol si este se pudo crear.
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/*
Guarda el par (clave,dato) en el arbol
Pre: El arbol fue creado
Post: Devuelve true si se puede cargar el dato en el arbol, sino devuelve false
*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/*
Borra el par (clave,dato) guardado y establece en ella un valor para que continue la regla del abb
Pre: El arbol fue creado
Post: Devuelve el dato del elemento borrado o NULL en caso de que la clave no esta en el arbol.
*/
void *abb_borrar(abb_t *arbol, const char *clave);

/*
Devuelve el dato al que esta unido la clave si existe.
Pre:El arbol fue creado.
Post: Devuelve el dato al que esta unido la clave o NULL en caso de que no exista.
*/
void *abb_obtener(const abb_t *arbol, const char *clave);

/*
Se fija si la clave existe en el arbol.
Pre:El arbol fue creado
Post: Devuelve true si la clave esta en el arbol, false en caso contrario.
*/
bool abb_pertenece(const abb_t *arbol, const char *clave);

/*
Devuelve la cantidad de elementos del arbol
Pre: El arbol fue creado.
Post: Devuelve la cantidad de elementos del arbol. En caso de que el arbol sea igual a NULL devuelve 0.
*/
size_t abb_cantidad(abb_t *arbol);

/*
Destruye el arbol entero.
Pre: El arbol fue creado.
Post: El arbol y sus datos son destruidos.
*/
void abb_destruir(abb_t *arbol);


/* 
******* ITERADOR *******
******* EXTERNO  *******
 */
void abb_in_order(abb_t *arbol, char* ini, char* fin, bool visitar(const char*, void*, void*, char*, char*), void *extra);

/*iterador interno */
typedef struct abb_iter abb_iter_t;

/*
Se crea un iterador externo para el arbol
Pre: El arbol fue creado
Post: Se devuelve un iterador apuntando a un elemento del arbol o a NULL si este esta vacio
*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/*
Avanza al siguiente elemento del ABB de forma que este se recorre in-order
Pre: El iterador fue creado
Post: Se avanzo al proximo elemento. Se devolvio true si se pudo o false si no se pudo
*/
bool abb_iter_in_avanzar(abb_iter_t *iter);

/*
Devuelve la clave del elemento actual o NULL si está al final o vacio
Pre: El iter fue creado
Post: Se devolvio la clave del elemento apuntado, o NULL si el abb está vacio o el iter al final
*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/*
Verifica si el iter está al final
Pre: El iter fue creado
Post: Se devolvio true si el iter está al final o false en caso contrario
*/
bool abb_iter_in_al_final(const abb_iter_t *iter);

/*Destruye el iterador
Pre: El iter fue creado
Post: Se destruyo liberando toda la memoria pedida
*/
void abb_iter_in_destruir(abb_iter_t* iter);

#endif  // ABB_H

