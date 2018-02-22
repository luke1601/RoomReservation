
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <string.h> 
#include "schedule.h"

/* schedule- program to create a schedule, see if a particular time interval is
 * busy, reserve a specific time for the schedule, cancel a time in schedule,
 * and print the schedule
 * Usage: schedule
 * The function Schedule createSchedule(Time start, Time end) create a schedle
 * according to the starting and ending times given.
 * The function int isBusy(Schedule s, Time start, Time end) checks whether a 
 * desired period of time is available.
 * The function int reserve(Schedule s, const char *name, Time start, Time end)  
 * reserves a particular time interval if it is busy
 * The function int cancel(Schedule s, const char *name, Time start) frees up previusly 
 * contingious idle intervals and merges contingious intervals
 * The function void printSchedule(Schedule s, FILE* stream) prints all free and 
 * busy intervals to specified stream
 * Mounark Patel, November 2017
 */
 
 
//This function compares two starting times, starting time
//of idle list and starting time for busy list in that order
//return 1 if start1 is less than start2
//return 0 if start1 is more than start2
//return -1 the times are exactly equal 
int lessThan(Time start1, Time start2)
{
    //times from 0 - 23
    //check hours first
    if(start1.hr < start2.hr)
    {
        return 1; //hour is less meaning time has to be less no matter 
    }
    else if(start1.hr > start2.hr)
    {
        return 0; //return 0 cause its greater
    }
    else if(start1.hr == start2.hr) //when they are equal you gotta use minutes
    {
        if(start1.min < start2.min) 
        {
            return 1; //minute is less meaning time has to be less
        }
        else if(start1.min > start2.min)
        {
            return 0; //return 0 cause its greater
        }
        else
        {
            return -1;
        }
    }
    return -1;
}
 

/* allocate and return a new schedule */
Schedule createSchedule(Time start, Time end)
{
    //malloc big box
    struct schedule* s;
    s = (struct schedule*)malloc(sizeof(struct schedule));
    
    //malloc little box
    struct iNode* I;
    I = (struct iNode*)malloc(sizeof(struct iNode));
    
    //program the times to what they are supposed to be originally nothing is busy
    s -> idle = I;
    s -> idle -> interval.start = start;
    s -> idle -> interval.end = end;
    s -> start = start;
    s -> end = end;
    strcpy(s -> idle -> interval.owner, "");
    s -> busy = NULL; 
    
    //this is only one big node
    s -> idle -> next = NULL;
    return s;
}

/* check whether a desired period of time is available */
/* return 1 if busy, 0 if not busy */
int isBusy(Schedule s, Time start, Time end)
{
    //if the busy intervals are set to default it means that in the time given nothing is busy
    if(s -> busy == NULL)
    {
        return 0;
    }
     
    struct iNode* it; 
    it = s -> busy; //points to head of busy list 
    
    while(it != NULL) //traverse through busy interval to check node by node if time interval 
    {
        //either the whole time interval could be before the busy time interval or it can overlap
        if((lessThan(it -> interval.start, start) == 0) && (lessThan(it -> interval.end, end) == 0))
        {
            //case where the time starts earlier than busy start but ends before busy end meaning it overlaps
            if(lessThan(end, it -> interval.start) == 0)
            {
                return 1;
            }
        }
        
        //if it starts earlier than busy start and ends later than busy end that means 
        //that the given time has a busy time inside of it
        else if(((lessThan(it -> interval.start, start) == 0) || 
        (lessThan(it -> interval.start, start) == -1)) && ((lessThan(it -> interval.end, end) == 1) || 
        (lessThan(it -> interval.end, end) == -1)))
        { 
            return 1;
        }
        
        //if it starts later than busy start but ends earlier than busy end that means 
        //that busy node has this time contained inside of it and is therfore a busy time
        else if(((lessThan(it -> interval.start, start) == 1) || (lessThan(it -> interval.start, start) == -1)) 
        && ((lessThan(it -> interval.end, end) == 0) || (lessThan(it -> interval.end, end) == -1)))
        {
            return 1;
        }
        
        //either the whole time interval could be after the busy time interval or 
        //it can overlap
        else if(((lessThan(it -> interval.start, start) == 1) || (lessThan(it -> interval.start, start) == -1)) 
        && ((lessThan(it -> interval.end, end) == 1) || (lessThan(it -> interval.end, end) == -1)))
        {
            //if start is greater than busy start but less than busy end and end is greater
            //than busy end that means that busy node is overlaped
            if(lessThan(start, it -> interval.end) == 1)
            {
                return 1;
            }
        }
        it = it -> next; //go to next node in busy list  
    }
    return 0;  
}


/* mark a particular time interval as busy, for specified owner */
/* return 1 if success, 0 if could not reserve */
int reserve(Schedule s, const char *name, Time start, Time end)
{
    //Test and start and end times are within schedule start and end times
    if(isBusy(s, start, end) == 1) //check if the time is already busy
    {
        return 0; 
    }

    
    struct iNode* it; 
    it = s -> idle; //points to head of idle list 
    struct iNode* prev;  
    prev = NULL; //initially points to NULL then always one element less that it
    
    //if start times is less than the first nodes end time
    if((it != NULL) && lessThan(start, it -> interval.end) == 1)
    {
        //if the whole list of idle needs to be reserved 
        if((lessThan(start, it -> interval.start) == -1) && (lessThan(end, it -> interval.end) == -1) && (it -> next) == NULL)
        {
            
            //insert(it, start, end, name);
            struct iNode* j;
            j = (struct iNode*)malloc(sizeof(struct iNode));
            j -> interval.start = start;
            j -> interval.end = end;
            strcpy(j -> interval.owner, name);
            j -> next = NULL;
            
            //make busy equal
            s -> busy = j;
         
            //delete 1 idle node
            s -> idle = NULL;
            return 1;
        }   
    }
    
    //keep on iterating until you find the node where the ending time is definitely 
    //greater than the starting time you provided
    while((it != NULL) && lessThan(start, it -> interval.end) == 0)
    {
        prev = it; //set prev = to the it
        it = it -> next; //it to the next node
    }
    
    //create a new node and initialize properly
    struct iNode* new_node;
    new_node = (struct iNode*)malloc(sizeof(struct iNode));
    //set the values of the new_node
    new_node -> interval.start = start;
    new_node -> interval.end = end;
    strcpy(new_node ->interval.owner, name); 
    new_node -> next = NULL; 
    
    //case where it -> interval.start = start
    if(lessThan(start, (it -> interval.start)) == -1)
    {
        //the end time is the same as the end time given
        if(lessThan(end, (it -> interval.end)) == -1)
        {
            prev -> next = it -> next; //deleted node 
        } 
        //only other option 
        else if(lessThan(end, (it -> interval.end)) == 1)
        {
            //currently it points to this node and prev points the node right before it
            //need to set it's fields properly
            it -> interval.start = end; //since this is when the idle node should start
        }  
    }
    //case where given start time is greater than it -> interval.start
    //Note: it cannot be the that start < it ->interval.start or isBusy == 1
    else if(lessThan(start, it -> interval.start) == 0)
    {
        //the end time is the same as the end time given
        if(lessThan(end, (it -> interval.end)) == -1)
        {
            //just got to manipulate settings of struct
            it -> interval.end = start;
        }
        //only other option
        else if(lessThan(end, (it -> interval.end)) == 1)
        {
            //create a new node to attach to idle list 
            struct iNode* new;
            new = (struct iNode*)malloc(sizeof(struct iNode));
            new -> interval.start = end; //idle from end time given 
            new -> interval.end = it -> interval.end; //to the end time
            new -> next = NULL; 
            
            //let the part of node before start be its own node
            it -> interval.end = start;
            
            //insert the new right after this
            new -> next = it -> next;
            it -> next = new;  
        }
    }

    //now its time to insert the new_node in its proper place in the busy linked list
    struct iNode* it2;
    it2 = s -> busy; //set it to the head of busy list
    struct iNode* prev2;
    prev2 = NULL; //same logic
    
    //iterate until the start time given is less than the start time of a certain node 
    while((it2 != NULL) && lessThan(end, (it2 -> interval.start)) == 0)
    {
        prev2 = it2; //set prev = to the it
        it2 = it2 -> next; //it to the next node
    }
    if(it2 == NULL && prev2 == NULL) //case where no nodes in busy
    {
        s -> busy = new_node;
        s -> busy -> next = NULL;
    }
    //Case where you have to insert idle node breofre the first busy node in busy node list
    else if(prev2 == NULL && it2 != NULL) 
    {
        s -> busy = new_node;
        s -> busy -> next = it2; 
    }
    else if(it2 != NULL && prev2 != NULL) //case where the node is in the sowhere after first and before last
    {
        prev2 -> next = new_node;
        new_node -> next = it2;
    }
    else if(it2 == NULL && prev2 != NULL) //case where u trying to add after the last node (currently) in the list
    {
        prev2 -> next = new_node;
    }
    return 1; 
}

/* free up a previously-reserved interval */
/* merge contiguous idle intervals */
/* return 1 if success, 0 if matching interval not found */
int cancel(Schedule s, const char *name, Time start)
{
    int bigblack = 0; //to see if matching interval is found, it's default is its not
    struct iNode* it; 
    it = s -> busy; //points to head of idle list 
    struct iNode* prev;
    prev = NULL;
    
    while(it != NULL) 
    {
        //if start the interval given matches up with any of the start times in busy
        if(lessThan((it -> interval.start), start) == -1) 
        {
            bigblack = 1; //bigblack = 1 if there is a possibility that there is a node that needs to be freed
            break;
        }
        it = it -> next; //iterate to next node
    }
    if(bigblack == 0) //this is the case there was never a match 
    {
        return 0; 
    }
    
    it = s -> busy; //points to head of busy list  
    prev = NULL; 
    
    //will iterate until start is equal to it -> interval.start
    while((it != NULL) && lessThan(start, (it -> interval.start)) == 0)
    {
        prev = it; //set prev = to the it
        it = it -> next; //it to the next node
    } //it points to node that you want to take out 
   
    //check if the name matches
    if(strcmp(it -> interval.owner, name) != 0) //if it does not than
    {
        return 0; //only matching time is found but person is different meaning he could have reserved a different room 
    }
    
    //there is only one node in busy and that node needs to be inserted
    if(s -> busy -> next == NULL)
    {
        //create a pointer to idle list and prev to NULL;
        struct iNode* it2;
        it2 = s -> idle;
        
        //copy the information about busy into new iNode
        struct iNode* new;
        new = (struct iNode*)malloc(sizeof(struct iNode));
        new -> interval.start = it -> interval.start;
        new -> interval.end = it -> interval.end;
        strcpy(new ->interval.owner, "");
        new -> next = NULL; 
        
        //delete busy node
        s -> busy = NULL;
        
        //if the user decided to reserv the whole idle list
        if(it2 == NULL)
        {
        	s -> idle = new;
        	return 1;
        }
        
        //there is only one node in idle too 
        if(it2 -> next == NULL)
        {
            //case1: the node needs to be inserted before the first in the idle list
            if(lessThan(start, (it2 -> interval.start)) == 1)
            {
                //reset the starting time of the idle list so you concatenate properly
                it2 -> interval.start = new -> interval.start;
            }
            //case2: the node needs to be inserted after the only node in the idle list
            else if(lessThan(start, (it2 -> interval.start)) == 0) 
            {
                //reset the ending time of the idle list so you concatenate properly
                it2 -> interval.end = new -> interval.end;
            }
        }
        //there are two nodes in idle list the concatenation needs to happen in the middle
        else
        {
            //set node we are currently on to the proper fields
            it2 -> interval.end = it2 -> next -> interval.end;
            //delete the the second idle node 
            it2 -> next = NULL;
        }   
        return 1; 
    }
    
    //create new_node
    struct iNode* new;
    new = (struct iNode*)malloc(sizeof(struct iNode));
    new -> interval.start = it -> interval.start;
    new -> interval.end = it -> interval.end;
    strcpy(new ->interval.owner, "");
    new -> next = NULL; 
    
    //test for case where no iteration happend in while loop but prev = NULL
    if(prev == NULL)
    {
    	s -> busy = it -> next; //head of busy list points to next link in busy
    }
    else
    {
    	//delete that node from busy linked list
    	prev -> next = it -> next;
    }
    
    //now time iterate though idle list
    struct iNode* it3;
    it3 = s -> idle;
    struct iNode* prev3;
    prev3 = NULL;
     
    //iterate until it points to the node after the insertion was suppose to take place 
    while((it3 != NULL) && lessThan(start, (it3 -> interval.end)) == 0)
    {
        prev3 = it3; //set prev = to the it
        it3 = it3 -> next; //it to the next node
    } //iterate once more
    prev3 = it3; //now points 
    it3 = it3 -> next; 
    
    //special case where it3 does equal null
    if(it3 == NULL)
    {
        prev3 -> interval.end = new -> interval.end;
    }
    //case where start < it3 -> interval.end
    if((it3 != NULL) && (lessThan(start, it3 -> interval.start) == 1))
    {
        //say you want to insert node 2-3 before node 3-5
        if(lessThan(new -> interval.end, (it3 -> interval.start)) == -1)
        {
            //concatentation has to occur twice 
            if(lessThan(prev3 -> interval.end, new -> interval.start) == -1)
            {
                //make prev point to the whole time interval
                prev3 -> interval.end = it3 -> interval.end;
                //delete whatever node it is pointing to
                prev3 -> next = it3 -> next;
            }
            //concatentation needs to occur from new to it
            else
            {
                //make it's fields correct
                it3 -> interval.start = new -> interval.start;
            }
                
        }
        //case where no concatentaion needs to occur from new to it
        else if(lessThan(new -> interval.end, (it3 -> interval.start)) == 1)
        {
            //case where concatenation needs to occur from prev to new
            if(lessThan(prev3 -> interval.end, new -> interval.start) == -1)
            {
                prev3 -> interval.end = new -> interval.end;
            }
            //no concatenation at all shit comes literally in between prev and it
            else
            {
                //time to insert new node into idle list
                prev3 -> next = new;
                new -> next = it3;
            }
        }
    }
    return 1; 
}

/* print all free and busy intervals to specified stream */
void printSchedule(Schedule s, FILE* stream)
{
    //create proper stuff to traverse
    struct iNode *itIdle;
    struct iNode *itBusy;
    itIdle = s -> idle;
    itBusy = s -> busy; 
    
    //while loop where both are not null
    while(itIdle != NULL && itBusy != NULL)
    {
        if(lessThan(itIdle -> interval.start, itBusy -> interval.start) == 1)
        {
            //print idle node
            fprintf(stream, "%s", "IDLE "); //IDLE
            fprintf(stream, "%02d", itIdle -> interval.start.hr); //IDLE 07
            fprintf(stream, "%c", ':'); //IDLE 07:
            fprintf(stream, "%02d", itIdle -> interval.start.min); //IDLE 07:00
            fprintf(stream, "%s", " - "); //IDLE 07:00 - 
            fprintf(stream, "%02d", itIdle -> interval.end.hr); //IDLE 07:00 - 08
            fprintf(stream, "%c", ':'); //IDLE 07:00 - 08:
            fprintf(stream, "%02d", itIdle -> interval.end.min); //IDLE 07:00 - 08:00
            fprintf(stream, "\n");
             //iterate to next nodes
            itIdle = itIdle -> next;
        }
        else if(lessThan(itIdle -> interval.start, itBusy -> interval.start) == 0)
        {
            //print busy node
            fprintf(stream, "%s", "BUSY "); //BUSY
            fprintf(stream, "%02d", itBusy -> interval.start.hr); //BUSY 08
            fprintf(stream, "%c", ':'); //BUSY 08:
            fprintf(stream, "%02d", itBusy -> interval.start.min); //BUSY 08:00
            fprintf(stream, "%s", " - "); //BUSY 08:00 - 
            fprintf(stream, "%02d", itBusy -> interval.end.hr); //BUSY 08:00 - 09
            fprintf(stream, "%c", ':'); //BUSY 08:00 - 09:
            fprintf(stream, "%02d", itBusy -> interval.end.min); //BUSY 08:00 - 09:10
            fprintf(stream, "%c", ' '); //BUSY 08:00 - 09:10 
            fprintf(stream, "%s", itBusy -> interval.owner); //BUSY 08:00 - 09:10 JANE
            fprintf(stream, "\n");
            itBusy = itBusy -> next;
        }
        
    } 
    
    //this while loop runs after every node in isBusy is printed out
    while(itIdle != NULL)
    {
        //print idle node
        fprintf(stream, "%s", "IDLE "); //IDLE
        fprintf(stream, "%02d", itIdle -> interval.start.hr); //IDLE 07
        fprintf(stream, "%c", ':'); //IDLE 07:
        fprintf(stream, "%02d", itIdle -> interval.start.min); //IDLE 07:00
        fprintf(stream, "%s", " - "); //IDLE 07:00 - 
        fprintf(stream, "%02d", itIdle -> interval.end.hr); //IDLE 07:00 - 08
        fprintf(stream, "%c", ':'); //IDLE 07:00 - 08:
        fprintf(stream, "%02d", itIdle -> interval.end.min); //IDLE 07:00 - 08:00
        fprintf(stream, "\n");
        //iterate to next nodes
        itIdle = itIdle -> next;
    }
    
    //this while loop runs after every node in isBusy is printed out
    while(itBusy != NULL)
    {
        //print busy node
        fprintf(stream, "%s", "BUSY "); //BUSY
        fprintf(stream, "%02d", itBusy -> interval.start.hr); //BUSY 08
        fprintf(stream, "%c", ':'); //BUSY 08:
        fprintf(stream, "%02d", itBusy -> interval.start.min); //BUSY 08:00
        fprintf(stream, "%s", " - "); //BUSY 08:00 - 
        fprintf(stream, "%02d", itBusy -> interval.end.hr); //BUSY 08:00 - 09
        fprintf(stream, "%c", ':'); //BUSY 08:00 - 09:
        fprintf(stream, "%02d", itBusy -> interval.end.min); //BUSY 08:00 - 09:10
        fprintf(stream, "%c", ' '); //BUSY 08:00 - 09:10 
        fprintf(stream, "%s", itBusy -> interval.owner); //BUSY 08:00 - 09:10 JANE
        fprintf(stream, "\n");
        //iterate to next nodes
        itBusy = itBusy -> next;
    }
}
