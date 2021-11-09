/*
 * @File		cbfifo.c
 * @Brief		The file contains functions related to handling of the Circular Buffer
 * 				implementation of the FIFO. It includes function to enqueue and dequeue
 * 				elements in the FIFO and functions to destroy it and return the length
 * 				and capacity of the queue.
 * @Author		Ruchit Naik
 * @Date		03-Nov-2021
 *
 * @Institute	University of Colorado, Boulder
 * @Course		ECEN 8513: Principles of Embedded Software
 *
 * @Attribute
 */

#include "queue.h"

/*//Initializing the structure to use the elements while calling function on cbfifo
CBfifo CBfifo_t ={
		.size = 0,
		.readp = 0,
		.writep = 0,
		.length = 0,
};*/

void cbfifo_Init(CBfifo * queue){
		queue->size = 0;
		queue->readp = 0;
		queue->writep = 0;
		for(int i = 0; i<MAXSIZE; i++){
			queue->data[i] = 0;
		}
		queue->full_queue = false;
}





bool cbfifo_full(CBfifo *queue){
	return (cbfifo_length(queue) == MAXSIZE);
}

bool cbfifo_empty(CBfifo *queue){
	return (cbfifo_length(queue) == 0);
}

int cbfifo_size(CBfifo *queue){
	return queue->size;
}



/*
 * @Function	cbfifo_length
 * @Param		none
 * @Returns		Number of bytes currently available to be dequeued from the FIFO
 * @Description	Returns the number of bytes currently on the FIFO.
 */
size_t cbfifo_length(CBfifo *queue){
	/*uint8_t length = 0;
	length = (CBfifo_t.writep - CBfifo_t.readp) & (MAXSIZE - 1);
	return length;*/
	size_t value = 0;
	if(queue->full_queue){
		value = MAXSIZE;
	}
	else if(queue->writep >= queue->readp){
		value = queue->writep - queue->readp;
	}
	else{
		value = MAXSIZE - (queue->readp - queue->writep);
	}
	return value;
}



/*
 * @Function	cbfifo_enqueue
 * @Param		buf - The pointer to the location from where data is to be entered in
 * 					  the fifo
 * 				nbytes - Number of bytes to be enqueued in fifo
 * @Returns		The number of bytes actually copied, which will be between 0 and nbyte.
 * @Description	Enqueues data onto the FIFO, up to the limit of the available FIFO
 * 				capacity.
 */
size_t cbfifo_enqueue(CBfifo *queue, const void *buf, size_t nbyte){
	size_t len1 = 0;
	size_t len2 = 0;

	if(queue->full_queue){
		return 0;
	}

	if(cbfifo_empty(queue)){
		len1 = nbyte;
		queue->writep = len1;
		if(nbyte == MAXSIZE){
			len1 = MAXSIZE;
			queue->full_queue = true;
			queue->writep = 0;
		}
		memcpy(queue->data, buf, len1);
		queue->readp = 0;
		queue->size = queue->size + len1 + len2;
		return len1 + len2;
	}

	if(queue->readp < queue->writep){
		len1 = min(nbyte, MAXSIZE - queue->writep);
		memcpy(queue->data + queue->writep, buf, len1);
		queue->writep = queue->writep + len1;

		if(queue->writep < MAXSIZE){
			return len1 + len2;
		}

		queue->writep = 0;
		if(queue->readp == 0){
			queue->size = queue->size + (len1 + len2);					//Updating the queue size after enqueue
			queue->full_queue = true;
			return len1 + len2;
		}

		nbyte = nbyte - len1;
		buf = buf + len1;														//Updating the buf pointer over the buffer roll-over point
	}

	//After roll-over stage
	len2 = min(nbyte, queue->readp - queue->writep);
	memcpy(queue->data + queue->writep, buf, len2);
	queue->writep = queue->writep + len2;										//Furthering updating the write pointer

	if(queue->writep == queue->readp){
		queue->full_queue = true;
	}

	queue->size = queue->size + len1 + len2;

	return (len1 + len2);
}



/*
 * @Function	cbfifo_dequeue
 * @Param		buf - The pointer to the location where the dequeued data is to be fetched
 * 				nbytes - Number of bytes to be dequeued from fifo
 * @Returns		The number of bytes actually copied, which will be between 0 and nbyte.
 * @Description	Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * 				FIFO. Removed data will be copied into the buffer pointed to by buf.
 */
size_t cbfifo_dequeue(CBfifo *queue, void *buf, size_t nbyte){
	size_t len1 = 0;
	size_t len2 = 0;

	if(cbfifo_empty(queue) && !queue->full_queue){
		queue->size = queue->size - (len1 + len2);
		return 0;
	}

	queue->full_queue = false;

	len1 = min(nbyte, MAXSIZE - queue->readp);
	if((queue->writep > queue->readp) && (len1 > queue->writep - queue->readp)) {
		len1 = queue->writep - queue->readp;
	}
	memcpy(buf, queue->data + queue->readp, len1);
	queue->readp = queue->readp + len1;											//Updating the read pointer

	if(queue->readp < MAXSIZE){
		queue->size = queue->size - (len1 + len2);										//Updating size of the buffer
		return (len1 + len2);
	}

	//Handling the roll-over condition of the buffer
	len2 = min(nbyte - len1, queue->writep);									//check the remaining length of the buffer
	memcpy(buf+len1, queue->data, len2);										//Return the dequeue data to the return buffer
	queue->readp = len2;

	return (len1 + len2);
}



/*
 * @Function	cbfifo_capacity
 * @Param		none
 * @Returns		the current capacity of the fifo in bytes
 * @Description	Returns the current capacity of the fifo
 */
size_t cbfifo_capacity(){
	return MAXSIZE;
}



///*
// * @Function	cbfifo_dump_state
// * @Param		none
// * @Returns		none
// * @Description	prints the current state of the fifo for debug purposes
// */
//void cbfifo_dump_state(CBfifo *queue){
//	if(queue->writep >= queue->readp){
//		int inc = queue->readp;
//		while(inc <= queue->writep){
//			printf("%c,", CBfifo_t.data[inc]);
//			inc++;
//		}
//	}
//	else{
//		int inc = CBfifo_t.readp;
//		while(inc <= MAXSIZE){
//			printf("%c,", CBfifo_t.data[inc]);
//			inc++;
//		}
//		inc = 0;
//		while(inc <= CBfifo_t.writep){
//			printf("%c,", CBfifo_t.data[inc]);
//			inc++;
//		}
//	}
//}

