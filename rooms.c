#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "rooms.h"
 
/* rooms- program to create the rooms
 * Once rooms are created each room has a scheule within itself,
 * The functions present here essentially call the functions in schedule to do
 * the work of reserving and etc. 
 * Mounark Patel, November 2017
 */

Schedule *Room; //global 
static int Nrooms; //global version of nrooms 


void setupRooms(int nrooms, Time open, Time close)
{
    //malloc the amount of scheules needed
    Room = (Schedule*)malloc(sizeof(Schedule) * (nrooms + 1)); 
    
    int i = 1;
    for(i = 1; i <= nrooms; i++)
    {
        Room[i] = createSchedule(open, close);
    }
    //set the value of Nrooms = nrooms
    Nrooms = nrooms;
}

/* function to return number of rooms --
 * must match value used in setupRooms call */
int numRooms()
{
    return Nrooms;
}

/* reserve a time in the first available room */
/* return the room number if success,
   0 if time not available in any room */
int makeReservation(const char* name, Time start, Time end)
{ 
    int i;
    for(i = 1; i <= Nrooms; i++) //for all rooms
    {
      if(reserve(Room[i], name, start, end) == 0) //check if reservation is possible
      {
        continue; //return the room number
      }
      else //check if reservation is possible
      {
        return i; //return the room number
      }
    }
    return 0; //if none was found return 0
}

/* delete an existing reservation in a particular room */
/* return 1 if success, 0 if matching reservation not found */
int cancelReservation(int room, const char* name, Time start)
{
    if(cancel(Room[room],name, start) == 1) //call cancel properly
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/* print reservations from a list, using specified stream */
void printReservations(const struct rNode* list, FILE* stream)
{
    int i = 1;
    while(list != NULL)//keep on printing while list != NULL
    {
        //print busy rNode
        fprintf(stream, "%s", "ROOM ");
        fprintf(stream, "%d", i);
        fprintf(stream, "%02d", Room[i] -> busy -> interval.start.hr); //BUSY 08
        fprintf(stream, "%c", ':'); //BUSY 08:
        fprintf(stream, "%02d", Room[i] -> busy -> interval.start.min); //BUSY 08:00
        fprintf(stream, "%s", " - "); //BUSY 08:00 - 
        fprintf(stream, "%02d", Room[i] -> busy -> interval.end.hr); //BUSY 08:00 - 09
        fprintf(stream, "%c", ':'); //BUSY 08:00 - 09:
        fprintf(stream, "%02d", Room[i] -> busy -> interval.end.min); //BUSY 08:00 - 09:10
        fprintf(stream, "%c", ' '); //BUSY 08:00 - 09:10 
        fprintf(stream, "%s", Room[i] -> busy -> interval.owner); //BUSY 08:00 - 09:10 JANE
        fprintf(stream, "\n");
        i++;
    }
    
}

/* print schedule for every room, using specified stream */
void printRoomSchedules(FILE* stream)
{
    int i;
    for(i = 1; i <= Nrooms; i++) //iterate through array
    {
        fprintf(stream, "%c", '\n'); //print newline
        fprintf(stream, "%s","Room "); //Room
        fprintf(stream, "%d", i);//Room 1
        fprintf(stream, "%c", ':');//Room 1:
        fprintf(stream, "%c", '\n'); //print newline
        printSchedule(Room[i], stream); 
    }        
}
