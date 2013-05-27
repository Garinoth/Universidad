/*
   Incluya en este fichero todas las implementaciones que pueden
   necesitar compartir todos los módulos (editor, subscriptor y
   proceso intermediario), si es que las hubiera.
*/

#include "comun.h"

void sendMessage (int sd, int op, const char* theme, const char* value){
    Message m;
    m.op = op;
    strcpy(m.theme, theme);
    strcpy(m.value, value);

    send(sd, &m, sizeof(m), 0);
}

void sendMessageSubs (int sd, int op, const char* theme, struct sockaddr_in saddr){
    Message m;
    m.op = op;
    strcpy(m.theme, theme);
    m.saddr = saddr;

    send(sd, &m, sizeof(m), 0);
}