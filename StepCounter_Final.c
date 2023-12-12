#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "FitnessDataStruct.h"

// Struct moved to header file

// Define any additional variables here
// Global variables for filename and FITNESS_DATA array


// This is your helper function. Do not change it in any way.
// Inputs: character array representing a row; the delimiter character
// Ouputs: date character array; time character array; steps character array
void tokeniseRecord(const char *input, const char *delimiter,
                    char *date, char *time, char *steps) {
    // Create a copy of the input string as strtok modifies the string
    char *inputCopy = strdup(input);
    
    // Tokenize the copied string
    char *token = strtok(inputCopy, delimiter);
    if (token != NULL) {        strcpy(date, token);
    }
    
    token = strtok(NULL, delimiter);
    if (token != NULL) {
        strcpy(time, token);
    }
    
    token = strtok(NULL, delimiter);
    if (token != NULL) {
        strcpy(steps, token);
    }
    
    // Free the duplicated string
    free(inputCopy);
   }


//this function was migrated from task 1 but edited so it returns the length of the Array as-
//there is no "get length" function within c for Array of custom classes
FITNESS_DATA* LoadFile(char *fileName, int *fileLength){
    int buffer_size = 100;
    char line_buffer[buffer_size];
    int count = 0;
    
    FILE *file = fopen(fileName, "r"); 
    if (file == NULL){
        perror("Error: when file name invalid");
        return NULL;
    }
    else{
        while(fgets(line_buffer, buffer_size, file) != NULL){
            count++;
        }
    }
    rewind(file);  //code is from suggestion from chatgbt
    FITNESS_DATA *Array = (FITNESS_DATA *)malloc(count * sizeof(FITNESS_DATA));
    count = 0;

    if (file == NULL){
        perror("Error: specified file is empty");
        return NULL;
    }
    else{
        char date[11];
        char time[6];
        char steps[10];
        int stepsint;

        while(fgets(line_buffer, buffer_size, file) != NULL){
            tokeniseRecord(line_buffer, ",", date, time, steps);
            stepsint = atoi(steps);
            strcpy(Array[count].date, date);
            strcpy(Array[count].time, time);
            Array[count].steps = stepsint;
            count++;

        }
    }

    //printf("\nNumber of records in file: %d\n", count);
    //for (int i = 0; i < count; i++) {
    //    printf("%s/%s/%d\n",Array[i].date, Array[i].time, Array[i].steps);
    //} error checking to make sure that the array which contains the contents of the file is loaded
    fclose(file);
    *fileLength = count;
    return Array;
}

char displayMenu() {
    printf("Menu:\n");
    printf("A: Specify the filename to be imported\n");
    printf("B: Display the total number of records in the file\n");
    printf("C: Find the date and time of the timeslot with the fewest steps\n");
    printf("D: Find the date and time of the timeslot with the largest number of steps\n");
    printf("E: Find the mean step count of all the records in the file\n");
    printf("F: Find the longest continuous period where the step count is above 500 steps\n");
    printf("Q: Exit\n");

    printf("Enter your choice: ");
    char choice;
    scanf(" %c", &choice); 

    return choice;
}




// Complete the main function
int main() {
    
    FITNESS_DATA* currentFile;
    char choice;
    int length = 0;
    int tempValue;
    int loop = 0;
    //tempValue is a general use variable that is used within all of the cases.

    do
    {
      choice = displayMenu();
      char fileName[30];
    
        switch (choice) {
            case 'A':
            printf("Input filename: ");
            scanf("%s", fileName);
            //the validation method for the file below was suggested by chatGPT
            if (access(fileName, F_OK) != -1){
                currentFile = LoadFile(fileName, &length);
                printf("File successfully loaded.\n");
            }
            else {
                printf("Error: could not open file.\n");
                return 1;
            }
                
            break;

            case 'B':
            printf("Total records: %d\n", length);
            break;

            case 'C':
            tempValue = 0;
            for (int i = 0; i < length; i++)
            {
                if(currentFile[i].steps < currentFile[tempValue].steps){
                    tempValue = i;
                }
            }
            printf("Fewest Steps: %s %s\n",currentFile[tempValue].date, currentFile[tempValue].time);
            break;

            case 'D': 
            tempValue = 0;
            for (int i = 0; i < length; i++)
            {
                if(currentFile[i].steps > currentFile[tempValue].steps){
                    tempValue = i;
                }
            }
            printf("Largest Steps:%s %s\n",currentFile[tempValue].date, currentFile[tempValue].time);        
            break;

            case 'E':
            tempValue = 0;
            for (int i = 0; i < length; i++)
            {
                tempValue += currentFile[i].steps;
                
            }
            tempValue = round(tempValue/length);
            printf("Mean Step Value: %d\n", tempValue);
            break;

            case 'F':
            tempValue = 0;
            int periodStart;
            int periodEnd;
            int currentLongest = 0;

            for (int i = 0; i < length; i++)
            {
                //this checks each line from the csv file for how many steps there are
                if(currentFile[i].steps > 500){
                    tempValue++; //if it is over 500 it will begin to start counting 
                }   
                else{
                    if (tempValue > currentLongest){         
                        currentLongest = tempValue;
                        periodEnd = i-1;
                        periodStart = periodEnd - tempValue + 1;
                        //it then checks the next record and if it stops having steps higher than 500
                        //it saves a new longest length of over 500 and where it begins and ends
                    }
                    tempValue = 0; //then as there is no longer records over 500 it will stop counting 
                }
            }
            
            
            printf("Longest period start: %s %s\n", currentFile[periodStart].date, currentFile[periodStart].time);
            printf("Longest period end: %s %s\n", currentFile[periodEnd].date, currentFile[periodEnd].time);
            
            break;

            case 'Q':         
            break;

            default:
            printf("Invalid choice. Try again.\n");
            break;
        }

    } while (choice != 'Q');
    
    
}

