#include "comun.h"


/* Structure that represents a them with an array of subscribers*/
typedef struct Theme {
    char name[ 128 ];
    int subscribers[ 128 ];
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
Theme *themes[ 128 ]; // Stored themes
int themeCount = 0; // Count of stored themes

struct sockaddr_in sa; // Server address
socklen_t size_sa = sizeof( sa );

struct sockaddr ca; // Client address
socklen_t size_ca = sizeof( ca );

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

    int connection;
    while ( 1 ) {
        fprintf(stdout,"MEDIATOR: Waiting for request\n");

        if ( ( connection = accept( sid, ( struct sockaddr* )&ca, &size_ca ) ) < 0 ) {
            fprintf(stdout,"MEDIATOR: Request recieved: ERROR\n");
        }
        fprintf(stdout,"MEDIATOR: Request recieved: SUCCESS\n");

        Message message;
        int r;  
        while ( ( r += read ( connection, &message, sizeof( message ) ) ) && r < sizeof( message ) );
        
        switch ( message.op ) {
            case SUBSCRIBE :
                addSubscriber( message.theme, connection );
            break;
            
            case UNSUBSCRIBE :
                removeSubscriber( message.theme, connection );
            break;
            
            case EVENT :
                notify( message.theme, message.value );
            break;

            default :
                fprintf(stdout,"MEDIATOR: Unknown operation: ERROR\n");
        }
        close( connection );
    }


    return 0;
}


/* Server management */
int startServer ( int port ) {
    int sid;

    /* Socket creation */
    if ( ( sid = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0){
        fprintf(stdout,"MEDIATOR: TCP socket creation : ERROR\n");
        exit(-1);
    }
    fprintf(stdout,"MEDIATOR: TCP socket creation : SUCCESS\n");

    /* Assign server port */
    bzero( ( char * )&sa, size_sa );
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons( port );
    if ( bind ( sid, ( struct sockaddr *)&sa, size_sa ) || getsockname ( sid, ( struct sockaddr * )&sa, &size_sa ) ) {
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

void removeSubscriber ( char *themeName, int id ) {

}

void notify ( char *themeName, char *event ) {

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
