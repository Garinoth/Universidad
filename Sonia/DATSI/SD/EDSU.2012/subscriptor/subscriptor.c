#include "subscriptor.h"
#include "comun.h"
#include "edsu_comun.h"

#include <pthread.h>

void (*reception)(const char*, const char*);

int sd_TCP;
struct sockaddr_in saddr_s, c_ain;
socklen_t size_TCP = sizeof(saddr_s), size_c_ain = sizeof(c_ain);
pthread_t thread;

void *connection(void *arg){
    int con;
    while(1 /* Bucle de procesar peticiones */){
        fprintf(stdout,"SUBSCRIPTOR: Esperando mensaje.\n");

        /* Esperamos la llegada de una conexion */
        if((con = accept(sd_TCP, (struct sockaddr *)&c_ain, &size_c_ain)) < 0){
            fprintf(stdout,"SUBSCRIPTOR: Llegada de un mensaje: ERROR\n");
        }
        else {
            fprintf(stdout,"SUBSCRIPTOR: Llegada de un mensaje: OK\n");
            int rd;
            Message m;
            while((rd += read(con, &m, sizeof(m))) && rd < sizeof(m));
            (*reception)(m.theme, m.value);
        }
    }
    close(con);
    return NULL;
}

int alta_subscripcion_tema(const char *tema) {
    if (reception == NULL) return -1;

    int sd;
    if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        return -1;
    }

    if ((conMediator(sd)) < 0) {
        return -1;
    }

    sendMessageSubs(sd, SUBSCRIBE, tema, saddr_s);

    Message m;
    recv(sd, &m, sizeof(m), 0);

    if (m.op == OK) return 0;
    else return -1;
}

int baja_subscripcion_tema(const char *tema) {
    if (reception == NULL) return -1;
    int sd;
    if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        return -1;
    }

    if ((conMediator(sd)) < 0) {
        return -1;
    }

    sendMessageSubs(sd, UNSUBSCRIBE, tema, saddr_s);

    Message m;
    recv(sd, &m, sizeof(m), 0);

    if (m.op == OK) return 0;
    else return -1;
}

int inicio_subscriptor(void (*notif_evento)(const char *, const char *),
                void (*alta_tema)(const char *),
                void (*baja_tema)(const char *)) {
    reception = notif_evento;


    /* Creacion del socket TCP de servicio */
    if((sd_TCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        fprintf(stdout,"SUBSCRIPTOR: Creacion del socket TCP: ERROR\n");
        return -1;
    }
    else{
        fprintf(stdout,"SUBSCRIPTOR: Creacion del socket TCP: OK\n");
    }

    /* Asignacion de la direccion local Puerto TCP*/
    bzero((char *)&saddr_s, sizeof(saddr_s));
    saddr_s.sin_family = AF_INET;
    saddr_s.sin_addr.s_addr = INADDR_ANY;
    saddr_s.sin_port = htons(0);
    if (bind(sd_TCP, (struct sockaddr *)&saddr_s, size_TCP) < 0){
        fprintf(stdout,"SUBSCRIPTOR: Asignacion del puerto cliente\n");
        return -1;
    }
    else if (getsockname(sd_TCP, (struct sockaddr *)&saddr_s, &size_TCP) < 0){
        fprintf(stdout,"SUBSCRIPTOR: Asignacion del puerto cliente: ERROR\n");
        return -1;
    }
    else{
        fprintf(stdout,"SUBSCRIPTOR: Asignacion del puerto cliente: OK\n");
    }

    /* Aceptamos conexiones por el socket */
    if (listen(sd_TCP,0) < 0){
        fprintf(stdout,"SUBSCRIPTOR: Aceptacion de peticiones: ERROR\n");
        return -1;
    }
    else{
        fprintf(stdout,"SUBSCRIPTOR: Aceptacion de peticiones: OK\n");
    }
  
    /* Puerto TCP ya disponible */
    fprintf(stdout,"SUBSCRIPTOR: Puerto TCP reservado: OK\n");

    pthread_create (&thread, NULL, &connection, NULL);

    return 0;
}


int fin_subscriptor() {
	return 0;
}

