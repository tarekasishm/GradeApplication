#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "InterfaceDAO.h"
#include "GradeApp.h"



int main(){

//	char opcion[25];
//	sprintf(opcion, "Nerea Zubillaga Garin");
//	alumno_t *alumno = DAO_getAlumno(opcion);
//	for(int i = 0; i < alumno->numeroAsignaturas; i++){
//		printf("nota %4d Asignatura: %s\n",
//							alumno->asignatura[i].nota,
//							alumno->asignatura[i].nombre);
//	}
	if(login() != -1) showMenu();
	return 1;
}


void showMenu(){
	char arg, opcion1[128], opcion2[128];
	alumno_t *alumno;
	asignatura_t **asignaturas;
	int curso = 0, n = 0;
	float notaMedia = 0.0;
	do{
		printf("-----------Aplicacion de notas-----------\n");
		printf("\t 1. Obtener notas por alumno\n");
		printf("\t 2. Nota por asignatura por alumno\n");
		printf("\t 3. Nota media de una asignatura\n");
		printf("\t 4. Nota media por asignaturas\n");
		printf("\t 5. Nota media por curso\n");
		printf("\t q. Salir\n");
		scanf("%c", &arg);
		while(getchar() != '\n');
		switch(arg){
		case '1':
			printf("Introduce nombre de alumno\n");
			getOpcion(opcion1);
			alumno = DAO_getAlumno(opcion1);
			if(alumno->numeroAsignaturas != 0){
				for(int i = 0; i < alumno->numeroAsignaturas; i++){
					printf("Asignatura: %18s nota: %d\n",
										alumno->asignatura[i].nombre,
										alumno->asignatura[i].nota);
				}
			}else {
				printf("El alumno indicado no está registrado\n");
			}
			free(alumno);
			break;
		case '2':
			printf("Introduce nombre de alumno: ");
			getOpcion(opcion1);
			printf("Introduce nombre de la asignatura: ");
			getOpcion(opcion2);
			alumno = DAO_getAlumno(opcion1);
			int i = buscarAsignatura(opcion2, alumno);
			if(alumno->id == -1){
				printf("Alumno no matriculado en el centro\n");
			}else{
				if(i != -1){
					printf("Asignatura: %s nota: %d\n", alumno->asignatura[i].nombre,
							alumno->asignatura[i].nota);
				}else{
					printf("Alumno no matriculado de la asignatura\n");
				}
			}
			free(alumno);
					break;
		case '3':
			asignaturas = DAO_getAsignatura();
			printf("Introduzca asignatura: ");
			getOpcion(opcion2);
			for(int i = 0; i < MAX_ASIGNATURA; i++){
				if(strcmp(opcion2, asignaturas[i]->nombre) == 0){
					printf("Nota media: %2d Asignatura: %s\n", asignaturas[i]->nota, asignaturas[i]->nombre);
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
				printf("Nota media: %2d Asignatura: %s\n", asignaturas[i]->nota, asignaturas[i]->nombre);
			}
			for(int i = 0; i < MAX_ASIGNATURA; i++){
				free(asignaturas[i]);
			}
			free(asignaturas);
					break;
		case '5':
			asignaturas = DAO_getAsignatura();
			printf("Introduzca curso ");
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
			printf("La nota media del curso %d es %f\n", curso, notaMedia);
					break;
		case 'q':
			break;
		default:
			printf("Introduce una opción válida\n");
		}
	}while(arg != 'q');
}

void getOpcion(char *s){
	fgets(s, 128, stdin);
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
	char user[128], paswd[128];
	do{
		printf("Introduzca usuario: ");
		getOpcion(user);
		if(strcmp(user, "admin") == 0){
			printf("Introduzca password: ");
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
