#include<stdio.h>
void bubble_sorting(int T[] , int n){
    int temp , i=0 , j=0;
    for(j ; j<n-1 ; j++){
        i=0;
    while(i<n-1-j){
        if(T[i]>T[i+1]){
        temp=T[i+1];
        T[i+1]=T[i];
        T[i]=temp;
        i++;
        }
        else if(T[i]<T[i+1]){
            i++;
        }
       
    }
    }
    i=0;
    while(i<n){
        printf("%d\t",T[i]);
        i++;
    }
}
int main(){
    int T[5]={9 , 7 , 8 , 6 , 5};
    bubble_sorting(T , 5);
}