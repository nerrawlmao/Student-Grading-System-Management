#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <direct.h>     
#include <sys/stat.h>
#include <unistd.h>

#define X 256

/* =================================================================================================================
                                                    PROTOTYPES
   ================================================================================================================= */

// GENERAL FUNCTIONS
void newPage();
void enterYourChoice();
void enterYourChoice0ToCancel();
void line();
void line1();
void invalidInput();
int hasFiles(char *dirLoc);
int directoryExists(const char *curLoc);
char** directoryChoices(char **buffer);
void directoryList();
void removeDirectory(char path[X]); 
void addFolderMain();
void addFolder();
void renameFolderMain();
void renameFolder();
void removeFolderMain();
void removeFolder();
//  PROGRAM
void viewProgram();
void addProgram();
void renameProgram();
void removeProgram();
// YEAR
void viewYearLevel();
void addYearLevel();
void renameYearLevel();
void removeYearLevel();
// SECTION
void viewSection();
void addSection();
void renameSection();
void removeSection();
// COURSE
void viewCourse();
void addCourse();
void renameCourse();
void removeCourse();
void manageCourse();
// ASSESSMENT
void viewAssessment();
void addAssessment();
void editAssessment();
void removeAssessment();
// STUDENT 
void viewStudent();
void addStudent();
void removeStudent();
void viewStudentRecord();
void viewStudentInfo();
void viewStudentAssessment();
void addStudentAssessment();
void removeStudentAssessment();
void viewStudentEnrollmentInfo();
void viewStudentGradeCourse();
void viewStudentCurrentGrade();
// GRADING SYSTEM SETTINGS
void viewGradingSystemSettings();
void editGradingSystemSettings();

// GLOBAL VARIABLES
int choice = 0, error = 0, directoryCount = 0, cancelled = 0, returned = 0, assessmentOverscore = 0;
char itemLocation[X], itemFolder[X], choosenToBeRemove[X], currentLocation[X] = "OLFU Valenzuela", garbage[X], mode[X], currentLocationHolder[X], fileLocationToBeRemove[X], *buffer[X], backToProgram[X], backToYearLevel[X], backToSection[X], backToCourse[X], assessmentName[X], courseLocation[X];
const char assessments[][X] = {"Activities", "Assignments", "Exams", "Performance Tasks", "Projects", "Quizzes"};
const char assessmentsSingular[][X] = {"Activity", "Assignment", "Exam", "Performance Task", "Project", "Quiz"};

// STYLES
char B[] = "\033[1m", b[] = "\033[1;34m", r[] = "\033[1;31m", g[] = "\033[1;32m", c[] = "\033[0m";
char borderColor[] = "\033[0;37m";

/* =================================================================================================================
                                                        MAIN
   ================================================================================================================= */

int main(){
    newPage();
    
    while(1){
        returned = 0;
        printf("    %sChoose an option:%s\n\n", B, c );
        printf("      [1] View Programs\n");
        printf("      [2] Add Program\n");
        printf("      [3] Rename Program\n");
        printf("      [4] Remove Program\n");
        printf("      [0] Exit\n");
        enterYourChoice();
        
        switch(choice){
            case 1: 
                viewProgram();
                break;
            case 2: 
                addProgram(); 
                break;
            case 3:
                renameProgram();
                break;
            case 4:
                removeProgram();
                break;
            case 0:
                newPage();
                printf("                     %sProgram exited successfully.%s\n\n", g,c);
                line();
                exit(0);
            default: 
                invalidInput();
        }
    }
}

/* =================================================================================================================
                                                GENERAL FUNCTIONS
   ================================================================================================================= */

void newPage(){
    system("cls");
    line1();
    printf("                   %sStudent Grading System Management%s\n", b, c);
    line();
}

void enterYourChoice(){
    line1();
    printf("%sEnter your choice:%s ", B, c);
    if (scanf("%d", &choice) == 1) {
    } else {
        scanf(" %[^\n]", garbage); 
        choice = -1;
    }
}

void enterYourChoice0ToCancel(){
    line1();
    printf("%sEnter your choice (0 to cancel):%s ", B, c);
    if (scanf("%d", &choice) == 1) {
    } else {
        scanf(" %[^\n]", garbage); 
        choice = -1;
        return;
    }
}

void line(){
    printf("%s=======================================================================%s\n\n", borderColor, c);
}

void line1(){
    printf("\n%s=======================================================================%s\n", borderColor, c);
}

void invalidInput(){
    newPage();
    printf("                    %sInvalid input. Please try again.%s\n\n", r, c);
    line();
}

int hasFiles(char *dirLoc) {
    DIR *dir = opendir(dirLoc);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

int directoryExists(const char *curLoc) {
    DIR *dir = opendir(curLoc);
    if (dir) {
        closedir(dir);
        return 1;
    }
    closedir(dir);
    return 0;
}

char** directoryChoices(char **buffer){
    DIR *dir;
    struct dirent *entry;
    int i = 1;
    directoryCount = 0;

    dir = opendir(currentLocation);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            buffer[i - 1] = strdup(entry->d_name);
            printf("      [%d] %s\n", i, entry->d_name); 
            i++;
            directoryCount++;
        }
    }
    closedir(dir);
    return buffer;
}

void directoryList(){
    DIR *dir;
    struct dirent *entry;
    dir = opendir(currentLocation);

    while ((entry = readdir(dir)) != NULL ) {
        if (entry->d_name[0] != '.'){
            printf("      - %s\n", entry->d_name);
        }
    }
    printf("\n");
    closedir(dir);
}

void removeDirectory(char locationToBeRemove[X]) {
    struct dirent *entry;
    DIR *dir = opendir(locationToBeRemove);
    
    while ((entry = readdir(dir)) != NULL) {
        char fileLoc[X];

        if (entry->d_name[0] == '.') {
            continue;
        }
        snprintf(fileLoc, X, "%s/%s", locationToBeRemove, entry->d_name);

        struct stat statbuf;
        if (stat(fileLoc, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                removeDirectory(fileLoc);
            } else {
                remove(fileLoc);
            }
        }
    }
    closedir(dir);
    rmdir(locationToBeRemove);
}

void addFolderMain(){
    char folderName[X];
    char folderLocation[X];

    newPage();
    printf("                 %s------- %sAdding a new %s%s %s-------%s\n", B, b, itemLocation, c, B, c);
    line1();       
    printf("%sEnter name (0 to cancel): %s", B, c);
    scanf(" %[^\n]", folderName);

    if (strcmp(folderName, "0") == 0){
        newPage();
        printf("                 %sAdding a %s has been cancelled.%s\n\n",r, itemLocation, c);
        line();
        return;
    } else {
        snprintf(folderLocation, X, "%s/%s", currentLocation, folderName);
        mkdir(folderLocation);
        newPage();
        printf("    %sThe %s '%s' was successfully added.%s\n\n", g,  itemLocation, folderName, c);
        line();
    }
}

void addFolder(){
    newPage();
    printf("    %sChoose the %s%s of %s%s%s you want to %sadd%s%s:%s\n\n", B, b, itemFolder, itemLocation, c, B, b, c, B, c);

    directoryChoices(buffer);
    enterYourChoice();

    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

    if(!directoryExists(currentLocation)){
        choice = -1;
    }
    strcpy(currentLocation, currentLocationHolder);

    if (choice == -1){
        invalidInput();
        returned = 1;
        error = 1;
        return;
    }
    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

    addFolderMain();
}

void renameFolderMain(){    
    char newName[X];
    char oldLocation[X];
    char newLocation[X];

    newPage();
    printf("    %sChoose the %s%s%s %syou want to %srename%s%s:%s\n\n", B, b, itemLocation, c,B, b, c, B ,c);
    directoryChoices(buffer);
    line1();
    
    printf("%sEnter your choice (0 to cancel): %s", B, c);
    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        invalidInput();
        return;
    }
    if (choice == 0){
        newPage();
        printf("                %sRenaming a %s has been cancelled.%s\n\n",r, itemLocation, c);
        line();
        return;
    }
    if (choice > directoryCount || choice < 1){
        invalidInput();
        return;
    }
    newPage();
    printf("                  %s------- %sRenaming a %s%s %s-------%s\n", B, b, itemLocation, c, B, c);
    line1();
    printf("%sEnter the new name (0 to cancel): %s",B, c);
    scanf(" %[^\n]", newName);
    if (strcmp(newName, "0") == 0){
        newPage();
        printf("                %sRenaming a %s has been cancelled.%s\n\n",r, itemLocation, c);
        line();
        return;
    }
    snprintf(oldLocation, X, "%s/%s", currentLocation, buffer[choice-1]);
    snprintf(newLocation, X, "%s/%s", currentLocation, newName);
    newPage();

    rename(oldLocation, newLocation);
    printf("    %sThe %s '%s' successfully renamed to '%s'.%s\n\n", g, itemLocation, buffer[choice - 1], newName, c);
    line();
}

void renameFolder(){
    printf("    %sChoose the %s%s of %s%s %syou want to %srename%s%s:%s\n\n", B,b, itemFolder, itemLocation, c, B, b, c, B, c);
    directoryChoices(buffer);
    line1();
    printf("%sEnter your choice (0 to cancel): %s", B, c);
    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        invalidInput();
        return;
    }
    if (choice == 0){
        newPage();
        return;
    }
    if (choice > directoryCount){
        invalidInput();
        return;
    }
    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);
    if(!hasFiles(currentLocation)){
        newPage();
        printf("                 %sNo %s available. Please add first.%s\n\n", r, itemLocation, c);
        line();
        strcpy(currentLocation, currentLocationHolder);
        returned = 1;
        return;
    }
    renameFolderMain();
}

void removeFolderMain(){
    char temporary[X];
    strcpy(temporary, currentLocation);
    newPage();
    printf("    %sChoose the %s%s%s%s you want to %sremove%s%s:%s\n\n", B, b, itemLocation, c, B, b, c, B, c);
    directoryChoices(buffer);
    line1();
    printf("%sEnter your choice (0 to cancel):%s ", B,c);
    
    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        invalidInput();
        strcpy(currentLocation, currentLocationHolder);
        return;
    }
    if (choice == 0){
        newPage();
        printf("                %sRemoving a %s has been cancelled.%s\n\n",r, itemLocation, c);
        line();
        strcpy(currentLocation, currentLocationHolder);
        return;
    }
    if (choice > directoryCount || choice < 1){
        invalidInput();
        return;
    }
    snprintf(fileLocationToBeRemove, X, "%s/%s", currentLocation, buffer[choice-1]);
    strcpy(choosenToBeRemove, buffer[choice-1]);
    
    newPage();
    printf("    %sRemove the %s '%s'?\n    This action cannot be undone.%s\n\n", r, itemLocation, buffer[choice -1], c);
    line();
    printf("    %sChoose an option:%s\n\n", B,c);
    printf("      [1] Yes        [0] No\n");
    line1();
    printf("%sEnter your choice: %s", B,c);

    if(scanf("%d", &choice) == 1){
        if (choice == 0){
            newPage();
            printf("                %sRemoving a %s has been cancelled.%s\n\n",r, itemLocation, c);
            line();
        } else if (choice == 1){
            removeDirectory(fileLocationToBeRemove); 
            newPage();
            printf("    %sThe %s '%s' was successfully removed.%s\n\n", g, itemLocation, choosenToBeRemove, c);
            line();  
        } else {
            invalidInput();
        }
    } else {
        scanf(" %[^\n]", garbage);
        invalidInput();
    }
    strcpy(currentLocation, temporary);
}

void removeFolder(){
    newPage();
    printf("    %sChoose the %s%s of %s%s %syou want to %sremove%s%s:%s\n\n",B,b,itemFolder, itemLocation, c,B,b,c,B,c);
    directoryChoices(buffer);
    line1();
    printf("%sEnter your choice (0 to cancel): %s", B,c);

    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        invalidInput();
        return;
    }
    if (choice == 0){
        newPage();
        printf("               %sRemoving a %s has been cancelled.%s\n\n",r, itemLocation, c);
        line();
        return;
    }
    if (choice > directoryCount){
        invalidInput();
        return;
    }
    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

    if(!hasFiles(currentLocation)){
        newPage();
        printf("             %sNo %s available. Please add one first.%s\n\n", r, itemLocation, c);
        line();
        strcpy(currentLocation, currentLocationHolder);
        returned = 1;
        return;
    }
    removeFolderMain();
}

void choose(){
    newPage();
    printf("    %sChoose the %s%s of %s%s %syou want to %sview%s%s:%s\n\n", B, b,itemFolder, itemLocation, c,B,b,c,B,c);
    directoryChoices(buffer);
    enterYourChoice();
    if (choice < 1 || choice > directoryCount){
        directoryCount = 0;
        invalidInput();
        returned = 1;
        error = 1;
        return;
    }
    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);
    if(!hasFiles(currentLocation)){
        newPage();
        printf("               %sNo %s available. Please add first.%s\n\n", r, itemLocation, c);
        line();
        strcpy(currentLocation, currentLocationHolder);
        returned = 1;
        error = 1;
    }
}

/* =================================================================================================================
                                            PROGRAM CURRENT LOCATION
   ================================================================================================================= */

void viewProgram(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(backToProgram, currentLocation);

    if(!hasFiles(currentLocation)){
        newPage();
        printf("              %sNo programs available. Please add one first.%s\n\n", r,c);
        line();
        return;
    }
    if (returned == 1){
        returned = 0;
    } else {
        newPage();
        printf("    %sPrograms:%s\n\n", B, c);
        directoryList();
        line();
    }
    printf("    %sChoose an option:%s\n\n", B,c);
    printf("      [1] View Year Levels\n");
    printf("      [2] Add Year Level\n");
    printf("      [3] Rename Year Level \n");
    printf("      [4] Remove Year Level\n");
    printf("      [0] Go Back\n");
    enterYourChoice();
    
    switch(choice){
        case 1: viewYearLevel(); break;
        case 2: addYearLevel(); break;
        case 3: renameYearLevel(); break;
        case 4: removeYearLevel(); break;
        case 0: main();
        default:
            invalidInput();
            returned = 1;
            viewProgram();
    }
    strcpy(currentLocation, currentLocationHolder);
}

void addProgram(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemLocation, "program");
    if(!directoryExists(currentLocation)){
        mkdir(currentLocation);
    }
    addFolderMain();
}

void renameProgram(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemLocation, "program");
    if(!hasFiles(currentLocation)){
        newPage();
        printf("              %sNo programs available. Please add one first.%s\n\n", r,c);
        line();
        return;
    }
    renameFolderMain(); 
}

void removeProgram(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemLocation, "program");
    
    if(!hasFiles(currentLocation)){
        newPage();
        printf("              %sNo programs available. Please add one first.%s\n\n", r, c);
        line();
        return;
    }
    removeFolderMain();
}

/* =================================================================================================================
                                            YEAR LEVEL CURRENT LOCATION
   ================================================================================================================= */

void viewYearLevel(){ 
    strcpy(currentLocationHolder, currentLocation);
    if (returned == 1){
        returned = 0;
    } else {
        strcpy(itemFolder, "program");
        strcpy(itemLocation, "year level");

        choose();
        if (error == 1){
            error = 0;
            viewProgram();
        }
        strcpy(backToYearLevel, currentLocation);
        newPage();
        printf("    %sYear levels:%s\n\n", B,c);
        directoryList();
        line();
    }
    printf("    %sChoose an option:%s\n\n", B,c);
    printf("      [1] View Sections\n");
    printf("      [2] Add Section\n");
    printf("      [3] Rename Section\n");
    printf("      [4] Remove Section\n");
    printf("      [0] Go Back\n");
    enterYourChoice();
    switch(choice){
        case 1: viewSection(); break;
        case 2: addSection(); break;
        case 3: renameSection(); break;
        case 4: removeSection(); break;
        case 0:
            strcpy(currentLocation, backToProgram);
            newPage();
            returned = 1;
            viewProgram();
        default:
            invalidInput();
            returned = 1;
            viewYearLevel();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewProgram();
}

void addYearLevel(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "program");
    strcpy(itemLocation, "year level");
    addFolder();
    if (error == 1){
        error = 0;
        viewProgram();
    }
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewProgram();
}

void renameYearLevel(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "program");
    strcpy(itemLocation, "year level");
    newPage();
    renameFolder();
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewProgram();  
}

void removeYearLevel(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "program");
    strcpy(itemLocation, "year level");
    removeFolder();
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewProgram();
}

/* =================================================================================================================
                                                     SECTION
   ================================================================================================================= */

void viewSection(){ 
    strcpy(currentLocationHolder, currentLocation);
    if (returned == 1){
        returned = 0;
    } else {
        strcpy(itemFolder, "year level");
        strcpy(itemLocation, "section");

        choose();
        if (error == 1) {
            error = 0;
            viewYearLevel();
        }
        strcpy(backToSection, currentLocation);

        newPage();
        printf("    %sSections:%s\n\n", B,c);
        directoryList();
        line();
    }
    printf("    %sChoose an option:%s\n\n", B,c);
    printf("      [1] View Courses\n");
    printf("      [2] Add Course\n");
    printf("      [3] Rename Course\n");
    printf("      [4] Remove Course\n");
    printf("      [0] Go Back\n");
    enterYourChoice();

    switch(choice){
        case 1: viewCourse(); break;
        case 2: addCourse(); break;
        case 3: renameCourse(); break;
        case 4: removeCourse(); break;
        case 0:
            strcpy(currentLocation, backToYearLevel);
            newPage();
            returned = 1;
            viewYearLevel();
        default:
            invalidInput();
            returned = 1;
            viewSection();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewYearLevel();
}

void addSection(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "year level");
    strcpy(itemLocation, "section");

    addFolder();
    if (error == 1){
        error = 0;
        viewYearLevel();
    }
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewYearLevel();
}

void renameSection(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "year level");
    strcpy(itemLocation, "section");

    newPage();
    renameFolder();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewYearLevel();  
}

void removeSection(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "year level");
    strcpy(itemLocation, "section");

    removeFolder();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewYearLevel();
}

/* =================================================================================================================
                                                     COURSE
   ================================================================================================================= */

void viewCourse(){
    strcpy(currentLocationHolder, currentLocation);
    
    if (returned == 1){
        returned = 0;
    } else {
        strcpy(itemFolder, "section");
        strcpy(itemLocation, "course");

        choose();
        if (error == 1){
            error = 0;
            viewSection();
        }
        strcpy(backToCourse, currentLocation);
        newPage();
        printf("    %sCourses:%s\n\n", B,c);
        directoryList();
        line();
    }
    printf("    %sChoose an option:%s\n\n", B,c);
    printf("      [1] Manage Course\n");
    printf("      [0] Go Back\n");
    enterYourChoice();
    
    switch(choice){
        case 1:
            manageCourse(); break;
        case 0:
            strcpy(currentLocation, backToSection);
            newPage();
            returned = 1;
            viewSection();
        default:
            invalidInput();
            returned = 1;
            viewCourse();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewSection();
}
    
void addCourse(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "section");
    strcpy(itemLocation, "course");
    addFolder();
    if (error == 1){
        error = 0;
        viewSection();
    }
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewSection();
}

void renameCourse(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "section");
    strcpy(itemLocation, "course");
    newPage();
    renameFolder();
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewSection();  
}

void removeCourse(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemFolder, "section");
    strcpy(itemLocation, "course");
    removeFolder();
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewSection();
}

void manageCourse(){
    strcpy(currentLocationHolder, currentLocation);
    strcpy(itemLocation, "course");
    if (returned == 1){
        returned = 0;
    } else {
        newPage();
        printf("    %sChoose the %scourse%s %syou want to %smanage%s%s:%s\n\n", B, b, c, B,b,c,B,c);
        directoryChoices(buffer);
        enterYourChoice();

        if (choice == -1 || choice < 1 || choice > directoryCount){
            invalidInput();
            returned = 1;
            viewCourse();
        }
        snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);
        strcpy(courseLocation, currentLocation);
        char temporary[X];
        if (!hasFiles(currentLocation)) {
            snprintf(temporary, X, "%s/%s", currentLocation, "Assessments");
            mkdir(temporary);

            for (int i = 0; i < 6; i++) {
                snprintf(currentLocation, X, "%s/%s.csv", temporary, assessments[i]);
                FILE *F1 = fopen(currentLocation, "w");
                fprintf(F1, "%s;Scores\n", assessments[i]);
                fclose(F1);
            }
            snprintf(temporary, X, "%s/Students", courseLocation);
            mkdir(temporary);
            strcpy(currentLocation, currentLocationHolder);
            snprintf(currentLocation, X, "%s/Grading System Settings.csv", courseLocation);

            FILE *file = fopen(currentLocation, "w");
            fprintf(file, "Assessment Type;Percentage\nActivities;0\nAssignments;0\nExams;60\nPerformance Tasks;40\nProjects;0\nQuizzes;0\n");
            fclose(file);
           
            snprintf(currentLocation, X, "%s/Students Current Grade.csv", courseLocation);
            file = fopen(currentLocation, "w");
            fprintf(file, "Students;Current Grade\n");
            fclose(file);

            snprintf(currentLocation, X, "%s/Students Info.csv", courseLocation);
            file = fopen(currentLocation, "w");
            fprintf(file, "Last Name;First Name;Middle Name;Student ID;Age;Birthday;Address;Contact No.;Email\n");
            fclose(file);
        }
        newPage();
    }
    printf("    %sChoose an option:%s\n\n", B,c);
    printf("      [1] View Assessments            [6] Add Score to Student\n");
    printf("      [2] Add Assessment              [7] View Students\n");
    printf("      [3] Edit Assessment             [8] Add Student\n");
    printf("      [4] Remove Assessment           [9] Remove Student \n");
    printf("      [5] View Grading Settings       [0] Go Back\n");
    enterYourChoice();
    
    switch(choice){
        case 1: viewAssessment(); break;
        case 2: addAssessment(); break;
        case 3: editAssessment(); break;
        case 4: removeAssessment(); break;
        case 5: viewGradingSystemSettings(); break;
        case 6: addStudentAssessment(); break;
        case 7: viewStudent(); break;
        case 8: addStudent(); break;
        case 9: removeStudent(); break;
        case 0:
            newPage();
            strcpy(currentLocation, backToCourse);
            returned = 1;
            viewCourse();
        default:
            invalidInput();
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            manageCourse();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewCourse();
}

/* =================================================================================================================
                                                  ASSESSMENT
   ================================================================================================================= */

void viewAssessment() {
    char temporary[X];
    snprintf(currentLocation, X, "%s/Assessments", courseLocation);
    strcpy(temporary, currentLocation);

    char text[X];
    int num, l = 0;
    FILE *file;

    newPage();
    for (int i = 0; i < 6; i++) {
        snprintf(temporary, X, "%s/%s.csv", currentLocation, assessments[i]);
        file = fopen(temporary, "r");
        int hasItem = 0;
        fgets(garbage, X, file);

        for (int j = 0; fscanf(file, "%[^;];%d\n", text, &num) == 2; j++) {
            if (j == 0){
                printf("    %s%s:%s\n\n", B, assessments[i], c);
            }
            printf("      - %s: %d\n", text, num);
            hasItem = 1;      
        }
        fclose(file);
        if (hasItem == 1){
            l++;
            printf("\n");
        }    
    }
    if (l < 1) {
        printf("            %sNo assessments available. Please add one first.%s\n\n", r,c);
    }
    line();
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    manageCourse();
}

void addAssessment() {
    char temporary[X];
    snprintf(currentLocation, X, "%s/Assessments", courseLocation);
    
    newPage();
    printf("    %sChoose the %stype of assessment%s %syou want to %sadd%s%s:%s\n\n", B, b, c, B, b, c,B,c);
    for (int i = 0; i < 6; i++) {
        printf("      [%d] %s\n", i + 1, assessments[i]);
    }
    enterYourChoice0ToCancel();
    if (choice == 0){
        newPage();
        printf("                %sAdding an assessment has been cancelled.%s\n\n",r, c);
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    if (choice > 0 && choice < 7){
        if (choice == 4){
            newPage();
            printf("             %s------- %sAdding a New %s%s %s-------%s\n",B, b, assessmentsSingular[choice - 1], c, B, c);
        } else {
            newPage();
            printf("                 %s------- %sAdding a New %s%s %s-------%s\n",B, b, assessmentsSingular[choice - 1], c, B, c);
        }
        line1();       
        printf("%sEnter the name: %s", B,c);
        scanf(" %[^\n]", assessmentName);
        printf("%sEnter the total score: %s", B,c);
        if (scanf("%d", &assessmentOverscore) != 1) {   
            scanf(" %[^\n]", garbage);
            invalidInput();
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            manageCourse();
        }
    } else {
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }     
    snprintf(currentLocation, X, "%s/%s.csv", currentLocation, assessments[choice -1]);
    FILE *file = fopen(currentLocation, "a");
    fprintf(file, "%s;%d\n", assessmentName, assessmentOverscore);   
    fclose(file);

    returned = 1;
    newPage();
    printf("    %sThe %s '%s' was added successfully.%s\n\n", g, assessmentsSingular[choice -1], assessmentName, c);
    line();
    strcpy(currentLocation, currentLocationHolder);
    manageCourse();
}

void editAssessment() {
    snprintf(currentLocation, X, "%s/Assessments", courseLocation); 

    newPage();
    printf("    %sChoose the %stype of assessment%s%s you want to %sedit%s%s:%s\n\n", B, b, c, B, b, c, B, c);
    for (int i = 0; i < 6; i++) {
        printf("      [%d] %s\n", i + 1, assessments[i]);
    }
    enterYourChoice0ToCancel();
    if (choice == 0){
        newPage();
        printf("               %sEditing an assessment has been cancelled.%s\n\n",r, c);
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    int choice1 = choice;

    if (choice1 < 1 || choice1 > 6) {
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    } 
    char temporary[X];
    snprintf(temporary, X, "%s/%s.csv", currentLocation, assessments[choice1 - 1]);
    FILE *file = fopen(temporary, "r");
    
    char header[X], name[X][X]; 
    int score[X], count = 0;

    fgets(header, X, file);
    while (fscanf(file, " %[^;];%d\n", name[count], &score[count]) == 2) {
        count++;
    }
    fclose(file);

    if (count == 0) {
        newPage();
        if (choice == 4){
            printf("      %sNo items found in %s. Please add one first.%s\n\n", r, assessments[choice - 1], c);
        } else {
            printf("          %sNo items found in %s. Please add one first.%s\n\n", r, assessments[choice - 1], c);
        }
        line();
        returned = 1;
        manageCourse();
    }
    newPage();
    printf("    %sChoose the %s%s%s %syou want to %sedit%s%s: %s\n\n", B, b, assessmentsSingular[choice1 - 1], c, B, b,c, B, c );
    for (int i = 0; i < count; i++) {
        printf("      [%d] %s: %d\n", i + 1, name[i], score[i]);
    }
    enterYourChoice0ToCancel();
    if (choice == 0){
        newPage();
        printf("               %sEditing an assessment has been cancelled.%s\n\n",r, c);
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    } else if (choice < 1 || choice > count){
        invalidInput();
        returned = 1;
        manageCourse();
    }
    char newName[X];
    int newScore;
    if (choice1 == 4){
        newPage();
        printf("                %s------- %sEditing %s%s%s -------%s\n\n",B, b, assessmentsSingular[choice1 - 1], c, B, c);
    } else {
        newPage();
        printf("                    %s------- %sEditing %s%s%s -------%s\n\n",B, b, assessmentsSingular[choice1 - 1], c, B, c);
    }
    line();
    printf("    %s%s Name:%s %s\n", b,assessmentsSingular[choice1 -1], c, name[choice - 1]);
    printf("    %sTotal Score:%s %d\n", b, c, score[choice -1]);
    line1();
    printf("%sEnter the new name: %s", B,c);
    scanf(" %[^\n]", newName);
    printf("%sEnter the new total score: %s", B,c);
    if (scanf("%d", &newScore) != 1) {   
        scanf(" %[^\n]", garbage);
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    char oldName[X];
    strcpy(oldName, name[choice - 1]);
    int oldScore = score[choice - 1];
    
    strcpy(name[choice - 1], newName);
    score[choice - 1] = newScore;

    file = fopen(temporary, "w");
    
    fprintf(file, "%s;Scores\n", assessments[choice1 - 1]);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%d\n", name[i], score[i]);
    }
    fclose(file);

    newPage();
    printf("    %s'%s: %d' was successfully edited to '%s: %d'.%s\n\n", g, oldName, oldScore, newName, newScore, c);
    line();
    returned = 1;
    manageCourse();
}

void removeAssessment() {
    snprintf(currentLocation, X, "%s/Assessments", courseLocation);
    newPage();
    printf("    %sChoose the %stype of assessment%s%s you want to %sremove%s%s:%s\n\n", B, b, c,B, b, c, B, c);
    for (int i = 0; i < 6; i++) {
        printf("      [%d] %s\n", i + 1, assessments[i]);
    }
    enterYourChoice0ToCancel();
    if (choice == 0){
        newPage();
        printf("               %sRemoving an assessment has been cancelled.%s\n\n",r, c);
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    if (choice < 1 || choice > 6) {
        invalidInput();
        returned = 1;
        manageCourse();
    }
    char temporary[X];
    char text[X][X];
    int number[X], lines = 0;
    int choice1 = choice;

    snprintf(temporary, X, "%s/%s.csv", currentLocation, assessments[choice - 1]);
    FILE *file = fopen(temporary, "r");

    fgets(garbage, X, file); 
    for (int i = 0; fscanf(file, "%[^;];%d\n", text[i], &number[i]) == 2; i++) {
        lines++;
    }
    fclose(file);

    if (lines == 0) {
        newPage();
        if (choice == 4){
            printf("      %sNo items found in %s. Please add one first.%s\n\n", r, assessments[choice - 1], c);
        } else {
            printf("          %sNo items found in %s. Please add one first.%s\n\n", r, assessments[choice - 1], c);
        }
        line();
        returned = 1;
        manageCourse();
    }
    newPage();
    printf("    %sChoose the %s%s%s %syou want to %sremove%s%s: %s\n\n", B, b, assessmentsSingular[choice - 1], c, B, b,c, B, c);
    for (int i = 0; i < lines; i++) {
        printf("      [%d] %s: %d\n", i + 1, text[i], number[i]);
    }
    enterYourChoice0ToCancel();
    if (choice == 0){
        newPage();
        printf("               %sRemoving an assessment has been cancelled.%s\n\n",r, c);
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    if (choice < 1 || choice > lines) {
        invalidInput();
        returned = 1;
        manageCourse();
    }
    char removedName[X];
    strcpy(removedName, text[choice - 1]);
    int removedScore = number[choice -1];
    for (int i = choice - 1; i < lines - 1; i++) {
        strcpy(text[i], text[i + 1]);
        number[i] = number[i + 1];
    }
    lines--;

    file = fopen(temporary, "w");

    fprintf(file, "%s;Scores\n", assessments[choice1 - 1]); 
    for (int i = 0; i < lines; i++) {
        fprintf(file, "%s;%d\n", text[i], number[i]);
    }
    fclose(file);

    newPage();
    printf("    %sThe '%s: %d' successfully removed from %s.%s\n\n", g, removedName, removedScore,assessments[choice1 - 1], c);
    line();
    returned = 1;
    manageCourse();
}

/* =================================================================================================================
                                                    STUDENT
   ================================================================================================================= */

void viewStudent(){
    strcpy(currentLocationHolder, currentLocation);
    if (returned == 1){
        returned = 0;
    } else {
        newPage();
        char temporary[X];
        snprintf(temporary, X, "%s/Students", courseLocation);
        
        if(!hasFiles(temporary)){
            newPage();
            printf("              %sNo students available. Please add one first.%s\n\n", r, c);
            line();
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            manageCourse();
        }
        snprintf(currentLocation, X, "%s/Students", courseLocation);
        newPage();
        printf("    %sStudents:%s\n\n", B,c);
        directoryList();
        line();
    }
    printf("    %sChoose an option:%s\n\n", B,c);
    printf("      [1] View Student Records\n");
    printf("      [2] View Students Current Grade\n");
    printf("      [0] Go Back\n");
    enterYourChoice();
    
    switch(choice){
        case 1: viewStudentRecord(); break;
        case 2: viewStudentGradeCourse();
        case 0:
            strcpy(currentLocation, backToCourse);
            newPage();
            returned = 1;
            manageCourse();
        default:
            invalidInput();
            returned = 1;
            viewStudent();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    manageCourse();
}

void addStudent(){
    strcpy(currentLocationHolder, currentLocation);
    newPage();
    newPage();
    printf("                 %s------- %sAdding a New Student%s %s-------%s\n\n", B, b, c, B, c);
    line();  
    printf("    %sEnter the %sstudent info%s%s:%s\n\n", B,b, c,B,c);
    char container[9][X] = {"Last Name    : ", "First Name   : ", "Middle Name  : ", "Student ID   : ", "Age          : ", "Birthday     : ", "Address      : ", "Contact No.  : ", "Email        : "};
    char info[X][X];

    for (int i = 0; i < 9; i++){
        printf("      %s", container[i]);
        scanf(" %[^\n]", info[i]);
    }
    char temporary[X];
    char fullName[X];
    snprintf(fullName, X, "%s, %s %s", info[0], info[1], info[2]);
    snprintf(temporary, X, "%s/Students/%s", courseLocation, fullName);
    mkdir(temporary);

    char temporary1[X];
    snprintf(temporary1, X, "%s/Student Info.csv", temporary);
    FILE *file = fopen(temporary1, "a");
    fprintf(file, "Last Name;First Name;Middle Name;Student ID;Age;Birthday;Address;Contact No.;Email\n");
    for (int i = 0; i < 9; i++){
        if(i == 0){
            fprintf(file, "%s",  info[i]);
        } else {
            fprintf(file, ";%s",  info[i]);
        }
    }
    fclose(file);

    snprintf(currentLocation, X, "%s/Students Info.csv", courseLocation);
    file = fopen(currentLocation, "a");
    fprintf(file, "%s",  info[0]);
    for (int i = 1; i < 9; i++){
        fprintf(file, ";%s",  info[i]);
    }
    fprintf(file, "\n");
    fclose(file);

    snprintf(temporary1, X, "%s/Student Current Grade.csv", temporary);
    file = fopen(temporary1, "a");
    fprintf(file, "Assessment Type;Student Scores;Total Scores;Grading System Percentage\n");
    fclose(file);

    snprintf(temporary1, X, "%s/Student Assessments", temporary);
    mkdir(temporary1);
    for (int i = 0; i < 6; i++) {
        snprintf(currentLocation, X, "%s/%s.csv", temporary1, assessments[i]);
        file = fopen(currentLocation, "a");
        fprintf(file, "%s;Scores\n", assessments[i]);
        fclose(file);
    }
    strcpy(currentLocation,currentLocationHolder);
    returned = 1;
    newPage();
    printf("    %sStudent '%s' was successfully added.%s\n\n", g, fullName, c);
    line();
    manageCourse();
}

void removeStudent(){
    snprintf(currentLocation, X, "%s/Students", courseLocation);
    strcpy(itemLocation, "student");

    if(!hasFiles(currentLocation)){
        newPage();
        printf("              %sNo students available. Please add one first.%s\n\n", r, c);
        line();
        returned = 1;
        manageCourse();
    }
    removeFolderMain();
    returned = 1;
    manageCourse();
}

void viewStudentInfo(){
    strcpy(currentLocationHolder,currentLocation);
    char text[X], text2[9][X];
    char container[9][X] = {"Last Name    : ", "First Name   : ", "Middle Name  : ", "Student ID   : ", "Age          : ", "Birthday     : ", "Address      : ", "Contact No.  : ", "Email        : "};

    newPage();
    printf("    %sStudent info:%s\n\n", B,c);
    snprintf(currentLocation, X, "%s/Student Info.csv", currentLocation);
    FILE *F = fopen(currentLocation, "r");

    for(int i = 0; i < 2; i++){
        if (i == 0){
            fgets(text, X, F);
            continue;
        }
        for(int j = 0; j < 9; j++){
            fscanf(F, "%[^;];", text2[j]);
            printf("      %s%s\n", container[j], text2[j]);
        }
    }
    printf("\n");
    line();
    fclose(F);

    strcpy(currentLocation,currentLocationHolder);
    returned = 1;
    viewStudentRecord();
}

void addStudentAssessment() {
    strcpy(currentLocationHolder, currentLocation);
    
    snprintf(currentLocation, X, "%s/Students", courseLocation);
    
    if(!hasFiles(currentLocation)){
        newPage();
        printf("              %sNo students available. Please add one first.%s\n\n", r, c);
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    newPage();

    printf("    %sChoose the %sstudent%s%s to add an %sassessment%s%s to:%s\n\n", B,b,c,B,b,c,B, c);
    directoryChoices(buffer);    
    enterYourChoice();
    if ( choice < 1 || choice > directoryCount){
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    } 
    char studentLocation[X];
    char studentName[X];
    strcpy(studentName, buffer[choice - 1]);
    snprintf(studentLocation, X, "%s/%s", currentLocation, buffer[choice - 1]);

    if(!hasFiles(studentLocation)){
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    newPage();
    printf("    %sChoose the %stype of assessment %s%syou want to %sadd%s%s:%s\n\n", B, b, c, B, b, c, B, c);

    for (int i = 0; i < 6; i++) {
        printf("      [%d] %s\n", i + 1, assessments[i]);
    }
    enterYourChoice();

    if (choice < 1 || choice > 6) {
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }

    snprintf(currentLocation, X, "%s/Assessments/%s.csv", courseLocation, assessments[choice - 1]);

    FILE *file = fopen(currentLocation, "r");

    int lines = 0;
    char text[X][X];
    int number[X];

    fgets(garbage, X, file);

    for (int i = 0; fscanf(file, "%[^;];%d\n", text[i], &number[i]) == 2; i++){
        lines++;
    }
    fclose(file);

    if (lines < 1) {
        newPage();
        if (choice == 4){
            printf("      %sNo items found in %s. Please add one first.%s\n\n", r, assessments[choice - 1], c);
        } else {
            printf("          %sNo items found in %s. Please add one first.%s\n\n", r, assessments[choice - 1], c);
        }
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    newPage();
    printf("    %sChoose the %s%s%s%s you want to %sadd a score%s%s:%s\n\n", B, b,assessmentsSingular[choice - 1], c, B, b, c, B, c);

    for (int i = 0; i < lines; i++) {
        printf("      [%d] %s: %d\n", i + 1, text[i], number[i]);
    }
    int choiceOfList = choice;
    enterYourChoice();

    if (choice < 1 || choice > lines) {
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }
    snprintf(currentLocation, X, "%s/Student Assessments/%s.csv", studentLocation, assessments[choiceOfList - 1]);
    FILE *F1 = fopen(currentLocation, "r");
    
    int alreadyAdded = 0;
    char text2[X][X];
    int number2[X];

    fgets(garbage, X, F1);
    for (int i = 0;fscanf(F1, "%[^;];%d\n", text2[i], &number[i]) == 2; i++){
        if (strcmp(text2[i], text[choice - 1]) == 0) {
            alreadyAdded = 1;
            break;
        }
    }
    fclose(F1);

    if (alreadyAdded == 1) {
        newPage();
        printf("                   %sThe item has already been added.%s\n\n", r,c);
        line();
        strcpy(currentLocation, currentLocationHolder);
        returned = 1;
        manageCourse();
    }
    char studentAssessment[X];
    int studentScore;

    strcpy(studentAssessment, text[choice - 1]);
    newPage();
    printf("               %s------- %sAdding a Score to Student%s %s-------%s\n\n", B, b, c, B, c);
    line();
    printf("    %sStudent Name:%s %s\n",  b, c, studentName);
    printf("    %s%s Name:%s %s\n", b,assessmentsSingular[choiceOfList -1], c, studentAssessment);
    printf("    %sTotal Score:%s %d\n", b, c, number[choice -1]);
    line1();
    printf("%sEnter the score: %s", B, c);

    if (scanf("%d", &studentScore) != 1 || studentScore < 0 || studentScore > number[choice -1 ]) {
        newPage();
        printf("            %sInvalid score. Must be between 0 and %d.%s\n\n", r, number[choice - 1], c);
        line();
        strcpy(currentLocation, currentLocationHolder);
        returned = 1;
        manageCourse();
    }

    FILE *F2 = fopen(currentLocation, "a");

    fprintf(F2, "%s;%d\n", text[choice - 1], studentScore);
    fclose(F2);

    newPage();
    printf("    %sThe '%s: %d' was successfully added to the student.%s\n\n", g, studentAssessment, studentScore,c);
    line();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    manageCourse();
}

void viewStudentRecord(){
    strcpy(currentLocationHolder, currentLocation);
    if (returned == 1){
        returned = 0;
    } else {
        strcpy(itemFolder, "record");
        strcpy(itemLocation, "the student");
        choose();
        if (error == 1){
            error = 0;
            viewStudent();
        }
        newPage();
    }
    printf("    %sChoose an option:%s\n\n", B, c);
    printf("      [1] View Student Info\n");
    printf("      [2] View Student Assessments\n");
    printf("      [3] Remove Student Assessment\n");
    printf("      [4] View Student Grade Overview\n");
    printf("      [0] Go Back\n");
    enterYourChoice();
    
    switch(choice){
        case 1: viewStudentInfo(); break;
        case 2: viewStudentAssessment(); break;
        case 3: removeStudentAssessment(); break;
        case 4: viewStudentCurrentGrade(); break;
        case 0:
            returned = 1;
            newPage();
            snprintf(currentLocation, X, "%s/Students", courseLocation);
            viewStudent();
        default:
            invalidInput();
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            viewStudentRecord();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewStudentRecord();
}

void viewStudentAssessment(){
    strcpy(currentLocationHolder, currentLocation);
    char temporary[X];
    char text[X];
    int number;
    
    snprintf(temporary, X, "%s/Student Assessments", currentLocation);
    strcpy(currentLocation, temporary);

    FILE *f;
    newPage();
    int l = 0;
    for (int i = 0; i < 6; i++){
        snprintf(temporary, X, "%s/%s.csv", currentLocation, assessments[i]);

        f = fopen(temporary, "r");
        fgets(garbage, X, f);
        int k = 0;
        while (fgets(text, X, f) != NULL) {
            k++;
        }
        fclose(f);
        if (k < 1) {
            continue;
        }
        f = fopen(temporary, "r");

        printf("    %s%s:%s\n\n", B, assessments[i],c);
        fgets(garbage, X, f);
        for (int j = 1; fscanf(f, "%[^;];%d\n", text, &number) == 2; j++) {
            printf("      - %s: %d\n", text, number);
        }
        fclose(f);
        strcpy(temporary, currentLocation);
        printf("\n");
        l++;
    }
    if (l < 1){
        printf("            %sNo assessments available. Please add one first.%s\n\n", r,c);
    }
    line();
    
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewStudentRecord();
}

void viewStudentCurrentGrade() {
    strcpy(currentLocationHolder, currentLocation);
    int sumOfEachAssessment[6] = {0}, percentage[6]= {0}, totalScore[6]= {0}, exist = 0;
    float studentPercentage[6]= {0}, assessmentPercentage[6]= {0};

    snprintf(currentLocation, X, "%s/Grading System Settings.csv", courseLocation);
    FILE *file = fopen(currentLocation, "r");
    fgets(garbage, X, file);
    for (int i = 0; i < 6; i++) {
        fscanf(file, "%[^;];%d\n", garbage, &percentage[i]);
    }
    fclose(file);

    for (int i = 0; i < 6; i++) {
        snprintf(currentLocation, X, "%s/Assessments/%s.csv", courseLocation, assessments[i]);
        FILE *file = fopen(currentLocation, "r");
        char activity[X];
        int score;
        fgets(garbage, X, file);
        while (fscanf(file, "%[^;];%d\n", activity, &score) == 2) {
            exist++;
            totalScore[i] += score;
        }
        fclose(file);
    }
    newPage();
    if (exist == 0){
        newPage();
        printf("               %sNo assessments available. Please add first.%s\n\n", r, c);
        line();
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        viewStudentRecord();
    }
    printf("    %sStudent Grade Overview:%s\n\n", B,c);
    for (int i = 0; i < 6; i++) {

        snprintf(currentLocation, X, "%s/Student Assessments/%s.csv", currentLocationHolder, assessments[i]);
        FILE *file = fopen(currentLocation, "r");
        char activity[X];
        int score;
        fgets(garbage, X, file);
        while (fscanf(file, "%[^;];%d\n", activity, &score) == 2) {
            sumOfEachAssessment[i] += score;
            
        }
        fclose(file);

        if (totalScore[i] > 0) {
            studentPercentage[i] = ((float)sumOfEachAssessment[i] / (float)totalScore[i]) * (float)percentage[i];
            assessmentPercentage[i] = ((float)sumOfEachAssessment[i] / (float)totalScore[i]) * 100;
        } else {
            studentPercentage[i] = 0;
            assessmentPercentage[i] = 0;
        }
        if (totalScore[i] == 0){
            continue;
        }
        snprintf(currentLocation, X, "%s/Student Current Grade.csv", currentLocationHolder);
        file = fopen(currentLocation, "w");
        fprintf(file, "Assessment Type;Student Scores;Total Scores;Grading System Percentage\n");
        fprintf(file, "%s;%d;%d;%.2f\n", assessments[i], sumOfEachAssessment[i], totalScore[i], studentPercentage[i]);
        fclose(file);
        printf("       %s%s%s\n         - Score: %d/%d\n", b, assessments[i], c, sumOfEachAssessment[i], totalScore[i]);
        printf("         - Percentage: %.2f%%\n\n", assessmentPercentage[i]);
    }
    float initialGrade = 0;
    for (int i = 0; i < 6; i++) {
        initialGrade += studentPercentage[i];
    }
    printf("    %sStudent Current Grade:%s %.2f\n\n", B, c, initialGrade);
    line();

    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewStudentRecord();
}

void viewStudentGradeCourse() {
    strcpy(currentLocationHolder, currentLocation);
    int sumOfEachAssessment[6] = {0};
    int percentage[6] = {0};
    int totalScore[6] = {0};
    float studentPercentage[6] = {0};

    snprintf(currentLocation, X, "%s/Grading System Settings.csv", courseLocation);
    FILE *file = fopen(currentLocation, "r");
    fgets(garbage, X, file);

    for (int i = 0; i < 6; i++) {
        fscanf(file, "%[^;];%d\n", garbage, &percentage[i]);
    }
    fclose(file);

    for (int i = 0; i < 6; i++) {
        snprintf(currentLocation, X, "%s/Assessments/%s.csv", courseLocation, assessments[i]);
        file = fopen(currentLocation, "r");
        char activity[X];
        int score;
        fgets(garbage, X, file);
        while (fscanf(file, "%[^;];%d\n", activity, &score) == 2) {
            totalScore[i] += score;
        }
        fclose(file);
    }
    snprintf(currentLocation, X, "%s/Students", courseLocation);
    DIR *directory = opendir(currentLocation);

    struct dirent *entry;
    char studentNames[X][X];
    int count = 0;

    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_name[0] != '.') {
            snprintf(studentNames[count], X, "%s", entry->d_name);
            studentNames[count][sizeof(studentNames[count]) - 1] = '\0';
            count++;
        }
    }
    closedir(directory);

    newPage();
    printf("    %sStudents Current Grade:%s\n\n", B,c);

    float initialGrade[X];

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 6; j++) {
            int sumStudentAssessment = 0;

            snprintf(currentLocation, X, "%s/Students/%s/Student Assessments/%s.csv", courseLocation, studentNames[i], assessments[j]);
            file = fopen(currentLocation, "r");

            char activity[X];
            int score;
            fgets(garbage, X, file);
            while (fscanf(file, "%[^;];%d\n", activity, &score) == 2) {
                sumStudentAssessment += score;
            }
            fclose(file);
            
            sumOfEachAssessment[j] = sumStudentAssessment;

            if (totalScore[j] > 0) {
                studentPercentage[j] = ((float)sumOfEachAssessment[j] / totalScore[j]) * percentage[j];
            } else {
                studentPercentage[j] = 0;
            }
            initialGrade[i] += studentPercentage[j];
        }
        printf("      - %s - %.2f\n", studentNames[i], initialGrade[i]);
    }
    snprintf(currentLocation, X, "%s/Students Current Grade.csv", courseLocation);
    file = fopen(currentLocation, "w");
    fprintf(file, "Students;Current Grade\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%.2f\n", studentNames[i], initialGrade[i]);
    }
    fclose(file);
    printf("\n");
    line();
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewStudent();
}

void removeStudentAssessment() {
    strcpy(currentLocationHolder, currentLocation);
    newPage();
    printf("    %sChoose the %stype of assessment %s%syou want to %sremove%s%s:%s\n\n", B, b,c,B,b,c,B,c);
    for (int i = 0; i < 6; i++) {
        printf("      [%d] %s\n", i + 1, assessments[i]);
    }
    enterYourChoice();

    int choice1 = choice;
    if (choice < 1 || choice > 6) {
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        viewStudentRecord();
    }
    char temporary[X];
    snprintf(temporary, X, "%s/Student Assessments", currentLocation);
    mkdir(temporary);

    char temporary1[X];
    
    snprintf(currentLocation, X, "%s/%s.csv", temporary, assessments[choice - 1]);
    strcpy(temporary1, assessments[choice - 1]);
    FILE *F = fopen(currentLocation, "r");

    char text[X][X];
    int number[X];
    int lines = 0;
    fgets(garbage, X, F);
    for(int i = 0; fscanf(F, "%[^;];%d\n", text[i], &number[i]) == 2; i++) {
        lines++;
    }
    fclose(F);

    if (lines < 1){
        newPage();
        if (choice == 4){
            printf("      %sNo items found in %s. Please add one first.%s\n\n", r, assessments[choice - 1], c);
        } else {
            printf("          %sNo items found in %s. Please add one first.%s\n\n", r, assessments[choice - 1], c);
        }
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        viewStudentRecord();
    }
    newPage();
    printf("    %sChoose the item in %s%s%s %syou want to %sremove%s%s:%s\n\n",  B, b, assessments[choice -1], c, B, b,c,B,c);
    for (int i = 0; i < lines; i++) { 
        printf("      [%d] %s: %d\n", i + 1, text[i], number[i]);
    }
    enterYourChoice();
    
    if (choice < 1 || choice > lines) {
        invalidInput();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        viewStudentRecord();
    }
    for (int i = choice - 1; i < lines - 1; i++) {
        strcpy(text[i], text[i + 1]);
        number[i] = number[i + 1];
    }
    lines--;
    FILE *F1 = fopen(currentLocation, "w");

    fprintf(F1, "%s;Scores\n", assessments[choice1 - 1]);
    for (int i = 0; i < lines; i++) {
        fprintf(F1, "%s;%d\n", text[i], number[i]);
    }
    fclose(F1);

    newPage();
    printf("    %sThe %s '%s' was successfully removed.%s\n\n", g, assessmentsSingular[choice1 - 1],text[choice - 1], c);
    line();
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewStudentRecord();
}

/* =================================================================================================================
                                            GRADING SYSTEM SETTINGS
   ================================================================================================================= */

void viewGradingSystemSettings() {
    strcpy(currentLocationHolder, currentLocation);
    snprintf(currentLocation, X, "%s/Grading System Settings.csv", courseLocation);
    const char assessments2[][X] = {"Activities         ", "Assignments        ", "Exams              ", "Performance Tasks  ", "Projects           ", "Quizzes            "};
    if(returned == 1){
        returned = 0;
    } else {
        newPage();
        int percentage[6];
        char text[X]; 
        FILE *F3 = fopen(currentLocation, "r");
        fgets(garbage, X, F3);
        for(int i = 0; i < 6; i++){
            (fscanf(F3, "%[^;];%d\n", text, &percentage[i]));
        }
        fclose(F3);

        printf("    %sCurrent Grading System Settings:%s\n\n", B, c);
        
        for (int i = 0; i < 6; i++) {
            printf("      %s: %d%s%%%s\n", assessments2[i], percentage[i], b, c);
        }
        line1();
    }
    
    printf("\n    %sChoose an option: %s\n\n", B, c);
    printf("      [1] Edit Grading System Settings\n");
    printf("      [0] Back\n");

    enterYourChoice();

    switch(choice){
        case 1:
            editGradingSystemSettings();
            break;
        case 0:
            returned = 1;
            newPage();
            manageCourse();
            break;
        default:
            invalidInput();
            returned = 1;
            viewGradingSystemSettings();
    }
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    manageCourse();
}

void editGradingSystemSettings() {
    strcpy(currentLocationHolder, currentLocation);

    int percentages[6];
    newPage();
    printf("            %s------- %sEditing Grading System Settings %s%s-------%s\n\n",B, b, c, B, c);
    line();
    printf("    %sEnter the %spercentage%s %sfor each %sassessent type%s%s:%s\n\n", B, b, c, B, b, c, B,c);

    int totalPercentage = 0;
    
    for (int i = 0; i < 6; i++) {
        printf("      %s: ", assessmentsSingular[i]);
        if (scanf("%d", &percentages[i]) != 1) {
            scanf(" %[^\n]", garbage);
            break;
        } else if (percentages[i] < 0 || percentages[i] > 100){
            break;
        }
        totalPercentage += percentages[i];
    }
    if (totalPercentage != 100) {
        newPage();
        printf("              %sThe total percentage must be equal to 100%%.%s\n\n",r, c);
        line();
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        viewGradingSystemSettings();
    } 
    snprintf(currentLocation, X, "%s/Grading System Settings.csv", courseLocation);
    FILE *F1 = fopen(currentLocation, "w");
    fprintf(F1, "Assessment Type;Percentage\n");
    for (int i = 0; i < 6; i++) {
        fprintf(F1, "%s;%d\n", assessmentsSingular[i], percentages[i]);
    }
    fclose(F1);
    strcpy(currentLocation, currentLocationHolder);
    viewGradingSystemSettings();
}
