#include <stdio.h>
#include <stdlib.h>

#define MAX 200

struct aday
{
    int no;
    char ad_soyad[21];
    int boy;
};

void listele(struct aday *,int,int);
struct aday dairesel_sil(struct aday *,int *,int *);
void dairesel_ekle(struct aday *,int *,int *,struct aday);
int bos_mu(int);
int eleman_say(int,int);

int main()
{
    struct aday kuyruk[MAX];
    struct aday bir_aday;
    int secim,on=-1,arka=-1;
    char cevap;


    do
    {

        printf("\n  1.Aday numarasi,adi soyadi ve boyu verilen adayin bekleme salonuna alinmasi\n");
        printf("  2.Bekleme salonundan siradaki adayin cagrilmasi\n");
        printf("  3.Bekleme salonundan bekleyen aday sayisinin goruntulenmesi\n");
        printf("  4.Bekleme salonunda bekleyen adaylarin bilgilerinin listelenmesi\n");
        printf("  5.Cikis\n");

        printf("\nLutfen Seciminizi Giriniz:");
        scanf("%d",&secim);

        switch(secim)
        {
        case 1:
            do
            {

                printf("Adayin nosunu giriniz:");
                scanf("%d",&bir_aday.no);

                printf("Adayin adini soyadini giriniz:");
                fflush(stdin);
                gets(bir_aday.ad_soyad);

                printf("Adayin boyunu giriniz:");
                scanf("%d",&bir_aday.boy);

                dairesel_ekle(kuyruk,&on,&arka,bir_aday);

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
            if(bos_mu(on))
                printf("Salon bos\n");
            else
            {
                bir_aday=dairesel_sil(kuyruk,&on,&arka);

                printf("\n NO    AD SOYAD        BOY\n");
                printf(" ----- --------------- ---\n");
                printf(" %d   %-15s %-15d\n",bir_aday.no,bir_aday.ad_soyad,bir_aday.boy);

            }
            break;
        case 3:
            if(bos_mu(on))
                puts("Bekleyen aday yok.");
            else
                printf("Bekleyen aday sayisi:%d",eleman_say(on,arka));
            break;
        case 4:
            listele(kuyruk,on,arka);
            break;

        }

    }
    while(secim!=5);

    return 0;
}

int eleman_say(int on,int arka)
{
    if((arka-on)<0)
        return arka-on+MAX+1;
    else
        return arka-on+1;
}

int bos_mu(int on)
{
    return on==-1;
}

void dairesel_ekle(struct aday *kuyruk, int *on, int *arka, struct aday yeni)
{
    if ((*arka+1)%MAX ==*on)
        printf("Kuyruk dolu\n");
    else
    {
        *arka=(*arka+1)%MAX;
        kuyruk[*arka]=yeni;
        if (*on==-1) *on=0;
    }
}

struct aday dairesel_sil(struct aday *kuyruk, int *on, int *arka)
{
    struct aday gecici;

    gecici=kuyruk[*on];
    if (*on==*arka)
    {
        *on=-1;
        *arka=-1;
    }
    else
        *on=(*on+1)%MAX;

    return gecici;
}

void listele(struct aday *kuyruk,int on,int arka)
{
    int i;

    if(!bos_mu(on))
    {

        printf("\n NO    AD SOYAD        BOY\n");
        printf(" ----- --------------- ---\n");

        for(i=on; i!=arka; i=(i+1)%MAX)
        {
            printf(" %d %-15s %-15d\n",kuyruk[i].no,kuyruk[i].ad_soyad,kuyruk[i].boy);
        }

        printf(" %d %-15s %-15d\n",kuyruk[i].no,kuyruk[i].ad_soyad,kuyruk[i].boy);

    }
    else
        printf("Salonda bekleyen kimse yok\n");
}
