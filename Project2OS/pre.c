#include <stdio.h>
#include <string.h>

typedef struct Data {
    char state[10];
    int population;
} Data;

int main(void) {
    Data data[10];
    int count = 0; 
    char state_input[10]; 
    int population_input; 

    while (count < 10 && scanf("%s %d", &state_input, &population_input) != EOF) {
        strcpy(data[count].state, state_input);  
        data[count++].population = population_input; 
    }

    for(int i = 0; i < count; i++) {
        if(data[i].population >= 10){
            printf("\n%s", data[i].state);
        }
    }
    printf("\n");
    return 0;
}