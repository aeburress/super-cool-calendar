/**
 * A set of functions for creating and manipulating a linked list of Event structs.
 * Each linked list has a head and a tail pointer, and a count of nodes on the list.
 *
 * Author: Alex Burress
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "linkedList.h"

/**
 * Creates an empty linked list.
 */
LinkedList* createList()
{
	LinkedList* newList;
	newList = (LinkedList*)malloc(sizeof(LinkedList));
	newList->head = NULL;
	newList->tail = NULL;
	newList->count = 0;
	
	return newList;
}

/**
 * Creates a node for the passed-in Event and inserts the node at the start of
 * the list.
 */
void insertFirst(LinkedList* list, Event* event)
{
	ListNode* newNode;
	ListNode* current;
	int ii;

	newNode = (ListNode*)malloc(sizeof(ListNode));
	newNode->data = event;

	/* if list is empty, point head and tail at the new node */
	if (list->head == NULL)
	{
		list->head = newNode;
		newNode->next = NULL;
		list->tail = newNode;
	}
	/* else make the new node point at the first node, and have head point to
	 * the new node.
	 */
	else
	{
		newNode->next = list->head;
		list->head = newNode;

		/* pointing tail to last node */
		current = list->head;
		for (ii = 1; ii < (list->count); ii++)
		{
			current = current->next;
		}
		list->tail = current;
	}

	list->count++;
}

/**
 * Creates a node for the passed-in Event, and inserts the node at the end
 * of the list.
 */
void insertLast(LinkedList* list, Event* event)
{
	ListNode* newNode;

	newNode = (ListNode*)malloc(sizeof(ListNode));
	newNode->data = event;
	newNode->next = NULL;

	/* if list is empty, point head and tail to the new node */
	if (list->count <= 0)
	{
		list->head = newNode;
		list->tail = newNode;
	}
	else
	{
		list->tail->next = newNode;
		list->tail = newNode;
	}
	list->count++;
}

/**
 * Removes the first node from the list, extracts and returns a pointer to
 * its Event, and frees the leftover node. Returns NULL if the passed-in list
 * has no nodes.
 */
Event* removeFirst(LinkedList* list)
{
	ListNode* removedNode;
	Event* outEvent;

	/* If list is empty, print error message */
	if (list->count <= 0)
	{
		printf("Error: linked list is empty\n");
		removedNode = NULL;
		outEvent = NULL;
		list->count = 0;
	}
	/* if list has only one node */
	else if (list->count == 1)
	{
		removedNode = list->head;
		list->head = NULL;
		list->tail = NULL;
		list->count--;
		outEvent = removedNode->data;
	}
	/* else list has 2+ nodes */
	else
	{
		removedNode = list->head;
		list->head = list->head->next;
		removedNode->next = NULL;
		list->count--;
		outEvent = removedNode->data;
	}
	
	free(removedNode);

	return outEvent;
}

/**
 * Deletes the n'th element on the linked list along with its node. The
 * passed-in int elNo represents n. Elements have a 0-based count.
 * i.e. if elNo = 0, the first node on the list is deleted. The variable
 * 'previous' is used to keep track of the node preceding to the node being
 * deleted. When the n'th node is deleted, previous->next is pointed to the
 * deleted node's next address.
 */
void deleteNthElement(LinkedList* list, int elNo)
{
	ListNode* current;
	ListNode* previous;
	int ii;
	
	assert(elNo >= 0);
	assert(elNo < list->count);
	
	current = list->head;
	previous = NULL;
	
	/* if deleting node that isn't head */
	if (elNo > 0)
	{
		for (ii = 0; ii < elNo; ii++)
		{
			previous = current;
			current = current->next;
		}
	
		previous->next = current->next;
	}
	/* else deleting head node */
	else
	{
		list->head = current->next;
	}

	list->count--;
	
	free(current->data);
	free(current);
}

/**
 * Returns a pointer to the n'th element on the list, where the count for n
 * is 0-based. i.e. if elNo = 0, the first event on the list is retrieved. 
 */
Event* retrieveElement(LinkedList* list, int elNo)
{
	ListNode* current;
	int ii;
	Event* returnEvent;

	current = list->head;

	/* check if list is empty */
	if (list->count == 0)
	{
		printf("Error: list is empty.\n");
		returnEvent = NULL;
	}
	/* validate that elNo is within bounds */
	else if (elNo > (list->count - 1))
	{
		printf("Error: There are fewer than %d elements in this list.\n", (elNo + 1));
		returnEvent = NULL;
	}
	else
	{
		/* if head is being retrieved */
		if (elNo == 0)
		{
			returnEvent = current->data;
		}
		/* else traverse list to desired element */
		else
		{
			for (ii = 0; ii < elNo; ii++)
			{
				current = current->next;
			}
			returnEvent = current->data;
		}
	}

	return returnEvent;
}

/**
 * Prints the state of each element in the list.
 */
void printList(LinkedList* list)
{
	int ii;
	ListNode* current;

	if (list->count == 0)
	{
		printf("Error: list is empty.\n");
	}
	else
	{
		current = list->head;
		current->data = retrieveElement(list, 0);
		printf("%s @ %s (%d minutes)\n%d %d %d, %d:%d\n---\n\n", current->data->activity, current->data->location, current->data->duration, current->data->eDate.day, current->data->eDate.month, current->data->eDate.year, current->data->eTime.hrs, current->data->eTime.mins);
		for (ii = 1; ii < (list->count); ii++)
		{
			current = current->next;
			printf("%s @ %s (%d minutes)\n%d %d %d, %d:%d\n---\n\n", current->data->activity, current->data->location, current->data->duration, current->data->eDate.day, current->data->eDate.month, current->data->eDate.year, current->data->eTime.hrs, current->data->eTime.mins);
		}
	}
}

/**
 * Frees each list node and event from memory, and the list itself.
 */
void freeList(LinkedList* list)
{
	ListNode* current;
	ListNode* next;
	int ii;

	current = list->head;

	for (ii = 0; ii < (list->count); ii++)
	{
		next = current->next;
		free(current->data); /* */
		free(current);
		current = next;
	}
	free(list);
}

