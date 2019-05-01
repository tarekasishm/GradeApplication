/*
 * InterfaceDAO.h
 *
 *  Created on: 18 abr. 2019
 *      Author: tarek
 */

#ifndef SRC_INTERFACEDAO_H_
#define SRC_INTERFACEDAO_H_

//TODO cambiar arrays por listas enlazadas.
//TODO Darle una pensada a la estructura de datos.

#define FILE_PATH "../src/datos.txt"

#define MAX_LEN				128
#define MAX_LENG			25
#define MAX_ASIGNATURA	10

typedef struct{
	int id;
	int cursoId;
	float nota;
	char nombre[MAX_LENG];
}asignatura_t;


typedef struct{
	int id;
	char nombre[MAX_LENG];
	char apellido1[MAX_LENG];
	char apellido2[MAX_LENG];
	char mail[MAX_LENG];
	asignatura_t asignatura[MAX_ASIGNATURA];
	int numeroAsignaturas;
}alumno_t;


typedef struct{
	int id;
	asignatura_t asignatura[MAX_ASIGNATURA];
}curso_t;

alumno_t *DAO_getAlumno(char *nombreCompleto);
void DAO_borratAlumno(char *nombreCompleto);
asignatura_t **DAO_getAsignatura();



#endif /* SRC_INTERFACEDAO_H_ */
