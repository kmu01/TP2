#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensajes.h"
#include "zyxcba_lib.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

bool hay_suficientes_parametros(char **params, int cant){
	int cont = 0;
	for (int i=0;params[i];i++) cont++;
	return cant == cont;
}

void procesar_comando(const char* comando, char** parametros, clinica_t* clinica) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
	    if (!hay_suficientes_parametros(parametros,3)){
			printf(ENOENT_PARAMS, comando);
		}
		else if (strcmp(parametros[2],"URGENTE")==0){
			if (clinica_pedir_turno(clinica, parametros[0], parametros[1], true)){
				size_t pacientes_espera = clinica_obtener_espera (clinica , parametros[1]);
				printf (PACIENTE_ENCOLADO, parametros[0]);
				printf (CANT_PACIENTES_ENCOLADOS, pacientes_espera , parametros[1]);
			}
		}
		else if (strcmp (parametros[2],"REGULAR")==0){
			if (clinica_pedir_turno(clinica, parametros[0], parametros[1], false)){
				size_t pacientes_espera = clinica_obtener_espera (clinica, parametros[1]);
				printf (PACIENTE_ENCOLADO, parametros[0]);
				printf (CANT_PACIENTES_ENCOLADOS, pacientes_espera , parametros[1]);
			}
		}
		else{
			printf (ENOENT_URGENCIA , parametros[2]);
		}
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!hay_suficientes_parametros(parametros,1)){
			printf(ENOENT_PARAMS, comando);
		}
		else if (!clinica_existe_doctor(clinica , parametros[0])){
			printf (ENOENT_DOCTOR , parametros[0]);
		}
		else{
			clinica_atender_paciente(clinica,parametros[0]);
		}
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if (!hay_suficientes_parametros(parametros,2)){
			printf(ENOENT_PARAMS, comando);
		}
		else{
			char* inicio = parametros[0];
			char* final = parametros[1];
			clinica_crear_informe(clinica, inicio, final);
		}
	} else {
		printf(ENOENT_CMD, comando);
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


int main(int argc, char** argv) {
	if (argc != 3){
		return 1;
	}

	abb_t* doctores = NULL;
	hash_t* especialidades = NULL;
	int aux = cargar_doctores_y_especialidades(argv[0], doctores, especialidades);
	if (aux == 1){
		return 1;
	}
	hash_t* pacientes = cargar_pacientes(argv[1]);
	if (!pacientes || !doctores || !especialidades) {
		if (pacientes) hash_destruir(pacientes);
		if (doctores) abb_destruir(doctores);
		if (especialidades) hash_destruir(especialidades);
		return 1;
	}
	clinica_t* clinica = clinica_crear (especialidades , doctores , pacientes);
	procesar_entrada(clinica);
	hash_destruir(pacientes);
	abb_destruir(doctores);
	hash_destruir(especialidades);
	clinica_destruir (clinica);
	return 0;
}
