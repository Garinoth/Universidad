/*
   Incluya en este fichero todas las definiciones que pueden
   necesitar compartir todos los módulos (editor, subscriptor y
   proceso intermediario), si es que las hubiera.
*/

#define SUBSCRIBE   1  
#define UNSUBSCRIBE 2  
#define EVENT       3  

typedef struct Message
{
  int  op;           /* Operacion */
  int  puerto;       /* Puerto del servidor para la transmision del fichero via TCP */
} Message;