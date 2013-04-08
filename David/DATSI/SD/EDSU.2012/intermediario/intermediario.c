#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "comun.h"


/* Structure that represents a them with an array of subscribers*/
typedef struct Theme {
    char *name;
    int subscribers[ 100 ];
    int count;
} Theme;

/* Function declaration (move to .h) */
void addThemes ( char *themesFile );
void addTheme ( char *name );
Theme* searchTheme ( char *name );
int startServer ( int port );
void addSubscriber ( char *themeName, int id );


/* DEBUG functions */
void printThemes ();

/* Global variables declaration */
Theme *themes[ 100 ]; // Stored themes
int themeCount = 0; // Count of stored themes


/* Main exectuion */
int main( int argc, char *argv[] ) {

    if ( argc!=3 ) {
        fprintf( stderr, "Uso: %s puerto fichero_temas\n", argv[ 0 ] );
        return 1;
    }

    int port = atoi( argv [ 1 ] );
    char *themesFile = argv[ 2 ];
    int sid;

    addThemes( themesFile );
    sid = startServer( port );

    while ( 1 ) {
        fprintf(stdout,"MEDIATOR: Waiting for request\n");

        // Message message;
        // if ( ( recv( sid, &message, sizeof( message ), 0 ) ) < 0 ) {
        //     fprintf( stdout,"MEDIATOR: Request recieved: ERROR\n" );
        // } 
        // else {
        //     fprintf( stdout,"MEDIATOR: Request recieved: SUCCESS\n" );
        // }

        if ( ( connection = accept(sid, (struct sockaddr*)&ca, &size_ca)) < 0) {
                fprintf(stdout,"SERVIDOR: Llegada de un mensaje: ERROR\n");
            }
            fprintf(stdout,"SERVIDOR: Llegada de un mensaje: OK\n");
            fd = open(message.remoto,O_RDONLY);
            while ((r = read(fd,buff,4096))){
                write(conection,buff,r);
            }
            close(conection);
    }


    return 0;
}


/* Server management */
int startServer ( int port ) {
    int sid;
    struct sockaddr_in sa;
    socklen_t size = sizeof( sa );

    /* Socket creation */
    if ( ( sid = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0){
        fprintf(stdout,"MEDIATOR: TCP socket creation : ERROR\n");
        exit(-1);
    }
    fprintf(stdout,"MEDIATOR: TCP socket creation : SUCCESS\n");

    /* Assign server port */
    bzero( ( char * )&sa, size );
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons( port );
    if ( bind ( sid, ( struct sockaddr *)&sa, size ) || getsockname ( sid, ( struct sockaddr * )&sa, &size ) ) {
        fprintf( stdout,"MEDIATOR: Server port assignation: ERROR\n" );
        exit ( -1 );
    }
    fprintf( stdout,"MEDIATOR: Server port assignation: SUCCESS\n" );

    /* Listen from socket */
    if ( listen( sid, 0 ) < 0 ) {
        fprintf( stdout,"MEDIATOR: Request acceptation: ERROR\n" );
        exit( -1 );
    }
    fprintf( stdout,"MEDIATOR: Request acceptation: SUCCESS\n" );

    /* Puerto TCP ya disponible */
    fprintf( stdout,"MEDIATOR: TCP socket READY\n" );

    return sid;
}


/* Theme management */
void addThemes ( char *themesFile ) {
    FILE *file = fopen ( themesFile, "r" );

    if ( file != NULL ){
        char *line = malloc( sizeof( char ) * 128 );

        while ( fgets( line, sizeof line, file ) != NULL ) {
            line[ strlen( line ) - 1 ] = '\0'; // Quickfix to trim \n
            addTheme ( line );
        }
        fclose( file );
    }
    else perror( themesFile );
}


void addTheme ( char *name ) {
    Theme *newTheme = malloc( sizeof( Theme ) );
    newTheme->name = malloc( sizeof name ); 
    strcpy( newTheme->name, name );
    newTheme->count = 0;

    themes[ themeCount++ ] = newTheme;

    fprintf( stdout,"MEDIATOR: Added new theme: %s\n", name );
}


Theme* searchTheme ( char *name ) {
    Theme *res;
    int i;
    for ( i = 0; i < themeCount; i++ ) {
        if ( !strcmp( name, themes[ i ]->name ) ) {
            res = themes[ i ];
        }
    }
    return res;
}


/* Subscription management */
void addSubscriber ( char *themeName, int id ) {
    Theme *theme;
    theme = searchTheme( themeName );

    theme->subscribers[ theme->count++ ] = id;
}

void removeSubscriber () {

}

void notify ( char *theme ) {

}


/* DEBUG functions */
void printThemes () {
    if ( themeCount == 0 ) {
        printf ( "NO THEMES" );
    }

    int i;
    for ( i = 0; i < themeCount; i++ ) {
        printf( "%s\n", themes[ i ]->name );
    }
}
