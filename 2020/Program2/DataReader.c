#include "../GlobalInterface.h"
#include "../BasicFunctions.c"

int main() {
    static StudentNode* all_students=NULL;
    static StudentNode* passed_students_tree=NULL;
    static StringArray* all_course_names=NULL;
    static FILE *entry_file_students;
    static FILE *entry_file_results;
    static char students_file_name[DIM/2], results_file_name[DIM/2];

    while (1) {
        system("cls");
        strcpy(students_file_name, ENTRY_FILES_DIR);
        receiveFileName(students_file_name, ID_STUDENT_FILE, ".bin");
        entry_file_students= fopen(students_file_name, "rb");
        if (entry_file_students!=NULL) break;
        else printf("\n404! FILE OF STUDENTS NOT FOUND!\n\n");
        sleep(2);
    }  while (1) {
        strcpy(results_file_name, ENTRY_FILES_DIR);
        receiveFileName(results_file_name, ID_RESULTS_FILE, ".bin");
        entry_file_results= fopen(results_file_name, "rb");
        if (entry_file_results!=NULL) break;
        else printf("\n404! FILE OF RESULTS NOT FOUND!\n\n");
        sleep(2);
    }
    
    all_students= readStudents(entry_file_students, all_students);
    all_students= readResults(entry_file_results, all_students);

    all_course_names= getAllCourses(all_students, all_course_names);
    if (all_course_names!=NULL) makeTables(all_students, all_course_names);
    else printf("\n400! THERE ARE NO STUDENTS TO MAKE TABLES, SO, TABLES' FILE WASN'T DONE\n\n");

    passed_students_tree= passedStudentsGenerator(all_students, passed_students_tree);
    
    if (passed_students_tree!=NULL) {
        strcpy(students_file_name, "");
        receiveFileName(students_file_name, 4, ".bin");
        FILE* passed_students_tree_file= fopen(students_file_name, "wb");
        writeStudentsInFile(passed_students_tree_file, NULL, passed_students_tree, PRINT_CUR_STUDENTS);        
        fclose(passed_students_tree_file);
        printf("\n200! TABLES CREATED WITH SUCESS!\n");
    } else printf("\n417 UPS! IT SEEMS THERE IS NO STUDENT PASSING!\n\n");
    fclose(entry_file_students);
    fclose(entry_file_results);
    return 0;
}


StringArray* addName(StringArray* array, char* name) {
    if (array == NULL) {
        array = (StringArray*) malloc(sizeof(StringArray));
        strcpy(array->string, name);
        array->occurences=1;
        array->next= NULL;
    } else if (strcasecmp(array->string, name)==0) { array->occurences++; return array;}
    else array->next = addName(array->next, name);
    return array;
}
StringArray* getStudentCourses(CourseNode* tree, StringArray* names_array) {
    if (tree!=NULL)  {
        names_array= getStudentCourses(tree->left, names_array);
        names_array= addName(names_array, tree->course->name);
        names_array= getStudentCourses(tree->right, names_array);
    } return names_array;
}    
StringArray* getAllCourses(StudentNode* tree, StringArray* names_array) {
    if (tree!=NULL) {
        names_array= getAllCourses(tree->left, names_array);
        names_array= getStudentCourses(tree->student->signed_in_courses, names_array);
        names_array= getAllCourses(tree->right, names_array);
    } return names_array;
}


void searchCourseInStudent(FILE* tables_file, CourseNode* courses_tree, char* name, StudentInfo* student) {
    if (courses_tree!=NULL)  {
        searchCourseInStudent(tables_file, courses_tree->left, name, student);
        if (strcasecmp(courses_tree->course->name, name)==0) {
            float final_average= (courses_tree->course->classifications[0] + courses_tree->course->classifications[1])/2;
            fwrite(student->name, sizeof(student->name), 1, tables_file);
            fwrite(student->id_number, sizeof(student->id_number), 1, tables_file);
            fwrite(&final_average, sizeof(float), 1, tables_file);
            return;
        }
        searchCourseInStudent(tables_file, courses_tree->right, name, student);
    }
}    
void searchStudentWithCourse(FILE* tables_file, StudentNode* students_tree, char* name) {
    if (students_tree!=NULL) {
        searchStudentWithCourse(tables_file, students_tree->left, name);
        searchCourseInStudent(tables_file, students_tree->student->signed_in_courses, name, students_tree->student);
        searchStudentWithCourse(tables_file, students_tree->right, name);
    }
}
void makeTables(StudentNode* students_tree, StringArray* names) {
    FILE *tables_file;
    char tables_file_name[DIM/2];
    receiveFileName(tables_file_name, 3, ".bin");
    tables_file= fopen(tables_file_name, "wb");
        
    while (names) {
        fwrite(names->string, sizeof(names->string), 1, tables_file);
        fwrite(&names->occurences, sizeof(int), 1, tables_file);
        searchStudentWithCourse(tables_file, students_tree, names->string);
        names=names->next;
    }
    fclose(tables_file);
}


StudentNode* passedStudentsGenerator(StudentNode* tree, StudentNode* passed_students_tree) {
    if (tree != NULL) {
        passed_students_tree= passedStudentsGenerator(tree->left, passed_students_tree);
        if (passedCoursesCounter(tree->student->signed_in_courses, 0)>=TOT_COURSES_PASSED)
            passed_students_tree= addStudent(passed_students_tree, tree->student);
        passed_students_tree= passedStudentsGenerator(tree->right, passed_students_tree);
    } return passed_students_tree;
}
int passedCoursesCounter(CourseNode* courses, int counter) {
    if (courses != NULL) {
        counter+= passedCoursesCounter(courses->left, counter);        
        if ((courses->course->classifications[0]+courses->course->classifications[1])/2>=PASSED_RESULT) counter+=1;
        counter+= passedCoursesCounter(courses->right, counter);
        return counter;
    }
    else return 0;
}