# RoomReservation
This program implements a reservation system for booking time in a collection of rooms.  It provides the creation and deletion of reservations (on a single day), and prints information about the overall schedule and the reservations held by a particular person. 
# Installing GCC compiler for Windows users
http://preshing.com/20141108/how-to-install-the-latest-gcc-on-windows/
# Compiling and Running Multiple Files without GCC
This is for Windows users. https://msdn.microsoft.com/en-us/library/bb384838.aspx
# Compiling and Running Multiple Files with GCC
After you have downloaded or forked the repository.
OSX:
1. Open Terminal 
2. cd to the directory where the project is
Windows:
1. Open Command Prompt
2. cd \ to the directory where the project is
Linux:
1. Open Terminal
2. cd ~/ to the directory where the project is
Common Steps:
1. gcc -c -Wall -Werror -o m main.c
2. gcc -c -Wall -Werror -o r rooms.c
3. gcc -c -Wall -Werror -o s schedule.c
4. gcc -o Program m r s
To run...
5. ./Program
# User Interface and Error Handeling
How many rooms?
#
Enter command (r,x,s,f,q): r 
#
=== MAKE RESERVATION ===
#
Enter name (one word): //There is a chunk of code that makes sure name does not exceed 20 chars the 20 chars can be comprimised of 
#
//chars that are not letters so no ERROR checking for the actual characters
#
Start Time (hh:mm, 24-hr): //note: you do not need to have a 0 before a single digit hour for example 09:00 == 9:00 also if you enter
#
//something like Bob for the time DONT
#
End Time (hh:mm, 24-hr): //note: you do not need to have a 0 before a single digit hour for example 09:00 == 9:00 also if you enter
#
//something like Bob for the time DONT
#
//Also defualt opeining and closing times are 00:00 - 24:00 if you do something like 
#
//Start Time (hh:mm, 24-hr): 24:00
#
//End Time (hh:mm, 24-hr): 25:00 
#
//the reservation will be successful becuase there is no checking to see if the time passed is within the bounds of opening and closing
#
times this is something you have to make sure of
#
Enter command (r,x,s,f,q): f
#
=== FIND RESERVATIONS ===
#
Enter name (one word): //Whatever name you enter it will take a long time to find all reservations associated with this name depending on
#
//the IDE or Text Editor you run it in you may accidently kill it










