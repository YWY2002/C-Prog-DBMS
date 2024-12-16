#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define.h"

Record *head = NULL; // Linked list head pointer
Record* tail = NULL;
char filename[50];

int open_database(const char *file) {
    strcpy(filename, file);

    if (head == NULL)   //Open file only when mem is empty
    {
        FILE* fp = fopen(file, "r");
        if (fp == NULL) {
            printf("WARNING: Failed to open the database file \"%s\".\n", file);
            return 1;
        }

        char line[MAX_LINE_LENGTH];
        int startingLine = 6, currentLine = 1;

        while (fgets(line, MAX_LINE_LENGTH, fp))
        {
            Record* record = (Record*)calloc(1, sizeof(Record));
            if (record == NULL) {
                perror("Memory allocation failed");
                fclose(fp);
                return 1;
            }

            if (currentLine < startingLine)     //skip reading 1st to 5th line
            {
                currentLine++;
                continue;
            }
            else
            {
                sscanf(line, "%d %19c %23c %4f", &record->studentID, record->name, record->programme, &record->marks);
                if (head == NULL)       //first row of data
                {
                    head = record;
                    head->next = tail;
                }
                else if (tail == NULL)  //place subsequent data at tail
                {
                    head->next = record;
                    tail = record;
                }
                else
                {
                    tail->next = record;
                    tail = record;
                }
                
                if (record == NULL)         //if record is empty
                    break;
            }
            record = NULL;
            free(record);
        }

        printf("The database file \"%s\" is successfully opened.\n\n", file);
        fseek(fp, 0, SEEK_SET);     //move file pointer to the start
        fclose(fp);
        return 0;
    }
    else
    {
        printf("Database file \"%s\" is already opened. Please save before reopening.\n\n", file);
        return 0;
    }
    
}

// Function to show all records
void show_all() {
    if (head == NULL && tail == NULL) {
        printf("No records found in the database.\n");
        return;
    }
    printf("Here are all the records found in the table \"StudentRecords\":\n");
    printf("\n========================= Student Records ==========================\n");
    printf("%-7s | %-19s | %-23s | %-5s\n", "ID", "Name", "Programme", "Marks");
    printf("--------------------------------------------------------------------\n");

    Record *current = head;
    while (current != NULL) {
        printf("%-7d | %-19s | %-23s | %.1f\n",
               current->studentID, current->name, current->programme, current->marks);
        current = current->next;
    }

    printf("--------------------------------------------------------------------\n");
}

void show_oneRecord(Record *current) {
    if (current == NULL) {
        printf("No record to display.\n");
        return;
    }

    printf("\n=== Student Record ===\n");
    printf(" %-10s: %d\n", "ID", current->studentID); 
    printf(" %-10s: %-20s\n", "Name", current->name); 
    printf(" %-10s: %-10s\n", "Programme", current->programme); 
    printf(" %-10s: %.2f\n\n", "Marks", current->marks);
}

// Function to search for a student record
Record *searchStudent(int input_studentID) {
    Record *current = head;
    while (current != NULL) {
        if (current->studentID == input_studentID) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to insert a new record
int insertRecord(Record *newRecord) {
    if (searchStudent(newRecord->studentID) != NULL) {
        return 0; // Record already exists
    }

    newRecord->next = head;
    head = newRecord;
    return 1; // Successful insertion
}

// Function to update a record
int updateRecord(int input_studentID, Record *updatedRecord) {
    Record *current = searchStudent(input_studentID);
    if (current == NULL) {
        return 0; // Record not found
    }

    // Update fields
    strcpy(current->name, updatedRecord->name);
    strcpy(current->programme, updatedRecord->programme);
    current->marks = updatedRecord->marks;
    return 1; // Successful update
}

// Function to delete a record
int deleteRecord(int input_studentID) {
    Record *current = head; //Create a pointer pointing to Head
    Record *previous = NULL; //Create a previous pointer and set it to NULL. Set to NULL because there is no Node before Head.


    //The loop continues until current is NULL or the studentID matches the input_studentID.
    while (current != NULL && current->studentID != input_studentID) { 
        previous = current;
        current = current->next;
    }

    //Traverse thru the whole linkedList and could not find the record
    if (current == NULL) { 
        return 0; // Record not found
    }
    //If Current Node is the Head, and Head is the node to be deleted, Make the next become the new Head
    if (current == head) {
        head = head->next;
    } else { //If node to delete is in the middle, Make the previous->next = current->next to skip the node
        previous->next = current->next;
    }

    free(current); //Free the memory for the deleted node
    return 1; // Successful deletion
}

// Function to save records into a separate txt file
int save_database(const char *file)
{
    FILE* f;
    f = fopen(file, "w");   //overwrites if file exists
    fprintf(f, "Database Name: %s\n", file);
    fprintf(f, "Authors: Irfan, Uwais, Nabbhil, Shamik, Wen Yuan\n\n");
    fprintf(f, "Table Name: StudentRecords\n");
    fprintf(f, "%-8s%-20s%-24s%-5s\n", "ID", "Name", "Programme", "Marks");

    Record* save = (Record*)calloc(1, sizeof(Record));;
    save = head;

    while (save != NULL)
    {
        fprintf(f, "%-8d%-20s%-24s%-5.1f\n", save->studentID, save->name, save->programme, save->marks);
        save = save->next;
    }
    head = NULL;
    tail = NULL;
    save = NULL;
    free(save);

    printf("The database file \"%s\" has been saved successfully.\n\n", file);
    
    fseek(f, 0, SEEK_SET);     //move file pointer to the start
    fclose(f);
    return 0;
}

int validateNames(char *name)
{
    int loop = 1;
    int valid = 0;
    char tempName[20];

    while (loop)
    {
        scanf("%[^\n]%*c", tempName);

        if (strlen(tempName) > 19)          //name too long
        {
            printf("Please enter a shorter name.\n\n");
            printf("Enter name: ");
            continue;
        }
        else
        {
            for (int i = 0; i < strlen(tempName) - 1; i++)
            {
                if (isalpha(tempName[i]) || tempName[i] == ' ')
                {
                    valid = 1;
                    continue;
                }
                else
                {
                    valid = 0;
                    printf("Please enter a valid name.\n\n");
                    printf("Enter name: ");
                    break;
                }
            }
            if (valid == 1)
            {
                strcpy(name, tempName);
                break;
            }
        }
    }
    return 1;
}

int validateProgramme(char *Programme)
{
    int loop = 1;
    int valid = 0;
    char Prog[24];

    while (loop)
    {
        scanf("%[^\n]%*c", Prog);

        if (strlen(Prog) > 23)          //name too long
        {
            printf("Please enter a shorter programme name.\n\n");
            printf("Enter programme: ");
            continue;
        }
        else
        {
            for (int i = 0; i < strlen(Prog); i++)
            {
                if (isalpha(Prog[i]) || Prog[i] == ' ')
                {
                    valid = 1;
                    continue;
                }
                else
                {
                    valid = 0;
                    printf("Please enter a valid programme.\n\n");
                    printf("Enter programme: ");
                    break;
                }
            }
            if (valid == 1)
            {
                strcpy(Programme, Prog);
                break;
            }
        }
    }
    return 1;

}

/*
int validateMarks(float marks)

{
    int loop = 1;
    int valid = 0;
    char tempmarks[6];

    while (loop)
    {
        printf("Enter Marks: ");
        fgets(tempmarks, 6, stdin);     
        clearBuffer();
  
        for (int i = 0; i < strlen(tempmarks) - 1; i++)
        {
            printf("%d", tempmarks[i]);
            if (isalpha(tempmarks[i]))
            {
                printf("Please enter a valid mark.\n");
                break;
            }
            else
            {
                valid = 1;
                continue;
            }
        }
        if (valid == 1)
        { 
            break;
        }
    }
    marks = atof(tempmarks);
    return 1;
}
*/

int studentIdValidation(int* studentID) {
    char input_studentID_char[8]; // new variable for char type studentID
    int loopCheck = 0;

    // while input is invalid, keep looping                 
    while (!loopCheck) {
        printf("Enter the student ID: ");
        scanf("%7s", input_studentID_char);

        if (input_studentID_char[7] != '\0') { // Limit user input to 7 characters
            printf("WARNING: Invalid student ID.\n\n");
            clearBuffer();
            continue;
        }
        
        // Check if there are extra characters in the input buffer
        int extraChar = 0;
        while (getchar() != '\n') {
            extraChar = 1;
        }

        // (Valid Input Conditions) The input length is exactly 7, all characters are digits, and no extra characters
        if (strlen(input_studentID_char) == 7 && strspn(input_studentID_char, "0123456789") == 7 && !extraChar) {
            *studentID = atoi(input_studentID_char); // Convert to integer and assign back to original studentID variable
            loopCheck = 1; // End loop if the input is valid
        }
        else {
            printf("WARNING: Invalid student ID.\n\n"); // Print warning message if input is invalid

            // Let user to continue or exit
            char choice;
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &choice);
            
            if (choice == 'N' || choice == 'n') {
                return 1; // Let user go back to the main loop
            }
        }
    }
    return 0;
}

void clearBuffer()
{
    int a;
    while ((a = getchar()) != '\n') {}
}
