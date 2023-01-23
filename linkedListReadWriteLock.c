#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

struct node {
    int data;
    struct node* next;
};

struct ThreadData {
    int threadID;
    int threadCount;
    struct node** head;
    int m;
    pthread_rwlock_t rwlock;

};

void insert(struct node** head_ref, int new_data,pthread_rwlock_t rwlock) {
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
    struct node *last = *head_ref;
    new_node->data = new_data;
    new_node->next = NULL;
    bool data_exists = false;
    
    if (*head_ref == NULL) {
        pthread_rwlock_wrlock(&rwlock);
        *head_ref = new_node;
        pthread_rwlock_unlock(&rwlock);
        return;
    }
    pthread_rwlock_wrlock(&rwlock);
    while (last->next != NULL){
        if(last->data== new_data){
            data_exists = true;
        }
        last = last->next;
    }
    if(data_exists != true){
        last->next = new_node;
        pthread_rwlock_unlock(&rwlock);
    }
    else{
        pthread_rwlock_unlock(&rwlock);
        free(new_node);
    }
    
    
    return;
}

bool member(struct node** head_ref, int new_data,pthread_rwlock_t rwlock) {
    struct node *current_node = *head_ref;
    bool data_exists = false;
    if (*head_ref == NULL) {
        //pthread_rwlock_unlock(&rwlock);
        return false;
    }
    pthread_rwlock_rdlock(&rwlock);
    while (current_node->next != NULL){
        if(current_node->data== new_data){
            data_exists = true;
            break;
        }
        current_node = current_node->next;
    }
    pthread_rwlock_unlock(&rwlock);
    if(data_exists == true){
        return true;
    }
    else{
        return false;
    }

}

// Function to delete a node with a given key
void delete(struct node** head_ref, int key,pthread_rwlock_t rwlock) {
    struct node* temp = *head_ref, *prev;
    
    if (temp != NULL && temp->data == key) {
        pthread_rwlock_wrlock(&rwlock);
        *head_ref = temp->next;
        pthread_rwlock_unlock(&rwlock);
        free(temp);
        return;
    }
    pthread_rwlock_wrlock(&rwlock);
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL){
        pthread_rwlock_unlock(&rwlock);
        return;
    } 
    prev->next = temp->next;
    pthread_rwlock_unlock(&rwlock);
    free(temp);
    return;
}

// Function to print the linked list
void printList(struct node* node) {
    while (node != NULL) {
        printf(" %d ", node->data);
        node = node->next;
    }
}

int thread_count;
int random_numbers[1000];

void *executeOperations(void* data){
    struct ThreadData *myData = (struct ThreadData*) data;
    double mMember = 0.99;
    double mInsert = 0.005;
    double mDelete = 0.005;
    // printf("data %d",myData->threadCount);

    double mMemberOperations = (mMember)*(myData->m);
    double mInsertOperations = (mInsert)*(myData->m/(myData->threadCount));
    double mDeleteOperations =(mDelete)*(myData->m/(myData->threadCount));
    //printf("data %f %f %f %d \n",mMemberOperations,mInsertOperations,mDeleteOperations,myData->m);

    for (int i = 0; i < mMemberOperations; i++) {
        int random_number = random_numbers[i];
        insert(&(myData->head), random_number,(myData->rwlock));
    }
    for (int i = 0; i < mInsertOperations; i++) {
        int random_number = random_numbers[i];
        member(&(myData->head), random_number,(myData->rwlock));
    }
    for (int i = 0; i < mDeleteOperations; i++) {
        int random_number = random_numbers[i];
        delete(&(myData->head), random_number,(myData->rwlock));
    }
       
}

int main(int argc, char* argv[]) {

    struct node* head = NULL;
    int thread;
    pthread_t* thread_handles;
    pthread_rwlock_t    rwlock;
    pthread_rwlock_init(&rwlock, NULL);

    srand(10);

    int n  =1000;
    int m  =10000;

    for (int i = 0; i < 1000; i++){
        int random_number = rand()%65536;
        random_numbers[i] = random_number;
    }

    int count = 0;
    while (count<n){
        int random_number = rand()%65536;
        insert(&head, random_number,rwlock);
        count++;
    }

    thread_count = strtol(argv[1],NULL,10);
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    
    clock_t start_time = clock(); 

    for (thread =0; thread<thread_count; thread++){
        struct ThreadData *data = malloc(sizeof(struct ThreadData));
        data -> threadID = thread;
        data -> threadCount = thread_count;
        data -> head = head;
        data -> m = m;
        data -> rwlock = rwlock;
        pthread_create(&thread_handles[thread],NULL, executeOperations, (void *) data);

    }

    for (thread=0;thread<thread_count;thread++){
        pthread_join(thread_handles[thread],NULL);
    }
    clock_t finish_time = clock();
    double time_elapsed = ((double)(finish_time - start_time))/CLOCKS_PER_SEC;
    printf("%f\n", time_elapsed);
    free(thread_handles);
    return 0;
    
}