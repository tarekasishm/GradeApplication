#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "InterfaceDAO.h"
#include "GradeApp.h"



int main(){
	if(login() != -1) showMenu();
	return 1;
}


void showMenu(){
	char arg, opcion1[MAX_LEN], opcion2[MAX_LEN];
	alumno_t *alumno;
	asignatura_t **asignaturas;
	int curso = 0, n = 0;
	float notaMedia = 0.0;
	do{
		printf("-----------GRADE APPLICATION-----------\n");
		printf("\t 1. Get student grades\n");
		printf("\t 2. Get a student grade\n");
		printf("\t 3. Get a subject average grade\n");
		printf("\t 4. Get subjects average grades\n");
		printf("\t 5. Get a course average grade\n");
		printf("\t 6. Delete student\n");
		printf("\t q. Exit\n");
		scanf("%c", &arg);
		while(getchar() != '\n');
		switch(arg){
		case '1':
			printf("Student name\n");
			getOpcion(opcion1);
			alumno = DAO_getAlumno(opcion1);
			if(alumno->numeroAsignaturas != 0){
				for(int i = 0; i < alumno->numeroAsignaturas; i++){
					printf("Subject: %18s Grade: %f\n",
										alumno->asignatura[i].nombre,
										alumno->asignatura[i].nota);
				}
			}else {
				printf("Student does not exist\n");
			}
			free(alumno);
			break;
		case '2':
			printf("Student name: ");
			getOpcion(opcion1);
			printf("Subject name: ");
			getOpcion(opcion2);
			alumno = DAO_getAlumno(opcion1);
			int i = buscarAsignatura(opcion2, alumno);
			if(alumno->id == -1){
				printf("Student does not exist\n");
			}else{
				if(i != -1){
					printf("Subject: %s grade: %f\n", alumno->asignatura[i].nombre,
							alumno->asignatura[i].nota);
				}else{
					printf("Alumno no matriculado de la asignatura\n");
				}
			}
			free(alumno);
					break;
		case '3':
			asignaturas = DAO_getAsignatura();
			printf("Subject name: ");
			getOpcion(opcion2);
			for(int i = 0; i < MAX_ASIGNATURA; i++){
				if(strcmp(opcion2, asignaturas[i]->nombre) == 0){
					printf("Average grade: %2f Subject: %s\n", asignaturas[i]->nota, asignaturas[i]->nombre);
				}
			}
			for(int i = 0; i < MAX_ASIGNATURA; i++){
				free(asignaturas[i]);
			}
			free(asignaturas);
					break;
		case '4':
			asignaturas = DAO_getAsignatura();
			for(int i = 0; i < MAX_ASIGNATURA; i++){
				printf("Average grade: %2f Subject: %s\n", asignaturas[i]->nota, asignaturas[i]->nombre);
			}
			for(int i = 0; i < MAX_ASIGNATURA; i++){
				free(asignaturas[i]);
			}
			free(asignaturas);
					break;
		case '5':
			asignaturas = DAO_getAsignatura();
			printf("Course: ");
			getOpcion(opcion2);
			notaMedia = 0;
			n = 0;
			curso = strtol(opcion2, 0, 10);
			for(int i = 0; i < MAX_ASIGNATURA; i++){
				if(curso == asignaturas[i]->cursoId){
					notaMedia += asignaturas[i]->nota;
					n++;
				}
			}
			notaMedia = notaMedia / n;
			printf("Course average grade %d es %f\n", curso, notaMedia);
					break;
		case '6':
			printf("Student name: ");
			getOpcion(opcion1);
			if(DAO_borratAlumno(opcion1) != -1){
				printf("Student deleted\n");
			}else{
				printf("Studnet not found\n");
			}
			break;
		case 'q':
			break;
		default:
			printf("Select a valid option.\n");
		}
	}while(arg != 'q');
}

void getOpcion(char *s){
	fgets(s, MAX_LEN, stdin);
	s[strlen(s) - 1] = '\0';
}

int buscarAsignatura(char *nombre, alumno_t *alumno){
	int found = -1;
	for(int i = 0; i < alumno->numeroAsignaturas; i++){
		if(strcmp(alumno->asignatura[i].nombre, nombre) == 0) found = i;
	}
	return found;
}
int login(){
	int result = -1, intentos = 0;
	char user[MAX_LEN], paswd[MAX_LEN];
	do{
		printf("User: ");
		getOpcion(user);
		if(strcmp(user, "admin") == 0){
			printf("Password: ");
			getOpcion(paswd);
			if(strcmp(paswd, "admin") != 0){
				intentos++;
			}else{
				result = 0;
			}
		}
	}while(intentos < 3 && strcmp(paswd, "admin") != 0);
	return result;
}
