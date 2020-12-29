#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "green.h"
#include "queue.h"


#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4*1024

static ucontext_t main_cntx = {0};

static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};

static green_t * running = &main_green;


queue_t * ready_queue;

// this function is called once program loads into memory regurdless of what function is called!
static void init( ) __attribute__((constructor));
void init(){
	getcontext(&main_cntx);
	ready_queue = createQueue();
	
}
// this functions is mapped to every Context
void green_thread(){
	green_t * this=running;
	// call target function and save its result
	// place waiting (joining) thread in ready queue
	// save result of execution and zombie status
	// find the next thread to run and write its address to next variable

	running =next;
	setcontext(next->context);
}

// will create a new green thread
int green_create(green_t *new ,void *(*fun)(void *),void *arg) {
	ucontext_t * cntx = (ucontext_t *) malloc(sizeof(ucontext_t));
	// intialize cntx
	void * stack = malloc(STACK_SIZE);
	// assign allocated stack to cntx
	// assign green_thread function to cntx

	new->context = cntx ;
	new->fun = fun ;
	new->arg = arg ;
	new->next = NULL;
	new->join = NULL;
	new->retval = NULL;
	new->zombie = FALSE;
	// add this new thread to end of linked list
	// add new thread to the ready queue
	return 0 ;
}

// will give other green thread opportunity of having CPU!
int green_yield(){
	green_t * susp = running ;
	// add susp to ready queue
	// select the next thread for execution
	running = next ;
	// save current state into susp->context and switch to next->context
	return 0 ;
}

// waits for specefied thread till it finishes and get result value
int green_join(green_t * thread ,void ** res) {
	green_t * susp = running ;
	// check if target thread has finished
	// add as joining thread
	// select the next thread for execution
	running = next ;
	// save current state into susp->context and switch to next->context
	
	// collect result
	// free context
	free(thread->context);
	return 0 ;
}
