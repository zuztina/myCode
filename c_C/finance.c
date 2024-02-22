/*The task is to create a program that will filter financial data to "look better".

We assume that we have information about the state of the account over time. It is necessary to select a suitable section for the presentation from this data. 
In the presentation, we want to document good management of resources in the long term. So we want to select the longest continuous data section (this is the primary criterion), 
but at the same time we require that the account balance at the end of the selected section be greater than or equal to the account balance at the beginning of the selected data section. 
In fact, we are only interested in the value at the beginning and at the end of the selected data section, the values ​​inside the selected data section are not important, they do not play any role in the selection.

If the input contains a sequence of values, e.g.: 115 111 57 56 103 96 -1000 100 83 45, the result will be a section from the value 57 to the value 83 (it is the longest possible and 83 ≥ 57).

The input of the program is a sequence of integers that indicates the state of the account over time. The sequence length is limited to a maximum of 250000 values. 
Entry of values ​​ends when EOF (end of input) is detected.

The output of the program is the found segment of maximum length such that the balance of the account at the end of the segment is greater than or equal to the balance of the account at the beginning of the segment. 
The program displays the length of the segment and the start and end indexes (indexes counted from 0). There can be more than one equally long sections, then the program will display all options in any order. 
It may happen that the input sequence does not contain any suitable section, in which case a special message is displayed (see example).
*/
#include <stdio.h>

#define MAX_ARRAY_SIZE 250000

typedef struct section{
    int length;
    int start_index;
    int end_index;
}Tsection;

void print_array(int* array, int size){
    for(int i=0; i<size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}
void print_struct(int len, int i, int j){
    printf("%d: %d - %d\n", len, i, j);
}
int main(void){
    long int account[MAX_ARRAY_SIZE + 10];
    int count=0;
    long int amount=0;
    int desc=1;
    Tsection val;
    val.end_index=0, val.start_index=0, val.length=0;
    printf("Hodnoty:\n");
    while(scanf(" %ld", &amount) != EOF){
        if(count==MAX_ARRAY_SIZE){
            printf("Nespravny vstup.\n");
            return 1;
        }
        else{
            account[count]=amount;
        }
        //printf("amount %d count-1 %d\n", amount, account[count-1]);
        if(amount>=account[count-1] && count>0){
            //printf("neni desc\n");
            desc=0;
        }
        count++;
    }
    //printf("pole:\n");
    //print_array(account, count);
    if(count<2){
        printf("Nespravny vstup.\n");
        return 1;       
    }
    //printf("desc %d\n", desc);
    if(desc==1){
        printf("Nelze najit.\n");
        return 0;
    }
    //printf("%d\n", count);
    if(account[count-1] >= account[0]){
        val.length=count;
        val.start_index=0;
        val.end_index=count-1;
        printf("%d: %d - %d\n", val.length, val.start_index, val.end_index);
        printf("Moznosti: 1\n");
        return 0;
    }
    int max_length=0;
    //Tsection tmp;
    for(int i=0; i<count; i++){
        /*if((count-i)<=max_length){
            break;
        }*/
        //printf("prvni for\n");
        for(int j=count-1; j>=i; j--){
            //printf("prvni for\n");
            if((j-i)<max_length){
                //printf("nema smysl hledat dal\n");
                break;
            }
            if(account[i]<=account[j]){
                //printf("novy max\n");
                max_length=j-i;
                val.start_index=i;
                val.end_index=j;
                val.length=(j+1)-i; 
            }
        }
    }
    int options=1;
    print_struct(val.length, val.start_index, val.end_index);
    for(int i=0; i<=count-val.length; i++){
        if(i!=val.start_index){
            if(account[i]<=account[i+max_length]){
                print_struct(val.length, i, i+max_length);
                options++;
            }
        }
    }
    printf("Moznosti: %d\n", options);
    return 0;
}