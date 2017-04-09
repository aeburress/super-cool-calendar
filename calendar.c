/**
 * A set of functions and a main function for loading, manipulating and saving
 * calendar text files through a GUI.
 *
 * Author: Alex Burress
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gui.h"
#include "calendar.h"
#include "linkedList.h"
#include "calText.h"
#define FALSE 0
#define TRUE !FALSE

/**
 * Main can optionally take one command line parameter, the command line
 * parameter is the name of a text file containing text formatted for the
 * calendar interface. Main creates a menu and runs a gui for manipulating
 * and viewing calendar data.
 */
int main(int argc, char** argv)
{
	MenuData* menuAndList;

	/* no parameter provided, no calendar loaded at startup */
	if (argc == 1)
	{
		menuAndList = (MenuData*)malloc(sizeof(MenuData));
	
		createMainMenu(menuAndList);

		runGUI(menuAndList->window);

		freeWindow(menuAndList->window);
		freeList(menuAndList->list); /* */
		free(menuAndList);
	}
	/* parameter provided, try and load file at startup */
	else if (argc == 2)
	{
		menuAndList = (MenuData*)malloc(sizeof(MenuData));
	
		createMenuFromFile(menuAndList, argv[1]);

		runGUI(menuAndList->window);

		freeWindow(menuAndList->window);
		freeList(menuAndList->list); /* */
		free(menuAndList);
	}
	/* more than 1 parameter entered */
	else
	{
		printf("Error: This program only accepts 1 parameter representing a text formatted calendar file\n");
	}

	return 0;
}

/**
 * Creates a gui window with 5 buttons for manipulating calendar data.
 * Used when the user doesn't provide a command line parameter.
 */
void createMainMenu(MenuData* menuAndList)
{
	menuAndList->window = createWindow("Amazing Calendar Interface");

	addButton(menuAndList->window, "Load a calendar from file", &loadCalFromFile, (void*)menuAndList);
	addButton(menuAndList->window, "Save the current calendar to file", &saveCalToFile, (void*)menuAndList);
	addButton(menuAndList->window, "Add a calendar event", &addEvent, (void*)menuAndList);
	addButton(menuAndList->window, "Edit a calendar event", &editEvent, (void*)menuAndList);
	addButton(menuAndList->window, "Delete a calendar event", &deleteEvent, (void*)menuAndList);
}

/**
 * Creates a gui window with 5 buttons for manipulating calendar data.
 * Used when the user provides a command line parameter.
 */
void createMenuFromFile(MenuData* menuAndList, char* filename)
{
	menuAndList->window = createWindow("Amazing Calendar Interface");
	loadCalFromCmd((void*)menuAndList, filename);

	addButton(menuAndList->window, "Load a calendar from file", &loadCalFromFile, (void*)menuAndList);
	addButton(menuAndList->window, "Save the current calendar to file", &saveCalToFile, (void*)menuAndList);
	addButton(menuAndList->window, "Add a calendar event", &addEvent, (void*)menuAndList);
	addButton(menuAndList->window, "Edit a calendar event", &editEvent, (void*)menuAndList);
	addButton(menuAndList->window, "Delete a calendar event", &deleteEvent, (void*)menuAndList);
}

/**
 * Used when the user supplies a command line parameter at startup. Creates a
 * linked list and adds events in the file to the linked list
 */
void loadCalFromCmd(void* data, char* filename)
{
	((MenuData*)data)->list = createList();

	readFile(data, filename);
}

/**
 * Prompts the user for a filename, and loads a corresponding file of event
 * data into a linked list
 */
void loadCalFromFile(void* data)
{
	InputProperties* properties;
	char** inputs;
	int clickedOk;
	
	/* if a linked list already exists in the MenuData, free it */
	if (((MenuData*)data)->list != NULL && ((MenuData*)data)->list->count > 0)
	{
		freeList(((MenuData*)data)->list);
	}
	
	((MenuData*)data)->list = createList();

	properties = (InputProperties*)malloc(sizeof(InputProperties));
	properties->label = "Enter filename";
	properties->maxLength = 20;
	properties->isMultiLine = FALSE;

	inputs = (char**)malloc(sizeof(char*));
	inputs[0] = (char*)malloc(21*sizeof(char));
	strcpy(inputs[0], "");

	/* have user enter filename */
	clickedOk = dialogBox(((MenuData*)data)->window, "Load a calendar from file", 1, properties, inputs);

	if (clickedOk == TRUE)
	{
		readFile(data, inputs[0]);
	}
	
	free(properties); /* */
	free(inputs[0]); /* */
	free(inputs); /* */
}

/**
 * Saves data in the linked list to a text file. The user is prompted for a
 * filename, the file is either created or overwritten.
 */
void saveCalToFile(void* data)
{
	FILE* dest;
	InputProperties* properties;
	char** inputs;
	int clickedOk;
	char* printedList;
	
	/* check that a list exists and has at least one event in it */
	if ((((MenuData*)data)->list == NULL) || ((MenuData*)data)->list->count <= 0)
	{
		messageBox(((MenuData*)data)->window, "Error: No calendar has been loaded");
	}
	else
	{
		properties = (InputProperties*)malloc(sizeof(InputProperties));
		properties->label = "Save file as";
		properties->maxLength = 20;
		properties->isMultiLine = FALSE;

		inputs = (char**)malloc(sizeof(char*));
		inputs[0] = (char*)malloc(21*sizeof(char));
		strcpy(inputs[0], "");

		/* have user enter filename */
		clickedOk = dialogBox(((MenuData*)data)->window, "Export calendar", 1, properties, inputs);
		dest = fopen(inputs[0], "w");
		
		/* check file opened properly */
		if (dest == NULL)
		{
			messageBox(((MenuData*)data)->window, "Error opening file");
		}
		else
		{
			printedList = listToText(((MenuData*)data)->list);
			fprintf(dest, "%s", printedList);
			
			free(printedList);
			fclose(dest);
		}
		
		free(properties); /* */
		free(inputs[0]); /* */
		free(inputs); /* */
	}
}

/**
 * Checks that the passed-in variables represent a valid date, time and duration
 * to go in an Event struct. Allows Feb 29th as a valid date on leap years.
 */
int eventValid(int inYear, int inMonth, int inDay, int inHours, int inMins, int inDuration)
{
	int dateValid = FALSE;
	int timeValid = FALSE;
	int durationValid = FALSE;
	int allValid = FALSE;

	/* validate date */
    if ( (inYear >= 1) && (inYear <= 3000) )
    {
        if ( (inMonth >= 1) && (inMonth <= 12) )
        {
            switch(inMonth)
            {
                case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                    if ( (inDay > 0) && (inDay < 32) )
					{
                        dateValid = TRUE;
					}
                    break;
                case 4: case 6: case 9: case 11:
                    if ( (inDay > 0) && (inDay < 31) )
					{
                        dateValid = TRUE;
					}
                    break;
                case 2:
                    if ( isLeapYear(inYear) == TRUE)
                    {
                        if ( (inDay > 0) && (inDay < 30) )
						{
                            dateValid = TRUE;
						}
                    }
                    else if ( isLeapYear(inYear) == FALSE)
                    {
                        if ( (inDay > 0) && (inDay < 29) )
						{
                            dateValid = TRUE;
						}
                    }
                default:
                    break;
            }
        }
    }

	/* validate time */
	if (inHours >= 0 && inHours <= 23)
	{
		if (inMins >= 0 && inMins <= 59)
		{
			timeValid = TRUE;
		}
	}

	/* validate duration */
	if (inDuration > 0)
	{
		durationValid = TRUE;
	}

	if (dateValid == TRUE && timeValid == TRUE && durationValid == TRUE)
	{
		allValid = TRUE;
	}

	return allValid;
}

/**
 * Determines whether a year is a leap year.
 */
int isLeapYear(int year)
{
	int leapYear = FALSE;

	if ( (year % 4 == 0) && ( (year % 100 != 0) || (year % 400 == 0) ) )
	{
        leapYear = FALSE;
	}

	return leapYear;
}

/**
 * Adds an event to a linked list, a linked list is created if the passed-in
 * data does not contain one. Event data is gathered from user input.
 */
void addEvent(void* data)
{
	Event* newEvent;
	int clickedOk;
	InputProperties properties[5];
	char** inputs;
	int ii;
	int dayEntry;
	int monthEntry;
	int yearEntry;
	int hrsEntry;
	int minsEntry;
	int durationEntry;
	char* printedList;
	
	/* create list if one doesn't exist */
	if (((MenuData*)data)->list == NULL)
	{
		((MenuData*)data)->list = createList();
	}

	properties[0].label = "Enter activity";
	properties[0].maxLength = 400;
	properties[0].isMultiLine = FALSE;

	properties[1].label = "Enter location";
	properties[1].maxLength = 100;
	properties[1].isMultiLine = FALSE;

	properties[2].label = "Enter date (DD/MM/YYYY)";
	properties[2].maxLength = 20;
	properties[2].isMultiLine = FALSE;

	properties[3].label = "Enter time in 24 hour format (HH:MM)";
	properties[3].maxLength = 10;
	properties[3].isMultiLine = FALSE;

	properties[4].label = "Enter activity duration";
	properties[4].maxLength = 10;
	properties[4].isMultiLine = FALSE;



	inputs = (char**)malloc(5*sizeof(char*));
	for (ii = 0; ii < 5; ii++)
	{
		inputs[ii] = (char*)malloc(properties[ii].maxLength*sizeof(char));
		strcpy(inputs[ii], "\0");
	}

	/* prompt user for inputs */
	clickedOk = dialogBox(((MenuData*)data)->window, "Create an event", 5, properties, inputs);
	/*newEvent = (Event*)malloc(sizeof(Event));*/

	/* parse user input for date, time and duration */
	sscanf(inputs[2], "%d/%d/%d", &dayEntry, &monthEntry, &yearEntry);
	sscanf(inputs[3], "%d:%d", &hrsEntry, &minsEntry);
	sscanf(inputs[4], "%d", &durationEntry);

	/* check that user input was valid */
	if (eventValid(yearEntry, monthEntry, dayEntry, hrsEntry, minsEntry, durationEntry) == TRUE)
	{
		if (strlen(inputs[0]) > 1)
		{
			/* initialise newEvent with inputted data */
			newEvent = (Event*)malloc(sizeof(Event));
			strcpy(newEvent->activity, inputs[0]);
			strcpy(newEvent->location, inputs[1]);
			newEvent->eDate.day = dayEntry;
			newEvent->eDate.month = monthEntry;
			newEvent->eDate.year = yearEntry;
			newEvent->eTime.hrs = hrsEntry;
			newEvent->eTime.mins = minsEntry;
			newEvent->duration = durationEntry;
		
			/* insert event in the list */
			insertFirst(((MenuData*)data)->list, newEvent);
			
			/* print new list state and display in the gui */
			printedList = listToWindow(((MenuData*)data)->list);
			setText(((MenuData*)data)->window, printedList);
			
			free(printedList); /* */
		}
	}
	else
	{
		messageBox(((MenuData*)data)->window, "Event contained invalid data and was not added");
	}
	
	for (ii = 0; ii < 5; ii++)
	{
		free(inputs[ii]); /* */
	}
	free(inputs); /* */
}

/**
 * Prompts the user to enter a search string corresponding to an activity
 * in the linked list. If a match is found, the user is prompted to modify
 * it.
 */
void editEvent(void* data)
{
	Event* foundEvent;
	int clickedOk;
	InputProperties properties[1];
	char** inputs;
	int elementNo;
	InputProperties foundEvProps[5];
	char** foundEvInputs;
	int ii;
	int dayEntry;
	int monthEntry;
	int yearEntry;
	int hrsEntry;
	int minsEntry;
	int durationEntry;
	char* printedList;
	char foundMsg[500];
	char foundActivity[400];

	inputs = (char**)malloc(sizeof(char*));
	inputs[0] = (char*)malloc(400*sizeof(char));
	properties[0].label = "Enter a case sensitive search string that matches or partially matches an activity in the calendar";
	properties[0].maxLength = 400;
	properties[0].isMultiLine = FALSE;
	strcpy(inputs[0], "");
	
	/* prompt user for search string */
	clickedOk = dialogBox(((MenuData*)data)->window, "Find matching event", 1, properties, inputs);
	
	/* find element number of matching event in linked list */
	elementNo = findEvent(((MenuData*)data)->list, inputs[0]);
	
	/* if no match was found, display message */
	if (elementNo == -1)
	{
		messageBox(((MenuData*)data)->window, "No matching activity found");
	}
	else
	{
		foundEvent = retrieveElement(((MenuData*)data)->list, elementNo);
		strcpy(foundMsg, "Matching event found: ");
		strcpy(foundActivity, foundEvent->activity);
		strcat(foundMsg, foundActivity);
		messageBox(((MenuData*)data)->window, foundMsg);
		
		foundEvProps[0].label = "Enter activity";
		foundEvProps[0].maxLength = 400;
		foundEvProps[0].isMultiLine = FALSE;
	
		foundEvProps[1].label = "Enter location";
		foundEvProps[1].maxLength = 100;
		foundEvProps[1].isMultiLine = FALSE;
	
		foundEvProps[2].label = "Enter date (DD/MM/YYYY)";
		foundEvProps[2].maxLength = 20;
		foundEvProps[2].isMultiLine = FALSE;
	
		foundEvProps[3].label = "Enter time in 24 hour format (HH:MM)";
		foundEvProps[3].maxLength = 10;
		foundEvProps[3].isMultiLine = FALSE;
	
		foundEvProps[4].label = "Enter activity duration";
		foundEvProps[4].maxLength = 10;
		foundEvProps[4].isMultiLine = FALSE;
		
		foundEvInputs = (char**)malloc(5*sizeof(char*));
		for (ii = 0; ii < 5; ii++)
		{
			foundEvInputs[ii] = (char*)malloc(foundEvProps[ii].maxLength*sizeof(char));
			strcpy(foundEvInputs[ii], "\0");
		}
		
		/* prompt user for new event values */
		clickedOk = dialogBox(((MenuData*)data)->window, "Editing found event", 5, foundEvProps, foundEvInputs);
	
		/* parse user input */
		sscanf(foundEvInputs[2], "%d/%d/%d", &dayEntry, &monthEntry, &yearEntry);
		sscanf(foundEvInputs[3], "%d:%d", &hrsEntry, &minsEntry);
		sscanf(foundEvInputs[4], "%d", &durationEntry);
	
		/* check user input is valid */
		if (eventValid(yearEntry, monthEntry, dayEntry, hrsEntry, minsEntry, durationEntry) == TRUE)
		{
			if (strlen(foundEvInputs[0]) > 1)
			{
				/* assign inputted values */
				strcpy(foundEvent->activity, foundEvInputs[0]);
				strcpy(foundEvent->location, foundEvInputs[1]);
				foundEvent->eDate.day = dayEntry;
				foundEvent->eDate.month = monthEntry;
				foundEvent->eDate.year = yearEntry;
				foundEvent->eTime.hrs = hrsEntry;
				foundEvent->eTime.mins = minsEntry;
				foundEvent->duration = durationEntry;
			
				/* refresh text in main window */
				printedList = listToWindow(((MenuData*)data)->list);
				setText(((MenuData*)data)->window, printedList);
				free(printedList);
			}
		}
		/* if input validation fails, display error message */
		else
		{
			messageBox(((MenuData*)data)->window, "Invalid value/s entered, event was not modified");
		}
		
		free(inputs[0]); /* */
		free(inputs); /* */
		for (ii = 0; ii < 5; ii++)
		{
			free(foundEvInputs[ii]); /* */
		}
		free(foundEvInputs); /* */
	}
}

/**
 * Checks every activity in a linked list against the passed-in string.
 * If inActivity exists in any activity string on the list, a corresponding
 * linked list element number is returned. The value -1 is returned if no
 * match is found. The first matching event is returned, following matching
 * events are ignored.
 */
int findEvent(LinkedList* list, char* inActivity)
{
	ListNode* current;
	ListNode* next;
	int ii;
	int match;
	int elementNo;

	ii = 0;
	match = FALSE;
	elementNo = -1;
	
	current = list->head;

	/* traverse list until a match is found or the list ends */
	while (ii < list->count && match == FALSE)
	{
		/* if a match is found, exit loop */
		if (strstr(current->data->activity, inActivity) != NULL)
		{
			match = TRUE;
			elementNo = ii;
		}
		/* else, keep searching */
		else
		{
			next = current->next;
			current = next;
			ii++;
		}
	}
	
	/* if no match was found */
	if (match == FALSE)
	{
		elementNo = -1;
	}
	
	return elementNo;
}

/**
 * Prompts the user to enter a search string corresponding to an activity
 * in the linked list. If a match is found, the event is deleted.
 */
void deleteEvent(void* data)
{
	Event* foundEvent;
	int clickedOk;
	InputProperties properties[1];
	char** inputs;
	int elementNo;
	char* printedList;
	char foundMsg[500];
	char foundActivity[400];

	inputs = (char**)malloc(sizeof(char*));
	inputs[0] = (char*)malloc(400*sizeof(char));
	properties[0].label = "Enter a case sensitive search string that matches or partially matches an activity in the calendar";
	properties[0].maxLength = 400;
	properties[0].isMultiLine = FALSE;
	strcpy(inputs[0], "");
	
	/* prompt user for search string */
	clickedOk = dialogBox(((MenuData*)data)->window, "Find matching event to delete", 1, properties, inputs);
	
	/* retrieve element number of matching event on the list */
	elementNo = findEvent(((MenuData*)data)->list, inputs[0]);
	
	/* if no match was found, display message */
	if (elementNo == -1)
	{
		messageBox(((MenuData*)data)->window, "No matching activity found");
	}
	/* else match found */
	else
	{
		/* display the name of the deleted event to the user */
		foundEvent = retrieveElement(((MenuData*)data)->list, elementNo);
		strcpy(foundMsg, "Event deleted: ");
		strcpy(foundActivity, foundEvent->activity);
		strcat(foundMsg, foundActivity);
		messageBox(((MenuData*)data)->window, foundMsg);
	
		/* delete the event AFTER displaying confirmation message */
		deleteNthElement(((MenuData*)data)->list, elementNo);
		
		/* refresh text in main window */
		printedList = listToWindow(((MenuData*)data)->list);
		setText(((MenuData*)data)->window, printedList);
		
		if (((MenuData*)data)->list->count > 0)
		{
			free(printedList);
		}
	}
	
	free(inputs[0]);
	free(inputs);
}

/**
 * Parses text from a file matching the passed-in filename. Text is formatted
 * for display in the gui main window.
 */
void readFile(void* data, char* filename)
{
	FILE* source;
	Event* newEvent;
	int tempYear;
	int tempMonth;
	int tempDay;
	int tempHrs;
	int tempMins;
	int tempDuration;
	char possEmptyLine[100];
	char* printedList;
	char junkString[500];
	
	source = fopen(filename, "r");
	/* if file didn't open correctly, display error message */
	if (source == NULL)
	{
		messageBox(((MenuData*)data)->window, "Error opening file");
	}
	else
	{
		/* scan until end of file is reached */
		while (!feof(source))
		{
			/* check that line is formatted as event data */
			if (fscanf(source, "%d-%d-%d %d:%d %d ", &tempYear, &tempMonth, &tempDay, &tempHrs, &tempMins, &tempDuration) == 6)
			{
				/* validate scanned values */
				if (eventValid(tempYear, tempMonth, tempDay, tempHrs, tempMins, tempDuration) == TRUE)
				{
					newEvent = (Event*)malloc(sizeof(Event));
			
					/* assign scanned values to an event */
					newEvent->eDate.year = tempYear;
					newEvent->eDate.month = tempMonth;
					newEvent->eDate.day = tempDay;
					newEvent->eTime.hrs = tempHrs;
					newEvent->eTime.mins = tempMins;
					newEvent->duration = tempDuration;
					
					/* remainder of line will contain th activity */
					fgets(newEvent->activity, 399, source);
					removeNewline(newEvent->activity);

					/* next line will have a location or be a blank line */
					fgets(possEmptyLine, 99, source);

					/* if newly scanned line contains a location */					
					if (strlen(possEmptyLine) > 1)
					{
						/* assign location */
						strcpy(newEvent->location, possEmptyLine);
						removeNewline(newEvent->location);

						/* read next empty line */
						fgets(possEmptyLine, 99, source);
					}
	
					/* else make location an empty string */
					else
					{
						strcpy(newEvent->location, "\0");
					}
			
					/* insert the new event into the linked list */
					insertFirst(((MenuData*)data)->list, newEvent);
				}

				/* if event data was invalid, scan lines from entry but don't send anything to the linked list */
				else
				{

					messageBox(((MenuData*)data)->window, "Invalid event data found, invalid entry will be omitted");
					fgets(junkString, 499, source);
				
					fgets(junkString, 99, source);

					/* if newly scanned line contains a location */					
					if (strlen(junkString) > 1)
					{
						/* read next empty line */
						fgets(possEmptyLine, 99, source);
					}
				}			
			}
		}

		/* display every activity in the linked list in the main window */
		printedList = listToWindow(((MenuData*)data)->list);
		setText(((MenuData*)data)->window, printedList);
		
		free(printedList);
		fclose(source);
	}
}

