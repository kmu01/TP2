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

clinica_t* clinica_crear (hash_t* especialidades, abb_t* doctores, hash_t* pacientes);

void clinica_destruir (clinica_t* clinica);

size_t clinica_obtener_espera (clinica_t* clinica , char* nombre_especialidad);

bool clinica_existe_doctor (clinica_t* clinica , char* nombre);

bool clinica_pedir_turno(clinica_t *clinica, char* nombre_paciente, char* nombre_especialidad, bool urgencia);

void clinica_atender_paciente (clinica_t* clinica, char* nombre_doctor);

void clinica_crear_informe(clinica_t *clinica, char *ini, char *fin);