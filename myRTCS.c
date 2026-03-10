#include "myRTCS.h"

/*****************************    Globals    *******************************/
Task taskPool[NOF_TASKS];
Semaphore semPool[NOF_SEMS];
Queue queuePool[NOF_QUEUES];

task_id currentTask;

/********************************functions********************************** */

void rtcs_init(void){
    systick_init(15999);
    uint8_t i;
    for(i=0; i<NOF_SEMS;i++){
        load_Sem(i,0);
    }
    for(i=0; i<NOF_TASKS;i++){
        taskPool[i].condition = NO_TASK;
    }
}

task_id init_task(task_id id, taskFunc func){
    taskPool[id].id = id;
    taskPool[id].condition = TASK_READY;
    taskPool[id].runEvent = RESET_EVENT;
    taskPool[id].TargetSem = NO_SEM;
    taskPool[id].timestamp = FALSE;
    taskPool[id].func = func;
    return id;

}

void rtcs_run(void){
    currentTask = 0;
    uint8_t data = 0;
    do{
        //First check if we should release any of the restricting conditions
        if(taskPool[currentTask].TargetSem != NO_SEM){ //check if we are waiting for a semaphore
            sem_id sem = taskPool[currentTask].TargetSem;
            if(semPool[sem].count > 0){
                /*
                if(sem >= NOF_QUEUES*2){ //if it is not the special queue semaphores
                    semPool[sem].count--;
                    data = sem; //tell the task which semaphore released it
                }*/
                taskPool[currentTask].runEvent = SEM_EVENT;//tell the task that a semaphore released it
                taskPool[currentTask].TargetSem = NO_SEM;  //remove semaphore from task
                taskPool[currentTask].condition = TASK_READY;
            }
        }
        if((taskPool[currentTask].timestamp != FALSE)&&(taskPool[currentTask].timestamp <= get_ticks())){ //check if a timeout is active and has occured
            taskPool[currentTask].timestamp = FALSE;
            taskPool[currentTask].runEvent = TIME_EVENT;
            taskPool[currentTask].condition = TASK_READY;
        }

        if(taskPool[currentTask].condition == TASK_READY){
            taskPool[currentTask].func(taskPool[currentTask].id, taskPool[currentTask].state, taskPool[currentTask].runEvent, data);
        }
        currentTask++;
    }while(taskPool[currentTask].condition != NO_TASK);
}

//TASK FUNCTIONS
void set_State(uint8_t state){
    taskPool[currentTask].state = state;
}

void set_Event(uint8_t event){
    taskPool[currentTask].runEvent = event;
}

void set_Timeout(uint64_t timeout){
    taskPool[currentTask].timestamp = get_ticks() + timeout;
    taskPool[currentTask].condition = TASK_WAIT;
}   

//SEMAPHORE FUNCTIONS
sem_id load_Sem(sem_id id, uint8_t count){
    semPool[id].id = id;
    semPool[id].count = count;
    return id;
}

void signal_Sem(sem_id id){
    semPool[id].count++;
}

boolean wait_Sem(sem_id id, uint64_t timeout){

    if(semPool[id].count > 0){ //if the semaphore is already free
        semPool[id].count--;
        taskPool[currentTask].timestamp = FALSE;
        return TRUE;
    }else{
        taskPool[currentTask].TargetSem = id;
        taskPool[currentTask].timestamp = (timeout) ? get_ticks() + timeout : FALSE;
        taskPool[currentTask].condition = TASK_WAIT;
        return FALSE;
    }

}