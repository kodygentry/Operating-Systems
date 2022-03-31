#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char state[10][30]; 
    char temp[30]; 
    int count = 0;
    
    while(count < 10 && scanf("%s", state[count]) != EOF) {
        count++;
    }
    
    for(int i = 0; i < count; i++){
        for(int j = 0; j < count-1-i; j++){
            if(strcmp(state[j], state[j+1]) > 0){
                strcpy(temp, state[j]);
                strcpy(state[j], state[j+1]);
                strcpy(state[j+1], temp);
            }
        }
    }

    for(int i = 0; i < count; i++){
        printf("%s\n", state[i]);
    }
    printf("\n");
    return 0;
}