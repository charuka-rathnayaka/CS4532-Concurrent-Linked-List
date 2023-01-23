# CS4532-Concurrent-Linked-List
Implementing a linked list as a:
a) Serial program
b) Parallel program (based on Pthreads) with one mutex for the entire linked list
c) Parallel program (based on Pthreads) with read-write locks for the entire linked list


How to Run the Program

1) Save and Extract the folder on a prefered Location
2) Open the terminal
3) To compile the Serial Linked List type the command "gcc linkedList1.c -o link_list.exe"
4) To compile the Parallel program with one mutex for the entire linked list type the command "gcc -g -Wall -o link_list_mutex linkedListOneMutex.c -Ipthread"
5) To compile the Parallel program (based on Pthreads) with read-write locks for the entire linked list type the command "gcc -g -Wall -o link_list_rwlock linkedListReadWriteLock.c -Ipthread"
6) To run the serial Linked List run the command "./link_list.exe"
7) To run the Parallel program with one mutex for the entire linked list with multiple threads run the command ".\link_list_mutex.exe <thread_count>"
8) To run the parallel program with read-write locks for the entire linked list with multiple threads run the command ".\link_list_rwlock.exe <thread_count>"
