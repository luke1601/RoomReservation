/*
 * User interface for room reservation system
 * Creates and initializes schedules for requested
 * number of rooms.  Implements the following commands:
 *
 * r = create reservation
 * x = cancel reservation
 * s = print complete room schedule
 * q = quit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedule.h"
#include "rooms.h"

/* weird macro stuff to limit the input size for names */
#define stringify(S) #S
#define STRFMT(X) "%" stringify(X) "s"
#define STRNAME  STRFMT(NAME_LENGTH)

int main() 
{
    
    /* rooms are typically avialable 24 hours a day */
    /* available to change however there is no error checking */
    Time open = {7, 0};   
    Time close = {24, 0}; 
    
    int nrooms = 0;
    char command[2];  /* one-letter command */
    char name[NAME_LENGTH+1]; /* user input for name */

    /* temporary variables */
    Time start, end;
    int status, r;
    //struct rNode *res_list = NULL;


    printf("How many rooms? ");
    fflush(stdout);  /* make sure prompt is printed before scanf */
    scanf("%d", &nrooms);
    setupRooms(nrooms, open, close);

    while (1) {
        printf("\nEnter command (r,x,s,q): ");
        fflush(stdout);
         /* why use string for a one-letter command?
         * so we don't have to worry about leading spaces */
        scanf("%1s", command);

        if (strcmp(command,"q") == 0) break;

        else if (strcmp(command, "s") == 0) {
            printf("=== CURRENT SCHEDULE ===\n");
            printRoomSchedules(stdout);
        }

        else if (strcmp(command, "r") == 0) {
            printf("=== MAKE RESERVATION ===\n");
            printf("Enter name (one word): ");
            fflush(stdout);
            scanf(STRNAME, name);
            printf("Start time (hh:mm, 24-hr): ");
            fflush(stdout);
            scanf("%d:%d", &start.hr, &start.min);
            printf("End time (hh:mm, 24-hr): ");
            fflush(stdout);
            scanf("%d:%d", &end.hr, &end.min);

            status = makeReservation(name, start, end);
            if (status) {
                printf("Success!  Room number: %d\n", status);
            }
            else {
                printf("Unable to make reservation.\n");
            }
        }

        else if (strcmp(command, "x") == 0) {
            printf("=== CANCEL RESERVATION ===\n");
            printf("Enter room number: ");
            fflush(stdout);
            scanf("%d", &r);
            printf("Enter name (one word): ");
            fflush(stdout);
            scanf(STRNAME, name);
            printf("Start time (hh:mm, 24-hr): ");
            fflush(stdout);
            scanf("%d:%d", &start.hr, &start.min);

            status = cancelReservation(r, name, start);
            if (status) {
                printf("Cancelled.\n");
            }
            else {
                printf("Unable to cancel.\n");
            }
        }
    }
    return EXIT_SUCCESS;
}
