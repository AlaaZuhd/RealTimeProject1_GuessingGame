#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void calculate_score();

int score1 = 0;             //the score for player1 in each seperate round 
int score2 = 0;             //the score for player2 in each seperate round     
int f_des[2][2];            //file descripters of the pipes for communicating with the parent
//static char message[BUFSIZ]; 

int main(int argc, char *argv[])
{
    f_des[0][0] = atoi(argv[1]); // read_pipe_number1
    f_des[0][1] = atoi(argv[2]); // write_pipe_number1
    f_des[1][0] = atoi(argv[3]); // read_pipe_number2
    f_des[1][1] = atoi(argv[4]); // write_pipe_number2
    //making sure the received arguments are five
    if (argc != 5) {
        printf("\033[0;31m"); // set the color to red 
        printf("There should be 5 argc!\n");
        printf("\033[0m");// reset the color to the default
        exit(-1);
    }
    close(f_des[0][1]); // close the write end for pipe one
    close(f_des[1][0]); // close the read end for pipe two

    while(1){
        char message[41]; 
        if (read(f_des[0][0], message, sizeof(message)) != -1 ) {
            calculate_score(message);
            char result[20];
            sprintf(result, "%d-%d", score1, score2);
            if ( write(f_des[1][1], result, sizeof(result)) != -1 ) {

            }
            else {
                    perror("Write");
                    printf("\033[0m");// reset the color to the default
                    exit(-1);
            }    
        } else {
            printf("\033[0;31m"); // set the color to red 
            perror("Read");
            printf("\033[0m");// reset the color to the default
            exit(-1);
        }
    }
    //Closing the used pipes ends
    close(f_des[0][0]);
    close(f_des[1][1]);


    return (0);
}


void calculate_score(char* message){
    //extracting the name of the files from the received message: 
    char *file1_name = strtok(message,"-");
    char *file2_name = strtok(NULL,"\n");
    score1 =0;
    score2 =0; 
    char read[2][50]; 
    //opening the two files and reading the 10 generated numbers for comparing
    FILE *fptr1, *fptr2;
    fptr1 = fopen(file1_name,"r"); 
    fptr2 = fopen(file2_name,"r"); 

    if (fptr1 == NULL || fptr2 == NULL)
    {
        printf("\033[0;31m"); // set the color to red 
        perror("Error while opening the file.\n");
        printf("\033[0m");// reset the color to the default
        exit(-1);
    }
    printf("P1's Random Numbers \t P2's Random Numbers:\n");
    while (fgets(read[0], sizeof(read), fptr1) && fgets(read[1], sizeof(read), fptr2)) {
        printf("%d\t \t \t %d\n", atoi(read[0]), atoi(read[1]));
        if(atoi(read[0]) > atoi(read[1])){
            score1 ++;
        }
        else if (atoi(read[1]) > atoi(read[0])){
            score2 ++; 
        }
    }
    printf("Score1: %d \t Score2: %d\n", score1, score2);
    
    if(fclose(fptr1)){
        exit(-1);
    }
    if(fclose(fptr2)){
        exit(-1);
    }
    //removing the files after reading.
    remove(file1_name);
    remove(file2_name);
}