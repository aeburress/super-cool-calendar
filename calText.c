/**
 * Contains functions for manipulating strings related to calendar content.
 *
 * Author: Alex Burress
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedList.h"
#include "calText.h"

#define FALSE 0
#define TRUE !FALSE

/**
 * Finds the first new line character in the passed-in string and replaces it
 * with a null terminator.
 */
void removeNewline(char* inString)
{
	int ii;

	ii = 0;
	while (inString[ii] != '\n')
	{
		ii++;
	}
	
	inString[ii] = '\0';
}

/**
 * Stores the state of every Event struct in the passed-in linked list
 * in a single string. The string is formatted to be displayed in a gui
 * window.
 */
char* listToWindow(LinkedList* list)
{
	int ii;
	ListNode* current;
	char temp[500];
	char* state;

	state = (char*)malloc(list->count * sizeof(char) * 500);
	strcpy(state, "\0");

	if (list->count == 0)
	{
		sprintf(state,"Error: list is empty.\n");
	}
	else
	{
		current = list->head;
		current->data = retrieveElement(list, 0);
		
		/* collect text from the first element */
		parseEventWindow(temp, current->data);
		strcat(state, temp);

		/* then collect text from the remaining elements */
		for (ii = 1; ii < (list->count); ii++)
		{
			current = current->next;
			parseEventWindow(temp, current->data);
			strcat(state, temp);
		}
	}

	return state;
}

/**
 * Stores the state of every Event struct in the passed-in linked list
 * in a single string. The string is formatted to be printed in a text
 * file.
 */
char* listToText(LinkedList* list)
{
	int ii;
	ListNode* current;
	char temp[500];
	char* state;

	state = (char*)malloc(list->count * sizeof(char) * 500);
	strcpy(state, "\0");

	if (list->count == 0)
	{
		sprintf(state,"Calendar is empty.\n");
	}
	else
	{
		current = list->head;
		current->data = retrieveElement(list, 0);
		
		/* collect text from the first element */
		parseEventText(temp, current->data);
		strcat(state, temp);

		/* then collect text from the remaining elements */
		for (ii = 1; ii < (list->count); ii++)
		{
			current = current->next;
			parseEventText(temp, current->data);
			strcat(state, temp);
		}
	}

	return state;
}

/**
 * Parses the data in a single Event to the string eventState. The string
 * is formatted for display in the gui. Values are added to eventState one
 * by one until the string represents the state of the passed in Event.
 */
void parseEventWindow(char* eventState, Event* event)
{
	char tempLocation[100];
	char tempHrs[50];
	char tempMins[50];
	char twoDigits[50];
	char tempDay[50];
	char tempMonth[50];
	char tempYear[50];
	char tempTimeHrs[50];
	char tempTimeMins[50];
	int durHrs;
	int durMins;
	int printedHrs;
	
	/*splitting the duration (minutes) into hours and minutes */
	durHrs = event->duration / 60;
	durMins = event->duration % 60;
	
	/* initialising temporary strings */
	strcpy(tempLocation, "\0");
	strcpy(tempHrs, "\0");
	strcpy(tempMins, "\0");
	strcpy(twoDigits, "\0");
	strcpy(tempDay, "\0");
	strcpy(tempMonth, "\0");
	strcpy(tempYear, "\0");
	strcpy(tempTimeHrs, "\0");
	strcpy(tempTimeMins, "\0");
	
	/* add activity */
	sprintf(eventState, "%s", event->activity);
	
	/* add '@' if needed */
	if (strcmp(event->location, "\0") != 0)
	{
		strcpy(tempLocation, " @ ");
		strcat(tempLocation, event->location);
	}
	
	/* add location */
	strcat(eventState, tempLocation);
	strcat(eventState, " (");
	
	/* add duration hours component */
	if (durHrs == 1)
	{
		sprintf(tempHrs, "1 hour"); 
	}
	else if (durHrs >= 2)
	{
		sprintf(tempHrs, "%d hours", durHrs); 
	}
	strcat(eventState, tempHrs);
	
	/* if hrs == 0 or mins == 0, then a comma isn't printed before mins */
	if (durHrs != 0 && durMins != 0)
	{
		strcpy(tempMins, ", ");
	}	

	/* add duration minutes component */
	if (durMins == 1)
	{
		strcat(tempMins, "1 minute");
	}
	else if (durMins > 1)
	{
		sprintf(twoDigits, "%d minutes", durMins);
		strcat(tempMins, twoDigits);
	}
	strcat(eventState, tempMins);
	strcat(eventState, ")\n");

	/* add day */
	sprintf(tempDay, "%d ", event->eDate.day);
	strcat(eventState, tempDay);
	
	/* add month */
	spellMonth(tempMonth, event->eDate.month);
	strcat(eventState, tempMonth);
	
	/* add year */
	sprintf(tempYear, "%d, ", event->eDate.year);
	strcat(eventState, tempYear);
	
	/* add times hours component */
	if (event->eTime.hrs > 0 && event->eTime.hrs <= 12)
	{
		sprintf(tempTimeHrs, "%d", event->eTime.hrs);
	}
	else if(event->eTime.hrs == 0)
	{
		sprintf(tempTimeHrs, "12");
	}
	else
	{
		printedHrs = event->eTime.hrs - 12;
		sprintf(tempTimeHrs, "%d", printedHrs);
	}
	
	/* add time minutes component */
	if (event->eTime.mins != 0)
	{
		strcat(tempTimeHrs, ":");
	}
	strcat(eventState, tempTimeHrs);
	
	if (event->eTime.mins != 0)
	{
		sprintf(tempTimeMins, "%02d", event->eTime.mins);
	}
	
	/* add "am" or "pm" to time */
	if (event->eTime.hrs < 12)
	{
		strcat(tempTimeMins, "am\n---\n\n");
	}
	else
	{
		strcat(tempTimeMins, "pm\n---\n\n");
	}
	strcat(eventState, tempTimeMins);
	
	/* eventState is now formatted for the gui! */
}

/**
 * Parses the data in a single Event to the string eventState. The string
 * is formatted for being printed in a text file.
 */
void parseEventText(char* eventState, Event* event)
{
	sprintf(eventState, "%d-%02d-%02d %02d:%02d %d %s\n", event->eDate.year, event->eDate.month, event->eDate.day, event->eTime.hrs, event->eTime.mins, event->duration, event->activity);
	
	/* optionally concatenate location */
	if (strcmp(event->location, "\0") != 0)
	{
		strcat(eventState, event->location);
		strcat(eventState, "\n");
	}
	
	strcat(eventState, "\n");
}

/**
 * Finds the spelling of the month corresponding to the passed-in int.
 */
void spellMonth(char* textMonth, int numMonth)
{
	switch(numMonth)
	{
		case 1:
			strcpy(textMonth, "January ");
			break;
		case 2:
			strcpy(textMonth, "February ");
			break;
		case 3:
			strcpy(textMonth, "March ");
			break;
		case 4:
			strcpy(textMonth, "April ");
			break;
		case 5:
			strcpy(textMonth, "May ");
			break;
		case 6:
			strcpy(textMonth, "June ");
			break;
		case 7:
			strcpy(textMonth, "July ");
			break;
		case 8:
			strcpy(textMonth, "August ");
			break;
		case 9:
			strcpy(textMonth, "September ");
			break;
		case 10:
			strcpy(textMonth, "October ");
			break;
		case 11:
			strcpy(textMonth, "November ");
			break;
		case 12:
			strcpy(textMonth, "December ");
			break;
		default:
			break;
	}
}
