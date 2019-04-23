/*
 * FileServices.c
 *
 *  Created on: 18 abr. 2019
 *      Author: tarek
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "InterfaceDAO.h"

/************************************************************
 * LOCAL DEFINES AND GLOBAL VARIABLES
 ************************************************************/
#define TABLA_ALUMNOS		"Alumno"
#define TABLA_ASIGNATURAS	"Asignatura"
#define TABLA_CURSO			"Curso"
#define TABLA_NOTAS			"Notas"
#define ALUMNOS				1
#define ASIGNATURAS			2
#define CURSO				3
#define NOTAS				4
/*************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************/
//TODO función a refactorizar

int FileService_getAlumno(alumno_t * alumno);
int FileService_esTabla(char *line);
int FileServive_getInfoAlumno(FILE *fp, alumno_t *alumno);
void FileService_getAsignaturasPorAlumno(FILE *fp, alumno_t *alumno);
/***************************************************************
 * EXPORTED FUNCTIONS
 ***************************************************************/
alumno_t *DAO_getAlumno(char *nombreCompleto){
	alumno_t *alumno = (alumno_t *)malloc(sizeof(alumno_t));
	char *aux;
	int result;
	aux = strtok(nombreCompleto, " ");
	strcpy(alumno->nombre, aux);
	aux = strtok(NULL, " ");
	strcpy(alumno->apellido1, aux);
	aux = strtok(NULL, " ");
	strcpy(alumno->apellido2, aux);
	alumno->numeroAsignaturas = 0;
	result = FileService_getAlumno(alumno);

	if(result == -1) alumno->id = result;
	return alumno;
}


/***************************************************************
 * LOCAL FUNCTIONS
 */
int FileService_getAlumno(alumno_t * alumno){
	FILE *fp;
	int result, tabla;
	char linea[100];
	fp = fopen(PATH_FILE, "r");
	if(fp == NULL){
		printf("Error al abrir el fichero\n");
		result = -1;
	}else{
		do{
			fgets(linea,100, fp); //Lectura de una línea... a mejorar.
			if(linea[0] == '#'){ // Comienzo de una tabla
				tabla = FileService_esTabla(linea);
				if(tabla == ALUMNOS){
					result = FileServive_getInfoAlumno(fp, alumno);
				}
				if(tabla  == NOTAS){
					printf("Tabla de notas\n");
					FileService_getAsignaturasPorAlumno(fp, alumno);
				}
			}
		}while(feof(fp) == 0);
	}
	fclose(fp);
	return result;
}
void FileService_getAsignaturasPorAlumno(FILE *fp, alumno_t *alumno){
	char linea[100];
	int alumnoId, asignaturaId, nota;
	do{
		fgets(linea, 100, fp);
		alumnoId = strtol(strtok(linea, ", "), 0, 10);

		if(alumnoId == alumno->id){
			asignaturaId = strtol(strtok(NULL, ", "), 0, 10);
			nota = strtol(strtok(NULL, ", "), 0, 10);
			alumno->asignatura[alumno->numeroAsignaturas].id = asignaturaId;
			alumno->asignatura[alumno->numeroAsignaturas].nota = nota;
			alumno->numeroAsignaturas++;
		}
	}while(linea[0] != '#' && feof(fp) == 0);

}
int FileService_esTabla(char *line){
	int result = 0;
	char *token;
	token = strtok(line, "# ");
	if(strcmp(token, TABLA_ALUMNOS) == 0) result = ALUMNOS;
	else if(strcmp(token, TABLA_CURSO) == 0) result = CURSO;
	else if(strcmp(token, TABLA_NOTAS) == 0) result = NOTAS;
	else if(strcmp(token, TABLA_ASIGNATURAS) == 0) result = ASIGNATURAS;
	return result;
}

int FileServive_getInfoAlumno(FILE *fp, alumno_t *alumno){
	int result = -1;
	char linea[100];
	char *nombre, *primerApellido, *segundoApellido, *id;
	fgets(linea, 100, fp);
	while(linea[0] != '\n' && result == -1){
		id = strtok(linea, ", ");
		nombre = strtok(NULL, ", ");
		primerApellido = strtok(NULL, ", ");
		segundoApellido = strtok(NULL, ", ");
//		printf("%s %s %s\n", nombre, primerApellido, segundoApellido);
		if((strcmp(nombre, alumno->nombre) == 0) &&
			(strcmp(primerApellido, alumno->apellido1) == 0) &&
			(strcmp(segundoApellido, alumno->apellido2) == 0)){
				//printf("%s", linea);
				//-- El nombre se establece al principio. Esto sobra.
				strcpy(alumno->nombre, nombre);
				strcpy(alumno->apellido1, primerApellido);
				strcpy(alumno->apellido2, segundoApellido);
				alumno->id = strtol(id, 0, 10);
				strcpy(alumno->mail, strtok(NULL, ", "));
				result = 1;
		}
		fgets(linea, 100, fp);
	}
	return result;
}


