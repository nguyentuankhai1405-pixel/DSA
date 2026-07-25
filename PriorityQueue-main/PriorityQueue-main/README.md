a priority queue system (triage) for a hospital emergency room. 


Level	Priority Codes Should be seen by the provider within:

1	immediate :	0 minutes,
2	emergency	: 15 minutes,
3	urgent : 30 minutes,
4	minimal : 120 minutes

you add and determine patients' priority

The interface of the program is a command-line prompt that supports the following commands:
add command, takes in the priority level and the name of the patient.
peek command, which displays the patient that is next in line, but keeps the patient in the waiting room.
next command, Remove the waiting patient from the priority system and announce the name.
list command, displays all patients in the waiting room their arrival number, and the order they will be seen in.
load command, reads a text file and executes each line as if you were to type it into the prompt, allowing you to add many patients.
help command, shows you all the commands and what they do.

the included text file is an example load file
