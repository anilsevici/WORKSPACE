#include <stdio.h>
#include <stdlib.h>

#define MAX 10

int main()
{

    int left=0;
    int right=MAX-1;
    int dizi[MAX]= {2,8,10,14,26,39,48,75,87,98};
    int middle1,middle2,indeks,search_value,flag=0,step=0;

    printf("Enter the number:");
    scanf("%d",&search_value);

    while(left<=right && flag==0)
    {
        middle1=(left*2+right)/3;
        middle2=(left+right*2)/3;

        if(dizi[middle1]==search_value)
        {
            flag=1;
            indeks=middle1;
        }
        else if(dizi[middle2]==search_value)
        {
            flag=1;
            indeks=middle2;
        }
        else if(search_value<dizi[middle1])
            right=middle1-1;
        else if(search_value>dizi[middle1] && search_value<dizi[middle2])
        {
            left=middle1+1;
            right=middle2-1;
        }
        else if(search_value>dizi[middle2])
            left=middle2+1;


        step++;

    }

    if(flag==1)
        printf("Number found in %d steps!! Location is:%d\n",step,indeks+1);
    else
        printf("Number is not found!!\n");


    return 0;
}
