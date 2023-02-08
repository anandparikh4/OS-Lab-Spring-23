#include<stdio.h>

int main(){
    int x,y,z;
    printf("Enter a number:");
    scanf("%d" , &x);
    printf("Enter another number:");
    scanf("%d" , &y);
    printf("%d + %d = %d\n" , x , y , x+y);

    return 0;
}