#include "zyxcba_lib.h"
#include "mensajes.h"


/*   Defino el struct clinica   */
typedef struct clinica{
    hash_t* especialidades;
    abb_t* doctores;
    hash_t* pacientes;
}clinica_t;


/*   Creo primitivas de la clinica   */
clinica_t* clinica_crear (hash_t* especialidades,abb_t* doctores,hash_t* pacientes){
    clinica_t* clinica = malloc (sizeof (clinica_t));
    if (clinica == NULL){
        return NULL;
    }
    clinica->especialidades = especialidades;
    clinica->doctores = doctores;
    clinica->pacientes = pacientes;
    return clinica;
}

void clinica_destruir (clinica_t* clinica){
    hash_destruir (clinica->pacientes);
    abb_destruir (clinica->doctores);
    hash_destruir (clinica->especialidades);
    free (clinica);
    return;
}

size_t clinica_obtener_espera (clinica_t* clinica , char* nombre_especialidad){
    especialidad_t* especialidad = hash_obtener (clinica->especialidades , nombre_especialidad);
    return especialidad->sin_atender;
}

paciente_t* clinica_obtener_paciente (clinica_t* clinica , char* nombre){
    return hash_obtener(clinica->pacientes , nombre);
}

especialidad_t* clinica_obtener_especialidad (clinica_t* clinica, char* nombre){
    return hash_obtener (clinica->especialidades , nombre);
}

doctor_t* clinica_obtener_doctor (clinica_t* clinica , char* nombre){
    return abb_obtener (clinica->doctores , nombre);
}

bool clinica_pedir_turno_urgente (paciente_t* paciente, especialidad_t* especialidad){
    if (paciente == NULL || especialidad == NULL){
        return false;
    }
    paciente_pedir_turno_urgente (paciente , especialidad);
    especialidad->sin_atender ++;
    return true;
}

bool clinica_pedir_turno_regular (paciente_t* paciente, especialidad_t* especialidad){
    if (paciente == NULL || especialidad == NULL){
        return false;
    }
    paciente_pedir_turno_regular(paciente , especialidad);
    especialidad->sin_atender ++;
    return true;
}

void clinica_atender_paciente (clinica_t* clinica, char* nombre_doctor){
    doctor_t* doctor = clinica_obtener_doctor (clinica , nombre_doctor);
    especialidad_t* especialidad = doctor->especialidad;
    size_t pacientes_previos = especialidad->sin_atender;
    if (pacientes_previos == 0){
        printf (SIN_PACIENTES);
        return;
    }
    paciente_t* paciente = doctor_atender_paciente (doctor);
    printf (PACIENTE_ATENDIDO,paciente->nombre);
    printf (CANT_PACIENTES_ENCOLADOS, especialidad->sin_atender, especialidad->nombre);
    return;
}



/*   Defino el struct paciente   */
typedef struct paciente{
    char* nombre;
    size_t año;
}paciente_t;

/*   Creo primitivas de paciente   */
paciente_t* paciente_crear (char* nombre , size_t año){
    paciente_t* paciente = malloc (sizeof(paciente_t));
    if (paciente == NULL){
        return NULL;
    }
    paciente->nombre = nombre;
    paciente->año = año;
    return paciente;
}

bool paciente_eliminar (paciente_t* paciente){
    free (paciente->nombre);
    free (paciente);
    return true;
}

bool paciente_pedir_turno_urgente (paciente_t* paciente , especialidad_t* especialidad){
    return cola_encolar (especialidad->urgentes , paciente);
}

bool paciente_pedir_turno_regular (paciente_t* paciente , especialidad_t* especialidad){
    return heap_encolar (especialidad->regulares , paciente);
}


/*   Defino el struct doctor   */
typedef struct doctor{
    char* nombre;
    especialidad_t* especialidad;
    size_t pacientes_atendidos;
}doctor_t;

/*   Creo las primirivas de doctor   */
doctor_t* doctor_crear (char* nombre , especialidad_t* especialidad){
    doctor_t* doctor = malloc (sizeof(doctor_t));
    if (doctor == NULL){
        return NULL;
    }
    doctor->nombre = nombre;
    doctor->especialidad = especialidad;
    doctor->pacientes_atendidos = 0;
    return doctor;
}

bool doctor_eliminar (doctor_t* doctor){
    free (doctor);
    return true;
}

paciente_t* doctor_atender_paciente (doctor_t* doctor){
    cola_t* cola_urgentes = doctor->especialidad->urgentes;
    if (!cola_esta_vacia(cola_urgentes)){
        doctor->especialidad->sin_atender --;
        return cola_desencolar(cola_urgentes);
    }
    doctor->especialidad->sin_atender --;
    heap_t* heap_regulares = doctor->especialidad->regulares;

    return heap_desencolar (heap_regulares);
}

/*   Defino el struct especialidad   */
typedef struct especialidad{
    char* nombre;
    cola_t* urgentes;
    heap_t* regulares;
    size_t sin_atender;
}especialidad_t;


/*   Funcion comparacion de años   */
int comparacion_años (paciente_t* a , paciente_t* b){
    if (a->año < b->año){
        return -1;
    }
    else if (a->año > b->año){
        return 1;
    }
    return 0;
}

/*   Creo las primirivas de especialidad   */
especialidad_t* especialidad_crear (char* nombre){
    especialidad_t* especialidad = malloc (sizeof(especialidad_t));
    if (especialidad == NULL){
        return NULL;
    }
    cola_t* urgentes = cola_crear ();
    if (urgentes == NULL){
        free (especialidad);
        return NULL;
    }
    heap_t* regulares = heap_crear (comparacion_años);
    if (regulares == NULL){
        free (especialidad);
        free (urgentes);
        return NULL;
    }
    especialidad->nombre = nombre;
    especialidad->urgentes = urgentes;
    especialidad->regulares = regulares;
    especialidad->sin_atender = 0;
    return especialidad;
}

bool especialidad_eliminar (especialidad_t* especialidad){
    cola_destruir (especialidad->urgentes , NULL);
    heap_destruir(especialidad->regulares , NULL);
    free (especialidad->nombre);
    free (especialidad);
    return true;
}