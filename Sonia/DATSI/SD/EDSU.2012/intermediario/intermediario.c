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


void addThemes (char *fileThemes) {
	FILE *file = fopen (fileThemes, "r");

	if (file != NULL){
		char *theme = malloc (sizeof (char)*128));
	
		while (fgets (theme, sizeof theme, file) =! NULL){
			addNewTheme (theme);
		}
		fclose(file);
	}
	else perror(fileThemes);
}

void addNewTheme (char *theme){
	Theme *newTheme = malloc (sizeof(Theme));
	newTheme->name = malloc (sizeof theme);
	strcpy(newTheme->name, theme);
	newTheme->nsubs = 0;

	themes[nThemes++] = newTheme;
}



int main(int argc, char *argv[]) {
	if (argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}

	int port = atoi(argv[1]);
	char *allThemes = argv[2];

	addThemes (allThemes);

	return 0;
}
