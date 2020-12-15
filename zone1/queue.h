/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#ifndef QUEUE_H_
#define QUEUE_H_


struct queue;

struct queue* queue_new(int size);

int queue_size(struct queue *q);
int queue_is_empty(struct queue *q);
int queue_is_full(struct queue *q);

void queue_put(struct queue *q, void* obj);

void* queue_get(struct queue *q);


#endif /* QUEUE_H_ */
