/*
	Programmers: James Pease. Zachary Huang
    Purpose: C programming, Group project spring 2023
    Last modified: 2/21/2023

    Program specification:

    "Result management System:

        It should have two accesses. One as a teacher who is allowed
        to upload grades. Other is students who can just view their grades. It should have
        multiple students (at least 5). The teacher feature should have Edit marks for students
        and view all grades of all students together. Adding features like Min, Max, and Mean
        of the class will make this project even better. Sort is needed for the teacher based
        on the grades.
    "

    --rough draft of program layout for functions are at the bottom of the program, They're currently
        set as comments, we should craft the fileIO, and build the core functionality first then add features.

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>      //used for isalpha() and toupper()
#include <conio.h>      //getch() to pause until user enters a character
#include <string.h>     //string library, contains string functions.

#define SIZE 5      //at least 5 students, used for testing, remove before presenting, have the program use a counter or sizeof(struct)
#define MAXSIZE 50  //max number of people that the array of struct will handle, increase this number for more people.
#define maxAssign 10    //more than 10 and they don't fit on the screen.
#define DEBUG 0    //program debug enabled when set to 1, change to 0 to build a new test file with preset students, grades, and so on.


//declare struct
typedef struct
{
    char name[40];
    char title[7];
    unsigned int idNumber;
    float grades[maxAssign];
    float finalGrade;
} person;

//function prototypes
void clearScreen();
void displayGrades(person group[], int idNumber);
void editAssign(person p[]);
void fileIO(person group[], int option);
int getEmptyPerson(person p[]);
int getMaxAssignments(person p[]);
float mean(person p[]);
int max(person p[]);
int min(person p[]);

void newPerson(person group[], int option);
void teacher(person group[], int code);
void sortGrades(person p[], int assignmentNumber, int numStudents);
void verifyUser(person group[]);

//DEBUG function prototypes
void printOutput(person group[]);
void testData(person group[]);

//driver for the program
int main(void)
{
    person group[MAXSIZE];

    //initialize the group
    for(int i = 0; i < MAXSIZE; i++)
    {
        strcpy(group[i].name, " ");
        strcpy(group[i].title, " ");
        group[i].idNumber = 0;
        group[i].finalGrade = 0;
        for(int j = 0; j < maxAssign; j++)
        {
            group[i].grades[j] = -1.0;
        }
    }

    if(DEBUG)
    {
        testData(group);
        printOutput(group);
        displayGrades(group, 0);
    }
    else
        //call fileIO to read file
        fileIO(group, 0);


    //call function to prompt user to login
    verifyUser(group);
}

void clearScreen()
{
    printf("\e[1;1H\e[2J");
}

//returns the next index value of an empty index location
int getEmptyPerson(person p[])
{
    int count = 0, emptyFound = 0;

    while(count < MAXSIZE && !emptyFound)
    {
        if(!p[count].idNumber)
        {
            emptyFound = 1;
            if(DEBUG)
                printf("\n\nSTUDENT %d IS EMPTY!\n\n", count);
            return count;
        }
        count++;
    }
    return count;
}

//iterates through assignments and returns then number of assignments that are created.
int getMaxAssignments(person p[])
{
    int count = 0;
    while(count < maxAssign && p[0].grades[count] == 0)
    {
        count++;
    }
    if(DEBUG)
        printf("\n\nmaxAssignment Count %d\n\n", count);
    return count;
}

//prompts teacher and edits assignments
void editAssign(person p[])
{
    //declare and initialize variables
    int numIn = 0, numStudents = getEmptyPerson(p);
    float tempGrade = 0.0;

    if(p[0].grades[0] == -1.0)
        p[0].grades[0] = 0;
    //prompt for input
    printf("Which Assignment(Numerical) would you like to Change: ");
    scanf("%d",&numIn);     //get input
    numIn= numIn -1;        //decrement the numIn to match the indexing of the array

    if(p[0].grades[numIn] == -1);
    p[0].grades[numIn] = 0;

    if(numIn >= 0 && numIn < 10)
    {
        //loop through the students and modify the grade fr that assignment for each one.
        for(int i = 1; i < numStudents; i++)
        {
            if(!strcmp(p[i].title, "Student"))
            {
                printf("\nEnter %s's grade for Assigment %d: ", p[i].name, (numIn+1));
                scanf("%f", &tempGrade);
                p[i].grades[numIn] = tempGrade;
            }
        }
    }
    printf("\n");

    teacher(p, 0);
}

//prints the grades to the consol
void displayGrades(person p[], int studentID)
{
    int numStudents = getEmptyPerson(p),    //subtract 1 since the location returned is the next open slot
        numAssignments = getMaxAssignments(p),
        assignmentNum = 1;

    //blank the screen before displaying output
    clearScreen();

    //Output chart
    printf("\nStudent\t");

    //populate the top of the chart.
    for (int i = 0; i < numAssignments; i++)
    {
        printf("\t   Assignment %d", i+1);
    }
    printf("\t   Final Grade");
    //print the chart
    for(int i = 1; i < numStudents; i++)
    {
        float sum = 0.0;

        if(studentID > 0)
        {
            i = studentID;
            numStudents = studentID;
            assignmentNum = 0;
        }

        if(i <= numStudents)
            printf("\n%4s\t", p[i].name);

        for(int j = 0; j < numAssignments; j++)
        {
            //rows -> columns
            printf("\t%.1f", p[i].grades[j] );
            sum += p[i].grades[j];
            printf("\t");
        }

        p[i].finalGrade = sum / numAssignments;
        printf("\t%.1f", p[i].finalGrade);

    }
    printf("\n\n");

    if(assignmentNum)
    {
        if(1)
        {
            printf("\n\nThe mean final grade of the class is: %.1f", mean(p));
            printf("\nThe student with the highest final grade is %s, Their grade is %.1f", p[max(p)].name, p[max(p)].finalGrade );
            printf("\nThe student with the lowest final grade is %s, Their grade is %.1f\n", p[min(p)].name, p[min(p)].finalGrade);
        }


        printf("\nIf you would like to sort by assignment, enter the assignment number(Enter 0 to stop)");
        scanf("%d", &assignmentNum);

        assignmentNum--;    //decrement the number by 1 to match the assignment array
        //make sure that assignment requested is not outside of assignment available
        if(assignmentNum > numAssignments)
            assignmentNum = 0;

        //if assignment is not 0, then sort and display the information
        if(assignmentNum != -1)
        {
            sortGrades(p, assignmentNum, numStudents);  //sort the students
            displayGrades(p, 0);    //display the table
        }
    }

    clearScreen();
}

//handles all file IO for the program, condition code triggers write.
void fileIO(person p[], int condition)
{
    //declare and initialize file pointer
    FILE *fPtr = NULL, *nfPtr = NULL;
    char line[80] = "";

    //writes the contents of the struct to the file.
    if(condition)
    {
        //set the new file pointer
        if((nfPtr = fopen("Students.txt", "w")) == NULL)    //if save file not present, create new file
        {
            nfPtr = fopen("Students.txt", "a");             //create file.
        }

        //write name, title, and idNumber to file
        for(int i = 0; i < MAXSIZE; i++)
        {
            fprintf(nfPtr, "%s,%s,%d,\n", p[i].name, p[i].title, p[i].idNumber);
        }

        fprintf(nfPtr, "\n");

        //write the contents of the student grades array to the file, csv file, newline for each student.
        //teacher grades arestored as well, storage place for totals.
        for(int i = 0; i < MAXSIZE; i++)
        {
            for(int j = 0; j < maxAssign; j++)
            {
                //rows -> columns
                fprintf(nfPtr, "%.1f,", p[i].grades[j] );
            }
            fprintf(nfPtr, "\n");
        }

    }

    else    //input condition
    {
        //set the new file pointer
        if((fPtr = fopen("Students.txt", "r")) == NULL)    //if save file not present, create new file
        {
            printf("file not found\n\n");
            newPerson(p, 1);
        }


        int i = 0;
        //read name, title, and idNumber from file
        while(fgets(line, sizeof(line), fPtr) != NULL && i < MAXSIZE)
        {

            strcpy(p[i].name, strtok(line, ","));   //get the name, and copy to person.name
            strcpy(p[i].title, strtok(NULL, ","));  //get next token, in this case title
            char* stringNum = strtok(NULL, ",");    //assign the final token to stringNUM, used to convert to int

            //call aoti, argument to integer to convert string to int.
            p[i].idNumber = atoi(stringNum);        //assign id number
            i++;
        }

        fscanf(fPtr, "\n");

        //write the contents of the student grades array to the file, csv file, newline for each student.
        //teacher grades are stored as well, storage place for totals.
        for(int i = 0; i < MAXSIZE; i++)
        {
            for(int j = 0; j < maxAssign; j++)
            {
                //rows -> columns
                fscanf(fPtr, "%f,", &p[i].grades[j] );
            }
            fscanf(fPtr, "\n");
        }
    }

    //close the files
    fclose(fPtr);   //close read file
    fclose(nfPtr);  //close write file

}
float mean(person p[])
{
    float sum = 0.0;
    int numStudents = getEmptyPerson(p) -1;
    for(int i = 1; i < numStudents; i++)
    {
        sum += p[i].finalGrade;
    }

    return sum/numStudents;

}

int max(person p[])
{
    float highGrade = 0;
    int person = 0,
        numStudents = getEmptyPerson(p) -1;
    for (int i = 1; i <= numStudents; i++)
    {
        if(p[i].finalGrade > highGrade)
        {
            highGrade = p[i].finalGrade;
            person = i;
        }
    }
    return person;
}

int min(person p[])
{
    float lowGrade = 100.0;
    int person = 0,
        numStudents = getEmptyPerson(p) -1;
    for(int i = 1; i <= numStudents; i++)
    {
        if(p[i].finalGrade < lowGrade)
        {
            lowGrade = p[i].finalGrade;
            person = i;
        }
    }
    return person;
}

//generates teacher if code = 0, else prompts teacher for number of new students.
void newPerson(person p[], int code)
{
    int newStudents = 0, count = 0;
    char fName[20] = " ", lName[20]= " ";
    int numAssignments = getMaxAssignments(p);

    if(code && p[0].idNumber == 0)
    {
        printf("Hello new Teacher! Please enter your name: ");
        scanf("%s %s", fName, lName);   //get first and lastname

        //concatenate the strings
        strcat(fName, " ");                     //add a space at the end of firstname
        strcpy(p[0].name, fName);               //copy first name to .name, assigned to teacher
        strcat(p[0].name, lName);               //concatenate lastname to the name

        strcpy(p[0].title, "Teacher");          //assign title for teacher
        printf("\nNow please enter your ID: "); //prompt for idNumber
        scanf("%d", &p[0].idNumber);            //get idNumber
    }
    else
    {
        printf("How many Students would you like to add?");
        scanf("%d", &newStudents);

        //loop while number of created students is less than the desired number of newStudents
        while(count < newStudents)
        {

            int num = getEmptyPerson(p);            //get the location of the next empty slot
            printf("Enter the student's Name: ");   //prompt for input
            scanf("%s %s", fName, lName);           //get first and last name

            //concatenate the strings
            strcat(fName, " ");                     //add space
            strcpy(p[num].name, fName);             //copy fName to p[index].name
            strcat(p[num].name, lName);             //concatenate lName to name

            //automatically assign student as their title
            strcpy(p[num].title, "Student");

            //generate an ID number, just index value + 1000
            p[num].idNumber = num + 1000;

            //increment the number of created students
            for(int i = 0; i < numAssignments; i++)
            {
                printf("\nEnter a grade for assignment %d: ", i + 1);
                scanf("%f", &p[num].grades[i]);
            }
            count++;
            if(DEBUG)
                printf("\n\nStudent added.\n\n");
        }

        if(DEBUG)
            printOutput(p);

        teacher(p, 0);
    }

}

//teacher function creates a teacher if code 0 is passed in, if code 1 then teacher function has teacher perform tasks.
void teacher(person p[], int condition)
{
    char c;
    int validInput = 0;

    //prompt teacher,
    printf("Welcome %s, \nWhat would you like to do?\n", p[0].name);

    while(!validInput)  //check the input, loop until valid option is entered.
    {
        if(isalpha(c))  //check that c is an alpha
        {
            c = toupper(c); //convert it to uppercase
            validInput = 1; //change condition code, letter is a valid char
        }
        else    //invalid input
        {
            //prompt user for input
            printf("Add students: A | Edit Grades: E | View all Grades: V | Class Roster: R | \nExit by entering any key not listed.\n");
            scanf("%s", &c);
        }
    }

    if (c == 'A')
    {
        newPerson(p, 0);    //create new student
    }
    else if (c == 'E')
    {
        if(DEBUG)
            puts("edit grades!");
        editAssign(p);
    }
    else if (c == 'V')
    {
        if(DEBUG)
            puts("display stuff");
        displayGrades(p, 0);
    }
    else if (c == 'R')
    {
        if(DEBUG)
            puts("list all people");
        printOutput(p);
    }
    else
    {
        clearScreen();
    }


}

//sorts the grades based on the assignment that the teacher want's to search by
void sortGrades(person p[], int assignment, int numStudents)
{
    person temp;

    for (int i = 1; i <= numStudents; i++)
    {
        for (int j = 1; j <= (numStudents-i); j++)
        {
            if (p[j].grades[assignment] < p[j + 1].grades[assignment])
            {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

//generates individuals to fill 5 slots in the structure
void testData(person p[])   //only used during DEBUG
{
    //populate names, position 0 will be used for the teacher.
    //strcpy assigns the name to the char array in the struct for the person
    strcpy(p[1].name, "Bob Tyler");
    strcpy(p[2].name, "Rose Smith");
    strcpy(p[3].name, "Seth Pickle");
    strcpy(p[4].name,"Doctor Who");
    strcpy(p[5].name, "Anita Job");

    for(int i = 0; i <= SIZE; i++)
    {
        strcpy(p[i].title, "Student");
        p[i].idNumber = (i + 1000);

        //assign grades
        for(int j = 0; j < 5; j++)
        {
            p[i].grades[j] = i;
        }
    }

    //hard code teacher
    strcpy(p[0].name, "Zach");
    strcpy(p[0].title, "Teacher");
    p[0].idNumber = 8;
}

//display output for test data, not called outside of debug mode.
void printOutput(person p[])    //only used during DEBUG
{
    for(int i = 0; i <= MAXSIZE && p[i].idNumber != 0; i++)
    {
        printf("Name: %s \n", p[i].name);
        printf("Title: %s \n", p[i].title);
        printf("ID Number: %d \n\n", p[i].idNumber);
    }
}

//checks if the IDof the person
void verifyUser(person p[])
{

    int exitCode = 1;

    while(exitCode)
    {
        int numIn = 0, idFound = 0;  //declare and initialize, teacher variable is a boolean, change exit code to 0 to stop input loop.
        //isTeacher is used to allow the teacher to add students.

        //remove after finalizing fileIO, read function will automatically call newPerson() if file load fails.
        if(!strcmp(p[0].title, ""))
        {
            teacher(p, 0); //create new teacher
        }

        else
        {

            printf("Please enter your Id: (0 to exit)");
            scanf("%d", &numIn);

            if(numIn == 0)
                exitCode = 0;

            for(int i = 0; i <= MAXSIZE && p[i].idNumber != 0; i++)
            {
                //check to see if the id is valid, if so, print welcome prompt for user. should move to own function and have it branch out.
                if(p[i].idNumber  == numIn)
                {
                    idFound = 1;
                    //call some method for user input/program modification.
                    if(!strcmp(p[i].title, "Teacher"))
                    {
                        //add check for password, could skip depending on time, or set a 5 digit pin?
                        teacher(p, 1); //authenticate teacher, then access teacher function.
                    }
                    else    //student
                    {
                        displayGrades(p, i);
                        printf("\nPress any key to clear the screen");
                        getch();
                        //clear the screen
                        clearScreen();
                    }
                }
            }
        }


        //if the id number was not found display prompt
        if(!idFound && exitCode)
            printf("ID not found\n");

        if(!idFound && !exitCode)
        {
            fileIO(p, 1);   //write data to file
            printf("\nProgram Terminated\n");
        }
    }
}
