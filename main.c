#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>

int n; // number of philosophers
int day = 1; // current day
int *porsion; // porsions of eaten by each philosophers
int *round_porsion; // porsions of eaten by each philosophers in a day
int *hungry_days; // number of days philosopher can not eat
int *max_hungry_days; // max hungry days
int amount = 0; // rice amount
int total_amount = 0; // total rice amount served by waiter for 1000 days
sem_t tray; // semaphore for tray, it is for if amount 1, 2 philosophers cannot be eat because amount will be negative in this scenario
sem_t *sticks; // semaphore for sticks

int right(int phnum) {
    // right func to control stick semaphore
    return (phnum+1)%n;
}

void eat(int phnum) {

    while(amount <= 0){ // empty tray
        if(day > 1000) // 1000th day completed, waiter will not put rice. can't eat
            return;
        usleep(1000); // wait waiter
    }
    amount--;
    porsion[phnum]++; // increase porsion
    round_porsion[phnum]++;
}
 
void take_sticks(int phnum) {
    
    // take sticks
    sem_wait(&sticks[phnum]);
    sem_wait(&sticks[right(phnum)]);

    //eat
    sem_wait(&tray);
    eat(phnum);
    sem_post(&tray);

    // sticks can be free
    sem_post(&sticks[phnum]);
    sem_post(&sticks[right(phnum)]);

    // think
    usleep(3000);
}

void* philosopher(void* param) {

    int* phnum = param;
    while (day <= 1000)
        take_sticks(*phnum);
    pthread_exit(NULL);
}

void serve() {

    // I assumed that if tray is still not empty after 10ms, waiter will wait philosophers before serve
    // So in each day waiter will serve
    int given_amount = (rand() % n) + 1; // random serve amount
    while(amount > 0) // if tray is not null, wait
        usleep(10);

    // amount is 0, endday calculations
    if(day != 1){
        for (int i = 0; i < n; i++){
            if(round_porsion[i] == 0) // if didnt eat in day
                hungry_days[i]++;
            else{ // if ate, calculate max hungry day
                if(hungry_days[i] > max_hungry_days[i])
                    max_hungry_days[i] = hungry_days[i];
                hungry_days[i] = 0;
            }
            round_porsion[i] = 0;
        }
    }

    amount = given_amount; // reset amount
    total_amount += given_amount;
}

void* waiter(void* param){

    while (day <= 1000) {
        if(day%100 == 0) // only info
            printf("Day %d simulating...\n", day);
        serve();
        day++;
        usleep(10000); // 10 ms is a day
    }
    pthread_exit(NULL);
}
 
void initialize(){

    // set attr and param variables
	struct sched_param param;
    pthread_t phil_threads[n];
    pthread_t waiter_thread;
    pthread_attr_t attr;
    pthread_attr_init (&attr);
    int a[n]; // this is for send phil nums to threads as parameter
    
    // first 1000 days , no priority difference
    printf("First 1000 days with equal priorities...\n");

    // semaphore and array inits
    sem_init(&tray, 0, 1);
    for (int i = 0; i < n; i++){
		sem_init(&sticks[i], 0, 1);
		porsion[i] = 0;
        a[i] = i;
	}
    
    // create threads
    pthread_create(&waiter_thread, &attr, waiter, NULL);
    for (int i = 0; i < n; i++)
        pthread_create(&phil_threads[i], &attr, philosopher, (void *)&a[i]);
    
    pthread_join(waiter_thread, NULL);    
    // join threads
    for (int i = 0; i < n; i++)
        pthread_join(phil_threads[i], NULL); 

    // print stats
    printf("Total number of porsions given by waiter: %d\n", total_amount);
    for (int i = 0; i < n; i++){
        printf("Number of porsions eaten by philosopher %d: %d\n", i+1, porsion[i]);
        printf("Max hungry days of philosopher %d: %d\n", i+1, max_hungry_days[i]);
    }

    // second 1000 days , with priority difference
    printf("\n\nRestarting simulation with make first philosopher prior...\n");
    
    // reset stats and some variables
    day = 1;
    amount = 0;
    total_amount = 0;
    sem_init(&tray, 0, 1);
    for (int i = 0; i < n; i++){
		sem_init(&sticks[i], 0, 1);
		porsion[i] = 0;
        round_porsion[i] = 0;
        hungry_days[i] = 0;
        max_hungry_days[i] = 0;
	}
    
    // set higher priority for first philosopher
    struct sched_param param_prior;
    pthread_attr_t attr_prior;
    pthread_attr_init (&attr_prior);
    pthread_attr_getschedparam (&attr_prior, &param_prior);
    param_prior.sched_priority = (10);
    pthread_attr_setschedparam (&attr_prior, &param_prior);

    // create threads, first philosopher with different attr. others are equal
    pthread_create(&waiter_thread, &attr, waiter, NULL);
    pthread_create(&phil_threads[0], &attr_prior, philosopher, (void *)&a[0]);
    for (int i = 1; i < n; i++){
        pthread_attr_getschedparam (&attr, &param);
        param.sched_priority = (1);
        pthread_attr_setschedparam (&attr, &param);
        pthread_create(&phil_threads[i], &attr, philosopher, (void *)&a[i]);
    }
    
    // join threads
    pthread_join(waiter_thread, NULL);
    for (int i = 0; i < n; i++)
        pthread_join(phil_threads[i], NULL); 
    
    // print stats
    printf("Total number of porsions given by waiter: %d\n", total_amount);
    for (int i = 0; i < n; i++){
        printf("Number of porsions eaten by philosopher %d: %d\n", i+1, porsion[i]);
        printf("Max hungry days of philosopher %d: %d\n", i+1, max_hungry_days[i]);
    }
}

int main(int argc, char **argv) {

    if(argc != 2){ // check there is only one argument
        printf("Please give one argument as number of philosophers!\n");
        exit(1);
    }
    n = atoi(argv[1]);
    if(n <= 1){ // check if argument is valid or not
        printf("Please give number of philosophers larger than 1\n");
        exit(1);
    }

    printf("Number of philosophers is: %d\n", n);
    // mallocs
    porsion = malloc( n * sizeof(int));
    round_porsion = malloc( n * sizeof(int)); 
    hungry_days = malloc( n * sizeof(int));
    max_hungry_days = malloc( n * sizeof(int));
    sticks = malloc( n * sizeof(sem_t));

    // running init and exit
    initialize();
    exit(0);
}  
