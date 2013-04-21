#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STACK 20

void ortaek_sonek_cevir(char *,char *);
int oncelik_bulma(char);
void push(int *,int *,int);
int pop(int *,int *);
int tepe_eleman(int *,int);
int bos_mu(int);


int main()
{
    char ortaek[MAX_STACK],sonek[MAX_STACK];

    printf("Ortaek giriniz:");
    scanf("%s",ortaek);

    ortaek_sonek_cevir(ortaek,sonek);

    printf("\nOrtaek ifadenin sonek ifadeye donusmus hali:%s\n",sonek);

    return 0;
}

void ortaek_sonek_cevir(char *ortaek,char *sonek)
{
    int yigin[MAX_STACK];
    int i,j=0,tepe=-1;

    push(yigin,&tepe,ortaek[0]);

    for(i=1; ortaek[i]!='\0'; i++)
    {
        if(oncelik_bulma(ortaek[i])>oncelik_bulma(tepe_eleman(yigin,tepe)))
            push(yigin,&tepe,ortaek[i]);
        else
        {
            do
            {
                sonek[j]=pop(yigin,&tepe);
                j++;
            }
            while(!bos_mu(tepe) && oncelik_bulma(tepe_eleman(yigin,tepe))>=oncelik_bulma(ortaek[i]));

            push(yigin,&tepe,ortaek[i]);
        }

    }

    while(!bos_mu(tepe))
    {
        sonek[j]=pop(yigin,&tepe);
        j++;
    }
    sonek[j]='\0';

}

int oncelik_bulma(char karakter)
{
    int oncelik=0;

    if(isalnum(karakter)!=0)
        oncelik=3;
    else
        switch(karakter)
        {
        case '*':
        case '/':
            oncelik=2;
            break;

        case '+':
        case'-':
            oncelik=1;
            break;

        }

    return oncelik;

}

void push(int *yigin,int *tepe,int yeni)
{
    if(*tepe==MAX_STACK-1)
        printf("Stack dolu\n");
    else
    {
        *tepe=*tepe+1;
        yigin[*tepe]=yeni;
    }
}

int pop(int *yigin,int *tepe)
{
    int gecici;

    gecici=yigin[*tepe];
    *tepe=*tepe-1;

    return gecici;
}

int tepe_eleman(int *yigin,int tepe)
{
    return yigin[tepe];
}

int bos_mu(int tepe)
{
    return tepe==-1;
}
