#include "green.h"

#ifndef QUEUE_H
#define QUEUE_H
typedef struct node_t
{
	green_t* point_val;
	struct node_t * next;
} node_t;

typedef struct queue_t
{
	node_t* front;
	node_t* rear;
} queue_t;

node_t* newNode(green_t * k);
queue_t * createQueue();
void enQueue(queue_t* q, green_t *k);
green_t* deQueue(queue_t* q);
/* you can use printQueue for your own debugging */
void printQueue(queue_t * q,green_t * head);

#endif
