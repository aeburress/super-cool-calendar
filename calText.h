/**
 * Contains functions for manipulating strings related to calendar content.
 *
 * Author: Alex Burress
 */

#ifndef CALTEXT_H
#define CALTEXT_H
#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Finds the first new line character in the passed-in string and replaces it
 * with a null terminator.
 */
void removeNewline(char* inString);

/**
 * Stores the state of every Event struct in the passed-in linked list
 * in a single string. The string is formatted to be displayed in a gui
 * window.
 */
char* listToWindow(LinkedList* list);

/**
 * Stores the state of every Event struct in the passed-in linked list
 * in a single string. The string is formatted to be printed in a text
 * file.
 */
char* listToText(LinkedList* list);

/**
 * Parses the data in a single Event to the string eventState. The string
 * is formatted for display in the gui. Values are added to eventState one
 * by one until the string represents the state of the passed in Event.
 */
void parseEventWindow(char* eventState, Event* event);

/**
 * Parses the data in a single Event to the string eventState. The string
 * is formatted for being printed in a text file.
 */
void parseEventText(char* eventState, Event* event);

/**
 * Finds the spelling of the month corresponding to the passed-in int.
 */
void spellMonth(char* textMonth, int numMonth);

#endif
