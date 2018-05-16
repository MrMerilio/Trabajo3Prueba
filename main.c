#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "fifo procesos"
#define QUEUESIZE 4





void solicitar(int);
void trabajar(int);
typedef struct
{
    int buf[QUEUESIZE];
    long head, tail;
    int full, empty;
} queue;
queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, int in);
void queueDel (queue *q, int *out);

int main()
{
    char hijos[]= {SOL,PIN,ATO,REM};
    mknod(FIFO_CONTROLADOR, C_IFIFO | 0666, 0);
    mknod(FIFO_SOL, S_IFIFO | 0666, 0);
    mknod(FIFO_PIN, P_IFIFO | 0666, 0);
    mknod(FIFO_ATO, A_IFIFO | 0666, 0);
    mknod(FIFO_REM, R_IFIFO | 0666, 0);
    int pid;
    //Inicio los procesos;
    printf("\nCTL: Arrancando plataforma\n");
    for(int i=0; i<4; i++)
    {
        pid=fork();
        if(pid==-1)
        {
            perror("No se ha creado el hijo");
            exit(-1);
        }
        else if(pid==0)
        {
            printf("\n %c ...",hijos[i]);
            solicitar(getpid());
            exit(0);
        }

    }
    printf("\nEsperando solictud\n");
    for(int j=0; j<4; j++)
    {
        int rf;
        rf=open(FIFO_NAME, O_RDONLY);
    }
}
void solicitar(int pid)
{
    int wf;
    printf("\nEnviada solicitud de proceso: %d\n",pid);
    wf=open(FIFO_NAME, O_WRONLY);
    if(wf==-1)
    {
        printf("\nPadre: Solcitud denegada encolamos el proceso: %d\n",pid);
    }
    else
    {
        printf("\nPadre: Solicitud aceptada del proceso: %d\n",pid);
        trabajar(pid);
    }
}
void trabajar(int pid)
{
    int tiempo=rand() %1+1;
    printf("\nEl proceso %d inicia su trabajo de %d segundos\n",pid,tiempo);
    sleep(tiempo);
    printf("\nEl proeso %d ha terminado su trabajo y se lo notifica al contrlador\n",pid);
    tiempo=rand() %1+1;
    printf("\nPausa del proceso %d de % segunso",pid,tiempo);
    sleep(tiempo);
}
queue *queueInit (void)
{
    queue *q;
    q = (queue *)malloc (sizeof (queue));
    if (q == NULL) return (NULL);
    q->empty = 1;
    q->full = 0;
    q->head = 0;
    q->tail = 0;
    return (q);
}
void queueDelete (queue *q)
{
    free (q);
}
void queueAdd (queue *q, int in)
{
    q->buf[q->tail] = in;
    q->tail++;
    if (q->tail == QUEUESIZE)
        q->tail = 0;
    if (q->tail == q->head)
        q->full = 1;
    q->empty = 0;
    return;
}
void queueDel (queue *q, int *out)
{
    *out = q->buf[q->head];
    q->head++;
    if (q->head == QUEUESIZE)
        q->head = 0;
    if (q->head == q->tail)
        q->empty = 1;
    q->full = 0;
    return;
}
