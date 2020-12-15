/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <stdlib.h>
//#include <stddef.h>
//#include <stdio.h>

// Data structure for queue
struct queue {

	void **items;	// array to store queue elements
	int maxsize;	// maximum capacity of the queue
	int front;		// front points to front element in the queue (if any)
	int rear;		// rear points to last element in the queue
	int size;		// current capacity of the queue

};

// Utility function to initialize queue
struct queue* queue_new(int size){

	struct queue *q = NULL;
	q = (struct queue*)malloc(sizeof(struct queue));

	q->items = malloc(size * sizeof(void *));
	q->maxsize = size;
	q->front = 0;
	q->rear = -1;
	q->size = 0;

	return q;
}

int queue_size(struct queue *q){ return q->size;}
int queue_is_empty(struct queue *q){ return q->size==0;}
int queue_is_full(struct queue *q){
	//if (q->size==q->maxsize) printf(">>> RX queue is full! (%d) \n", q->maxsize);
	return q->size==q->maxsize;
}

void queue_put(struct queue *q, void* obj){

	if (queue_is_full(q)) return;

	q->rear = (q->rear + 1) % q->maxsize;
	q->items[q->rear] = obj;
	q->size++;

	//printf("(+) queue size %d \n", q->size);

}

void* queue_get(struct queue *q){

	if (queue_is_empty(q)) return NULL;

	void* retval = q->items[q->front];

	q->items[q->front]=NULL;
	q->front = (q->front + 1) % q->maxsize;
	q->size--;

	//printf("(-) queue size %d \n", q->size);

	return retval;

}
