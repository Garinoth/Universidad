/*
   Incluya en este fichero todas las implementaciones que pueden
   necesitar compartir todos los módulos (editor, subscriptor y
   proceso intermediario), si es que las hubiera.
*/

#include "comun.h"

/* FUNCION DE ERROR */
void error (int e, int out){ // e = error code, out = flag to exit
    switch (e) {
        case 1: m = "No se puede ejecutar"; break;
        case 2: m = "Timeout"; break;
        case 3: m = "Recurso por encima del limite"; break;

    }
    fprintf(stderr, "%s: Error(%d), %s\n", Name, e, m);
    if(out) exit(e);
}
