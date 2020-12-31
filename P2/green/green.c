#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "green.h"
#include "queue.h"


#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4*1024

static ucontext_t main_cntx = {0};

static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};

green_t * head = &main_green;

static green_t * running = &main_green;


queue_t * ready_queue;


// Signal handler funciton
void time_handler(int signo)
{
    write(STDOUT_FILENO, "interrupt\n", sizeof("interrupt\n"));
    green_yield();
}

// this function is called once program loads into memory regurdless of what function is called!
static void init( ) __attribute__((constructor));
void init(){
	getcontext(&main_cntx);
	ready_queue = createQueue();
    // pid_t parent;
    // signal(SIGUSR1, &time_handler);
    // parent = getpid();
    // if (fork() == 0)
    // {
    //     while(1)
    //     {
    //         sleep(0.02);
    //         kill(parent, SIGUSR1);
    //     }
    //     exit(0);
    // }
}
// this functions is mapped to every Context
void green_thread(){
	green_t * this=running;
	// call target function and save its result
    // ===========================
    void* result = (*this->fun)(this->arg);
    // ===========================
	// place waiting (joining) thread in ready queue
    // ===========================
    for(green_t * temp = head; temp != NULL; temp = temp->next)
        if (temp->join == this)
            enQueue(ready_queue, temp);
    // ===========================
	// save result of execution and zombie status
    // ===========================
    this->zombie = 1;
    this->retval = result;
    // ===========================
	// find the next thread to run and write its address to next variable
    // ===========================
    green_t* next = deQueue(ready_queue);
	running =next;
	setcontext(next->context);
    // ===========================
}

// will create a new green thread
int green_create(green_t *new ,void *(*fun)(void *),void *arg) {
	ucontext_t * cntx = (ucontext_t *) malloc(sizeof(ucontext_t));
	// intialize cntx
    // ===========================
    getcontext(cntx);
    // ===========================
	void * stack = malloc(STACK_SIZE);
	// assign allocated stack to cntx
    // ===========================
    cntx->uc_stack.ss_size = STACK_SIZE;
    cntx->uc_stack.ss_sp = stack;
    // ===========================
	// assign green_thread function to cntx
    // ===========================
    makecontext(cntx, (void(*) (void)) green_thread,0);
    // ===========================

	new->context = cntx ;
	new->fun = fun ;
	new->arg = arg ;
	new->next = NULL;
	new->join = NULL;
	new->retval = NULL;
	new->zombie = FALSE;
	// add this new thread to end of linked list
    green_t * temp = head;
    for(; temp->next != NULL; temp = temp->next);
    temp->next = new;
	// add new thread to the ready queue
    enQueue(ready_queue, new);

	return 0;
}

// will give other green thread opportunity of having CPU!
int green_yield(){
	green_t * susp = running ;
	// add susp to ready queue
    // ===========================
    enQueue(ready_queue, susp);
    // ===========================
	// select the next thread for execution
    // ===========================
    green_t * next = deQueue(ready_queue);
	running = next ;
    // ===========================
	// save current state into susp->context and switch to next->context
    // ===========================
    swapcontext(susp->context, next->context);
    // ===========================
	return 0;
}

// waits for specefied thread till it finishes and get result value
int green_join(green_t * thread ,void ** res) {
	green_t * susp = running ;
	// check if target thread has finished
    // ===========================
    if (!thread->zombie)
    {
	    // add as joining thread
        susp->join = thread;
	    // select the next thread for execution
	    green_t * next = deQueue(ready_queue);
        running = next;
	    // save current state into susp->context and switch to next->context
        swapcontext(susp->context, next->context);
    }
    // ===========================
	
	// collect result
    // ===========================
    *res = thread->retval;
    // ===========================
	// free context
	free(thread->context);
	return 0;
}
