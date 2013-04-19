#include "comun.h"

/*Estructura de cada tema con un nombre, la lista de sus suscriptores
  y el número de los mismos*/
typedef struct Theme {
	char *name; 
	struct sockaddr_in subscribers[1000];
	int nsubs;
} Theme;

Theme *themes[1000];
int nThemes = 0;

struct sockaddr_in s_ain_TCP, c_ain;
socklen_t size_TCP = sizeof(s_ain_TCP), size_c_ain = sizeof(c_ain);

void addThemes (char *fileThemes) {
	FILE *file = fopen (fileThemes, "r");

	if (file != NULL){
		char *theme = malloc (sizeof (char)*128);
	
		while (fgets (theme, sizeof theme, file) != NULL){
			theme[strlen(theme)-1] = '\0';
			Theme *newTheme = malloc (sizeof(Theme));
			newTheme->name = malloc (sizeof theme);
			strcpy (newTheme->name, theme);
			newTheme->nsubs = 0;

			themes[nThemes++] = newTheme;
		}
		fclose(file);
	}
	else perror(fileThemes);
}

int addSubscriber (struct sockaddr_in subscriber, char* theme){
	int i;
	for (i = 0; i < nThemes; i++){
		if (!strcmp(theme,themes[i]->name)){
			themes[i]->subscribers[themes[i]->nsubs++] = subscriber;
			return 0;
		}
	}
	return -1;
}

int removeSubscriber (struct sockaddr_in subscriber, char* theme){
	int i;
	for (i = 0; i < nThemes; i++){
		if (!strcmp(theme,themes[i]->name)){
			int j;
			for (j = 0; j < themes[i]->nsubs; j++){
				if (subscriber.sin_addr.s_addr == themes[i]->subscribers[j].sin_addr.s_addr){
					int k;
					for (k = j; k < themes[i]->nsubs; k++) {
						themes[i]->subscribers[k]=themes[i]->subscribers[k+1];
					}
				themes[i]->nsubs--;
				return 0;
				}
			}
		}
	}
	return -1;
}


int sendEvent (char* theme, char* value){
	int i;
	for (i = 0; i < nThemes; i++){
		if (!strcmp(theme,themes[i]->name)){
			int j;
			for (j = 0; j < themes[i]->nsubs; j++){
				int sd;
				sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				connect(sd, (struct sockaddr*)&themes[i]->subscribers[j], sizeof(struct sockaddr_in));
				sendMessage(sd, EVENT, theme, value);
			}
		return 0;
		}
	}
	return -1;
}

void responseSubs (int op, int c){
		int wr;
		Message message;
		message.op = op;

		while((wr += write(c, &message, sizeof(message))) && wr < sizeof(message));
}


int initServer (int port) {
	
	int sd_TCP;

	/* Creacion del socket TCP de servicio */
    if((sd_TCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		fprintf(stdout,"INTERMEDIARIO: Creacion del socket TCP: ERROR\n");
		exit(1);
	}
	else{
		fprintf(stdout,"INTERMEDIARIO: Creacion del socket TCP: OK\n");
	}

	/* Asignacion de la direccion local (del servidor) Puerto TCP*/
	bzero((char *)&s_ain_TCP, sizeof(s_ain_TCP));
	s_ain_TCP.sin_family = AF_INET;
	s_ain_TCP.sin_addr.s_addr = INADDR_ANY;
	s_ain_TCP.sin_port = htons(port);
	if (bind(sd_TCP, (struct sockaddr *)&s_ain_TCP, size_TCP) < 0){
		fprintf(stdout,"INTERMEDIARIO: Asignacion del puerto servidor: ERROR\n");
		exit(1);
	}
	else if (getsockname(sd_TCP, (struct sockaddr *)&s_ain_TCP, &size_TCP) < 0){
		fprintf(stdout,"INTERMEDIARIO: Asignacion del puerto servidor: ERROR\n");
		exit(1);
	}
	else{
		fprintf(stdout,"INTERMEDIARIO: Asignacion del puerto servidor: OK\n");
	}

	/* Aceptamos conexiones por el socket */
	if (listen(sd_TCP,0) < 0){
		fprintf(stdout,"INTERMEDIARIO: Aceptacion de peticiones: ERROR\n");
		exit(1);
	}
	else{
		fprintf(stdout,"INTERMEDIARIO: Aceptacion de peticiones: OK\n");
	}
  
	/* Puerto TCP ya disponible */
	fprintf(stdout,"INTERMEDIARIO: Puerto TCP reservado: OK\n");

	return sd_TCP;
}

void connection (int sd_TCP) {
	while(1 /* Bucle de procesar peticiones */){
		fprintf(stdout,"INTERMEDIARIO: Esperando mensaje.\n");

		int con;
		/* Esperamos la llegada de una conexion */
		if((con = accept(sd_TCP, (struct sockaddr *)&c_ain, &size_c_ain)) < 0){
			fprintf(stdout,"INTERMEDIARIO: Llegada de un mensaje: ERROR\n");
		}
		else{
			fprintf(stdout,"INTERMEDIARIO: Llegada de un mensaje: OK\n");
			int rd;
			Message m;
			while((rd += read(con, &m, sizeof(m))) && rd < sizeof(m));
	  
			if(m.op == SUBSCRIBE){
				if (addSubscriber(m.saddr, m.theme) == 0){
					responseSubs(OK, con);
				}
				else {
					responseSubs(ERROR,con);
				}
			}

			else if(m.op == UNSUBSCRIBE){
				if (removeSubscriber(m.saddr, m.theme) == 0){
					responseSubs(OK, con);
				}
				else {
					responseSubs(ERROR,con);
				}
			}		

			else if(m.op == EVENT){
				if (sendEvent(m.theme, m.value) == 0) {
					printf("WAAAAAAAAAAAAAAAAAAAAAAAAARG\n");
					responseSubs(OK,con);
				}
				else {
					responseSubs(ERROR,con);
				}
			}

			else {
				fprintf(stderr, "INTERMEDIARIO: Operación no admitida, ERROR.");
			}
		}	
	}
}


int main(int argc, char *argv[]) {
	if (argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}

	int port = atoi(argv[1]);
	char *allThemes = argv[2];
	int sd_TCP;

	addThemes (allThemes);
	sd_TCP = initServer(port);
	connection(sd_TCP);



	return 0;
}
