#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strtok() split the string or parse
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <getopt.h> // getopt() function parses the command-line arguments.

#define MAX_PROCESSES 200
#define MAX_ARGUMENTS 10

int isEmpty();
int length(int length);
struct Double_LinkedList *find(int quantum);
void insertFirst_HelperFunction(int quantum, char *program);
void insertLast_HelperFunction(int quantum, char *program);
int insertAfter_HelperFunction(int quantum, char *proram);

/*
void checkArgs(int argc, char *argv[])
{

}
*/
//#1
// Develop a Double Linked List struct
struct Double_LinkedList
{
    int quantum;
    char *program;
    char arguments[MAX_ARGUMENTS];
    struct Double_LinkedList *next; // Points to the next node in the Linked List.
    struct Double_LinkedList *prev; // Points to the previous node in the Linked List.
};

struct Double_LinkedList *head = NULL; // This link always point to the 1st link.
struct Double_LinkedList *last = NULL; // This link always point to the 2nd link.
struct Double_LinkedList *current = NULL;

int isEmpty()
{
    return head == NULL;
}

int length(int length)
{
    length = 0;

    struct Double_LinkedList *current;

    for (current = head; current != NULL; current = current->next)
    {
        length++;
    }
    return length;
}

struct Double_LinkedList *find(int quantum)
{
    struct Double_LinkedList *current = head; // Start from the 1st link.

    if (head == NULL) // If the list is empty.
    {
        return NULL;
    }

    while (current->quantum != quantum) // Navigating via the list.
    {
        if (current->next == NULL) // If it's the last node.
        {
            return NULL;
        }
        else
        {
            // Go to the next link.
            current = current->next;
        }
    }

    // If data found, return the current link.
    return current;
}

void insertFirst_HelperFunction(int quantum, char *program)
{
    // Creating a link.
    struct Double_LinkedList *link = (struct Double_LinkedList *)malloc(sizeof(struct Double_LinkedList));
    link->quantum = quantum;
    link->program = program;

    if (isEmpty())
    {
        // Connect to the last link.
        last = link;
    }
    else
    {
        // Update 1st previous link.
        head->prev = link;
    }

    // Point it to the old first link.
    link->next = head;

    // Point 1st to new 1st link.
    head = link;

    return;
}

void insertLast_HelperFunction(int quantum, char *program) // Insert the link at the last location. 
{
    // Creating a link.
    struct Double_LinkedList *link = (struct Double_LinkedList*) malloc(sizeof(struct Double_LinkedList));
    link->quantum = quantum;
    link->program = program;

    if(isEmpty())
    {
        last = link; // Setting this as the last link. 
    }
    else
    {
        last->next = link; // Making the link a new last link. 
        link->prev = last; // This mark the old last node as previous of new link. 
    }

    last = link; // point to the new last node. 
}

int insertAfter_HelperFunction(int quantum, char *program)
{
    struct Double_LinkedList *current = head;

    if(head == NULL)
    {
        return 0;
    }

    while(current->quantum != quantum)
    {
        if(current->next == NULL)
        {
            return 0;
        }
        else
        {
            current = current->next;
        }
    }

    struct Double_LinkedList *link_connection = (struct Double_LinkedList*) malloc(sizeof(struct Double_LinkedList));
    link_connection->quantum = quantum;
    link_connection->program = program;

    if(current == last)
    {
        link_connection->next = NULL;
        last = link_connection;
    }
    else
    {
        link_connection->next = current->next;
        current->next->prev = link_connection;
    }

    link_connection->prev = current;
    current->next = link_connection;

    return 1; // 1 represents True.
}

struct Double_LinkedList *deleteFirst() // Delete 1st item.
{
    struct Double_LinkedList *link_connection = head;

    if(head->next == NULL)
    {
        last = NULL;
    }
    else
    {
        head->next->prev = NULL;
    }

    head = head->next;

    return link_connection;
}
struct Double_LinkedList *delete (int quantum) // Delete a link with given quantum.
{
    // Start from the 1st link.
    struct Double_LinkedList *current = head;
    struct Double_LinkedList *previous = NULL;

    // Checking if the list is empty.
    if (head == NULL)
    {
        return NULL;
    }

    // Navigating via the list.
    while (current->quantum != quantum)
    {
        // Checking if it is the last node in the list.
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            // Storing the reference to the current link.
            previous = current;

            // Now moving to the next link in the list.
            current = current->next;
        }
    }

    // If you found a match in the list, update the link.
    if (current == head)
    {
        // Changing the 1st to point to the next link.
        head = head->next;
    }
    else
    {
        // Bypassing the current link in the list.
        current->prev->next = current->next;
    }

    if (current == last)
    {
        // Changing the last link to point to the previous link in the list.
        last = current->prev;
    }
    else
    {
        current->next->prev = current->prev;
    }
    return current;
}
// function that returns us to head of linked list (Go back to head if on last program)
// function that deletes based off program name (key in document)

int main(int argc, char *argv[])
{
    // parse *argv for quantum and program arguments

    int quantum = argv[0]; // quantum
    int opt;

    //#2
    // reading all of argv and putting into struct
    // optind starts at 1
    while ((opt = getopt(argc, argv, ":if:lrx")) != -1)
    {
        switch (opt)
        {
            case ':':
                        printf("You are at the end of Prog\n");
                        // check first insert program into linkedlist
                        if (isEmpty() == 1)
                        {
                            // insert first program function
                            //  insertFirst_HelperFunction
                        }
                        else
                        {
                            // insertAfter function
                        }
                        break;

            default:
                    for (; optind < argc; optind++)
                    {
                        // insert last program into linked list
                        if (optind == argc - 1)
                        {
                            // insert last function
                            //  insertLast_HelperFunction()
                        }
                        // avoid quantum
                        if (optind != 1)
                        {
                            printf("Extra arguments that won't be parsed: %s\n", argv[optind]);
                        }
                    }
                    break;
        }
    }

    // prof notes:
    // take whole argv
    // read each program once at a time
    // if colon, move to other program or stop at last one (?)

    // create struc of linked lists of programs
    // this is the dynamically allocated array

    // getopt() for parsing

    // steps to take for program
    // 1. Develop doubly linked list struct
    // 2. Read in all argv and put program information into the struct
    // 3. Use struct to cycle through each process excution
    // 4. Remove/Free processes that are completed in the linked list
    // 5. Repeat until all processes completed

    return 0;
}