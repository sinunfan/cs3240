#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#define SIZE 256

typedef struct Node
{
    char data[SIZE];
    struct Node *next;
} Node;

struct queue
{
    struct Node *front;
    struct Node *rear;
    int size;
};

typedef struct queue Queue;

void init_queue(Queue *q)
{
    q->front = q->rear = NULL;
    q->size = 0;
}

int enter_queue(Queue *q, char *dir)
{
    Node *newNode;
    newNode = (Node *)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("queue is empty");
        exit(1);
    }
    strcpy(newNode->data, dir);
    newNode->next = NULL;
    if (q->rear == NULL)
    {
        q->front = q->rear = newNode;
    }
    else
    {
        q->rear = q->rear->next = newNode;
    }
    q->size++;
    return 0;
}
char *del_queue(Queue *q)
{
    Node *p;
    char *temp = malloc(SIZE);
    if (q->front == NULL)
    {
        printf("the queue is empty");
        return NULL;
    }
    strcpy(temp, q->front->data);
    p = q->front;
    q->front = p->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    free(p);
    q->size--;
    return temp;
}
void recurse_fill_queue(Queue *q, char *path)
{
    struct dirent *pDirent;
    DIR *pDir = opendir(path);
    if(pDir==NULL) return;
    //strcpy(temp,path);

    while((pDirent = readdir(pDir)) != NULL)
    {
        printf("[%s | %ld]\n", pDirent->d_name, pDirent->d_ino);
        
            if(pDirent-> d_type == DT_DIR && strcmp(pDirent->d_name, ".") != 0 
            && strcmp(pDirent->d_name, "..") != 0){
                char temp[SIZE]="";
                strcat(temp,path);
                strcat(temp,"/");
                strcat(temp,pDirent->d_name);
                enter_queue(q, temp);
                printf("%s\n", temp);
                //printf("%s\n", del_queue(q));
                recurse_fill_queue(q,temp);
            }
    }
    
    closedir(pDir);
}
void breadth_first_search_dir(char *path)
{
    struct dirent *pDirent;
    DIR *pDir;
    Queue q;
    init_queue(&q);
    recurse_fill_queue(&q, path);
}

int main(int argc, char *argv[])
{

    breadth_first_search_dir(argv[1]);

    return 0;
}