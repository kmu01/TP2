#include "strutil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************
 *                		FUNCIONES AUXILIARES					   *
 *******************************************************************/

// Cuenta cuantas veces se encuentra el seprarador en un string
// Pre: Recibe un string y un caracter que represente el separador.
// Post: Devuelve la cantidad de veces que se encuentra al separador.
size_t contar_separadores(const char *str, char sep){
	size_t cont = 0;
	while (*str){
 		if (*str == sep) {
 			cont++;
 		}
 		str++;
 	}
 	return cont;
}

// Calcula el largo que va a tener el string unido, incluyendo el caracter separador.
// Pre: Recibe un arreglo de strings.
// Post: Devuelve el largo total que sería necesario tener para almacenarlos.
size_t calcular_largo_total(char** strv){
	size_t largo = 0;
	while (*strv){
 		largo += strlen(*strv)+2;
 		strv++;
 	}
 	return largo;
}

/*******************************************************************
 *                   IMPLEMENTACIÓN DE FUNCIONES				   *
 *******************************************************************/

char *substr(const char *str, size_t n){
	size_t tam = strlen(str);
	if (tam > n){
		tam = n;
	}
	char* nuevo = malloc(sizeof(char) * (tam+1));
	if (nuevo == NULL){
		return NULL;
	}
	memcpy(nuevo, str, tam);
	nuevo[tam] = '\0';
	return nuevo;
}

char **split(const char *str, char sep){
	size_t seps = contar_separadores(str, sep);
	char **cadena = malloc(sizeof(char*) * (seps+2));
	if (!cadena){
		return NULL;
	}
	int n_subcadena = 0;
	size_t pos_sig = 0;	
	for(size_t i = 0; i <= strlen(str); i++){
		if (str[i] == sep || str[i] == '\0'){		
			cadena[n_subcadena] = substr(str + pos_sig, i - pos_sig);
			pos_sig = i+1;
			n_subcadena++;
		}
	}
	cadena[n_subcadena] = NULL;
	return cadena;
}



char *join(char **strv, char sep){
	
	if (strv == NULL){
		return NULL;
	}
	size_t largo_total = calcular_largo_total(strv);
	if (largo_total == 0){
		largo_total = 1;
	}
	char* junta = malloc(sizeof(char*)*largo_total);
	if (!junta){
		return NULL;
	}
	junta[0] = '\0'; //Para que funcione strcat.
	size_t largo_actual = 0;
 	while (*strv){
 		size_t largo_str = strlen(*strv);
 		strcat(junta + largo_actual, *strv);
 		largo_actual += largo_str+1;
 		strv++;
 		if (*strv && sep != '\0'){
 			strncat(junta+largo_actual-1, &sep, 1);
 		}
 		else{
 			largo_actual--;
 		}
 	}
 	junta[largo_total] = '\0';
	return junta;
}

void free_strv(char *strv[]){
	for (int i = 0; strv[i]; i++){
		free(strv[i]);
	}
	free(strv);
}