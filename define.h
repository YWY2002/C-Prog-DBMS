#ifndef DEFINE_H
#define DEFINE_H

#define MAX_NAME 32
#define MAX_PROGRAMME 32
#define MAX_LINE_LENGTH 256

typedef struct Record {
    int studentID;
    char name[MAX_NAME];
    char programme[MAX_PROGRAMME];
    float marks;
    struct Record *next;
} Record;

// Global head pointer
extern Record *head;

// Function prototypes
int open_database(const char *file);
void show_all();
void show_oneRecord(Record *current);
Record *searchStudent(int input_studentID);
int insertRecord(Record *newRecord);
int updateRecord(int input_studentID, Record *updatedRecord);
int deleteRecord(int input_studentID);
int save_database(const char* file);
int studentIdValidation(int* studentID);
void clearBuffer();
int validateNames(char *name);
int validateProgramme(char* Programme);
//int validateMarks(float marks);

#endif // DEFINE_H