typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

typedef struct message {
    int sender_pid;        // ID del proceso remitente
    char content[128];     // Contenido del mensaje
} message;

