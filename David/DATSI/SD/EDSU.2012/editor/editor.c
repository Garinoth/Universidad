#include "editor.h"
#include "comun.h"
#include "edsu_comun.h"

int generar_evento(const char *tema, const char *valor) {
	Message message;
	message.op = EVENT;
    strcpy( message.theme, tema );
    strcpy( message.value, valor );

    int sid = connectToMediator();
	send( sid, &message, sizeof( message ), 0 );
	return 0;
}

/* solo para la version avanzada */
int crear_tema(const char *tema) {
	return 0;
}

/* solo para la version avanzada */
int eliminar_tema(const char *tema) {
	return 0;
}

