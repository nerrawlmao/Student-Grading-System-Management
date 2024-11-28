#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <direct.h>
#include <sys/stat.h>
#include <unistd.h>

#define X 200

/* =================================================================================================================
                                                    PROTOTYPES
   ================================================================================================================= */

// GENERAL FUNCTIONS
void newPage();
void enterYourChoice();
int hasFiles(char *dirPath);
int directoryExists(const char *currentLocation);
char** directoryChoices(char **buffer);
void directoryList();
void removeDirectory(char path[255]); 
void addFolder();
void renameFolderMain();
void renameFolder();
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
void viewStudentRecord();
void viewStudentInfo();
void viewStudentAssessment();
void addStudentAssessment();
void removeStudentAssessment();
void viewStudentEnrollmentInfo();
void viewStudentGradeCourse();
void viewStudentGradeSummary();

// GRADING SYSTEM SETTINGS
void viewGradingSystemSettings();
void editGradingSystemSettings();

// GLOBAL VARIABLES
int choice;
char currentLocation[X] = "OLFU Valenzuela";
char garbage[X];
int returned = 0;
char currentLocationHolder[X];
char fileLocationToBeRemove[X];
char *buffer[X];
char backToProgram[X];
char backToYearLevel[X];
char backToSection[X];
char backToCourse[X];
char assessmentName[X];
int assessmentOverscore;
char courseLocation[X];
const char assessments[][X] = {"Activities", "Assignments", "Exams", "Performance Tasks", "Projects", "Quizzes"};

/* =================================================================================================================
                                                        MAIN
   ================================================================================================================= */

int main(){
    newPage();
    
    while(1){
        returned = 0;
        printf("  \033[1mPlease choose an option:\033[0m\n\n");

        printf("    [1] View programs\n");
        printf("    [2] Add program\n");
        printf("    [3] Rename program\n");
        printf("    [4] Remove program\n");
        printf("    [0] Exit\n");
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
                printf("               \033[1;32mProgram exited successfully.\033[0m\n\n");
                printf("===========================================================\n\n");
                exit(0);
            default: 
                newPage();
                printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
                printf("===========================================================\n\n");
        }
    }
}

/* =================================================================================================================
                                                GENERAL FUNCTIONS
   ================================================================================================================= */

void newPage(){
    system("cls");
    printf("\n===========================================================\n");
    printf("        \033[1;34mEduTrack: Student Grading System Management\033[0m\n");
    printf("===========================================================\n\n");
}

void enterYourChoice(){
    printf("\n===========================================================\n");
    printf("Enter your choice: ");
    if (scanf("%d", &choice) == 1) {
    } else {
        scanf(" %[^\n]", garbage); 
        choice = -1;
    }
}

int hasFiles(char *directoryLocation) {
    DIR *dir = opendir(directoryLocation);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            closedir(dir);
            return 1;
        }
    }

    return 0;  // No files found
}

int directoryExists(const char *currentLocation) {
    DIR *dir = opendir(currentLocation);
    if (dir) {
        closedir(dir);
        return 1;
    }
    return 0;
}

char** directoryChoices(char **buffer){
    DIR *dir;
    struct dirent *entry;
    int i = 1;

    dir = opendir(currentLocation);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            buffer[i - 1] = strdup(entry->d_name);
            printf("    [%d] %s\n", i, entry->d_name); 
            i++;
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
            printf("    - %s\n", entry->d_name);
        }
    }
    printf("\n");
    closedir(dir);
}

void removeDirectory(char locationToBeRemove[X]) {
    struct dirent *entry;
    DIR *dir = opendir(locationToBeRemove);
    
    while ((entry = readdir(dir)) != NULL) {
        char fileLoc[500];

        if (entry->d_name[0] == '.') {
            continue;
        }

        snprintf(fileLoc, 500, "%s/%s", locationToBeRemove, entry->d_name);

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

void addFolder(){
    char folderName[X];
    char folderLocation[X];
    newPage();
    printf("--------------------- \033[3mAdding an item\033[0m ----------------------\n\n");
    printf("===========================================================\n");        
    printf("Enter name (0 to cancel): ");
    scanf(" %[^\n]", folderName);

    if (strcmp(folderName, "0") == 0){
        newPage();
        printf("            \033[1;31mAdding an item has been cancelled.\033[0m\n\n");
        printf("===========================================================\n\n");
        return;
    } else {
        snprintf(folderLocation, X, "%s/%s", currentLocation, folderName);
        mkdir(folderLocation);
        newPage();
        printf("  \033[1;32m'%s' is successfully added.\033[0m\n\n", folderName);
        printf("===========================================================\n\n");
    }
}

void renameFolderMain(){
    char newName[X];
    char oldLocation[X];
    char newLocation[X];
    newPage();
    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    directoryChoices(buffer);
    printf("\n===========================================================\n");
    
    printf("Enter to be rename (0 to cancel): ");
    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        return;
    }
    if (choice == 0){
        newPage();
        return;
    }

    newPage();
    printf("------------------- \033[3mRenaming an item\033[0m --------------------\n\n");
    printf("===========================================================\n");
    printf("Enter the new name: ");
    scanf(" %[^\n]", newName);

    snprintf(oldLocation, X, "%s/%s", currentLocation, buffer[choice-1]);
    snprintf(newLocation, X, "%s/%s", currentLocation, newName);
    newPage();
    rename(oldLocation, newLocation);
    printf("  \033[1;32m'%s' is successfully renamed to '%s'.\033[0m\n", buffer[choice - 1], newName);
    printf("\n===========================================================\n\n");
}

void renameFolder(){
    char newName[X];
    char oldLocation[X];
    char newLocation[X];
    printf("  \033[1mEnter the location of the item to be renamed:\033[0m\n\n");
    directoryChoices(buffer);
    printf("\n===========================================================\n");
    
    printf("Enter your choice (0 to cancel): ");
    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        return;
    }
    if (choice == 0){
        newPage();
        return;
    }
    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);
    if(!hasFiles(currentLocation)){
        newPage();
        printf("           \033[1;31mNo items available. Please add first.\033[0m\n\n");
        printf("===========================================================\n\n");
        snprintf(currentLocation, X, "%s", currentLocationHolder);
        returned = 1;
        return;
    }
    renameFolderMain();
}

void removeFolder(){
    snprintf(currentLocationHolder, 255, "%s", currentLocation);
    char choosenToBeRemove[255];
    printf("Enter your choice (0 to cancel): ");

    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        return;
    }

    if (choice == 0){
        newPage();
        return;
    }

    snprintf(currentLocation, 255, "%s/%s", currentLocation, buffer[choice-1]);

    if(!hasFiles(currentLocation)){
        newPage();
        printf("           \033[1;31mNo items available. Please add first.\033[0m\n\n");
        printf("===========================================================\n\n");
        snprintf(currentLocation, 255, "%s", currentLocationHolder);
        returned = 1;
        snprintf(currentLocation, 255, "%s", currentLocation);
        return;
    }
    
    newPage();
    printf("  \033[1mChoose the item to be remove:\033[0m\n\n");
    directoryChoices(buffer);
    printf("\n===========================================================\n");
    printf("Enter your choice (0 to cancel): ");
    
    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        snprintf(currentLocation, 255, "%s", currentLocationHolder);
        return;
    }

    if (choice == 0){
        newPage();
        snprintf(currentLocation, 255, "%s", currentLocationHolder);
        return;
    }

    snprintf(fileLocationToBeRemove, 255, "%s/%s", currentLocation, buffer[choice-1]);
    strcpy(choosenToBeRemove, buffer[choice-1]);
    newPage();
    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("   [1] Yes\n");
    printf("   [2] No\n");
    printf("\n  \033[1;31mAre you sure you want to remove the '%s'?\n  This action cannot be undone.\033[0m\n", buffer[choice -1]);
    printf("\n===========================================================\n");
    printf("Enter your choice (0 to cancel): ");
    if(scanf("%d", &choice) == 1){
        if (choice == 0){
            newPage();
        } else if (choice == 1){
            removeDirectory(fileLocationToBeRemove); 
            newPage();
            printf("  \033[1;32m'%s' is successfully removed.\033[0m\n", choosenToBeRemove);
            printf("\n===========================================================\n\n");
            
        } else if ( choice == 2){
            newPage();
            printf("         \033[1;32mThe removal of '%s' has been cancelled.\033[0m\n", choosenToBeRemove);
            printf("\n===========================================================\n\n");
        } else {
            scanf(" %[^\n]", garbage);
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            
        }
    } else {
        scanf(" %[^\n]", garbage);
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
    }
    strcpy(currentLocation, currentLocationHolder);
}

/* =================================================================================================================
                                            PROGRAM CURRENT LOCATION
   ================================================================================================================= */

void viewProgram(){
    strcpy(backToProgram, currentLocation);
    strcpy(currentLocationHolder, currentLocation);

    if(!hasFiles(currentLocation)){
        newPage();
        printf("           \033[1;31mNo items available. Please add first.\033[0m\n\n");
        printf("===========================================================\n\n");
        strcpy(currentLocation, currentLocationHolder);
        return;
    }
    

    if (returned == 1){
        returned = 0;
    } else {
        newPage();
        printf("  \033[1mPrograms:\033[0m\n\n");
        directoryList();
        printf("===========================================================\n\n");
    }
    
    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("    [1] View year levels\n");
    printf("    [2] Add year level\n");
    printf("    [3] Rename year level \n");
    printf("    [4] Remove year level\n");
    printf("    [0] Back\n");
    enterYourChoice();
    
    switch(choice){
        case 1: viewYearLevel(); break;
        case 2: addYearLevel(); break;
        case 3: renameYearLevel(); break;
        case 4: removeYearLevel(); break;
        case 0:
            main();
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            viewProgram();
            return;
        
    }
    strcpy(currentLocation, currentLocationHolder);
}

void addProgram(){
    if(!directoryExists(currentLocation)){
        mkdir(currentLocation);
    }
    addFolder();

}

void renameProgram(){
    snprintf(currentLocationHolder, 255, "%s", currentLocation);
    char newName[255];
    char oldLocation[255];
    char newLocation[255];

    if(!hasFiles(currentLocation)){
        newPage();
        printf("           \033[1;31mNo items available. Please add first.\033[0m\n\n");
        printf("===========================================================\n\n");
        snprintf(currentLocation, 255, "%s", currentLocationHolder);
        return;
    }

    renameFolderMain();
    snprintf(currentLocation, 255, "%s", currentLocationHolder);
}

void removeProgram(){
    strcpy(currentLocationHolder, currentLocation);
    char choosenToBeRemove[X];
    if(!hasFiles(currentLocation)){
        newPage();
        printf("           \033[1;31mNo items available. Please add first.\033[0m\n\n");
        printf("===========================================================\n\n");
        strcpy(currentLocation, currentLocationHolder);
        return;
       
    }

    newPage();
    printf("  \033[1mPlease choose what to remove:\033[0m\n\n");
    directoryChoices(buffer);
    printf("\n===========================================================\n");
    
    printf("Enter your choice (0 to cancel): ");
    if(scanf("%d", &choice) == 1){
    } else {
        scanf(" %[^\n]", garbage);
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        strcpy(currentLocation, currentLocationHolder);
        return;
    }
    if (choice == 0){
        newPage();
        strcpy(currentLocation, currentLocationHolder);
        return;
    }
    
    strcpy(choosenToBeRemove, buffer[choice-1]);
    
    newPage();
    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("   [1] Yes\n");
    printf("   [2] No\n");
    printf("\n  \033[1;31mAre you sure you want to remove the file '%s'?\n  This action cannot be undone.\033[0m\n", buffer[choice -1]);
    
    printf("\n===========================================================\n");
    printf("Enter your choice (0 to cancel): ");
    if(scanf("%d", &choice) == 1){
        if (choice == 0){
            newPage();
        } else if (choice == 1){
            snprintf(fileLocationToBeRemove, X, "%s/%s", currentLocation, choosenToBeRemove);
            removeDirectory(fileLocationToBeRemove);
            
            newPage();
            printf("  \033[1;32m'%s' is successfully removed.\033[0m\n", choosenToBeRemove);
            printf("\n===========================================================\n\n");
            
        } else if ( choice == 2){
            newPage();
            printf("  \033[1;32mThe removal of program '%s' has been cancelled.\033[0m\n", choosenToBeRemove);
            printf("\n===========================================================\n\n");
        } else {
            scanf(" %[^\n]", garbage);
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            
        }
    } else {
        scanf(" %[^\n]", garbage);
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
    }
    strcpy(currentLocationHolder, currentLocation);
}

/* =================================================================================================================
                                            YEAR LEVEL CURRENT LOCATION
   ================================================================================================================= */

void viewYearLevel(){
    strcpy(currentLocationHolder, currentLocation);

    if (returned == 1){
        returned = 0;
    } else {
        newPage();
        printf("  \033[1mChoose the \033[34mprogram of year level\033[0m \033[1myou want to view:\033[0m\n\n");
        directoryChoices(buffer);
        enterYourChoice();
        if (choice == -1){
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            viewProgram();
            return;
        }
        snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

        if(!hasFiles(currentLocation)){
            newPage();
            printf("           \033[1;31mNo items available. Please add first.\033[0m\n\n");
            printf("===========================================================\n\n");
            strcpy(currentLocation, currentLocationHolder);
            returned = 1;
            viewProgram();
            return;
        }

        strcpy(backToYearLevel, currentLocation);
        newPage();
        printf("  \033[1mYear levels:\033[0m\n\n");
        directoryList();
        printf("===========================================================\n\n");
    }
    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("    [1] View sections\n");
    printf("    [2] Add section\n");
    printf("    [3] Rename section\n");
    printf("    [4] Remove section\n");
    printf("    [0] Back\n");
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
            break;
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            viewYearLevel();
            return;
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewProgram();
}

void addYearLevel(){
    strcpy(currentLocationHolder, currentLocation);

    newPage();
    printf("  \033[1mTo which \033[34mprogram\033[0m \033[1mshould you add the new \033[34myear level\033[0m\033[1m?\033[0m\n\n");
    directoryChoices(buffer);
    enterYourChoice();
    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);
    if(!directoryExists(currentLocation)){
        choice = -1;
    }
    strcpy(currentLocation, currentLocationHolder);
    if (choice == -1){
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        viewProgram();
        return;
    }
    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

    addFolder();
    
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewProgram();
}

void renameYearLevel(){
    strcpy(currentLocationHolder, currentLocation);
    newPage();
    renameFolder();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewProgram();  
}

void removeYearLevel(){
    strcpy(currentLocationHolder, currentLocation);
    
    newPage();
    printf("  \033[1mChoose the \033[34mprogram of year level\033[0m \033[1mto be removed:\033[0m\n\n");
    directoryChoices(buffer);
    printf("\n===========================================================\n");
    removeFolder();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewProgram();
}

/* =================================================================================================================
                                            SECTION CURRENT LOCATION
   ================================================================================================================= */

void viewSection(){ 
    strcpy(currentLocationHolder, currentLocation);
    if (returned == 1){
        returned = 0;
    } else {
        newPage();
        printf("  \033[1mChoose the \033[34myear level of section\033[0m \033[1myou want to view:\033[0m\n\n");
        directoryChoices(buffer);
        enterYourChoice();
        if (choice == -1){
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            viewYearLevel();
        }
        snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

        if(!hasFiles(currentLocation)){
            newPage();
            printf("           \033[1;31mNo items available. Please add first.\033[0m\n\n");
            printf("===========================================================\n\n");
            strcpy(currentLocation, currentLocationHolder);
            returned = 1;
            viewYearLevel();
        }
        snprintf(backToSection, X, "%s", currentLocation);
        newPage();
        printf("  \033[1mSections:\033[0m\n\n");
        directoryList();
        printf("===========================================================\n\n");
    }
    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("    [1] View courses\n");
    printf("    [2] Add course\n");
    printf("    [3] Rename course\n");
    printf("    [4] Remove course\n");
    printf("    [0] Back\n");
    enterYourChoice();
    
    switch(choice){
        case 1: viewCourse(); break;
        case 2: addCourse(); break;
        case 3: renameCourse(); break;
        case 4: removeCourse(); break;
        case 0:
            snprintf(currentLocation, X, "%s", backToYearLevel);
            newPage();
            returned = 1;
            viewYearLevel();
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            viewSection();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewYearLevel();
}

void addSection(){
    strcpy(currentLocationHolder, currentLocation);

    newPage();
    printf("  \033[1mTo which \033[34myear level\033[0m \033[1mshould you add the new \033[34msection\033[0m\033[1m?\033[0m\n\n");
    
    directoryChoices(buffer);
    enterYourChoice();

    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

    if(!directoryExists(currentLocation)){
        choice = -1;
    }

    strcpy(currentLocation, currentLocationHolder);

    if (choice == -1){
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        viewYearLevel();
    }

    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

    addFolder();
    
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewYearLevel();
}

void renameSection(){
    strcpy(currentLocationHolder, currentLocation);
    newPage();
    renameFolder();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewYearLevel();  
}

void removeSection(){
    strcpy(currentLocationHolder, currentLocation);
    
    newPage();
    printf("  \033[1mChoose the \033[1;34myear level of the section\033[0m to be removed:\033[0m\n\n");

    directoryChoices(buffer);

    printf("\n===========================================================\n");

    removeFolder();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewYearLevel();
}

/* =================================================================================================================
                                            COURSE CURRENT LOCATION
   ================================================================================================================= */

void viewCourse(){
    strcpy(currentLocationHolder, currentLocation);
    
    if (returned == 1){
        returned = 0;
    } else {
        newPage();

        printf("  \033[1mChoose the \033[34msection of course\033[0m \033[1myou want to view:\033[0m\n\n");

        directoryChoices(buffer);

        enterYourChoice();

        if (choice == -1){
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            viewSection();
        }

        snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);
        
        if(!hasFiles(currentLocation)){
            newPage();
            printf("           \033[1;31mNo items available. Please add first.\033[0m\n\n");
            printf("===========================================================\n\n");
            strcpy(currentLocation, currentLocationHolder);
            returned = 1;
            viewSection();
        }

        strcpy(backToCourse, currentLocation);
        newPage();
        printf("  \033[1mCourses:\033[0m\n\n");
        directoryList();
        printf("===========================================================\n\n");

    }

    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("    [1] Manage course\n");
    printf("    [0] Back\n");

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
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            viewCourse();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewSection();
}
    
void addCourse(){
    strcpy(currentLocationHolder, currentLocation);

    newPage();
    printf("  \033[1mTo which \033[34msection\033[0m \033[1mshould you add the new \033[34mcourse\033[0m\033[1m?\033[0m\n\n");

    directoryChoices(buffer);

    enterYourChoice();

    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

    if(!directoryExists(currentLocation)){
        choice = -1;
    }

    strcpy(currentLocation, currentLocationHolder);

    if (choice == -1){
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        viewSection();
        return;
    }

    snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

    addFolder();
    
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewSection();
}

void renameCourse(){
    strcpy(currentLocationHolder, currentLocation);
    newPage();
    renameFolder();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewSection();  
}

void removeCourse(){
    snprintf(currentLocationHolder, 255, "%s", currentLocation);
    
    newPage();

    printf("  \033[1mChoose the \033[1;34msection of course \033[0m\033[1mto be remove:\033[0m\n\n");

    directoryChoices(buffer);

    printf("\n===========================================================\n");

    removeFolder();

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewSection();
}

void manageCourse(){
    strcpy(currentLocationHolder, currentLocation);

    if (returned == 1){
        returned = 0;
    } else {
        newPage();

        printf("  \033[1mChoose the \033[34mcourse\033[0m \033[1myou want to manage:\033[0m\n\n");

        directoryChoices(buffer);

        enterYourChoice();

        if (choice == -1){
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
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
                snprintf(currentLocation, X, "%s/%s", temporary, assessments[i]);
                FILE *F1 = fopen(currentLocation, "a");
                fclose(F1);
            }

            snprintf(temporary, X, "%s/Students", courseLocation);
            mkdir(temporary);
            
            strcpy(currentLocation, currentLocationHolder);
        }

        newPage();
    }

    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("   [1] View assessments     [5] Add assessment to student\n");
    printf("   [2] Add assessment       [6] View students\n");
    printf("   [3] Edit assessment      [7] Add student\n");
    printf("   [4] Remove assessment    [8] View grading settings\n\n");
    printf("   [0] Back\n");

    enterYourChoice();
    
    switch(choice){
        case 1: viewAssessment(); break;
        case 2: addAssessment(); break;
        case 3: editAssessment(); break;
        case 4: removeAssessment(); break;
        case 5: addStudentAssessment(); break;
        case 6: viewStudent(); break;
        case 7: addStudent(); break;
        case 8: viewGradingSystemSettings(); break;
        case 0:
            newPage();
            returned = 1;
            viewCourse();
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            manageCourse();
    }
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewCourse();
}

/* =================================================================================================================
                                            ASSESSMENT CURRENT LOCATION
   ================================================================================================================= */

void viewAssessment(){
    strcpy(currentLocationHolder, currentLocation);
    
    char temporary[123];
    snprintf(currentLocation, X, "%s/Assessments", courseLocation);
    strcpy(temporary, currentLocation);

    
    char text[123];
    FILE *f[10];
    newPage();
    for (int i = 0; i < 6; i++){
        snprintf(temporary, X, "%s/%s", currentLocation, assessments[i]);
        f[i] = fopen(temporary, "r");
        printf("  \033[1m%s:\033[0m\n\n", assessments[i]);
        int j = 1;
        while (fgets(text, X, f[i]) != NULL) {
            
            text[strlen(text) - 1] = '\0';

            if(j % 2 == 0){
                printf("%s\n", text);
            } else {
                printf("    - %s: ", text);
            }
            j++;
        }
        fclose(f[i]);
        strcpy(temporary, currentLocation);
        printf("\n");
    }
    printf("===========================================================\n\n");
    
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    manageCourse();
}

void addAssessment() {
    strcpy(currentLocationHolder, currentLocation);

    char temporary[X];
    
    snprintf(currentLocation, X, "%s/Assessments", courseLocation);
   
    newPage();
    printf("  \033[1mChoose the \033[34mtype of assessment\033[0m \033[1myou want to add:\033[0m\n\n");

    directoryChoices(buffer);

    enterYourChoice();

    switch (choice) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            newPage();
            printf("--------------------- \033[3mAdding an item\033[0m ----------------------\n\n");
            printf("===========================================================\n");        
            printf("Enter the name of the assessment: ");
            scanf(" %[^\n]", assessmentName);
            printf("Enter the total score: ");
            if(scanf("%d", &assessmentOverscore) == 1){   
            } else {
                scanf(" %[^\n]", garbage);
                newPage();
                printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
                printf("===========================================================\n\n");
                returned = 1;
                strcpy(currentLocation, currentLocationHolder);
                manageCourse();
            }
            break;
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            manageCourse();
    }
    
    
    snprintf(currentLocation, X, "%s/%s", currentLocation, assessments[choice - 1]);
    FILE *f2 = fopen(currentLocation, "a");
    
    fprintf(f2, "%s\n%d\n", assessmentName, assessmentOverscore);
    
    fclose(f2);
    snprintf(currentLocation, X, "%s/%s", currentLocation, assessments[choice - 1]);

    returned = 1;
    newPage();
    printf("  \033[1;32m'%s' is successfully added at %s.\033[0m\n\n", assessmentName, assessments[choice -1]);
    printf("===========================================================\n\n");
    manageCourse();
}

void editAssessment() {
    strcpy(currentLocationHolder, currentLocation);

    char text[X][X];

    snprintf(currentLocation, X, "%s/Assessments", courseLocation);

    newPage();
    printf("  \033[1mChoose the type of assessment to edit:\033[0m\n\n");

    directoryChoices(buffer);

    enterYourChoice();

    switch(choice){
        case 1: case 2: case 3: case 4: case 5: case 6:
            break;
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            manageCourse();
    }

    snprintf(currentLocation, X, "%s/Assessments/%s", courseLocation, assessments[choice - 1]);
    FILE *F = fopen(currentLocation, "r");

    char text1[X][X];
    int lines1 = 0;

    while (fgets(text1[lines1], X, F) != NULL) {
        text1[lines1][strlen(text1[lines1]) - 1] = '\0';
        lines1++;
    }

    fclose(F);

    if (lines1 == 0) {
        newPage();
        printf("    \033[1;31mNo items in '%s'. Please add assessments first.\033[0m\n\n", assessments[choice - 1]);
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, X, "%s", currentLocationHolder);
        manageCourse();
    }

    snprintf(currentLocation, X, "%s/Assessments/%s", courseLocation, assessments[choice - 1]);

    FILE *f = fopen(currentLocation, "r");
 
    int lines = 0;

    while (fgets(text[lines], X, f) != NULL) {
        text[lines][strcspn(text[lines], "\n")] = '\0'; 
        lines++;
    }

    fclose(f);

    int choiceLine;

    newPage();

    printf(" \033[1mWhich assessment of '%s' would you like to edit? \033[0m\n\n", assessments[choice-1]);

    for (int i = 1; i <= lines; i++) {
        if (i % 2 != 0 ){
            printf("    [%d] %s: ", (i + 1)/2 , text[i-1]);
        } else {
            printf("%s\n", text[i-1]);
        }
    }
    
    printf("\n===========================================================\n");

    printf("Enter your choice: ");

    if (scanf("%d", &choiceLine) != 1 || choiceLine < 1 || choiceLine > lines / 2) {
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        strcpy(currentLocation, currentLocationHolder);
        returned = 1;
        manageCourse();
    }

    choiceLine = (choiceLine - 1) * 2;

    char newName[X];
    int newOverscore;

    newPage();
    printf("--------------------- \033[3mEditing an item\033[0m ---------------------\n\n");
    printf("===========================================================\n"); 

    printf("Enter the new name for the assessment: ");
    scanf(" %[^\n]", newName);

    printf("Enter the new total score: ");
    if (scanf("%d", &newOverscore) != 1) {
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        strcpy(currentLocation, currentLocationHolder);
        returned = 1;
        manageCourse();
    }

    snprintf(text[choiceLine], X, "%s", newName);
    snprintf(text[choiceLine + 1], X, "%d", newOverscore);

    f = fopen(currentLocation, "w");
   
    for (int i = 0; i < lines; i++) {
        fprintf(f, "%s\n", text[i]);
    }

    fclose(f);

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    newPage();
    printf("                \033[1;32mItem successfully edited.\033[0m\n\n");
    printf("===========================================================\n\n");
    manageCourse();
}

void removeAssessment() {
    strcpy(currentLocationHolder, currentLocation);
    
    char text[X][X];
    int lines = 0;  

    snprintf(currentLocation, X, "%s/Assessments", courseLocation);
    newPage();
    printf("  \033[1mChoose the type of assessment to remove:\033[0m\n\n");
    directoryChoices(buffer);
    enterYourChoice();

    switch(choice){
        case 1: case 2: case 3: case 4: case 5: case 6:
            break;
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            manageCourse();
    }


    snprintf(currentLocation, X, "%s/Assessments/%s", courseLocation, assessments[choice - 1]);
    FILE *F = fopen(currentLocation, "r");

    char text1[X][X];
    int lines1 = 0;

    while (fgets(text1[lines1], X, F) != NULL) {
        text1[lines1][strlen(text1[lines1]) - 1] = '\0';
        lines1++;
    }

    fclose(F);

    if (lines1 == 0) {
        newPage();
        printf("    \033[1;31mNo items in '%s'. Please add assessments first.\033[0m\n\n", assessments[choice - 1]);
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, X, "%s", currentLocationHolder);
        manageCourse();
    }


    snprintf(currentLocation, X, "%s/Assessments/%s", courseLocation, assessments[choice - 1]);

    FILE *f = fopen(currentLocation, "r");

    while (fgets(text[lines], X, f) != NULL) {
        text[lines][strcspn(text[lines], "\n")] = '\0';
        lines++;
    }

    fclose(f);

    int choiceLine;

    newPage();
    printf(" \033[1mWhich assessment of '%s' would you like to remove? \033[0m\n\n", assessments[choice-1]);
   
    for (int i = 0; i < lines; i += 2) {
        printf("    [%d] %s\n", i / 2 + 1, text[i]);
    }
    
    printf("\n===========================================================\n");

    printf("Enter your choice: ");   
    if (scanf("%d", &choiceLine) != 1 || choiceLine < 1 || choiceLine > lines / 2 ) {
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }

    choiceLine = (choiceLine - 1) * 2;

    for (int i = choiceLine; i < lines - 2; i++) {
        strcpy(text[i], text[i + 2]);
    }

    lines -= 2;

    f = fopen(currentLocation, "w");

    for (int i = 0; i < lines; i++) {
        fprintf(f, "%s\n", text[i]);
    }

    fclose(f);

    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    newPage();
    printf("                \033[1;32mItem successfully removed.\033[0m\n\n");
    printf("===========================================================\n\n");
    manageCourse();
}

/* =================================================================================================================
                                             STUDENT CURRENT LOCATION
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
            printf("          \033[1;31mNo students available. Please add first.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            manageCourse();
            return;
        }

        snprintf(currentLocation, X, "%s/Students", courseLocation);
        newPage();
        printf("  \033[1mStudents:\033[0m\n\n");
        directoryList();
        printf("===========================================================\n\n");

    }
    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("    [1] View student records\n");
    printf("    [2] View students initial grade\n");
    printf("    [0] Back\n");

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
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
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
    printf("  \033[1mEnter the student info:\033[1m\033[0m\n\n");

    char name[X][X];

    printf("    Last name: ");
    scanf(" %[^\n]", name[0]);
    printf("    First name: ");
    scanf(" %[^\n]", name[1]);
    printf("    Middle name: ");
    scanf(" %[^\n]", name[2]);
    printf("    Age: ");
    scanf(" %[^\n]", name[3]);
    printf("    Birthday: ");
    scanf(" %[^\n]", name[4]);
    printf("    Address: ");
    scanf(" %[^\n]", name[5]);
    printf("    Contact no.: ");
    scanf(" %[^\n]", name[6]);
    printf("    Email: ");
    scanf(" %[^\n]", name[7]);
    
    char temporary[X];
    snprintf(temporary, X, "%s/Students/%s, %s %s", courseLocation, name[0], name[1], name[2]);
    mkdir(temporary);

    char temporary1[X];
    snprintf(temporary1, X, "%s/Student Info", temporary);
    FILE *F = fopen(temporary1, "w");
    for (int i = 0; i < 8; i++){
        fprintf(F, "%s\n",  name[i]);
    }

    fclose(F);

    snprintf(temporary1, X, "%s/Student Percentage", temporary);
    FILE *F2 = fopen(temporary1, "a");
    fclose(F2);

    snprintf(temporary1, X, "%s/Student Assessments", temporary);
    mkdir(temporary1);
    
    for (int i = 0; i < 6; i++) {
        snprintf(currentLocation, X, "%s/%s", temporary1, assessments[i]);
        FILE *f1 = fopen(currentLocation, "a");
        fclose(f1);
    }
    
    strcpy(currentLocation,currentLocationHolder);
    returned = 1;
    newPage();
    manageCourse();
}

void viewStudentRecord(){
    strcpy(currentLocationHolder, currentLocation);
    
    if (returned == 1){
        returned = 0;
    } else {
        newPage();
        printf("  \033[1mChoose the \033[34mstudent\033[0m \033[1myou want to view:\033[0m\n\n");

        directoryChoices(buffer);

        enterYourChoice();

        if (choice == -1){
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            strcpy(currentLocation,currentLocationHolder);
            viewStudent();
            return;
        }

        snprintf(currentLocation, X, "%s/%s", currentLocation, buffer[choice-1]);

        if(!hasFiles(currentLocation)){
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            strcpy(currentLocation,currentLocationHolder);
            viewStudent();
            return;
        }
        newPage();
    }

    printf("  \033[1mPlease choose an option:\033[0m\n\n");
    printf("    [1] View student info\n");
    printf("    [2] View student assessments\n");
    printf("    [3] Remove student assessment\n");
    printf("    [4] View student grade percentage\n");
    printf("    [0] Back\n");
    
    enterYourChoice();
    
    switch(choice){
        case 1: viewStudentInfo(); break;
        case 2: viewStudentAssessment(); break;
        case 3: removeStudentAssessment(); break;
        case 4: viewStudentGradeSummary(); break;
        case 0:
            returned = 1;
            newPage();
            strcpy(currentLocation, currentLocationHolder);
            viewStudent();
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
            returned = 1;
            strcpy(currentLocation, currentLocationHolder);
            viewStudentRecord();
    }

    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewStudent();
}

void viewStudentInfo(){
    strcpy(currentLocationHolder,currentLocation);

    char text[X];
    
    newPage();
    printf("  \033[1mStudent info:\033[0m\n\n");
    
    char container[10][X] = {"Last name: ", "First name: ", "Middle name: ", "Age: ", "Birthday: ", "Address: ", "Contact no.: ", "Email: "};

    snprintf(currentLocation, X, "%s/Student Info", currentLocation);

    FILE *F = fopen(currentLocation, "r");

    int i = 0;

    while(fgets(text, X, F) != NULL){
        printf("    %s%s", container[i], text);
        i++;
    }
    printf("\n===========================================================\n\n");

    fclose(F);

    strcpy(currentLocation,currentLocationHolder);
    returned = 1;
    viewStudentRecord();
}

void viewStudentAssessment(){
    strcpy(currentLocationHolder, currentLocation);
    
    char temporary[X];
    char text[X];
    
    snprintf(temporary, X, "%s/Student Assessments", currentLocation);
    strcpy(currentLocation, temporary);
    
    FILE *f[10];

    newPage();

    for (int i = 0; i < 6; i++){
        snprintf(temporary, X, "%s/%s", currentLocation, assessments[i]);

        f[i] = fopen(temporary, "r");

        printf("  \033[1m%s:\033[0m\n\n", assessments[i]);

        int j = 1;

        while (fgets(text, X, f[i]) != NULL) {
            
            text[strlen(text) - 1] = '\0';

            if(j % 2 == 0){
                printf("%s\n", text);
            } else {
                printf("    - %s: ", text);
            }

            j++;
        }

        fclose(f[i]);
        strcpy(temporary, currentLocation);
        printf("\n");
    }

    printf("===========================================================\n\n");
    
    strcpy(currentLocation, currentLocationHolder);
    returned = 1;
    viewStudentRecord();
}

void addStudentAssessment() {
    strcpy(currentLocationHolder, currentLocation);

    char temporary5[X];

    snprintf(temporary5, X, "%s/Students", courseLocation);
    
    if(!hasFiles(temporary5)){
        newPage();
        printf("          \033[1;31mNo students available. Please add first.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }

    newPage();

    snprintf(currentLocation, X, "%s/Students", courseLocation);
    printf("  \033[1mChoose the student to add an assessment for:\033[0m\n\n");
    
    directoryChoices(buffer);    
    enterYourChoice();

    char studentLocation[X];

    snprintf(studentLocation, X, "%s/%s", currentLocation, buffer[choice - 1]);

    if(!hasFiles(studentLocation)){
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        strcpy(currentLocation, currentLocationHolder);
        manageCourse();
    }

    newPage();
    printf("  \033[1mChoose the type of assessment to add:\033[0m\n\n");

    for (int i = 0; i < 6; i++) {
        printf("    [%d] %s\n", i + 1, assessments[i]);
    }

    enterYourChoice();

    if (choice < 1 || choice > 6) {
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, X, "%s", currentLocationHolder);
        manageCourse();
    }

    snprintf(currentLocation, X, "%s/Assessments/%s", courseLocation, assessments[choice - 1]);
    FILE *F = fopen(currentLocation, "r");

    char text[X][X];
    int lines = 0;

    while (fgets(text[lines], X, F) != NULL) {
        text[lines][strlen(text[lines]) - 1] = '\0';
        lines++;
    }

    fclose(F);

    if (lines == 0) {
        newPage();
        printf("    \033[1;31mNo items in '%s'. Please add assessments first.\033[0m\n\n", assessments[choice - 1]);
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, X, "%s", currentLocationHolder);
        manageCourse();
    }

    newPage();
    printf("  \033[1mWhich '%s' would you like to add a score for?\033[0m\n\n", assessments[choice - 1]);

    for (int i = 0; i < lines; i++) {
        if (i % 2 == 0) {
            printf("    [%d] %s\n", i / 2 + 1, text[i]);
        }
    }

    int choiceOfList = choice;

    enterYourChoice();

    if (choice < 1 || choice > lines / 2) {
        newPage();
        printf("             \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, X, "%s", currentLocationHolder);
        manageCourse();
        return;
    }

    char studentAssessment[X];
    int studentScore;

    snprintf(studentAssessment, X, "%s", text[(choice - 1) * 2]);
    printf("===========================================================\n");
    printf("Enter the student score for '%s': ", studentAssessment);

    int totalScore1 = atoi(text[(choice - 1) * 2 + 1]);

    if (scanf("%d", &studentScore) != 1 || studentScore < 0 || studentScore > totalScore1) {
        newPage();
        printf("          \033[1;31mInvalid score. Must be between 0 and %d.\033[0m\n\n", totalScore1);
        printf("===========================================================\n\n");
        snprintf(currentLocation, X, "%s", currentLocationHolder);
        returned = 1;
        manageCourse();
    }

    snprintf(currentLocation, X, "%s/Student Assessments/%s", studentLocation, assessments[choiceOfList - 1]);
    
    FILE *F1 = fopen(currentLocation, "r");

    char existingAssessment[X];
    int alreadyAdded = 0;

    while (fgets(existingAssessment, X, F1) != NULL) {
        existingAssessment[strlen(existingAssessment) - 1] = '\0';
        if (strcmp(existingAssessment, studentAssessment) == 0) {
            alreadyAdded = 1;
            break;
        }
    }

    fclose(F1);

    if (alreadyAdded == 1) {
        newPage();
        printf("      \033[1;31mThe item you will add has already been added.\033[0m\n\n");
        printf("===========================================================\n\n");
        strcpy(currentLocation, currentLocationHolder);
        returned = 1;
        manageCourse();
    }

    FILE *F2 = fopen(currentLocation, "a");

    fprintf(F2, "%s\n%d\n", studentAssessment, studentScore);
    fclose(F2);

    newPage();
    printf("   \033[1;32mAssessment '%s' added successfully!\033[0m\n\n", studentAssessment);
    printf("===========================================================\n\n");

    snprintf(currentLocation, X, "%s", currentLocationHolder);
    returned = 1;
    manageCourse();
}

void viewStudentGradeSummary() {
    strcpy(currentLocationHolder, currentLocation);
    
    int sumOfEachAssessment[6] = {0}; 
    int percentage[6] = {0}, lines = 0;
    float studentPercentage[6] = {0};
    float assessmentPercentage[6] = {0};
    int sumStudentAssessment;
    snprintf(currentLocation, X, "%s/Grading System Settings", courseLocation);
    FILE *Fpercentage = fopen(currentLocation, "a+");
    
    for (int i = 0; i < 6; i++) {
        fscanf(Fpercentage, "%d", &percentage[i]);
    }


    int totalScore[6] = {0};

    for (int i = 0; i < 6; i++) {
        snprintf(currentLocation, X, "%s/Assessments/%s", courseLocation, assessments[i]);
        FILE *Fassessment = fopen(currentLocation, "r");
        
        char line[X];
        int lineIndex = 0;

        while (fgets(line, X, Fassessment) != NULL) {
            line[strlen(line) - 1] = '\0'; 

            if (lineIndex % 2 == 1) {
                totalScore[i] += atoi(line);
            }
            lineIndex++;
        }
        fclose(Fassessment);
    }
    

    for (int j = 0; j < 6; j++) {
        sumStudentAssessment = 0;
        int lines2 = 0;           

        snprintf(currentLocation, X, "%s/Student Assessments/%s", currentLocationHolder, assessments[j]);
        FILE *Fassessment = fopen(currentLocation, "r");

        if (Fassessment != NULL) {
            char text[X][X];
            while (fgets(text[lines2], X, Fassessment) != NULL) {
                text[lines2][strlen(text[lines2]) - 1] = '\0'; 
                if (lines2 % 2 == 1) {
                    sumStudentAssessment += atoi(text[lines2]);
                }
                lines2++;
            }
            fclose(Fassessment);
        }

        sumOfEachAssessment[j] = sumStudentAssessment;

        if (totalScore[j] > 0) { 
            studentPercentage[j] = ((float)sumOfEachAssessment[j] / (float)totalScore[j]) * percentage[j];
            assessmentPercentage[j] = ((float)sumOfEachAssessment[j] / (float)totalScore[j]) * 100;
        } else {
            studentPercentage[j] = 0;
        }
    }

    newPage();
    for (int j = 0; j < 6; j++) {
        printf("     - \033[0;34m%s\033[0m score: %d/%d\n", assessments[j], sumOfEachAssessment[j], totalScore[j]);
        printf("       Grade percentage: %.2f%%\n\n", assessmentPercentage[j]);
    }

    float initialGrade = 0;
    for (int j = 0; j < 6; j++) {
            initialGrade += studentPercentage[j];
    }

    printf("     - \033[0;34mInitial grade\033[0m: %.2f%%\n", initialGrade);
    printf("\n===========================================================\n\n");
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewStudentRecord();
}

void viewStudentGradeCourse() {
    strcpy(currentLocationHolder, currentLocation);
    
    int sumOfEachAssessment[6] = {0};
    int percentage[6] = {0}, lines = 0;
    float studentPercentage[6] = {0};
    int sumStudentAssessment;

    snprintf(currentLocation, X, "%s/Grading System Settings", courseLocation);
    FILE *Fpercentage = fopen(currentLocation, "r");
   
    for (int i = 0; i < 6; i++) {
        fscanf(Fpercentage, "%d", &percentage[i]);
    }
    
    snprintf(currentLocation, X, "%s/Students", courseLocation);

    DIR *temporary = opendir(currentLocation);

    struct dirent *entry;
    char studentNames[X][X];
    int studentCount = 0;

    while ((entry = readdir(temporary)) != NULL) {
        if (entry->d_name[0] != '.') {
            snprintf(studentNames[studentCount], X, "%s", entry->d_name);
            studentNames[studentCount][sizeof(studentNames[studentCount]) - 1] = '\0';
            studentCount++;
        }
    }

    int totalScore[6] = {0};

    for (int i = 0; i < 6; i++) {
        snprintf(currentLocation, X, "%s/Assessments/%s", courseLocation, assessments[i]);
        FILE *Fassessment = fopen(currentLocation, "r");
        
        char line[X];
        int lineIndex = 0;

        while (fgets(line, X, Fassessment) != NULL) {
            line[strlen(line) - 1] = '\0';

            if (lineIndex % 2 == 1) {
                totalScore[i] += atoi(line);
            }
            lineIndex++;
        }
        fclose(Fassessment);
    }
    
    closedir(temporary);
    newPage();
    printf("  \033[1mStudent initial grade:\033[0m\n\n");
    for (int i = 0; i < studentCount; i++) {

        for (int j = 0; j < 6; j++) {
            sumStudentAssessment = 0;
            int lines2 = 0;           

            snprintf(currentLocation, X, "%s/Students/%s/Student Assessments/%s", courseLocation, studentNames[i], assessments[j]);
            FILE *Fassessment = fopen(currentLocation, "r");

            
            char text[X][X];
            while (fgets(text[lines2], X, Fassessment) != NULL) {
                text[lines2][strlen(text[lines2]) - 1] = '\0'; // Remove newline
                if (lines2 % 2 == 1) { // Only process score lines
                    sumStudentAssessment += atoi(text[lines2]);
                }
                lines2++;
            }

            fclose(Fassessment);
        
            sumOfEachAssessment[j] = sumStudentAssessment;

            if (totalScore[j] > 0) { 
                studentPercentage[j] = ((float)sumOfEachAssessment[j] / (float)totalScore[j]) * percentage[j];
            } else {
                studentPercentage[j] = 0;
            }
            
        }

        float initialGrade = 0;
        for (int j = 0; j < 6; j++) {
            initialGrade += studentPercentage[j];
        }
        
        printf("    - %s - %.2f%%\n", studentNames[i], initialGrade);
    }
    
    printf("\n===========================================================\n\n");
    returned = 1;
    strcpy(currentLocation, currentLocationHolder);
    viewStudent();
}

void removeStudentAssessment() {
    strcpy(currentLocationHolder, currentLocation);

    newPage();
    printf("  \033[1mChoose the type of assessment to remove:\033[0m\n\n");
    for (int i = 0; i < 6; i++) {
        printf("    [%d] %s\n", i + 1, assessments[i]);
    }

    enterYourChoice();

    if (choice < 1 || choice > 6) {
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, X, "%s", currentLocationHolder);
        viewStudentRecord();
        return;
    }

    char temporary[X];
    snprintf(temporary, X, "%s/Student Assessments", currentLocation);
    mkdir(temporary);

    if (!hasFiles(temporary)) {
        newPage();
        printf("              \033[1;31mNo assessment files found.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, 255, "%s", currentLocationHolder);
        viewStudentRecord();
    }

    char temporary1[X];
    
    snprintf(currentLocation, X, "%s/%s", temporary, assessments[choice - 1]);
    strcpy(temporary1, assessments[choice - 1]);
    FILE *F = fopen(currentLocation, "r");

    char text[X][X];
    int lines = 0;

    while (fgets(text[lines], X, F) != NULL) {
        text[lines][strlen(text[lines]) - 1] = '\0';
        lines++;
    }

    fclose(F);

    while (fgets(text[lines], X, F) != NULL) {
        text[lines][strlen(text[lines]) - 1] = '\0';
        lines++;
    }

    fclose(F);

    if (lines < 1){
        newPage();
        printf("        \033[1;31mThere's no item available. Please add first.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, 255, "%s", currentLocationHolder);
        viewStudentRecord();
    }

    newPage();
    printf("\n  \033[1mWhich assessment would you like to remove?\033[0m \n");
    printf("\n    \033[1mExisting assessments for '%s':\n\n\033[0m", assessments[choice - 1]);
    for (int i = 0; i < lines; i += 2) { 
        printf("      [%d] %s\n", (i / 2) + 1, text[i]);
    }
    
    enterYourChoice();

    if (choice < 1 || choice > lines / 2) {
        newPage();
        printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, 255, "%s", currentLocationHolder);
        viewCourse();
    }

    FILE *F1 = fopen(currentLocation, "w");

    for (int i = 0; i < lines; i++) {
        if (i != (choice - 1) * 2 && i != (choice - 1) * 2 + 1) { 
            fprintf(F1, "%s\n", text[i]);
        }
    }

    fclose(F1);

    newPage();
    printf("                \033[1;32mAssessment removed successfully!\033[0m\n");
    printf("===========================================================\n\n");
    snprintf(currentLocation, 255, "%s", currentLocationHolder);
    returned = 1;
    viewStudentRecord();
}

/* =================================================================================================================
                                      GRADING SYSTEM SETTINGS CURRENT LOCATION
   ================================================================================================================= */

void viewGradingSystemSettings() {
    strcpy(currentLocationHolder, currentLocation);

    if(returned == 1){
        returned = 0;
    } else {
        newPage();
        snprintf(currentLocation, 200, "%s/Grading System Settings", courseLocation);

        FILE *F1 = fopen(currentLocation, "r");

        if (F1 == NULL) {
            FILE *F2 = fopen(currentLocation, "a");
            fprintf(F2, "10\n10\n10\n10\n30\n30\n");
            fclose(F2);
        } else {
            fclose(F1);
        }

        int percentages[6];
        int lines = 0;
        char text[X][X];  

        FILE *F3 = fopen(currentLocation, "r");
        
        for (int i = 0; fgets(text[lines], X, F3) != NULL ; i++) {
            text[lines][strlen(text[lines]) - 1] = '\0';
            lines++;
        }

        for (int i = 0; i < lines; i++) {
            percentages[i] = atoi(text[i]);
        }

        fclose(F3);

        printf("              \033[1;34mViewing Grading System Settings\033[0m\n\n");
        printf("===========================================================\n\n");

        printf("  \033[1mCurrent Grading System Settings:\033[0m\n\n");

        for (int i = 0; i < 6; i++) {
            printf("    - %s: %d%%\n", assessments[i], percentages[i]);
        }
        printf("\n===========================================================\n\n");
    }
 
    printf("  \033[1mChoose an option: \033[0m\n\n");
    printf("    [1] Edit Grading System Settings\n");
    printf("    [0] Back\n");
    
    enterYourChoice();

    switch(choice){
        case 1:
            editGradingSystemSettings();
            break;
        case 0:
            returned = 1;
            newPage();
            manageCourse();
        default:
            newPage();
            printf("              \033[1;31mInvalid input. Please try again.\033[0m\n\n");
            printf("===========================================================\n\n");
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
    
    printf("              \033[1;34mEditing Grading System Settings\033[0m\n\n");
    printf("===========================================================\n\n");
    printf("  \033[1mEnter the percentage for each assessment type:\033[0m\n\n");

    int totalPercentage = 0;
    
    for (int i = 0; i < 6; i++) {
        printf("    %s: ", assessments[i]);
        if (scanf("%d", &percentages[i]) != 1) {
            if (percentages[i] < 0 || percentages[i] > 100){
                scanf(" %[^\n]", garbage );
               break;
            }
        }
        totalPercentage += percentages[i];
    }

    if (totalPercentage != 100) {
        newPage();
        printf("     \033[1;31mInvalid input. The total percentage must be equal to 100%%.\033[0m\n\n");
        printf("===========================================================\n\n");
        returned = 1;
        snprintf(currentLocation, 255, "%s", currentLocationHolder);
        viewGradingSystemSettings();
    } 

    snprintf(currentLocation, 255, "%s/Grading System Settings", courseLocation);

    FILE *F1 = fopen(currentLocation, "w");

    for (int i = 0; i < 6; i++) {
        fprintf(F1, "%d\n", percentages[i]);
    }
    
    fclose(F1);

    strcpy(currentLocation, currentLocationHolder);

    viewGradingSystemSettings();
}
