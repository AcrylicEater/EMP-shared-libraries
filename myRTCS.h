/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: myRTCS.h
*
* PROJECT....: EMP
*
* DESCRIPTION: my implementation of a run to complete scheduler
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150303  MoH   Module created.
*
*****************************************************************************/

#ifndef MY_RTCS_H
  #define MY_RTCS_H

/***************************** Include files *******************************/
#include <stdint.h>
#include <systick.h>

/*****************************    Defines    *******************************/
typedef uint8_t task_id;
typedef uint8_t sem_id;
typedef uint8_t queue_id;
typedef uint8_t boolean;
typedef void (*taskFunc)(task_id, uint8_t, uint8_t, uint8_t);

//number of elements
#define NOF_TASKS 32
#define NOF_QUEUES 32
#define NOF_SEMS (NOF_QUEUES*2) + 32

#define QUEUE_SIZE 64

#define NO_SEM NOF_SEMS + 1 //index used for no semaphore
#define NO_QUEUE NOF_QUEUES + 1 //index used for no queue
#define USER_SEM (NOF_QUEUES*2)

//execution state
#define NO_TASK    0
#define TASK_WAIT  1
#define TASK_READY 2

//Events that caused a task to run
#define NO_EVENT    0 //No particular thing caused this to run. Free running task
#define TIME_EVENT  1 //A timestamp being reached caused the task to run
#define SEM_EVENT   2 //A semaphore was released for this task to be run
#define RESET_EVENT 3 //First time this task has been run

#define TRUE  1
#define FALSE 0

typedef struct{
    task_id  id;       //pool index of task
    uint8_t  condition;//what execution state the task is in
    uint8_t  runEvent; //what caused the task to run
    uint8_t  state;    //for the state machine
    uint64_t timestamp;//time stamp for next run time
    sem_id   TargetSem;//semaphore for next activation
    taskFunc func;     //function for task, in order: id, state, runEvent and data.
} Task;

typedef struct{
    sem_id id;
    uint8_t count;
} Semaphore;

typedef struct{
    queue_id id;
    uint8_t head;
    uint8_t tail;
    sem_id  q_not_empty; //semaphore for how many elements in queue
    sem_id  q_not_full; //semaphore for how many elements not in queue
    uint8_t data[QUEUE_SIZE];
} Queue;




/*****************************   Functions   *******************************/

//GENERAL RTCS FUNCTIONS
void rtcs_init(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Setup pools
******************************************************************************/
task_id init_task(task_id id, taskFunc func);
/*****************************************************************************
*   Input    : id of task to setup, function for task to run
*   Output   : -
*   Function : Setup a task
******************************************************************************/
void rtcs_run();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Run the RTCS
******************************************************************************/

//SEMAPHORE FUNCTIONS
sem_id load_Sem(sem_id id, uint8_t count);
/*****************************************************************************
*   Input    : semaphore to preload, value of semaphore
*   Output   : -
*   Function : Preload a semaphore value
******************************************************************************/
void signal_Sem(sem_id id);
/*****************************************************************************
*   Input    : semaphore to signal
*   Output   : -
*   Function : Signal semaphore, (increase count by 1)
******************************************************************************/
boolean wait_Sem(sem_id id, uint64_t timeout);
/*****************************************************************************
*   Input    : semaphore to wait for, and a timeout if needed (else set 0)
*   Output   : if it is already free
*   Function : Set the task to wait for a semaphore, if it is not free
******************************************************************************/

//QUEUE FUNCTIONS
//queue_id setup_Queue(queue_id id);
/*****************************************************************************
*   Input    : queue to setup
*   Output   : if it is already free
*   Function : Set the task to wait for a semaphore, if it is not free
******************************************************************************/

//boolean put_Queue(queue_id id, uint8_t data);
/*****************************************************************************
*   Input    : id of queue to put, data to put in queue
*   Output   : True if success (queue not full), false if failure (queue full)
*   Function : Store an element in the queue
******************************************************************************/
//boolean get_Queue(queue_id id, uint8_t *data);
/*****************************************************************************
*   Input    : id of queue to get, address of data to put queue data in
*   Output   : True if success (queue not empty), false if failure (queue empty)
*   Function : Get an element from the queue
******************************************************************************/

//Task functions
void set_State(uint8_t state);
void set_Event(uint8_t event);
void set_Timeout(uint64_t timeout);

/****************************** End Of Module *******************************/
#endif

