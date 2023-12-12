#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<unistd.h>
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

FITNESS_DATA* LoadFile(char *fileName, int *fileLength){
    int buffer_size = 100;
    char line_buffer[buffer_size];
    int count = 0;
    
    FILE *file = fopen("FitnessData_2023.csv", "r"); //change from filename
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

    do
    {
      choice = displayMenu();
      char fileName[30];

        switch (choice) {
            case 'A':
            printf("Input filename:");
            scanf("%s", fileName);
            if (access(fileName, F_OK) != -1){
                currentFile = LoadFile(fileName, &length);
                printf("File successfully loaded .\n");
            }
            else {
                printf("Error: Could not find or open the file.\n");
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
            printf("Fewest Steps: %s %s\n",currentFile[tempValue].date, currentFile[tempValue].time);        
            break;

            case 'E':
            tempValue = 0;
            for (int i = 0; i < length; i++)
            {
                tempValue += currentFile[i].steps;
                
            }
            tempValue = round(tempValue/length);
            printf("Mean Step Value: %d", tempValue);
            break;

            case 'F':
            tempValue = 0;
            int periodStart;
            int periodEnd;
            int currentLongest = 0;

            for (int i = 0; i < length; i++)
            {
                if(currentFile[i].steps > 500){
                    tempValue++;
                }   
                else{
                    if (tempValue > currentLongest){         
                        currentLongest = tempValue;
                        periodEnd = i-1;
                        periodStart = periodEnd - tempValue + 1;
                    }
                    tempValue = 0;
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









