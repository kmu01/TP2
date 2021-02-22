#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "zyxcba_lib.h"
#include "mensajes.h"


//***************************  DEFINICIÒN DE ESTRUCTURAS  ***************************
/*   Defino el struct clinica   */
struct clinica{
    hash_t* especialidades;
    abb_t* doctores;
    hash_t* pacientes;
};

/*   Defino el struct paciente   */
typedef struct paciente{
    char* nombre;
    size_t anio;
} paciente_t;

/*   Defino el struct especialidad   */
typedef struct especialidad{
    char* nombre;
    cola_t* urgentes;
    heap_t* regulares;
    size_t sin_atender;
} especialidad_t;

/*   Defino el struct doctor   */
typedef struct doctor{
    char* nombre;
    especialidad_t* especialidad;
    size_t pacientes_atendidos;
} doctor_t;


//***************************  AUXILIARES  ***************************

bool contar_doctores(const char *clave, void *valor, void *extra, char* ini, char* fin){
    if (strcmp(clave, ini)<0){
        return true;
    }
    else if (strcmp(clave, ini) >= 0 && strcmp (clave, fin)<=0 ||strcmp(clave, ini) >= 0 && strcmp (fin, "")==0){
        size_t* cont = (size_t*)(extra);
        *cont = *cont + 1;
        return true;
    }
    else{
        return false;
    }
}

bool imprimir_doctores(const char *clave, void *valor, void *extra, char* ini, char* fin){
    if (strcmp(clave, ini)<0){
        return true;
    }
    else if (strcmp(clave, ini) >= 0 && strcmp (clave, fin)<=0 ||strcmp(clave, ini) >= 0 && strcmp (fin, "")==0){
        size_t* cont = (size_t*)(extra);
        *cont = *cont + 1;
        doctor_t* doctor = (doctor_t*)(valor);
        printf(INFORME_DOCTOR, *cont, doctor->nombre, doctor->especialidad->nombre, doctor->pacientes_atendidos);
        return true;
    }
    else{
        return false;
    }
}

int comparacion_anios (const void* a , const void* b){
    paciente_t* paciente_a = (paciente_t*) (a);
    paciente_t* paciente_b = (paciente_t*) (b);
    if (paciente_a->anio < paciente_b->anio){
        return -1;
    }
    else if (paciente_a->anio > paciente_b->anio){
        return 1;
    }
    return 0;
}

//***************************  PACIENTE  ***************************


/*   Creo primitivas de paciente   */
paciente_t* paciente_crear (char* nombre , size_t anio){
    paciente_t* paciente = malloc (sizeof(paciente_t));
    if (paciente == NULL){
        return NULL;
    }
    paciente->nombre = nombre;
    paciente->anio = anio;
    return paciente;
}

bool paciente_eliminar (paciente_t* paciente){
    free (paciente->nombre);
    free (paciente);
    return true;
}

void _paciente_eliminar (void* paciente_a){
    paciente_t* paciente = (paciente_t*) paciente_a;
    paciente_eliminar (paciente);
}

bool paciente_pedir_turno_urgente (paciente_t* paciente , especialidad_t* especialidad){
    return cola_encolar (especialidad->urgentes , paciente);
}

bool paciente_pedir_turno_regular (paciente_t* paciente , especialidad_t* especialidad){
    return heap_encolar (especialidad->regulares , paciente);
}

//***************************  DOCTOR  ***************************



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
    free (doctor->nombre);
    free (doctor);
    return true;
}

void _doctor_eliminar (void* doc){
    doctor_t* doctor = (doctor_t*)(doc);
    doctor_eliminar (doctor);
}

paciente_t* doctor_atender_paciente (doctor_t* doctor){
    cola_t* cola_urgentes = doctor->especialidad->urgentes;
    if (!cola_esta_vacia(cola_urgentes)){
        doctor->especialidad->sin_atender--;
        return cola_desencolar(cola_urgentes);
    }
    doctor->especialidad->sin_atender--;
    heap_t* heap_regulares = doctor->especialidad->regulares;
    return heap_desencolar (heap_regulares);
}


//***************************  ESPECIALIDAD  ***************************

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
    heap_t* regulares = heap_crear (comparacion_anios);
    if (regulares == NULL){
        free (especialidad);
        cola_destruir(urgentes, NULL);
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

void _especialidad_eliminar (void* especialidad_a){
    especialidad_t* especialidad = (especialidad_t*)especialidad_a;
    especialidad_eliminar(especialidad);
}

//***************************  CLÌNICA  ***************************


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
    free (clinica);
    return;
}

size_t clinica_obtener_espera(clinica_t* clinica , char* nombre_especialidad){
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

bool clinica_existe_doctor (clinica_t* clinica , char* nombre){
    return abb_obtener(clinica->doctores , nombre) != NULL;
}

bool clinica_pedir_turno(clinica_t *clinica, char* nombre_paciente, char* nombre_especialidad, bool urgencia){
    paciente_t* paciente = clinica_obtener_paciente(clinica , nombre_paciente);
    if (paciente == NULL){
        printf (ENOENT_PACIENTE, nombre_paciente);
        return false;
    }
    especialidad_t* especialidad = clinica_obtener_especialidad(clinica , nombre_especialidad);
    if (especialidad == NULL){
        printf (ENOENT_ESPECIALIDAD , nombre_especialidad);
        return false;
    }
    if (urgencia)
        paciente_pedir_turno_urgente(paciente, especialidad);
    else
        paciente_pedir_turno_regular(paciente, especialidad);
    especialidad->sin_atender++;
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
    paciente_t* paciente = doctor_atender_paciente(doctor);
    printf (PACIENTE_ATENDIDO,paciente->nombre);
    printf (CANT_PACIENTES_ENCOLADOS, especialidad->sin_atender, especialidad->nombre);
    return;
}

void clinica_crear_informe(clinica_t *clinica, char *ini, char *fin){
    size_t cont = 0;
    abb_in_order(clinica->doctores, ini, fin, contar_doctores, &cont);
    printf (DOCTORES_SISTEMA,cont);
    cont = 0;
    abb_in_order(clinica->doctores, ini, fin, imprimir_doctores, &cont);
}


//***************************  CARGA DE DATOS  ***************************


void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

int cargar_doctores_y_especialidades (char* archivo , abb_t* doctores , hash_t* especialidades){
	char* linea = NULL;
	size_t buffer = 0;
	FILE* archivo_abierto = fopen(archivo , "r");
	if (archivo_abierto == NULL){
        printf (ENOENT_ARCHIVO,archivo);
		return 1;
	}
	while (getline(&linea , &buffer , archivo_abierto) > 0){
		eliminar_fin_linea(linea);
		char** campos = split (linea , ',');
		if (!hash_pertenece (especialidades , campos[1])){
			char* nombre_especialidad = malloc (sizeof(char)*strlen(campos[1])+1);
			strcpy (nombre_especialidad , campos[1]);
			especialidad_t* especialidad = especialidad_crear (nombre_especialidad);
			hash_guardar (especialidades , campos[1] , especialidad);
		}
        char* nombre_doctor = malloc (sizeof(char)*strlen(campos[0])+1);
        strcpy (nombre_doctor, campos[0]);
		especialidad_t* especialidad = hash_obtener (especialidades , campos[1]);
		doctor_t* doctor = doctor_crear (nombre_doctor , especialidad);
		abb_guardar (doctores , campos[0] , doctor);
		free_strv(campos);
	}
    free (linea);
    fclose (archivo_abierto);
	return 0;
}

int procesar_pacientes (char* archivo , hash_t* pacientes){
	char* linea = NULL;
	size_t buffer = 0;
	FILE* archivo_abierto = fopen(archivo , "r");
	if (archivo_abierto == NULL){
        printf (ENOENT_ARCHIVO,archivo);
		return 1;
	}
	while (getline(&linea , &buffer , archivo_abierto) > 0){
		eliminar_fin_linea(linea);
		char** campos = split (linea , ',');
		char* nombre_paciente = malloc (sizeof(campos[0])+1);
		strcpy (nombre_paciente, campos[0]);
		if (!isdigit (campos[1])){
            printf (ENOENT_ANIO,campos[1]);
			return 1;
		}
		size_t anio = (size_t)(campos[1]);
		paciente_t* paciente = paciente_crear (nombre_paciente , anio);
		hash_guardar (pacientes , campos[0] , paciente);
		free_strv(campos);
	}
    free (linea);
    fclose(archivo_abierto);
	return 0;
}

abb_t* crear_abb_doctores (void){
    return abb_crear (strcmp, _doctor_eliminar);
}

hash_t* crear_hash_especialidades (void){
    return hash_crear (_especialidad_eliminar);
}


hash_t* cargar_pacientes(char* archivo){
    hash_t *pacientes = hash_crear(_paciente_eliminar);
    if (procesar_pacientes(archivo, pacientes) == 1) return NULL;
    return pacientes;
}
