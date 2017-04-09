/**
 * A set of functions and a main function for loading, manipulating and saving
 * calendar text files through a GUI.
 *
 * Author: Alex Burress
 */

#ifndef CALENDAR_H
#define CALENDAR_H
#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * A struct for holding a gui window and linked list ov events. Used to
 * conveniently pass windows and linked lists to callback functions
 */
typedef struct MenuData {
	Window* window;
	LinkedList* list;
} MenuData;

/**
 * Creates a gui window with 5 buttons for manipulating calendar data.
 * Used when the user doesn't provide a command line parameter.
 */
void createMainMenu(MenuData* menuAndList);

/**
 * Creates a gui window with 5 buttons for manipulating calendar data.
 * Used when the user provides a command line parameter.
 */
void createMenuFromFile(MenuData* menuAndList, char* filename);

/**
 * Used when the user supplies a command line parameter at startup. Creates a
 * linked list and adds events in the file to the linked list
 */
void loadCalFromCmd(void* data, char* filename);

/**
 * Prompts the user for a filename, and loads a corresponding file of event
 * data into a linked list
 */
void loadCalFromFile(void* data);

/**
 * Saves data in the linked list to a text file. The user is prompted for a
 * filename, the file is either created or overwritten.
 */
void saveCalToFile(void* data);

/**
 * Checks that the passed-in variables represent a valid date, time and duration
 * to go in an Event struct. Allows Feb 29th as a valid date on leap years.
 */
int eventValid(int inYear, int inMonth, int inDay, int inHours, int inMins, int inDuration);

/**
 * Determines whether a year is a leap year.
 */
int isLeapYear(int year);

/**
 * Adds an event to a linked list, a linked list is created if the passed-in
 * data does not contain one. Event data is gathered from user input.
 */
void addEvent(void* data);

/**
 * Prompts the user to enter a search string corresponding to an activity
 * in the linked list. If a match is found, the user is prompted to modify
 * it.
 */
void editEvent(void* data);

/**
 * Checks every activity in a linked list against the passed-in string.
 * If inActivity exists in any activity string on the list, a corresponding
 * linked list element number is returned. The value -1 is returned if no
 * match is found. The first matching event is returned, following matching
 * events are ignored.
 */
int findEvent(LinkedList* list, char* inActivity);

/**
 * Prompts the user to enter a search string corresponding to an activity
 * in the linked list. If a match is found, the event is deleted.
 */
void deleteEvent(void* data);

/**
 * Parses text from a file matching the passed-in filename. Text is formatted
 * for display in the gui main window.
 */
void readFile(void* data, char* filename);

#endif
