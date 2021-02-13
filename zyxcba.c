#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "zyxcba_lib.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void procesar_comando(const char* comando, const char** parametros, clinica_t* clinica) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
	    paciente_t* paciente = clinica_obtener_paciente (clinica , parametros[0]);
		if (paciente == NULL){
			printf (ENOENT_PACIENTE, parametros[0]);
		}
		especialidad_t* especialidad = clinica_obtener_especialidad (clinica , parametros[1]);
		if (especialidad == NULL){
			printf (ENOENT_ESPECIALIDAD , parametros[1]);
		}
		if (strcmp(parametros[2],"URGENTE")==0){
			if (clinica_pedir_turno_urgente(paciente , especialidad)){
				size_t pacientes_espera = clinica_obtener_espera (clinica , parametros[1]);
				printf (PACIENTE_ENCOLADO, parametros[0]);
				printf (CANT_PACIENTES_ENCOLADOS, pacientes_espera , parametros[1]);
			}
		}
		else if (strcmp (parametros[2],"REGULAR")==0){
			if (clinica_pedir_turno_regular (paciente , especialidad)){
				size_t pacientes_espera = clinica_obtener_espera (clinica, parametros[1]);
				printf (PACIENTE_ENCOLADO, parametros[0]);
				printf (CANT_PACIENTES_ENCOLADOS, pacientes_espera , parametros[1]);
			}
		}
		else{
			printf (ENOENT_URGENCIA , parametros[2]);
		}
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (clinica_obtener_doctor (clinica , parametros[0])== NULL){
			printf (ENOENT_DOCTOR , parametros[0]);
		}
		else{
			clinica_atender_paciente(clinica,parametros[0]);
		}
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {

	} else {

	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(clinica_t* clinica) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros, clinica);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

int procesar_archivo_doctores (char* archivo , abb_t* doctores , hash_t* especialidades){
	char* linea = NULL;
	size_t buffer = 0;
	FILE* archivo_abierto = fopen(archivo , r);
	if (archivo_abierto == NULL){
		return 1;
	}
	while (getline(&linea , &buffer , archivo) > 0){
		eliminar_fin_linea(linea);
		char** campos = split (linea , ',');
		if (!hash_pertenece (especialidades , campos[1])){
			char* nombre_especialidad = malloc (sizeof (campos[1]));
			strcpy (nombre_especialidad , campos[1]);
			especialidad_t* especialidad = especialidad_crear (nombre_especialidad);
			hash_guardar (especialidades , campos[1] , especialidad);
		}
		especialidad_t* especialidad = hash_obtener (especialidades , campos[1]);
		doctor_t* doctor = doctor_crear (campos[0] , especialidad);
		abb_guardar (doctores , campos[0] , doctor);
		free_strv(campos);
	}
	return 0;
}

int procesar_archivo_pacientes (char* archivo , hash_t* pacientes){
	char* linea = NULL;
	size_t buffer = 0;
	FILE* archivo_abierto = fopen(archivo , r);
	if (archivo_abierto == NULL){
		return 1;
	}
	while (getline(&linea , &buffer , archivo_abierto) > 0){
		eliminar_fin_linea(linea);
		char** campos = split (linea , ',');
		char* nombre_paciente = malloc (sizeof(campos[0]));
		strcpy (nombre_paciente, campos[0]);
		if (!isdigit (campos[1])){
			return 1;
		}
		size_t año = size_t(campos[1]);
		paciente_t* paciente = paciente_crear (nombre_paciente , año);
		hash_guardar (pacientes , campos[0] , paciente);
		free_strv(campos);
	}
	return 0;
}


int main(int argc, char** argv) {
	if (argc != 2){
		return 1;
	}
	hash_t* pacientes = hash_crear(paciente_eliminar);
	abb_t* doctores = abb_crear (strcmp , doctor_eliminar);
	hash_t* especialidades = hash_crear (especialidad_eliminar);
	int aux = procesar_archivo_doctores (argv[0] , doctores , especialidades);
	if (aux == 1){
		return 1;
	}
	aux = procesar_archivo_pacientes(argv[1] , pacientes);
	if (aux == 1){
		return 1;
	}
	clinica_t* clinica = clinica_crear (especialidades , doctores , pacientes);
	procesar_entrada(clinica);
	clinica_destruir (clinica);
	return 0;
}
