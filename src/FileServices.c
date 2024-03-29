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
#define SHIFT_NOMBRE		1
#define SHIFT_HEADER		2
#define BLANK				"                                                                  "
#define DEBUG
/*************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************/
//TODO función a refactorizar
//TODO ordenar notas por Id.

int FileService_getAlumno(alumno_t * alumno);
int FileService_getAsignaturas(asignatura_t **asignaturas);
void FileService_getNotaMedia(FILE *fp, asignatura_t **asignaturas);
void FileService_getAsignaturasInfo(FILE *fp, asignatura_t **asignaturas);
void FileService_goToTable(FILE *fp, char const * table);
int FileService_getAlumnoInfo(FILE *fp, alumno_t *alumno);
void FileService_getAlumnoNotas(FILE *fp, alumno_t *alumno);
void FileService_getNombreNotas(FILE *fp, alumno_t *alumno);
int FileService_borrarAlumno(alumno_t *alumno);
int isNotNull(char *s);
void p(char *s);
/***************************************************************
 * EXPORTED FUNCTIONS
 ***************************************************************/
alumno_t *DAO_getAlumno(char *nombreCompleto){
	alumno_t *alumno = (alumno_t *)malloc(sizeof(alumno_t));
	char *nombre, *apellido1, *apellido2;
	int result;
	nombre = strtok(nombreCompleto, " ");
	apellido1 = strtok(NULL, " ");
	apellido2 = strtok(NULL, " ");
	if(isNotNull(nombre) && isNotNull(apellido1) && isNotNull(apellido2)){
		strcpy(alumno->nombre, nombre);
		strcpy(alumno->apellido1, apellido1);
		strcpy(alumno->apellido2, apellido2);
		alumno->numeroAsignaturas = 0;
		result = FileService_getAlumno(alumno);
	}else{
		result = -1;
	}


	if(result == -1) alumno->id = result;
	return alumno;
}

asignatura_t **DAO_getAsignatura(){
	asignatura_t **asignaturas;
	asignaturas = malloc(MAX_ASIGNATURA*sizeof(asignatura_t *));
	for(int i = 0; i < MAX_ASIGNATURA; i++){
		asignaturas[i] = malloc(sizeof(asignatura_t));
		asignaturas[i]->nota = 0.0;
		if(asignaturas[i] == NULL) printf("Error al asignar memoria");
	}
	FileService_getAsignaturas(asignaturas);
	return asignaturas;
}

int DAO_borratAlumno(char *nombreCompleto){
	alumno_t *alumno = (alumno_t *)malloc(sizeof(alumno_t));
	int result;
	char *nombre, *apellido1, *apellido2;
	nombre = strtok(nombreCompleto, " ");
	apellido1 = strtok(NULL, " ");
	apellido2 = strtok(NULL, " ");
	if(isNotNull(nombre) && isNotNull(apellido1) && isNotNull(apellido2)){
		strcpy(alumno->nombre, nombre);
		strcpy(alumno->apellido1, apellido1);
		strcpy(alumno->apellido2, apellido2);
		result = FileService_borrarAlumno(alumno);
	}
	return result;
}
/***************************************************************
 * LOCAL FUNCTIONS
 */

int FileService_borrarAlumno(alumno_t *alumno){
	FILE *fp = fopen(PATH_FILE, "r+");
	char *nombre, *apellido1, *apellido2;
	char linea[MAX_LEN], aux[MAX_LEN];
	int found = -1;
	do{
		fgets(linea, MAX_LEN, fp);
		strcpy(aux, linea);
		strtok(linea, ", ");
		nombre = strtok(NULL, ", ");
		apellido1 = strtok(NULL, ", ");
		apellido2 = strtok(NULL, ", ");
		if(isNotNull(nombre) && isNotNull(apellido1) && isNotNull(apellido2)){
			if(strcmp(nombre, alumno->nombre) ==  0 &&
				strcmp(apellido1, alumno->apellido1) == 0 &&
				strcmp(apellido2, alumno->apellido2) == 0){
				fwrite(BLANK, sizeof(char), 66, fp);
				found = 1;
			}else{
				fwrite(aux, sizeof(char), strlen(aux), fp);
			}
		}
	}while(feof(fp) == 0);
	return found;
}

int FileService_getAsignaturas(asignatura_t **asignaturas){
	int result = 0;
	FILE *fp = fopen(PATH_FILE, "r");
	if(fp == NULL) result = -1;
	FileService_goToTable(fp, TABLA_ASIGNATURAS);
	FileService_getAsignaturasInfo(fp, asignaturas);
	FileService_goToTable(fp, TABLA_NOTAS);
	FileService_getNotaMedia(fp, asignaturas);
	fclose(fp);
	return result;
}

void FileService_getAsignaturasInfo(FILE *fp, asignatura_t **asignaturas){
	char *id, *cursoId, *nombre;
	char linea[MAX_LEN];
	int i = 0;
	do{
		fgets(linea, MAX_LEN, fp);
		id = strtok(linea, ", ");
		cursoId = strtok(NULL, ", ");
		nombre = strtok(NULL, "\n");
		if(isNotNull(id) && isNotNull(cursoId) && isNotNull(nombre)){
			asignaturas[i]->id = strtol(id, 0, 10);
			asignaturas[i]->cursoId = strtol(cursoId, 0, 10);
			strcpy(asignaturas[i]->nombre, nombre + SHIFT_NOMBRE);
			i++;
		}
	}while(linea[0] != '#' && feof(fp) == 0 && i < MAX_ASIGNATURA);
}

void FileService_getNotaMedia(FILE *fp, asignatura_t **asignaturas){
	char *alumnoId, *asignaturaId, *nota;
	char linea[MAX_LEN];
	int count = 0;
	for(int i = 0; i < MAX_ASIGNATURA; i++){
		do{
			fgets(linea, MAX_LEN, fp);
			alumnoId = strtok(linea, ", ");
			asignaturaId = strtok(NULL, ", ");
			nota = strtok(NULL, ", ");
			if(isNotNull(alumnoId) && isNotNull(asignaturaId) && isNotNull(nota)){
				if(asignaturas[i]->id == strtol(asignaturaId, 0, 10)){
					asignaturas[i]->nota += strtof(nota, NULL);
					count++;
				}
			}
		}while(linea[0] == '#' && feof(fp) == 0);
		asignaturas[i]->nota = asignaturas[i]->nota/count;
		count = 0;
	}
}

void p(char *s){
#ifdef DEBUG
	printf("%s\n", s);
#endif
}

void FileService_goToTable(FILE *fp, char const * table){
	rewind(fp);
	int found = 0;
	char linea[MAX_LEN], *aux;
	do{
		fgets(linea, MAX_LEN, fp);
		if(linea[0] == '#'){
			aux = strtok(linea + SHIFT_HEADER, " ");
			if(strcmp(aux, table) == 0) found = 1;
		}
	}while(feof(fp) == 0 && found == 0);

}

int FileService_getAlumnoInfo(FILE *fp, alumno_t *alumno){
	char *id, *nombre, *apellido1, *apellido2, *email;
	char linea[MAX_LEN];
	int found = -1;
	do{
		fgets(linea, MAX_LEN, fp);
		id = strtok(linea, ", ");
		nombre = strtok(NULL, ", ");
		apellido1 = strtok(NULL, ", ");
		apellido2 = strtok(NULL, ", ");
		email = strtok(NULL, ", ");
		if(isNotNull(nombre) && isNotNull(apellido1) && isNotNull(apellido2)){
			if(strcmp(nombre, alumno->nombre) ==  0 &&
				strcmp(apellido1, alumno->apellido1) == 0 &&
				strcmp(apellido2, alumno->apellido2) == 0){
				alumno->id = strtol(id, 0, 10);
				strcpy(alumno->mail, email);
				found = 1;
			}
		}

	}while(linea[0] != '#' && feof(fp) == 0 && found == -1);
	return found;
}

void FileService_getAlumnoNotas(FILE *fp, alumno_t *alumno){
	char *alumnoId, *asignaturaId, *nota;
	char linea[MAX_LEN];
	do{
		fgets(linea, MAX_LEN, fp);
		alumnoId = strtok(linea, ", ");
		asignaturaId = strtok(NULL, ", ");
		nota = strtok(NULL, ", ");
		if(isNotNull(alumnoId) && isNotNull(asignaturaId)
				&& isNotNull(nota)){
			if(strtol(alumnoId, 0, 10) == alumno->id){
				alumno->asignatura[alumno->numeroAsignaturas].id =
						strtol(asignaturaId, 0, 10);
				alumno->asignatura[alumno->numeroAsignaturas].nota =
						strtof(nota, NULL);
				alumno->numeroAsignaturas++;
			}
		}
	}while(linea[0] != '#' && feof(fp) == 0);
}

void FileService_getNombreNotas(FILE *fp, alumno_t *alumno){
	char *id, *cursoId, *nombre;
	char linea[MAX_LEN];
	do{
		fgets(linea, MAX_LEN, fp);
		id = strtok(linea, ", ");
		cursoId = strtok(NULL, ", ");
		nombre = strtok(NULL, "\n");
		if(isNotNull(id) && isNotNull(cursoId) && isNotNull(nombre)){
			for(int i = 0; i < alumno->numeroAsignaturas; i++){
				if(alumno->asignatura[i].id == strtol(id, 0, 10)){
					alumno->asignatura[i].cursoId = strtol(cursoId, 0, 10);
					strcpy(alumno->asignatura[i].nombre, nombre + SHIFT_NOMBRE);
				}
			}
		}
	}while(feof(fp) == 0 && linea[0] != '#');
}

int FileService_getAlumno(alumno_t * alumno){
	int result;
	FILE *fp = fopen(PATH_FILE, "r");
	FileService_goToTable(fp, TABLA_ALUMNOS);
	result = FileService_getAlumnoInfo(fp, alumno);
	if(result != -1){
		FileService_goToTable(fp, TABLA_NOTAS);
		FileService_getAlumnoNotas(fp, alumno);
		FileService_goToTable(fp, TABLA_ASIGNATURAS);
		FileService_getNombreNotas(fp, alumno);
	}
	fclose(fp);
	return result;
}

int isNotNull(char *s){
	return s != NULL;
}
