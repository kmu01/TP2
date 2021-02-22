#include "hash/hash.h"
#include "abb/abb.h"
#include "cola/cola.h"
#include "heap/heap.h"
#include "strutil/strutil.h"
#include <stdbool.h>

typedef struct clinica clinica_t;

void eliminar_fin_linea(char* linea);

/*
 * Recibe un archivo de pacientes que contiene su 
 * nombre y el año en el que se inscribieron. A partir de este
 * devuelve un hash donde se almacena la informaciòn de todos los pacientes.
 * En caso de error devuelve NULL
 */
hash_t* cargar_pacientes(char* archivo);

/*
 * Recibe un archivo de doctores que contiene su nombre y especialidad,
 * un abb donde almacenar los doctores y un hash para las especialidades.
 * Si se pudieron cargar los datos devuelve 0.
 * En caso de error devuelve 1.
 */
int cargar_doctores_y_especialidades(char* archivo, abb_t* doctores, hash_t* especialidades);

/*
 * Se encarga de crear un abb donde se almacenarán doctores
 * Pre: nada
 * Post: Se devolvió un abb
 */
abb_t* crear_abb_doctores (void);

/*
 * Se encarga de crear un hash donde se almacenarán especialidades
 * Pre: nada
 * Post: se devolvio un hash
 */
hash_t* crear_hash_especialidades (void);

/*
 * Recibe un hash de especialidades, uno de pacientes y un ABB de doctores.
 * Crea y devuelve la estructura "clinica" que almacena dichos elementos.
 */
clinica_t* clinica_crear (hash_t* especialidades, abb_t* doctores, hash_t* pacientes);

/*
 * Destruye la estructura clinica liberando la memoria pedida
 */
void clinica_destruir (clinica_t* clinica);

/*
 * Recibe la clinica y el nombre de una especialidad y devuelve
 * la cantidad de pacientes en espera para esa especialidad la cual puede ser 0.
 * Pre: Existe la clinica y la especialidad
 * Post: Se devolvio la cantidad de pacientes en espera para la especialidad
 */
size_t clinica_obtener_espera (clinica_t* clinica , char* nombre_especialidad);

/*
 * Recibe como parámetros la clinica y el nombre de un doctor. Devuelve
 * true si el doctor existe dentro de la clinica y false en caso contrario.
 * Pre: La clinica fue creada
 * Post: Se devolvio true o fals
 */
bool clinica_existe_doctor (clinica_t* clinica , char* nombre);

/*
 * Recibe como parámetros la clinica, el nombre del paciente, la especialidad que desea y un
 * booleano con la información sobre la urgencia. Se encola el paciente en la especialidad deseada.
 * Si el paciente o la especialidad no existen, se informa por stdout y se devuelve false
 * Pre: Se creo la clinica
 * Post: Se devuelve true o false en caso de haber podido o no encolar al paciente
 */ 
bool clinica_pedir_turno(clinica_t *clinica, char* nombre_paciente, char* nombre_especialidad, bool urgencia);

/*
 * Se recibe como parámetros la clinica y el nombre de doctor que debe atender.
 * Se atiende al paciente correspondiente o se informa que no hay pacientes en espera.
 * Pre: La clinica fue creada y el doctor también
 * Post: Se atiende al paciente o se informa que no hay pacientes por atender.
 */
void clinica_atender_paciente (clinica_t* clinica, char* nombre_doctor);

/*
 * Se crea e imprime por pantalla un informe con los doctores
 * dentro del intervalo [ini , fin]
 * Pre: La clinica fue creada
 * Post: Se imprimio por stdout un informe con los doctores solicitados
 */
void clinica_crear_informe(clinica_t *clinica, char *ini, char *fin);