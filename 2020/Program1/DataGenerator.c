#include "../GlobalInterface.h"
#include "../BasicFunctions.c"


int main(int argc, char *argv[]) {
    static FILE *students_file;
    static FILE *results_file;
    static char students_file_name[DIM/2], results_file_name[DIM/2];
    static StudentNode *students_tree=NULL;

    receiveFileName(students_file_name, ID_STUDENT_FILE, ".bin");
    receiveFileName(results_file_name, ID_RESULTS_FILE, ".bin");
    if(access(students_file_name, F_OK)!=-1) {
        students_file= fopen(students_file_name, "rb");
        students_tree= readStudents(students_file, students_tree);
        fclose(students_file);
    } if(access(results_file_name, F_OK)!=-1) {
        results_file= fopen(results_file_name, "rb");
        students_tree= readResults(results_file, students_tree);
        fclose(results_file);
    }

    if (access(students_file_name, F_OK)==-1 && access(results_file_name, F_OK)==-1) {
        strcpy(students_file_name, strcat(strtok(students_file_name, "."), ".txt") );
        strcpy(results_file_name, strcat(strtok(results_file_name, "."), ".txt") );
        students_file= fopen(students_file_name, "r");
        if (students_file!=NULL) students_tree= readTextStudents(students_file, students_tree);
        fclose(students_file);
        
        results_file= fopen(results_file_name, "r");
        if (results_file!=NULL) students_tree= readTextResults(results_file, students_tree);
        fclose(results_file);
    } 

    int option;
    char str_option[5];

    //  RECEIVES OPTIONS
    while (1) {
        do {
            mainMenu();
            printf("\n\tOption=> ");
            scanf(" %[^\n]", str_option);
            option= intChecker(str_option, 'i', "INTRODUCE A VALID OPTION");
        } while (option<EXIT_OPTION || option>PRINT_CUR_RESULTS);

        if (option==ADD_STUDENT_OPTION || option==ADD_RESULT_OPTION) {
            if (option==ADD_STUDENT_OPTION) students_tree= newStudent(students_tree);
            else students_tree= newResult(students_tree);
            sleep(3);
        } else if (option==MODIFY_STUDENT_OPTION || option==MODIFY_RESULT_OPTION) {
            if (option==MODIFY_STUDENT_OPTION) students_tree= modifyStudent(students_tree);
            else students_tree= modifyResult(students_tree);
            sleep(3);
        } else if (option==DELETE_STUDENT_OPTION || option==DELETE_RESULT_OPTION) {
            //if (option==DELETE_STUDENT_OPTION) deleteElement(ID_STUDENT_FILE);
            //else deleteElement(ID_RESULTS_FILE);
        } else if (option==PRINT_CUR_STUDENTS) {
            if (students_tree==NULL) {
                printf("\n400! THERES IS NO STUDENTS' DATA SAVED YET!\nBACKING TO MAIN MENU...\n\n");
                sleep(3);
            } else {
                system("cls");
                printf("\n=============SIGNED IN STUDENTS=============\n\n");
                printStudentsTree(students_tree, JUST_PRINT_STUDENTS);
                sleep(7);
            }
        } else if (option==PRINT_CUR_RESULTS) {
            system("cls");
            printf("\n=============SIGNED IN RESULTS=============\n\n");
            printStudentsTree(students_tree, JUST_PRINT_RESULTS);
            sleep(7);
        } else if (option==EXIT_OPTION) {
            saveData(students_file, results_file, students_tree, students_file_name, results_file_name);
            printf("\n200! PROGRAM CLOSED AND DATA SAVED WITH SUCESS!!\n\n");
            break;
        }
    }
    fclose(students_file);
    fclose(results_file);
    
    return 0;
}

StudentNode* readTextStudents(FILE* file, StudentNode* students_tree) {
    char line[DIM*4], aux_str[DIM];
    StudentInfo* new_student= NULL;
    while (!feof(file)) {
        new_student= (StudentInfo*) malloc(sizeof(StudentInfo));
        fgets(line, DIM*4, file);
        strcpy(aux_str, strtok(line, "|"));
        if (stringChecker(aux_str, "ERROR ON FILE FORMAT")==1) strcpy(new_student->name, aux_str);
        else continue;
    
        strcpy(aux_str, strtok(NULL, "\n"));
        if (intChecker(aux_str, 's', "ERROR ON FILE FORMAT")==1) strcpy(new_student->id_number, aux_str);
        students_tree= addStudent(students_tree, new_student);
    }
    free(new_student);
    return students_tree;
}

StudentNode* readTextResults(FILE* file, StudentNode* students_tree) {
    char line[DIM*4], aux_str[DIM];
    int prove_id;
    float result;
    StudentInfo* new_result_student=NULL;
    CourseInfo* new_result=NULL;

    while (!feof(file)) {
        fgets(line, DIM*4, file);
        strcpy(aux_str, strtok(line, " "));
        if (intChecker(aux_str, 's', "ERROR ON FILE FORMAT")!=1) continue;
        if((new_result_student= searchStudentTreeByID(students_tree, aux_str))==NULL) continue;
        strcpy(aux_str, strtok(NULL, " "));
        if (stringChecker(aux_str, "ERROR ON FILE FORMAT")==-1) continue;
        if ((new_result=searchResultTreeByName(new_result_student->signed_in_courses, aux_str))==NULL) {
            new_result= (CourseInfo*) malloc(sizeof(CourseInfo));
            strcpy(new_result->name, aux_str); 
            strcpy(new_result->aux_id, new_result_student->id_number);
        }
        strcpy(aux_str, strtok(NULL, " "));
        if ((prove_id=intChecker(aux_str, 'i', "ERROR ON FILE FORMAT")-1)==-1) continue;

        strcpy(aux_str, strtok(NULL, "\n"));  
        if ((result=floatChecker(aux_str, 'f', "ERROR ON FILE FORMAT"))==-1) continue;
        new_result->classifications[prove_id]= result;

        new_result_student->signed_in_courses= addCourse(new_result_student->signed_in_courses, new_result);
    }
    return students_tree;
}

void mainMenu() {
    system("cls");
    printf("\t|-------------------------------------------------------|\n");
    printf("\t|\t\t\tMAIN MENU\t\t\t|\n");
    printf("\t|-------------------------------------------------------|\n");
    printf("\t|\t1=> REGIST A STUDENT\t\t\t\t|\n");
    printf("\t|\t2=> MODIFY A STUDENT\t\t\t\t|\n");
    printf("\t|\t3=> DELETE A STUDENT (not funcional)\t\t|\n");
    printf("\t|-------------------------------------------------------|\n");
    printf("\t|\t4=> REGIST A RESULT\t\t\t\t|\n");
    printf("\t|\t5=> MODIFY A RESULT\t\t\t\t|\n");
    printf("\t|\t6=> DELETE A RESULT (not funcional)\t\t|\n");
    printf("\t|-------------------------------------------------------|\n");
    printf("\t|\t7=> PRINT CURRENT STUDENTS\t\t\t|\n");
    printf("\t|\t8=> PRINT CURRENT RESULTS\t\t\t|\n");
    printf("\t|-------------------------------------------------------|\n");
    printf("\t|\t0=> EXIT\t\t\t\t\t|\n");
    printf("\t|-------------------------------------------------------|\n");
}

StudentNode* newStudent(StudentNode* tree) {
    StudentInfo* new_student= (StudentInfo*) malloc(sizeof(StudentInfo));
    while(1) {
        printf("\n\tNAME OF THE NEW STUDENT: ");
        scanf(" %[^\n]", new_student->name);
        if (stringChecker(new_student->name, "INTRODUCE A VALID STUDENT NAME")==1) break;
    }
    printf("\n\t200! STUDENT NAME ADDED WITH SUCESS!!\n");
    while(1) {
        printf("\n\tID NUMBER OF THE NEW STUDENT: ");
        scanf(" %[^\n]", new_student->id_number);
        if (intChecker(new_student->id_number, 's', "INTRODUCE A VALID STUDENT ID NUMBER")==1) break;
    }
    printf("\n\t200! STUDENT ID NUMBER ADDED WITH SUCESS!!\n");
    tree= addStudent(tree, new_student);
    return tree;
}

StudentNode* newResult(StudentNode* tree) {
    CourseInfo* new_result= (CourseInfo*) malloc(sizeof(CourseInfo));
    StudentInfo* student_result= (StudentInfo*) malloc(sizeof(StudentInfo));
    char str_result_id[DIM/5], str_result[DIM/5];
    int result_id;
    new_result->classifications[0]= new_result->classifications[1]= 0;
    
    while(1) {
        printf("\n\tSTUDENT ID NUMBER OF THE NEW RESULT: ");
        scanf(" %[^\n]", new_result->aux_id);
        if (intChecker(new_result->aux_id, 's', "INTRODUCE A VALID STUDENT ID NUMBER")==1) break;
    } 
    
    if ((student_result= searchStudentTreeByID(tree, new_result->aux_id))==NULL) {
        printf("\n404! STUDENT NOT FOUND! BACKING TO MAIN MENU\n\n");
        sleep(3);
        return tree;
    } else printf("\n\t200! STUDENT ID NUMBER ADDED WITH SUCESS!!\n");

    while(1) {
        printf("\n\tCOURSE NAME: ");
        scanf(" %[^\n]", new_result->name);
        if (stringChecker(new_result->name, "INTRODUCE A VALID COURSE NAME")==1) break;
    }  printf("\n\t200! COURSE NAME ADDED WITH SUCESS!!\n");
    
    while(1) {
        printf("\n\tPROVE NUMBER (1 OR 2): ");
        scanf(" %[^\n]", str_result_id);
        result_id= intChecker(str_result_id, 'i', "INTRODUCE A VALID PROVE NUMBER");
        if (result_id==1 || result_id==2) break;
        else printf("\n\t406! INTRODUCE A VALID PROVE NUMBER!\n\n");
    } printf("\n\t200! PROVE NUMBER ADDED WITH SUCESS!!\n");
    
    while(1) {
        printf("\n\t%s RESULT IN PROVE NUMBER %d OF %s: ", new_result->aux_id, result_id--, new_result->name);
        scanf(" %[^\n]", str_result);
        if ((new_result->classifications[result_id]= floatChecker(str_result, 'f', "INTRODUCE A VALID PROVE NUMBER"))>0 && new_result->classifications[result_id]<=20) break;
        else printf("\n\t406! INTRODUCE A VALID RESULT!\n\n");
    } printf("\n\t200! RESULT ADDED WITH SUCESS!!\n");

    student_result->signed_in_courses=addCourse(student_result->signed_in_courses, new_result);
    return tree;
}

StudentNode* modifyStudent(StudentNode* tree) {
    if (tree==NULL) {
        printf("\n400! THERES IS NO STUDENTS' DATA SAVED YET!\nBACKING TO MAIN MENU...\n\n");
        sleep(3);
    } else {
        char id_pretended[DIM/2];
        StudentInfo* student_found= (StudentInfo*)malloc(sizeof(student_found));
        system("cls");
        printf("\n=============SIGNED IN STUDENTS=============\n\n");
        printStudentsTree(tree, JUST_PRINT_STUDENTS);
        printf("\nWHAT IS THE ID OF THE STUDENT YOU WANT TO MODIFY?\nID: ");
        scanf(" %[^\n]", id_pretended);
        if ((student_found= searchStudentTreeByID(tree, id_pretended))==NULL) {
            printf("\n404! STUDENT NOT FOUND!\nBACKING TO MAIN MENU...\n");
            sleep(3);
        } else {
            char str_option[5], new_name[DIM*2], new_id[DIM/2];
            int option;
            printf("\n200! STUDENT FOUND!!\n\n");
            printf("NAME: %s\nID NUMBER: %s\n\n", student_found->name, student_found->id_number);
            do {
                printf("WHAT YOU WANT TO CHANGE? [1- NAME | 2- ID]\nOption=> ");
                scanf(" %[^\n]", str_option);
                option= intChecker(str_option, 'i', "INTRODUCE A VALID OPTION");
            } while (option!=1 && option!=2);
            if (option==1) {
                while (1) {
                    printf("INSERT A NEW NAME: ");
                    scanf(" %[^\n]", new_name);
                    if (stringChecker(new_name, "INTRODUCE A VALID STUDENT NAME")==1) break;
                } strcpy(student_found->name, new_name);
                printf("\n200! STUDENT NAME CHANGED WITH SUCESS!!\n\n");
            } else {
                while (1) {
                    printf("INSERT A NEW ID NUMBER: ");
                    scanf(" %[^\n]", new_id);
                    if (intChecker(new_id, 's', "INTRODUCE A VALID STUDENT ID NUMBER")==1) break;
                } strcpy(student_found->id_number, new_id);
                printf("\n200! STUDENT ID NUMBER CHANGED WITH SUCESS!!\n\n");
            }
        }
    }
    return tree;
}

StudentNode* modifyResult(StudentNode* tree) {
    if (tree==NULL) {
        printf("\n400! THERES IS NO STUDENTS' DATA SAVED YET!\nBACKING TO MAIN MENU...\n\n");
        sleep(3);
    } else {
        char student_id[DIM/2], course_name[DIM*2];
        CourseInfo* result_found= (CourseInfo*)malloc(sizeof(CourseInfo));
        StudentInfo* student_found= (StudentInfo*)malloc(sizeof(StudentInfo));
        system("cls");
        printf("\n=============SIGNED IN RESULTS=============\n\n");
        printStudentsTree(tree, JUST_PRINT_RESULTS);
        printf("\nWHAT IS THE STUDENT ID OF THE RESULT YOU WANT TO MODIFY?\nID: ");
        scanf(" %[^\n]", student_id);
        if ((student_found= searchStudentTreeByID(tree, student_id))==NULL) {
            printf("\n404! STUDENT NOT FOUND!\nBACKING TO MAIN MENU...\n\n");
            sleep(3);
            return tree;
        } else printf("\n200! STUDENT FOUND!\n\n");

        printf("\nWHAT IS THE COURSE NAME OF THE RESULT YOU WANT TO MODIFY?\nNAME: ");
        scanf(" %[^\n]", course_name);

        if ((result_found= searchResultTreeByName(student_found->signed_in_courses, course_name))==NULL) {
            printf("\n404! RESULT NOT FOUND!\nBACKING TO MAIN MENU...\n");
            sleep(3);
        } else {
            char str_option[5];
            int option;
            printf("\n200! RESULT FOUND!!\n\n");
            printf("NAME: %s\nPROVE1: %.2f\tPROVE2: %.2f\n\n", result_found->name, result_found->classifications[0], result_found->classifications[1]);
            do {
                printf("WHAT YOU WANT TO CHANGE? [1- NAME | 2- STUDENT ID | 3- PROVE 1 | 4- PROVE 2]\nOption=> ");
                scanf(" %[^\n]", str_option);
                option= intChecker(str_option, 'i', "INTRODUCE A VALID OPTION");
            } while (option!=1 && option!=2 && option!=3 && option!=4);
            if (option==MODIFY_NAME) {
                char new_name[DIM*2];
                while (1) {
                    printf("INSERT A NEW COURSE NAME: ");
                    scanf(" %[^\n]", new_name);
                    if (stringChecker(new_name, "INTRODUCE A VALID STUDENT NAME")==1) break;
                } strcpy(result_found->name, new_name);
                printf("\n200! COURSE NAME CHANGED WITH SUCESS!!\n\n");
            } else if (option==MODIFY_ID) {
                char new_id[DIM/2];
                while (1) {
                    printf("INSERT A NEW ID NUMBER: ");
                    scanf(" %[^\n]", new_id);
                    if (intChecker(new_id, 's', "INTRODUCE A VALID STUDENT ID NUMBER")==1) break;
                } strcpy(result_found->aux_id, new_id);
                printf("\n200! STUDENT ID NUMBER CHANGED WITH SUCESS!!\n\n");
            } else {
                char str_result[5];
                float new_result;
                while (1) {
                    printf("INSERT THE NEW RESULT: ");
                    scanf(" %[^\n]", str_result);
                    if ((new_result= floatChecker(str_result, 'f', "INTRODUCE A VALID STUDENT ID NUMBER"))!=-1) break;
                } 
                if (option==MODIFY_PROVE1) {
                    printf("\n200! PROVE 1 RESULT CHANGED WITH SUCESS!!\n\n");
                    result_found->classifications[0]= new_result;
                } else {
                    result_found->classifications[1]= new_result;
                    printf("\n200! PROVE 2 RESULT CHANGED WITH SUCESS!!\n\n");
                }
            }
        }
    } return tree;
}

void saveData(FILE* students_file, FILE* results_file, StudentNode* students_tree, char* students_file_name, char* results_file_name) {
    strcpy(students_file_name, strcat(strtok(students_file_name, "."), ".bin") );
    strcpy(results_file_name, strcat(strtok(results_file_name, "."), ".bin") );
    students_file=fopen(students_file_name, "wb");
    results_file= fopen(results_file_name, "wb");
    writeStudentsInFile(students_file, results_file, students_tree, PRINT_ALL);
    fclose(students_file);
    fclose(results_file);
}
