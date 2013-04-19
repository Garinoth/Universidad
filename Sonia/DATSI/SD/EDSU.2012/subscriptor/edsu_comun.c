/*
   Incluya en este fichero todas las implementaciones que pueden
   necesitar compartir los módulos editor y subscriptor,
   si es que las hubiera.
*/

#include "edsu_comun.h"
#include "comun.h"

int conMediator (int sd) {
    
    char* mediator = getenv("SERVIDOR");
    int port = atoi(getenv("PUERTO"));

    struct hostent *Nmediator;
    Nmediator = gethostbyname(mediator);

    struct sockaddr_in s_ain_TCP;

    bzero((char *)&s_ain_TCP, sizeof(s_ain_TCP));
    s_ain_TCP.sin_family = AF_INET;
    memcpy(&s_ain_TCP.sin_addr.s_addr, Nmediator->h_addr, Nmediator->h_length);
    s_ain_TCP.sin_port = htons(port);

    int con;

    if ((con = connect(sd, (struct sockaddr*)&s_ain_TCP, sizeof(struct sockaddr_in))) < 0){
    return -1;
    }

    return con;
}
