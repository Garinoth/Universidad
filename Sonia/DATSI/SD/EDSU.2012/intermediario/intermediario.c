#include <stdio.h>

/*Estructura de cada tema con un nombre, la lista de sus suscriptores
  y el número de los mismos*/
typedef struct {
	char *name; 
	char *subscribers[1000];
	int nsubs;
} Theme;

Theme *themes[1000];
int nThemes = 0;

struct sockaddr sa {
	unsigned shot sa_family;
	char sa_data[14];
}

void addThemes (char *fileThemes) {
	FILE *file = fopen (fileThemes, "r");

	if (file != NULL){
		char *theme = malloc (sizeof (char)*128));
	
		while (fgets (theme, sizeof theme, file) =! NULL){
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

int initServer (int port) {
	
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

		/* Recibo mensaje */
		if(recvfrom(sd_UDP, &mensaje, sizeof(mensaje), 0, (struct sockaddr *)&c_ain, &size_c_ain) < 0){ 
			fprintf(stdout,"INTERMEDIARIO: Mensaje del cliente: ERROR\n");
		}
		else{
			fprintf(stdout,"INTERMEDIARIO: Mensaje del cliente: OK\n");
		}
	  
		if(ntohl(mensaje.op) == QUIT/* Mensaje QUIT*/){
			fprintf(stdout,"INTERMEDIARIO: QUIT\n");
			mensaje.op = htonl(OK);
			sendto(sd_UDP, &mensaje, sizeof(mensaje), 0, (struct sockaddr *)&c_ain, size_c_ain);
			fprintf(stdout,"INTERMEDIARIO: Finalizado\n");
			exit(0);
		}
		else{
			fprintf(stdout,"INTERMEDIARIO: REQUEST(%s,%s)\n", mensaje.local, mensaje.remoto);
			/* Envio del resultado */
			if((fd = open(mensaje.remoto, O_RDONLY)) < 0){
				mensaje.op = htonl(ERROR);
				if(sendto(sd_UDP, &mensaje, sizeof(mensaje), 0, (struct sockaddr *)&c_ain, size_c_ain) < 0){
					fprintf(stdout,"INTERMEDIARIO: Enviando del resultado [ERROR]: ERROR\n");
				}
				else{
					fprintf(stdout,"INTERMEDIARIO: Enviando del resultado [ERROR]: OK\n");
				}
			}  
			else{
				mensaje.op = htonl(OK);
				mensaje.puerto = s_ain_TCP.sin_port;
				if(sendto(sd_UDP, &mensaje, sizeof(mensaje), 0, (struct sockaddr *)&c_ain, size_c_ain) < 0){
					fprintf(stdout,"INTERMEDIARIO: Enviando del resultado [OK]: ERROR\n");
				}
				else{
					fprintf(stdout,"INTERMEDIARIO: Enviando del resultado [OK]: OK\n");
				}
			}

			/* Esperamos la llegada de una conexion */
			if((cd = accept(sd_TCP, (struct sockaddr *)&c_ain, &size_c_ain)) < 0){
				fprintf(stdout,"INTERMEDIARIO: Llegada de un mensaje: ERROR\n");
			}
			else{
				fprintf(stdout,"INTERMEDIARIO: Llegada de un mensaje: OK\n");
				int rd;
				char buf[4096];
				while((rd = read(fd, buf, 4096)) != 0){
					write(cd, buf, rd);
				}
				close(fd);
				close(cd);
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
