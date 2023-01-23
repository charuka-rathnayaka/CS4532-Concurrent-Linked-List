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
    struct node** head_ref;
    int m;
    double mMember;
    double mInsert;
    double mDelete;

};

void insert(struct node** head_ref, int new_data) {
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
    struct node *last = *head_ref;
    new_node->data = new_data;
    new_node->next = NULL;
    bool data_exists = false;
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }
    while (last->next != NULL){
        if(last->data== new_data){
            data_exists = true;
        }
        last = last->next;
    }
    if(data_exists != true){
        last->next = new_node;
    }
    else{
        free(new_node);
    }
    
    return;
}

bool member(struct node** head_ref, int new_data) {
    struct node *current_node = *head_ref;
    bool data_exists = false;
    if (*head_ref == NULL) {
        return false;
    }
    while (current_node->next != NULL){
        if(current_node->data== new_data){
            data_exists = true;
            break;
        }
        current_node = current_node->next;
    }
    if(data_exists == true){
        return true;
    }
    else{
        return false;
    }
}

// Function to delete a node with a given key
void delete(struct node** head_ref, int key) {
    struct node* temp = *head_ref, *prev;
    if (temp != NULL && temp->data == key) {
        *head_ref = temp->next;
        free(temp);
        return;
    }
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
}

// Function to print the linked list
void printList(struct node* node) {
    while (node != NULL) {
        printf(" %d ", node->data);
        node = node->next;
    }
}
int thread_count;

void *executeOperations(void* data){
    //struct ThreadData *myData=data;
    struct ThreadData myData = (struct ThreadData) data;
    
    //printf("Hello from thread %d of %d\n",threadID, m);
    //return NULL;
    //printf("Operations memeber %d insert %d delete %d\n",mMember, myData->mInsert,myData->mDelete);

    /*int mMemberOperations =(int) (myData->mMember)*(m/(threadCount));
    int mInsertOperations =(int) (myData->mInsert)*(m/(threadCount));
    int mDeleteOperations =(int) (myData->mDelete)*(m/(threadCount));
    printf("Operations memeber %d insert %d delete %d\n",mMemberOperations, mInsertOperations,mDeleteOperations);
*/
    /*

    for (int i = 0; i < mInsert*m; i++) {
        
        pthread_mutex_lock(&mutex);
        int random_number = rand()%65536;
        insert(&head, random_number);
        pthread_mutex_unlock(&mutex);
        
    }
    for (int i = 0; i < mMember*m; i++) {
        
        pthread_mutex_lock(&mutex);
        int random_number = rand()%65536;
        member(&head, random_number);
        pthread_mutex_unlock(&mutex);
    }
    for (int i = 0; i < mDelete*m; i++) {
        
        pthread_mutex_lock(&mutex);
        int random_number = rand()%65536;
        delete(&head, random_number);
        pthread_mutex_unlock(&mutex);
    }
    int finish_time = clock();
    double time_elapsed = (finish_time - start_time)/CLOCKS_PER_SEC;
    printf("%.20f\n", time_elapsed);*/
}

int main(int argc, char* argv[]) {

    struct node* head = NULL;
    int thread;
    pthread_t* thread_handles;
    

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int seed = rand()%100;
    srand(seed);

    int n  =1000;

    int m  =10000;
    double mMember = 0.99;
    double mInsert =0.005;
    double mDelete = 0.005;

    

    int count = 0;
    while (count<n){
        
        pthread_mutex_lock(&mutex);
        int random_number = rand()%65536;
        insert(&head, random_number);
        pthread_mutex_unlock(&mutex);
        // printf("\n%d", random_number);
        count++;
    }

    

    thread_count = strtol(argv[1],NULL,10);
    //printf("Count \n%d", thread_count);
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    


    for (thread =0; thread<thread_count; thread++){
        struct ThreadData *data = malloc(sizeof(struct ThreadData));;
        data -> threadID = thread;
        data -> threadCount = thread_count;
        data -> head_ref = &head;
        data -> m = m;
        data -> mMember = mMember;
        data -> mInsert = mInsert;
        data -> mDelete = mDelete;
        //printf("thread number creating \n%d", thread);
        pthread_create(&thread_handles[thread],NULL, executeOperations, (void *) data);

    }
    // printf("Hello from the main thread\n");

    for (thread=0;thread<thread_count;thread++){
        pthread_join(thread_handles[thread],NULL);
    }
    free(thread_handles);
    return 0;



    
    
    
    //printList(head);
    
}