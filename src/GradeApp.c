#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "InterfaceDAO.h"
#include "GradeApp.h"

#define FILE_PATH "../src/datos.txt"

int main(){
	char nombre[] = "Nerea Zubillaga Garin";
	alumno_t *alumno = DAO_getAlumno(nombre);
	if(alumno->id == -1){
		printf("No existe el/la alumno/a introducido.\n");
	} else{
		printf("%d %s %s %s %s\n",alumno->id, alumno->nombre,
					alumno->apellido1, alumno->apellido2, alumno->mail);
		for(int i = 0; i < alumno->numeroAsignaturas; i++){
			printf("Id: %d nota: %d\n",
					alumno->asignatura[i].id,
					alumno->asignatura[i].nota);
		}
	}

	return 1;
}
