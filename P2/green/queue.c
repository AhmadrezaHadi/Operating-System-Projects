#include "green.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>


/* this function is used to create a new Node you don't need to use it */
node_t* newNode(green_t * k) 
{ 
    node_t* temp = (node_t*)malloc(sizeof(node_t)); 
    temp->point_val = k; 
    temp->next = NULL; 
    return temp; 
} 

/* this function created a new queue */
queue_t * createQueue() 
{ 
	queue_t* q = (queue_t*)malloc(sizeof(queue_t)); 
	q->front = q->rear = NULL; 
	return q; 
}

/* this function adds k(element) into q(queue) */
void enQueue(queue_t* q, green_t *k) 
{ 
	node_t* temp = newNode(k); 

	// If queue is empty, then new node is front and rear both 
	if (q->rear == NULL) { 
		q->front = q->rear = temp;
		return; 
	}
	// Add the new node at the end of queue and change rear 
	q->rear->next = temp; 
	q->rear = temp; 
}

/* reverse of enQueue function and returns ommited element */
green_t* deQueue(queue_t* q)
{ 
    //If queue is empty, return NULL. 
	if(q->front == NULL) 
		return NULL; 

	node_t* temp = q->front;
	green_t* deleted_green = temp->point_val;
	// Store previous front and move front one Node* temp = q->front; 
	q->front = q->front->next; 

	// If front becomes NULL, then change rear also as NULL 
	if (q->front == NULL) 
		q->rear = NULL; 
	free(temp); 
	return deleted_green;
}


void printQueue(queue_t * q,green_t * head)
{
	if(q->front == NULL) {
		printf("empry!\n");
		return;
	}
	node_t* temp = q->front;
	while(temp!=NULL)
	{
		int i=-1;
		int dn=0;
		green_t* cur=head;
		while(cur!=NULL)
		{
			if(cur==temp->point_val)
			{
				printf("%d|",i);
				dn=1;
				break;
			}
			i++;
			cur=cur->next;
		}
		if(!dn)
		{
			printf("&%p-",temp->point_val);
		}
		
		
		temp=temp->next;
	}
	// sleep(1);
	printf("\n");
}
/*
void removeifexist(queue_t * q, green_t * el)
{
	node_t *cur = q->front;
	node_t * pre=NULL;
	if(cur==NULL)
		return;

	if(cur->next == NULL)
	{
		if(cur->point_val==el)
		{
			free(cur);
			q->front=NULL;
			q->rear=NULL;
		}
		return;
	}
	while(cur->next!= NULL)
	{
		
		if(cur->point_val==el)
		{
			if(pre!=NULL)
				pre->next=cur->next;
			else
				q->front = cur->next;
			if(cur == q->rear)
			{
				q->rear = pre;
			}
			free(cur);
			pre=cur;
		}	cur=cur->next;
	}
}
*/