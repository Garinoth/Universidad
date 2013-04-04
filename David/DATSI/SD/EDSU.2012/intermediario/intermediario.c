#include <stdio.h>
#include <stdlib.h> 


/* Structure that represents a them with an array of subscribers*/
typedef struct {
    char *name;
    char *subcribers[ 100 ];
    int count;
} Theme;

/* Function declaration (move to .h) */
void addThemes ( char *themesFile );
void addTheme ( char *name );

/* DEBUG functions */
void printThemes ();

/* Global variables declaration */
Theme themes[ 100 ];
int themeCount = 0;


/* Main exectuion */
int main( int argc, char *argv[] ) {

    if ( argc!=3 ) {
        fprintf( stderr, "Uso: %s puerto fichero_temas\n", argv[ 0 ] );
        return 1;
    }

    int port = atoi( argv [ 1 ] );
    char *themesFile = argv[ 2 ];

    addThemes ( themesFile );

    printThemes();

    return 0;
}


void addThemes ( char *themesFile ) {
    FILE *file = fopen ( themesFile, "r" );

    if ( file != NULL ){
        char line [ 128 ];

        while ( fgets ( line, sizeof line, file ) != NULL ) {
            addTheme ( line );
        }
        fclose ( file );
    }
    else perror ( themesFile );
}


void addTheme ( char *name ) {
    Theme newTheme;
    newTheme.name = name;
    newTheme.count = 0;

    themes[ themeCount++ ] = newTheme;

    fprintf( stdout,"MEDIATOR: Added new theme: %s", name );
}


/* DEBUG functions */
void printThemes () {
    if ( themeCount == 0 ) {
        printf ( "NO THEMES" );
    }

    int i;
    for ( i = 0; i < themeCount; i++ ) {
        printf( "%s\n", themes[ i ].name );
    }
}
    // /* Creacion del socket TCP de servicio */
    // if ((sid_TCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    //     fprintf(stdout,"SERVIDOR: Creacion del socket TCP: ERROR\n");
    //     exit(-1);
    // }
    // fprintf(stdout,"SERVIDOR: Creacion del socket TCP: OK\n");

    // /* Asignacion de la direccion local (del servidor) Puerto TCP*/
    // bzero((char *)&sa_TCP, size_TCP);
    // sa_TCP.sin_family = AF_INET;
    // sa_TCP.sin_addr.s_addr = INADDR_ANY;
    // sa_TCP.sin_port = htons(0);
    // if (bind (sid_TCP, (struct sockaddr *)&sa_TCP, size_TCP) || getsockname (sid_TCP, (struct sockaddr *)&sa_TCP, &size_TCP)) {
    //     fprintf(stdout,"SERVIDOR: Asignacion del puerto servidor: ERROR\n");
    //     exit (-1);
    // }
    // fprintf(stdout,"SERVIDOR: Asignacion del puerto servidor: OK\n");

    // /* Aceptamos conexiones por el socket */
    // if (listen(sid_TCP,1) < 0) {
    //     fprintf(stdout,"SERVIDOR: Aceptacion de peticiones: ERROR\n");
    //     exit(-1);
    // }
    // fprintf(stdout,"SERVIDOR: Aceptacion de peticiones: OK\n");

    // /* Puerto TCP ya disponible */
    // fprintf(stdout,"SERVIDOR: Puerto TCP reservado: OK\n");

    // while (1) { /* Bucle de procesar peticiones */
    //     fprintf(stdout,"SERVIDOR: Esperando mensaje.\n");

    //     /* Recibo mensaje */
    //     if ((recvfrom(sid_UDP,&message,sizeof(message),0,(struct sockaddr *)&ca,&size_ca)) < 0) {
    //             fprintf(stdout,"SERVIDOR: Mensaje del cliente: ERROR\n");
    //     }
    //         fprintf(stdout,"SERVIDOR: Mensaje del cliente: OK\n");

    //     if (ntohl(message.op) == QUIT) { /* Mensaje QUIT*/
    //         fprintf(stdout,"SERVIDOR: QUIT\n");
    //         message.op = htonl(OK);
    //         sendto(sid_UDP,&message, sizeof(message),0,(struct sockaddr*)&ca,size_ca);
    //         break;
    //     }
    //     else {
    //         fprintf(stdout,"SERVIDOR: REQUEST(%s,%s)\n", message.local,message.remoto);

    //         /* Envio del resultado */
    //         if (access(message.remoto,F_OK) < 0) 
    //             message.op = htonl(ERROR);
    //         else message.op = htonl(OK);
    //         char* res = "OK";
    //         message.puerto = sa_TCP.sin_port;
    //         if ((sendto(sid_UDP,&message, sizeof(message),0,(struct sockaddr*)&ca,size_ca)) < 0) res = "ERROR";
    //         if (message.op == htonl(ERROR))
    //             fprintf(stdout,"SERVIDOR: Enviando del resultado [ERROR]: %s\n",res);
    //         else
    //             fprintf(stdout,"SERVIDOR: Enviando del resultado [OK]: %s\n",res);

    //         /* Esperamos la llegada de una conexion */
    //         if ((conection = accept(sid_TCP, (struct sockaddr*)&ca, &size_ca)) < 0) {
    //             fprintf(stdout,"SERVIDOR: Llegada de un mensaje: ERROR\n");
    //         }
    //         fprintf(stdout,"SERVIDOR: Llegada de un mensaje: OK\n");
    //         fd = open(message.remoto,O_RDONLY);
    //         while ((r = read(fd,buff,4096))){
    //             write(conection,buff,r);
    //         }
    //         close(conection);
    //         close(fd);
    //     }
    //     }
