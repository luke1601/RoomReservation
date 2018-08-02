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

Enter command (r,x,s,f,q): r
=== MAKE RESERVATION ===
Enter






