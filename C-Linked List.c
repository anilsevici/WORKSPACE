#include <stdio.h>
#include <stdlib.h>

struct aday
{
    int no;
    char ad_soyad[21];
    int boy;
    struct aday *sonraki;
};

void sirali_ekle(struct aday **ilk,struct aday **son,struct aday *yeni);
struct aday *bastan_cik(struct aday **ilk,struct aday **son);
int bos_mu(struct aday *ilk);
void listele(struct aday *ilk);

int main()
{

    struct aday *ilk=NULL;
    struct aday *son=NULL;
    struct aday *bir_aday;
    int secim;
    char cevap;

    do
    {

        printf("\n  1.Aday numarasi,adi soyadi ve boyu verilen adayin bekleme salonuna alinmasi\n");
        printf("  2.Bekleme salonundan siradaki adayin cagrilmasi\n");
        printf("  3.Bekleme salonunda bekleyen adaylarin bilgilerinin listelenmesi\n");
        printf("  4.Cikis\n");

        printf("\nLutfen Seciminizi Giriniz:");
        scanf("%d",&secim);

        switch(secim)
        {
        case 1:
            do
            {
                bir_aday=malloc(sizeof(struct aday));

                printf("Adayin nosunu giriniz:");
                scanf("%d",&bir_aday->no);

                printf("Adayin adini soyadini giriniz:");
                fflush(stdin);
                gets(bir_aday->ad_soyad);

                printf("Adayin boyunu giriniz:");
                scanf("%d",&bir_aday->boy);

                sirali_ekle(&ilk,&son,bir_aday);

                do
                {
                    printf("Yeni aday numarasi girecekmisiniz?:");
                    fflush(stdin);
                    cevap=getchar();
                }
                while(cevap!='E' && cevap!='e' && cevap!='H' && cevap!='h');
            }
            while(cevap=='E' || cevap=='e');
            break;
        case 2:
            if(bos_mu(ilk))
                printf("Salon bos\n");
            else
                bir_aday=bastan_cik(&ilk,&son);
            printf("\nNO AD SOYAD        BOY\n");
            printf("-- --------------- ---\n");
            printf(" %d %-15s %-15d\n",bir_aday->no,bir_aday->ad_soyad,bir_aday->boy);
            free(bir_aday);
            break;
        case 3:
            listele(ilk);
            break;

        }

    }
    while(secim!=4);

    return 0;
}

void sirali_ekle(struct aday **ilk,struct aday **son,struct aday *yeni)
{
    struct aday *onceki;
    struct aday *gecici;

    if(*ilk==NULL)
    {
        yeni->sonraki=NULL;
        *ilk=yeni;
        *son=yeni;
    }
    else if(yeni->boy<(*son)->boy)
    {
        yeni->sonraki=NULL;
        (*son)->sonraki=yeni;
        *son=yeni;
    }
    else if (yeni->boy>(*ilk)->boy)
    {
        yeni->sonraki=*ilk;
        *ilk=yeni;
    }
    else
    {
        onceki=*ilk;
        gecici=(*ilk)->sonraki;

        while(gecici!=NULL && yeni->boy<gecici->boy)
        {
            onceki=gecici;
            gecici=gecici->sonraki;
        }

        yeni->sonraki=gecici;
        onceki->sonraki=yeni;

    }
}

struct aday *bastan_cik(struct aday **ilk,struct aday **son)
{
    struct aday *gecici;

    gecici=*ilk;

    if(*ilk==NULL)
    {
        *son=NULL;
    }

    *ilk=(*ilk)->sonraki;
    return gecici;
}

int bos_mu(struct aday *ilk)
{
    return ilk==NULL;
}

void listele(struct aday *ilk)
{
    if(ilk==NULL)
        printf("Salonda bekleyen kimse yok.\n");
    else
    {
        printf("\nNO AD SOYAD        BOY\n");
        printf("-- --------------- ---\n");

        while(ilk!=NULL)
        {
            printf(" %d %-15s %-15d\n",ilk->no,ilk->ad_soyad,ilk->boy);
            ilk=ilk->sonraki;
        }
    }
}
