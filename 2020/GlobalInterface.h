//  ========================================
//                  LIBRARIES
//  ========================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

//  ========================================
//                  CONSTATNS
//  ========================================

//  COMMON
#define ID_STUDENT_FILE 1
#define ID_RESULTS_FILE 2
#define TOT_PROVES 2
#define DIM 50

#define JUST_PRINT_STUDENTS 0
#define JUST_PRINT_RESULTS 1
#define PRINT_ALL 2

//  MAIN MENU OPTIONS PROGRAM 1
#define ADD_STUDENT_OPTION 1
#define MODIFY_STUDENT_OPTION 2
#define DELETE_STUDENT_OPTION 3
#define ADD_RESULT_OPTION 4
#define MODIFY_RESULT_OPTION 5
#define DELETE_RESULT_OPTION 6
#define PRINT_CUR_STUDENTS 7
#define PRINT_CUR_RESULTS 8
#define EXIT_OPTION 0

//  MODIFY MENU OPTIONS PROGRAM 1
#define MODIFY_NAME 1
#define MODIFY_ID 2
#define MODIFY_PROVE1 3

//  FILE'S DIRECTORY PROGRAM 2
#define ENTRY_FILES_DIR "../Program1/"
#define PASSED_RESULT 9.5
#define TOT_COURSES_PASSED 8

//  FILE'S DIRECTORY PROGRAM 3
#define ENTRY_FILES_DIR_2 "../Program2/"


//  ==============================================
//                  DATA STRUCTURES
//  ==============================================

//  LINKED LIST WITH (IN THIS CASE) A NAME OF A COURSE AND HOM MANY STUDENTS ARE SIGNED IN IT
typedef struct aux_course_names StringArray;
static struct aux_course_names {
    char string[DIM];
    int occurences;
    /*------------*/
    StringArray *next;
};

//  NODE WITH COURSES' INFORMATION
typedef struct crs_info CourseInfo;
static struct crs_info {
    char name[DIM];
    float classifications[TOT_PROVES];
    //  ONLY TO USE IN PRINTS OF PROGRAM 1
    char aux_id[DIM/2];
};

//  TREE WITH ALL COURSES OF EACH STUDENT
typedef struct aux_course CourseNode;
static struct aux_course {
    CourseInfo *course;
    CourseNode *left;
    CourseNode *right;
};

//  NODE WITH EACH STUDENT INFORMATION (INCLUDING THEIR COURSES)
typedef struct std_info StudentInfo;
static struct std_info {
    char name[DIM*2];
    char id_number[DIM/2];
    CourseNode* signed_in_courses;
};

//  TREE WITH ALL THE DATA, THIS WILL BE OUR MAIN TREE
//  ALL THE COURSES ARE INSIDE OF EACH STUDENT
typedef struct aux_student StudentNode;
static struct aux_student {
    StudentInfo *student;
    StudentNode *left;
    StudentNode *right;
};


//  ========================================
//                  FUNCTIONS
//  ========================================

/*============================BASIC FUNCTIONS============================*/

//  RECEIVE A STRING AND A MESSAGE (IN CASE OF ERROR) TO PRINT
//  RETURNS 1 IF IT IS A CORRECT STRING, -1 IF IT ISN'T
int stringChecker(char *str, char* error);

//  RECEIVE A STRING, A MODE TO OPERATE AND A MESSAGE (IN CASE OF ERROR) TO PRINT
//  THE CHARACTER CAN ONLY BE 'i', 's' DEPENDING THE TYPE OF DATA WE WANT
//      IF 'i' RETURN atoi() OF THE NUMBER
//      IF THE STRING NUMBER IS WRONG RETURN -1 
int intChecker(char *str_number, char number_type, char* error);

//  RECEIVE A STRING, A MODE TO OPERATE AND A MESSAGE (IN CASE OF ERROR) TO PRINT
//  THE CHARACTER CAN ONLY BE 'f', 's' DEPENDING THE TYPE OF DATA WE WANT
//      IF 'i' RETURN atof() OF THE NUMBER
//      IF THE STRING NUMBER IS WRONG RETURN -1 
float floatChecker(char* str_number, char number_type, char* error);

//  RECEIVES THE FILE'S DIRECTORY AND AN INT TO IDENITFY WICH FILE IT IS
//  BASICALLY VALIDATES A FILE NAME
void receiveFileName(char* file_name, int file_id, char* extension);

//  RECEIVES THE STUDENT'S FILE AND AN EMPTY TREE
//  RETURNS THE FILLED TREE
StudentNode* readStudents(FILE* file, StudentNode* tree);

//  RECEIVES THE RESULTS' FILE AND AN EMPTY TREE
//  FILLS STUDENT'S NODES WITH THEIR RESPECTIVE COURSES AND RESULTS
//  RETURNS THE FILLED TREE
StudentNode* readResults(FILE* file, StudentNode* tree);

//  RECURSIVELY RECEIVES A TREE AND A NODE TO BE ADDED
//  RECURSIVELY RETURNS THE ORIGINAL TREE WITH THE NODE ADDED
StudentNode* addStudent(StudentNode *tree, StudentInfo* new_student);

//  RECURSIVELY RECEIVES A TREE AND A NODE TO BE ADDED
//  RECURSIVELY RETURNS THE ORIGINAL TREE WITH THE NODE ADDED
CourseNode* addCourse(CourseNode *tree, CourseInfo* new_course);

//  RECEIVES A TREE AND AN ID NUMBER
//  RETURNS THE CORRESPONDING ID NODE
StudentInfo* searchStudentTreeByID(StudentNode* tree, char* id);
CourseInfo* searchResultTreeByName(CourseNode* tree, char* name);

//  RECURSIVELY RECEIVES A TREE AND A MODE TO PRINT, IF ONLY PRNTS STUDENTS, RESULTS OR BOTH
//  PRINTS IT'S NODES
void printStudentsTree(StudentNode* tree, int print_mode);
//  RECURSIVELY RECEIVES A TREE AND PRINTS IT'S NODES
void printCoursesTree(CourseNode* tree);

//  RECEIVES RESULTS' AND STUDENTS' FILES, A STUDENT NODE AND AN INT THAT DEFINES IF IT IS SUPOST TO WRITE RESULTS OR NO
//  WRITES DATA IN STUDENTS FILE (AND RESULTS FILE)
void writeStudentsInFile(FILE* students_file, FILE* results_file, StudentNode* tree, int write_mode);
//  RECEIVES RESULTS' FILE AND A TREE OF COURSES
//  WRITES DATA IN RESULTS FILE
void writeResultsInFile(FILE* fp, CourseNode* tree);

/*============================PROGRAM 1 FUNCTIONS============================*/

//  JUST CLEAR THE CONSOLE AND PRINTS THE ENTIRE MAIN MENU
void mainMenu();

//  RECEIVES A TEXT FILE AND AN EMPTY TREE OF STUDENTS
//  IF THE BINARY FILES DOESN'T EXIST, GETS INFO FROM A TEXT FILE
//  IF TEXT FILE ALSO DOESN'T EXTIST, PROGRAM STARTS WITH NO INFORMATION
//  RETURNS A FILLED STUDENTS' TREE
StudentNode* readTextStudents(FILE* file, StudentNode* students_tree);
//  RECEIVES A TEXT FILE AND AN EMPTY TREE OF STUDENTS
//  IF THE BINARY FILES DOESN'T EXIST, GETS INFO FROM A TEXT FILE
//  IF TEXT FILE ALSO DOESN'T EXTIST, PROGRAM STARTS WITH NO INFORMATION
//  RETURNS A FILLED STUDENTS' TREE
StudentNode* readTextResults(FILE* file, StudentNode* students_tree);

//  ADD A NEW STUDENT NODE TO THE MAIN TREE
//  RETURN SAME TREE WITH THAT NEW NODE
StudentNode* newStudent(StudentNode* tree);
//  ADD A NEW RESULT NODE TO THE MAIN TREE
//  RETURN SAME TREE WITH THAT NEW NODE
StudentNode* newResult(StudentNode* tree);

//  RECEIVE THE MAIN TREE, SEARCH A STUDENT AND CHANGE ITS INFORMATION
//  RETURN SAME TREE WIHT INFORMATION CHANGED
StudentNode* modifyStudent(StudentNode* tree);
//  RECEIVE THE MAIN TREE, SEARCH A STUDENT AND CHANGE ITS INFORMATION
//  RETURN SAME TREE WIHT INFORMATION CHANGED
StudentNode* modifyResult(StudentNode* tree);

//  BASICALLY RECEIVES ALL VARIABLES NEEDED TO WRITE DATA IN THE RIGHT FILE, CREATE THE FILES AND CALL FUNCTION 'writeStudentsInFile(...)'
void saveData(FILE* students_file, FILE* results_file, StudentNode* students_tree, char* students_file_name, char* results_file_name);

/*============================PROGRAM 2 FUNCTIONS============================*/

//  RECEIVES A LINKED LIST AND ADD A NAME TO THAT LINKED LIST
//  RETURN SAME LINKED LIST WITH NEW NODE
StringArray* addName(StringArray* array, char* name);
//  RECURSIVELY ADD THE COURSES NAMES THAT AREN'T IN THE LINKED LIST YET
//  RETURN SAME LINKED LIST IN CASE OF A NEW COURSE NAME HAVE BEEN ADDED
StringArray* getStudentCourses(CourseNode* tree, StringArray* names_array);
//  BASICALLY, CALL RECURSIVELY THE FUNCTION ABOVE WITH A DIFFERENT STUDENT IN EACH CALLING
//  RETURN THE FINAL LINKED LIST WITH ALL COURSES THAT STUDENTS MAY ARE SIGNED IN
StringArray* getAllCourses(StudentNode* tree, StringArray* names_array);

//  RECEIVES THE MAIN TREE WITH A LIKED LIST WITH ALL COURSES STUDENTS MAY ARE SIGNED IN
//  WRITES THE TABLES OF THE COURSES IN NEW BINARY FILES, WICH NAMES ARE REQUESTED BY THE USER
void makeTables(StudentNode* students_tree, StringArray* names);
//  RUN RECURSIVELY THE MAIN TREE CALLING, IN EACH NODE, THE FUNCTION BELOW
void searchStudentWithCourse(FILE* tables_file, StudentNode* students_tree, char* name);
//  RECURSIVELY SEARCH A CERTAIN COURSE, WHEN FOUND, WRITE IT INFORMATION IN THE FILE
void searchCourseInStudent(FILE* tables_file, CourseNode* courses_tree, char* name, StudentInfo* student);

//  RECEIVES THE MAIN TREE AND AN EMPTY TREE
//  ADD A STUDENT THAT PASSED TO THAT EMPTY TREE
//  RETURNS A NEW TREE WITH ALL PASSED STUDENTS
StudentNode* passedStudentsGenerator(StudentNode* tree, StudentNode* passed_students_tree);
//  RECEIVES THE COURSES' TREE AND A COUNTER
//  COUNTS HOW MANY COURSES THAT STUDENT PASSED
//  RETURNS THAT COUNTER
int passedCoursesCounter(CourseNode* courses, int counter);

/*============================PROGRAM 3 FUNCTIONS============================*/

//  RECEIVES THE ENTRY BINARY FILES AND TWO STRINGS TO CREATE NEW TWO TEXT FILES
//  READS DATA IN BINARY FILES AND WRITES IT IN A TEXT FILE (FINAL RESULTS AND PASSED STUDENTS DATA)
void translateFiles(FILE* results_table_bin, FILE* passed_students_bin, char* file_name1, char* file_name2);
//  RECURSIVELY WRITES THE INFO PRESENT IN PASSED STUDENTS TREE IN TEXT FILE
void writePassedStudents(StudentNode* passed_students, FILE* passed_students_txt);
