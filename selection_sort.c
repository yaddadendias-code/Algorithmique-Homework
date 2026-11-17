#include<stdio.h>
void swap(int *a , int *b){
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}
void select_sort(int t[] , unsigned int size){
    for(int i=0 ; i<size ; i++){
        int min_indx=i;
        for(int j=i+1 ; j<size ; j++){
            if(t[min_indx]>t[j]){
                min_indx=j;
            }
        }
        swap(&t[min_indx] , &t[i]);
    }
    int i=0;
    while(i<size){
        printf("%d\t",t[i]);
        i++;
    }
}
int main(){
    unsigned int size;
    printf("Entre size of array:");
    scanf("%u",&size);
    int t[size];
    int i=0;
    while(i<size){
        printf("Entre value of T[%d]:",i+1);
        scanf("%d",&t[i]);
        i++;
    }
    select_sort(t , size);
    return 0;
}