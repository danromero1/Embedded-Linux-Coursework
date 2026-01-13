#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define UNITTIME 4 //in microseconds
#define CTX_SWITCH 2 //in microseconds
#define N 1000000.0

int next_pid = 1;

typedef struct _Process {
    int pid;
    char name;
    int size; //remaining work
    int tickets;
    double stride;
    double penalty; //this is the same as pass

    struct _Process* next;
} Process;

typedef struct _Queue {
    Process* first;
    int count;

} Queue;

Queue* create_queue();      //allocates and intializes empty queues
Process* dequeue(Queue* q); //removes the front process
Process* create_process(char name,int size,int tickets);

int enqueue_sorted(Queue* q, Process* p);
double update_process(Process* p);
int run_scheduler(Queue* q);
void free_queue(Queue* q);

int main() {
    Queue* lottery_queue = create_queue();
    int n, mode;

    printf("Enter number of processes to create: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("No processes to schedule.\n");
        free(lottery_queue);
        return 0;
    }

    printf("Ticket mode:\n");
    printf("  1 - Manual ticket entry\n");
    printf("  2 - Random ticket generation\n");
    printf("Select mode: ");
    scanf("%d", &mode);

    if (mode == 2) srand(time(NULL));  // seed RNG for random tickets

    for (int i = 0; i < n; i++) {
        char name;
        int size, tickets;

        printf("\nEnter process name (single letter): ");
        scanf(" %c", &name);

        printf("Enter process size (work units): ");
        scanf("%d", &size);
        if (size <= 0) {
            printf("Invalid size, setting to 1.\n");
            size = 1;
        }

        if (mode == 1) {
            printf("Enter number of tickets: ");
            scanf("%d", &tickets);
            if (tickets <= 0) {
                printf("Invalid tickets, setting to 1.\n");
                tickets = 1;
            }
        } else {
            tickets = (rand() % 200) + 1; // random between 1–200
            printf("Randomly assigned %d tickets.\n", tickets);
        }

        Process* p = create_process(name, size, tickets);
        enqueue_sorted(lottery_queue, p);
    }

    printf("\nRunning scheduler...\n");
    run_scheduler(lottery_queue);
    free_queue(lottery_queue);

    return 0;
}


Queue* create_queue() {
    return calloc(1, sizeof(Queue)); //replaces all the code under it
    
    // Queue* result = malloc(sizeof(Queue)); 
    // if(result == NULL)
    // return NULL;
    
    // result->first = NULL;
    // result->count = 0;

    // return result;
}

Process* create_process(char name,int size,int tickets) {
    Process* result;
    result = malloc(sizeof(Process));
    if(result == NULL)
    return NULL;

    result->pid = next_pid++;
    result->next = NULL;
    result->name = name;
    result->size = size;
    result->tickets = tickets;
    
    if(result->tickets <= 0) result->tickets = 1;
    result->penalty = 0.0;
    result->stride = N / result->tickets;

    return result;

}

int enqueue_sorted(Queue* q, Process* p) {
    if (q == NULL) return -1;
    if (p == NULL) return 0;

    // defensive: don't carry stale links
    p->next = NULL;

    // Case 1: empty queue
    if (q->first == NULL) {
        q->first = p;
        q->count++;
        return 0;
    }

    // Case 2: insert before head (smaller penalty or better tie-break)
    Process* head = q->first;
    if (p->penalty < head->penalty ||
        (p->penalty == head->penalty && p->tickets > head->tickets)) {
        p->next = head;
        q->first = p;
        q->count++;
        return 0;
    }

    // Case 3: walk to the insertion point
    Process* prev = q->first;
    Process* curr = q->first->next;

    // Keep advancing while 'curr' should stay before 'p'
    while (curr != NULL &&
           (curr->penalty < p->penalty ||
            (curr->penalty == p->penalty && curr->tickets >= p->tickets))) {
        prev = curr;
        curr = curr->next;
    }

    // Insert p between prev and curr
    prev->next = p;
    p->next = curr;
    q->count++;
    return 0;
}


Process* dequeue(Queue* q) {
    if(q == NULL) return NULL;          //invalid argument from main
    if(q->first == NULL) return NULL;   //queue is empty
    
    Process* result = q->first;         //record first process
    q->first = q->first->next;          //move pointer to next queue
    result->next = NULL;                //detach node
    q->count--;                         //decrement queue list count
    
    return result;                      //return last node in queue
}

double update_process(Process* p) { //sim running p and update fairness
    if(p == NULL) return 0;
    
    double actual = (UNITTIME < p->size) ? UNITTIME : p->size;
    p->size -= actual;
    
    p->penalty += p->stride * (actual / (double)UNITTIME );
    
    return actual;
    
}

int run_scheduler(Queue* q) {
    if(q == NULL) return -1;

    double sim_time = 0.0;
    double util_time = 0.0;
    double switch_time = 0.0;

    printf("Slice log:\n");
    printf("  name used(us)  size(before->after)     pass(before->after)    stride\n");

    while(q->count > 0) {
        Process* p = dequeue(q);
        if (p == NULL) break;

        
        int    size_before  = p->size;
        double pass_before  = p->penalty;

  
        double used = update_process(p);

   
        int    size_after   = p->size;
        double pass_after   = p->penalty;

        util_time += used;
        sim_time  += used;

        /* per-slice printout */
        printf("   %c    %6.0f        %6d -> %-6d     %8.0f -> %-8.0f    %6.1f\n",
               p->name, used, size_before, size_after,
               pass_before, pass_after, p->stride);

        if(p->size > 0) {
            enqueue_sorted(q, p);
        } else {
            free(p);
        }

        /* account for context switch cost between slices (optional) */
        if (q->count > 0) {
            switch_time += CTX_SWITCH;
            sim_time    += CTX_SWITCH;
            printf("        (context switch +%d us)\n", CTX_SWITCH);
        }
    }

    /* summary */
    double eta = (sim_time > 0) ? (100.0 * util_time / sim_time) : 0.0;
    printf("\nSummary:\n");
    printf("  Util time:   %.0f us\n", util_time);
    printf("  Switch time: %.0f us\n", switch_time);
    printf("  Total time:  %.0f us\n", sim_time);
    printf("  Efficiency:  %.1f%%\n\n", eta);

    return 0;
}


void free_queue(Queue* q) {
    if (q == NULL) return;

    Process* curr = q->first;
    while (curr != NULL) {
        Process* next = curr->next;
        free(curr);
        curr = next;
    }
    free(q);
}
