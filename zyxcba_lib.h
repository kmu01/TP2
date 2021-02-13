#include "hash.h"
#include "abb.h"
#include "cola.h"
#include "lista.h"
#include "heap.h"


struct clinica;
typedef struct clinica clinica_t;
struct doctor;
typedef struct doctor doctor_t;
struct paciente;
typedef struct paciente paciente_t;
struct especialidad;
typedef struct especialidad especialidad_t;


clinica_t* clinica_crear (hash_t* especialidades,abb_t* doctores,hash_t* pacientes);
void clinica_destruir (clinica_t* clinica);
size_t clinica_obtener_espera (clinica_t* clinica , char* nombre_especialidad);
paciente_t* clinica_obtener_paciente (clinica_t* clinica , char* nombre);
especialidad_t* clinica_obtener_especialidad (clinica_t* clinica, char* nombre);
doctor_t* clinica_obtener_doctor (clinica_t* clinica , char* nombre);
bool clinica_pedir_turno_urgente (paciente_t* paciente, especialidad_t* especialidad);
bool clinica_pedir_turno_regular (paciente_t* paciente, especialidad_t* especialidad);
void clinica_atender_paciente (clinica_t* clinica, char* nombre_doctor);


paciente_t* paciente_crear (char* nombre , size_t año);
bool paciente_eliminar (paciente_t* paciente);
bool paciente_pedir_turno_urgente (paciente_t* paciente , especialidad_t* especialidad);
bool paciente_pedir_turno_regular (paciente_t* paciente , especialidad_t* especialidad);


doctor_t* doctor_crear (char* nombre , especialidad_t* especialidad);
bool doctor_eliminar (doctor_t* doctor);
paciente_t* doctor_atender_paciente (doctor_t* doctor);


especialidad_t* especialidad_crear (void);
bool especialidad_eliminar (especialidad_t* especialidad);

int comparacion_años (paciente_t* a , paciente_t* b);