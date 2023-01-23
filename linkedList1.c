#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <math.h>


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
    return;
}

// Function to print the linked list
void printList(struct node* node) {
    while (node != NULL) {
        printf(" %d ", node->data);
        node = node->next;
    }
}
int random_numbers[1000];
// Main function
int main() {
    int iterations=283;
    double data_points[iterations];
    double total_time = 0;
    for (int j=0;j<iterations;j++){

        struct node* head = NULL;

        srand(10);

        int n  =1000;

        int m  =10000;

        double mMember = 0.50;
        double mInsert = 0.25;
        double mDelete = 0.25;

        for (int i = 0; i < 1000; i++){
            int random_number = rand()%65536;
            random_numbers[i] = random_number;
        }

        int count = 0;
        while (count<n){
            int random_number = rand()%65536;
            insert(&head, random_number);
            // printf("\n%d", random_number);
            count++;
        }

        clock_t start_time = clock(); 
        for (int i = 0; i < mInsert*m; i++) {
            int random_number =  random_numbers[i];
            insert(&head, random_number);
        }
        for (int i = 0; i < mMember*m; i++) {
            int random_number =  random_numbers[i];
            member(&head, random_number);
        }
        for (int i = 0; i < mDelete*m; i++) {
            int random_number =  random_numbers[i];
            delete(&head, random_number);
        }
        clock_t finish_time = clock();
        //printf("%.10f\n", start_time/CLOCKS_PER_SEC);
        //printf("%.10f\n", finish_time/CLOCKS_PER_SEC);
        double time_elapsed = ((double)(finish_time - start_time))/CLOCKS_PER_SEC;
        //printf("%.20f\n", time_elapsed);
        data_points[j] = time_elapsed;
        total_time+=time_elapsed;

    }
    double average_time= total_time/iterations;
    printf("Average Time %.5f\n", average_time);
    double squaredTotal = 0.0;
    for (int t=0; t<iterations;t++){
        double value =  (data_points[t]-average_time)*(data_points[t]-average_time);
        squaredTotal += value;
    }
    double sd = squaredTotal/iterations;
    //printf("Standard total %f\n", squaredTotal);
    printf("Standard Deviation %.5f\n", sd);
    return 0;
    //printList(head);
    
}