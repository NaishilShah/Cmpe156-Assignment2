/*
===========================================================================================================================
Name: Naishil Shah
cruzid: nanshah@ucsc.edu 
===========================================================================================================================


There are 4 directories(folders) in the submission. Following is the explanation of the file organization:

lib - This directory contains all the necessary header files.

src - This contains the source files of server and client, named servcheck.c and clicheck.c respectively. Also contains makefile.

bin - The direcotry contains the object and excecutables of the codes.

doc - Contains the README.txt file and Documentation.pdf file

---------------------------------------------------------------------------------------------------------------------------

***Steps to run: 

Assuming you are in the submitted folder.

1. cd src/

2. make myserver

3. make myclient

4. cd ../bin/
Run as many servers as required - one in each terminal by running the following command:

5. ./myserver <portnum> 
PLEASE NOTE: The portnumber entered should be in accordance with the number mentioned in the server-info.txt file

Run a different terminal for the client using the following command:

6. ./myclient server-info.txt <number of servers to be connected>


***Description: 

1. The code of the previous assignment was used to build this code. The servers work perfectly and need a port number in 
   their command.
2. The client thread when used with the correct syntax as shown below, it is able to connect to the server and and check 
   whether the required file is present or not.
3. If the file is found, the client gets the file size from the first server and then creates threads to fetch the
   separate chunks from different servers.
4. The chunk size is calculated by dividing the size of the file by the number of servers provided by the user. If the 
   entered number is greater than the available servers, then all the servers will be used and the user will be notified.
5. The threads give user the information of the amount of chunk size and the offset they have received.  


***ISSUES and PROBLEMS NOT TACKLED***

1. The servers is not concurrent. Hence, in cases of where the server is down and the client tries to access it, the 
   application fails. (This is the case where number of servers entered by user is more than the servers available, and 
   the thread tries to access the previous server more than once)
2. There might be an issue of varying characters at the junctions where the threads take over each other. Please rerun
   the application to remoce such errors. I have not figured out the cause of this problem yet. Due to this I have not
   created any script file to compare the outputs directly, as it will give some error.
3. There might be some minor faults other than this which I may have missed. Since I was out of town the whole week I could
   not attend any of the discussion sessions to calrify my doubts. I hope I do not lose a lot of points due to these errors.

Thank you for grading the assignment.

