#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

struct node {
    int data;
    struct node* next;
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

int main() {

    struct node* head = NULL;

    pthread_rwlock_t    rwlock;
    pthread_rwlock_init(&rwlock, NULL);

    int seed = rand()%100;
    srand(seed);

    int n  =rand()%10000+100;

    int m  =rand()%100000+2000000;

    double mMember = ((double)rand() / RAND_MAX);
    double mInsert = ((double)rand() / RAND_MAX);
    double mDelete = (1 - (mMember + mInsert));

    int count = 0;
    while (count<n){
        pthread_rwlock_wrlock(&rwlock);
        int random_number = rand()%65536;
        insert(&head, random_number);
        
        // printf("\n%d", random_number);
        count++;
    }

    int start_time = clock(); 
    for (int i = 0; i < mInsert*m; i++) {
        
        pthread_rwlock_wrlock(&rwlock);
        int random_number = rand()%65536;
        insert(&head, random_number);
        pthread_rwlock_unlock(&rwlock);
        
    }
    for (int i = 0; i < mMember*m; i++) {
        
        pthread_rwlock_rdlock(&rwlock);
        int random_number = rand()%65536;
        member(&head, random_number);
        pthread_rwlock_unlock(&rwlock);
    }
    for (int i = 0; i < mDelete*m; i++) {
        
        pthread_rwlock_wrlock(&rwlock);
        int random_number = rand()%65536;
        delete(&head, random_number);
        pthread_rwlock_unlock(&rwlock);
    }
    int finish_time = clock();
    double time_elapsed = (finish_time - start_time)/CLOCKS_PER_SEC;
    printf("%.20f\n", time_elapsed);
    //printList(head);
    
}