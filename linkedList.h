/**
 * A set of functions for creating and manipulating a linked list of Event structs.
 * Each linked list has a head and a tail pointer, and a count of nodes on the list.
 *
 * Author: Alex Burress
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/**
 * A struct representing a date with three integers.
 */
typedef struct Date {
	int day;
	int month;
	int year;
} Date;

/**
 * A struct representing time with two integers. Time should be stored
 * in 24 hour format.
 */
typedef struct Time {
	int hrs;
	int mins;
} Time;

/**
 * A struct representing a calendar event. Each event has a time, date,
 * duration in minutes, an activity, and optionally, a location.
 */
typedef struct Event {
	Date eDate;
	Time eTime;
	int duration;
	char activity[400];
	char location[100];
} Event;


/**
 * A linked list node. It holds a pointer to an Event and a pointer to another
 * node.
 */
typedef struct ListNode{
	Event* data;
	struct ListNode* next;
} ListNode;

/**
 * A double ended, singly linked list struct. It has head and tail pointers
 * for pointing at the first and last nodes respectively, and an int for
 * storing the count of nodes on the list.
 */
typedef struct {
	ListNode* head;
	ListNode* tail;
	int count;
} LinkedList;

/**
 * Creates an empty linked list.
 */
LinkedList* createList();

/**
 * Creates a node for the passed-in Event and inserts the node at the start of
 * the list.
 */
void insertFirst(LinkedList* list, Event* event);

/**
 * Creates a node for the passed-in Event, and inserts the node at the end
 * of the list.
 */
void insertLast(LinkedList* list, Event* event);

/**
 * Removes the first node from the list, extracts and returns a pointer to
 * its Event, and frees the leftover node. Returns NULL if the passed-in list
 * has no nodes.
 */
Event* removeFirst(LinkedList* list);

/**
 * Deletes the n'th element on the linked list along with its node. The
 * passed-in int elNo represents n. Elements have a 0-based count.
 * i.e. if elNo = 0, the first node on the list is deleted. The variable
 * 'previous' is used to keep track of the node preceding to the node being
 * deleted. When the n'th node is deleted, previous->next is pointed to the
 * deleted node's next address.
 */
void deleteNthElement(LinkedList* list, int elNo);

/**
 * Returns a pointer to the n'th element on the list, where the count for n
 * is 0-based. i.e. if elNo = 0, the first event on the list is retrieved. 
 */
Event* retrieveElement(LinkedList* list, int elNo);

/**
 * Prints the state of each element in the list.
 */
void printList(LinkedList* list);

/**
 * Frees each list node and event from memory, and the list itself.
 */
void freeList(LinkedList* list);

#endif
