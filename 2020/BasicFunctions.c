
int stringChecker(char *str, char* error) {
    for (unsigned int i=0 ; i<strlen(str);i++){
        if (str[i] >= '0' && str[i] <= '9') {
            printf("\n406! %s!!\n\n", error);
            return -1;
        }
    }
    return 1;
}

int intChecker(char *str_number, char number_type, char* error) {
    if (number_type=='i') {
        for (unsigned int i=0 ; i<strlen(str_number);i++){
            if (str_number[i] < '0' || str_number[i] > '9') {
                printf("\n406! %s!!\n\n", error);
                return -1;
            }
        } return atoi(str_number);
    } else {
        for (unsigned int i=0 ; i<strlen(str_number);i++){
            if ((str_number[i] < '0' || str_number[i] > '9')) {
                printf("\n406! %s!!\n\n", error);
                return -1;
            }
        } return 1;
    }
}

float floatChecker(char* str_number, char number_type, char* error) {
    if (number_type=='f') {
        for (unsigned int i=0 ; i<strlen(str_number);i++){
            if ((str_number[i] < '0' || str_number[i] > '9') && str_number[i]!=',' && str_number[i]!='.') {
                printf("\n406! %s!!\n\n", error);
                return -1;
            }
            else if (str_number[i]==',') str_number[i]='.';
        } return atof(str_number);
    } else {
        for (unsigned int i=0 ; i<strlen(str_number);i++){
            if ((str_number[i] < '0' || str_number[i] > '9') && str_number[i]!=',' && str_number[i]!='.') {
                printf("\n406! %s!!\n\n", error);
                return -1;
            }
        } return -2;
    }
}


void receiveFileName(char* file_name, int file_id, char* extension) {
    char aux_file_name[DIM/2];
    if (file_id==1) printf("WHAT IS THE NAME OF STUDENTS' DATA FILE? (no extension needed)\nNAME: ");
    else if (file_id==2) printf("WHAT IS THE NAME OF RESULTS' DATA FILE? (no extension needed)\nNAME: ");
    else if (file_id==3) printf("WHAT WILL BE THE NAME OF FINAL RESULTS' TABLES FILE? (no extension needed)\nNAME: ");
    else if (file_id==4) printf("WHAT WILL BE THE NAME OF PASSED STUDENTS' FILE? (no extension needed)\nNAME: ");
    scanf(" %[^\n]", aux_file_name);
    strcat(file_name, aux_file_name);
    strcat(file_name, extension);
}

StudentNode* readStudents(FILE* file, StudentNode* tree) {
    StudentInfo* new_student= NULL;
    while (!feof(file)) {
        new_student= (StudentInfo*) malloc(sizeof(StudentInfo));
        fread(new_student->name, sizeof(new_student->name), 1, file);
        if (feof(file)) break;
        fread(new_student->id_number, sizeof(new_student->id_number), 1, file);
        tree= addStudent(tree, new_student);
    }
    free(new_student);
    return tree;
}

StudentNode* readResults(FILE* file, StudentNode* tree) {
    CourseInfo* new_course= NULL;
    StudentInfo* aux_student= NULL;
    int prove_id;
    while (!feof(file)) {
        new_course= (CourseInfo*) malloc(sizeof(CourseInfo));
        aux_student=(StudentInfo*) malloc(sizeof(StudentInfo));
        new_course->classifications[0]= new_course->classifications[1]=0;
        
        fread(new_course->aux_id, sizeof(new_course->aux_id), 1, file);
        if (feof(file)) break;
        fread(new_course->name, sizeof(new_course->name), 1, file);
        if (feof(file)) break;
        fread(&prove_id, sizeof(int), 1, file);
        if (feof(file)) break;
        fread(&new_course->classifications[prove_id], sizeof(float), 1, file);
        
        aux_student= searchStudentTreeByID(tree, new_course->aux_id);        
        if (aux_student!=NULL) aux_student->signed_in_courses= addCourse(aux_student->signed_in_courses, new_course);
    }
    free(new_course);
    free(aux_student);
    return tree;
}

StudentNode* addStudent(StudentNode *tree, StudentInfo* new_student) {
    int cond;
    if (tree == NULL) {
        tree = (StudentNode*) malloc(sizeof(StudentNode));
        tree->student= new_student;
        tree->student->signed_in_courses=NULL;
        tree->left= tree->right= NULL;
    } 
    else if (strcasecmp(new_student->id_number, tree->student->id_number)==0) return tree;
    else if ((cond = strcasecmp(new_student->name, tree->student->name))<=0) tree->left= addStudent(tree->left, new_student);
    else tree->right = addStudent(tree->right, new_student);
    return tree;
}

CourseNode* addCourse(CourseNode *tree, CourseInfo* new_course) {
    int cond;

    if (tree == NULL) {
        tree = (CourseNode*) malloc(sizeof(CourseNode));
        tree->course= new_course;
        tree->left= tree->right= NULL;
    } 
    else if ((cond = strcasecmp(new_course->name, tree->course->name)) == 0) {
        if (tree->course->classifications[0]!=0) tree->course->classifications[1]= new_course->classifications[1];
        else if (tree->course->classifications[1]!=0) tree->course->classifications[0]= new_course->classifications[0];
        else printf("\n\t406! PROVE ADDED ALREADY\n\n");
    }
    else if (cond<0) tree->left= addCourse(tree->left, new_course);
    else tree->right = addCourse(tree->right, new_course);
    return tree;
}

StudentInfo* searchStudentTreeByID(StudentNode* tree, char* id) {
    if (tree==NULL) return NULL;
    else {
        StudentInfo* student_searched= (StudentInfo*)malloc(sizeof(StudentInfo));
        if (strcasecmp(tree->student->id_number, id)!=0) {
            student_searched= searchStudentTreeByID(tree->left, id);
            if (student_searched!=NULL) return student_searched;
            student_searched= searchStudentTreeByID(tree->right, id);
            return student_searched;
        } else return tree->student;
    }
}

CourseInfo* searchResultTreeByName(CourseNode* tree, char* name) {
    if (tree==NULL) return NULL;
    else {
        if (strcasecmp(tree->course->name, name)!=0) {
            CourseInfo* course_searched= (CourseInfo*)malloc(sizeof(CourseInfo));
            course_searched= searchResultTreeByName(tree->left, name);
            if (course_searched!=NULL) return course_searched;
            course_searched= searchResultTreeByName(tree->right, name);
            return course_searched;
        } else return tree->course;
    }
}

void printCoursesTree(CourseNode* tree) {
    if (tree != NULL) {
        printCoursesTree(tree->left);
        printf("Student: %s\tCourse: %s\tProve1: %.2f\tProve2: %.2f\n", tree->course->aux_id, tree->course->name, tree->course->classifications[0], tree->course->classifications[1]);
        printCoursesTree(tree->right);
    }
}

void printStudentsTree(StudentNode* tree, int print_mode) {
    if (tree != NULL) {
        printStudentsTree(tree->left, print_mode);
        if (print_mode==JUST_PRINT_STUDENTS || print_mode==PRINT_ALL)
            printf("Name: %s\tID Number: %s\n", tree->student->name, tree->student->id_number);
        if ((print_mode==JUST_PRINT_RESULTS || print_mode==PRINT_ALL) && tree->student->signed_in_courses!=NULL)
            printCoursesTree(tree->student->signed_in_courses);
        printStudentsTree(tree->right, print_mode);
    }
}

void writeStudentsInFile(FILE* students_file, FILE* results_file, StudentNode* tree, int write_mode) {
    if (tree != NULL) {
        writeStudentsInFile(students_file, results_file, tree->left, write_mode);
        fwrite(tree->student->name, sizeof(tree->student->name), 1, students_file);
        fwrite(tree->student->id_number, sizeof(tree->student->id_number), 1, students_file);
        if (write_mode==PRINT_ALL) writeResultsInFile(results_file, tree->student->signed_in_courses);
        writeStudentsInFile(students_file, results_file, tree->right, write_mode);
    }
}

void writeResultsInFile(FILE* fp, CourseNode* tree) {
    if (tree != NULL) {
        writeResultsInFile(fp, tree->left);
        int prove_id=0;
        if (tree->course->classifications[prove_id]!=0) {
            fwrite(tree->course->aux_id, sizeof(tree->course->aux_id), 1, fp);
            fwrite(tree->course->name, sizeof(tree->course->name), 1, fp);
            fwrite(&prove_id, sizeof(int), 1, fp);
            fwrite(&tree->course->classifications[prove_id], sizeof(tree->course->classifications[prove_id]), 1, fp);
        } 
        if (tree->course->classifications[++prove_id]!=0) {
            fwrite(tree->course->aux_id, sizeof(tree->course->aux_id), 1, fp);
            fwrite(tree->course->name, sizeof(tree->course->name), 1, fp);
            fwrite(&prove_id, sizeof(int), 1, fp);
            fwrite(&tree->course->classifications[prove_id], sizeof(tree->course->classifications[prove_id]), 1, fp);
        }
        writeResultsInFile(fp, tree->right);
    }
}