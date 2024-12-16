#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "define.h"
#include "functions.c" 


int main() {
    printf("                                              Declaration\n");
    printf("SIT's policy on copying does not allow the students to copy source code as well as assessment solutions\n"
        "from another person or other places. It is the students' responsibility to guarantee that their assessment\n"
        "solutions are their own work. Meanwhile, the students must also ensure that their work is not accessible\n"
        "by others. Where such plagiarism is detected, both of the assessments involved will receive ZERO mark.\n\n"
        "We hereby declare that:\n"
        "    - We fully understand and agree to the abovementioned plagiarism policy.\n"
        "    - We did not copy any code from others or from other places.\n"
        "    - We did not share our codes with others or upload to any other places for public access and will\n"
        "      not do that in the future.\n"
        "    - We agree that our project will receive Zero mark if there is any plagiarism detected.\n"
        "    - We agree that we will not disclose any information or material of the group project to others or\n"
        "      upload to any other places for public access.\n\n"
        "Declared by: P1 Group 8\n"
        "Team members:\n"
        "   1. 2400710 Irfan\n"
        "   2. 2403169 Uwais\n"
        "   3. 2402566 Nabbhil\n"
        "   4. 2401189 Shamik\n"
        "   5. 2401651 Wen Yuan\n"
        "Date: 29/11/24\n\n\n");

    char option[MAX_LINE_LENGTH];
    int input_studentID, counter = 0; //Counter to return true or False for Opening of File
    Record *current = NULL; 

    while (1) {
        printf("  ___  ___ __  __ ___ \n"
               " |   \\| _ )  \\/  / __|\n"
               " | |) | _ \\ |\\/| \\__ \\\n"
               " |___/|___/_|  |_|___/\n");

        printf("\n=======COMMANDS=======\n");
        printf("1. OPEN\n2. SHOW ALL\n3. INSERT\n4. QUERY\n5. UPDATE\n6. DELETE\n7. SAVE\n0. EXIT\n");
        printf("======================\n\n");

        printf("Enter command option number: ");
        fgets(option, MAX_LINE_LENGTH, stdin);

        //ascii (0=48,1=49,2=50,3=51,4=52,5=53,6=54,7=55)
        if (option[1] != '\n' || isdigit(option[0]) == 0 || option[0] > 55 || option[0] < 48)   //check if option > 2 char OR alphabet OR more than 7 OR less than 0
        {
            printf("\n(INVALID option selected)\n\n");
        }
        else if (option[0] == '0')
        {
            printf("\n(EXIT Successfully)\n");
            return 0;
        }
        else if (counter == 0 && option[0] > 49 && option[0] <= 55)   //Prompt user to open file first before using program
        {
            printf("\nPlease open a database to proceed\n\n");
        }
        else if (option[0] > 48 && option[0] <= 55)       //based on ascii (0=48,1=49...7=55)
        {
            switch (option[0])
            {
                
                case 49:
                    printf("\n(OPEN command selected)\n");
                    open_database("P1_8-CMS.txt");
                    counter++;
                    break;
                
                case 50:
                    printf("\n(SHOW ALL command selected)\n");
                    // showAll();
                    show_all();

                    break;

                case 51:
                    printf("\n(INSERT command selected)\n");
                    Record* newRecord = (Record*)calloc(1, sizeof(Record)); //Allocate Memory space for new record. Calloc initialize memory space to zero
                    if (newRecord == NULL) 
                    {
                        perror("Memory allocation failed"); //If the Memory allocation Fails Then Raise A Error Message
                        break;
                    }

                     // Call the function to validate userinput ID. The address of the &newRecord->studentID is passed. ==1 Means the statement return True. Validation Failed.
                    if (studentIdValidation(&newRecord->studentID) == 1) {
                        clearBuffer();
                        break; // Exit to main loop if the input is invalid
                    }

                    //Negated Statement. If program return 0, Enter if block. If program return other value, skip if block and carry on.
                    if (!insertRecord(newRecord)) {
                        printf("WARNING: Record with ID %d already exists.\n", newRecord->studentID);
                        free(newRecord); //Free the memory allocated for New Record
                        break;
                    }
                    
                    printf("Enter name: ");
                    validateNames(newRecord->name);
                    
                    printf("Enter programme: ");
                    validateProgramme(newRecord->programme);

                    float marks = 0;
                    do
                    {      
                        printf("Enter Marks: ");        
                        scanf("%f", &newRecord->marks);
                        if (newRecord->marks > 100 || newRecord->marks < 0)         //Entered marks must be betw 0 - 100
                            printf("Invalid marks entered. Please try again.\n");
                    } while (newRecord->marks > 100 || newRecord->marks < 0); //Loop till user input correct data

                    clearBuffer();

                    printf("Record successfully inserted.\n");
                    // newRecord = NULL;
                    // free(newRecord);
                    break;

                case 52:
                    printf("\n(QUERY command selected)\n");

                    if (studentIdValidation(&input_studentID) == 1) {
                        clearBuffer();
                        break; // Exit to main loop if the input is invalid
                    }

                    current = searchStudent(input_studentID);
                    if (current != NULL) {
                        printf("\nRecord Found:\n");
                        show_oneRecord(current);
                    }
                    else {
                        printf("WARNING: The student ID %d does not exist.\n", input_studentID);
                    }
                    break;

                case 53:
                    printf("\n(UPDATE command selected)\n");

                    if (studentIdValidation(&input_studentID) == 1) {
                        break; // Exit to main loop if the input is invalid
                    }

                    current = searchStudent(input_studentID); // Find the record to update
                    if (current != NULL) {
                        printf("Record Found:\n");
                        show_oneRecord(current); // Show the existing record before updating

                        printf("Enter the updated name: ");
                        validateNames(current->name);
                        printf("Enter the updated programme: ");
                        validateProgramme(current->programme);

                        do
                        {
                            printf("Enter the updated marks: ");
                            scanf("%f", &current->marks);
                            if (current->marks > 100 || current->marks < 0)         //Entered marks must be betw 0 - 100
                                printf("Invalid marks entered. Please try again.\n");
                        } while (current->marks > 100 || current->marks < 0);

                        clearBuffer();

                        printf("\nRecord successfully updated.\n");
                        printf("Updated Record:\n");
                        show_oneRecord(current); // Show the updated record
                    }
                    else {
                        printf("WARNING: The record with ID %d does not exist.\n", input_studentID);
                    }
                    break;

                case 54:
                    printf("\n(DELETE command selected)\n");

                    if (studentIdValidation(&input_studentID) == 1) {
                        break; // Exit to main loop if the input is invalid
                    }

                    while (getchar() != '\n'); // Clear the input buffer            

                    current = searchStudent(input_studentID); // Search for Use Input ID
                    if (current != NULL) { //If current in not NULL, then the userID exist in the db.
                        char choice;  // variable choice for user to confirm deletion                
                        printf("Are you sure you want to delete record with ID = %d? Type 'Y' to Confirm or 'N' to Cancel: ", input_studentID); //Ask for confirmation
                        scanf(" %c", &choice);                                  // Use " %c" since only 1 character
                        clearBuffer();                                          // Remove any leftover character
                        if (choice == 'Y' || choice == 'y') {
                            if (deleteRecord(input_studentID)) {                //Perform deletion if user input == 'Y'
                                printf("Record successfully deleted.\n");
                            }
                            else {
                                printf("WARNING: Deletion failed.\n");
                            }
                        }
                        else if (choice == 'N' || choice == 'n') {
                            printf("Deletion was cancelled.\n");           //Cancel the deletion
                        }
                        else {
                            printf("WARNING: Invalid input. Deletion operation aborted.\n"); // If user input any other value besides 'Y' or 'N'
                        }
                    }
                    else {
                        printf("WARNING: Record with ID %d does not exist.\n", input_studentID);
                    }
                    break;

                case 55:
                    save_database("P1_8-CMS.txt");
                    counter = 0;            //reset input counter
                    break;       
            }
        }

    }
}
