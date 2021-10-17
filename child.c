/*Alaa Zuhd - 1180865
  Rawan Yassin - 1182224
  */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void start_generating(int );
void new_round();
void inform_parent();

char file_name[20];            //storing the name of the file for this specific chil


int main(int argc, char *argv[])
{

    //Checking that the received arguments are three
    if (argc != 3) {
        printf("\033[0;31m"); // set the color to red 
        printf("There should be 3 argc!\n");
        printf("\033[0m");// reset the color to the default       
        exit(-1);
    }
    //storing the file_name received as an argument
    strcpy(file_name,argv[1]);
    //setting the seed for random generation
    srand((unsigned int)getpid());
    //making the child process sensitive to SIGUSR1 
    if (sigset(SIGUSR1, start_generating) == SIG_ERR ) {
        printf("\033[0;31m"); // set the color to red 
        perror("Sigset can not set SIGUSR1");
        printf("\033[0m");// reset the color to the default
        exit(-1);
    } 
    //first round
    if(strcmp("Start",argv[2]) == 0){  
        start_generating(10);      
    }
    while(1);
    return (0);
}
//if the child process receives a SIGUSR1 it starts a new round of generating  numbers, and then when generating is done, it informs the parent.
void start_generating(int sig_num){
    new_round();
    inform_parent();
}
//new_round function, geneates 10 random numbers and stores them in the file. 
void new_round(){
    int i;
    int *random_numbers = malloc(10 * sizeof(int)); //storing the 10 random number before writing them to the file
    for( i = 0 ; i < 10 ; i++ ) {
     *(random_numbers+i) = rand() % 100 + 1;
    }
    //opening and writing in the file
    FILE *fptr;
    fptr = fopen(file_name,"w");
    if (fptr == NULL){
        perror("fopen error");
        exit(-1);
    }
    for( i = 0 ; i < 10; i++ ){
        fprintf(fptr,"%d\n",*(random_numbers+i));
    }
    if(fclose(fptr)){
        exit(-1);
    }
    free(random_numbers);
}
//Informing the parent that the 10 random numbers are generated successfully.
//If P1 is finished, it sends a SIGINT to the parent, else P2 sends SIGQUIT 
void inform_parent(){
    if(strcmp(file_name,"child1.txt") == 0)  
        kill(getppid(),SIGINT);  
    else
        kill(getppid(),SIGQUIT); 

}
