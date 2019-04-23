/*
 * InterfaceDAO.h
 *
 *  Created on: 18 abr. 2019
 *      Author: tarek
 */

#ifndef SRC_INTERFACEDAO_H_
#define SRC_INTERFACEDAO_H_

//TODO cambiar arrays por listas enlazadas.


#define MAX_LEN			25
#define MAX_ASIGNATURA	10

typedef struct{
	int id;
	int cursoId;
	int nota;
	char nombre[MAX_LEN];
}asignatura_t;


typedef struct{
	int id;
	char nombre[MAX_LEN];
	char apellido1[MAX_LEN];
	char apellido2[MAX_LEN];
	char mail[MAX_LEN];
	asignatura_t asignatura[MAX_ASIGNATURA];
	int numeroAsignaturas;
}alumno_t;


typedef struct{
	int id;
	asignatura_t asignatura[MAX_ASIGNATURA];
}curso_t;

alumno_t *DAO_getAlumno(char *nombreCompleto);
alumno_t DAO_getCurso(char *curso);
alumno_t DAO_getAsignatura(char *asignatura);



#endif /* SRC_INTERFACEDAO_H_ */
