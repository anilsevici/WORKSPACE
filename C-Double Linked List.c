#include <stdio.h>
#include <stdlib.h>

struct ogrenci
{
    int ogrenci_no;
    float ogrenci_not;
    struct ogrenci *onceki;
    struct ogrenci *sonraki;
};

void d_cift_sirali_ekle(struct ogrenci *liste_basi,struct ogrenci *yeni);
void artarak_listele(struct ogrenci *liste_basi);
void azalarak_listele(struct ogrenci *liste_basi);
int bos_mu(struct ogrenci *liste_basi);
struct ogrenci *ara(struct ogrenci *liste_basi,int aranan);
void d_cift_cikar(struct ogrenci *dugum);
void d_cift_sil(struct ogrenci *dugum);

int main()
{
    struct ogrenci *liste_basi;
    struct ogrenci *bir_ogrenci;
    int secim,aranan,sayac;
    float ogr_not;
    char cevap;

    liste_basi=malloc(sizeof(struct ogrenci));
    liste_basi->onceki=liste_basi;
    liste_basi->sonraki=liste_basi;

    do
    {

        printf("\n  1.Yeni ogrencinin eklenmesi\n");
        printf("  2.Tum ogrencilerin ortalamalarina gore kucukten buyuge dogru sirali olarak listelenmesi\n");
        printf("  3.Tum ogrencilerin ortalamalarina gore buyukten kucuge dogru sirali olarak listelenmesi\n");
        printf("  4.Numarasi verilen bir ogrencinin not ortalamasinin goruntulenmesi\n");
        printf("  5.Numarasi verilen bir ogrencinin not ortalamasinin guncellenmesi\n");
        printf("  6.Numarasi verilen bir ogrencinin silinmesi\n");
        printf("  7.Not ortalamasi verilen bir sinir degerinin altinda olan ogrencilerin silinmesi\n");
        printf("  8.Cikis\n");

        printf("\nLutfen Seciminizi Giriniz:");
        scanf("%d",&secim);

        switch(secim)
        {
        case 1:
            do
            {
                bir_ogrenci=malloc(sizeof(struct ogrenci));

                printf("Ogrencinin nosunu giriniz:");
                scanf("%d",&bir_ogrenci->ogrenci_no);

                printf("Ogrencinin notunu giriniz:");
                scanf("%f",&bir_ogrenci->ogrenci_not);

                d_cift_sirali_ekle(liste_basi,bir_ogrenci);

                do
                {
                    printf("Yeni ogrenci girecekmisiniz?:");
                    fflush(stdin);
                    cevap=getchar();
                }
                while(cevap!='E' && cevap!='e' && cevap!='H' && cevap!='h');
            }
            while(cevap=='E' || cevap=='e');
            break;
        case 2:
            if(bos_mu(liste_basi))
                printf("Listelencek kimse yok\n");
            else
                artarak_listele(liste_basi);
            break;
        case 3:
            if(bos_mu(liste_basi))
                printf("Listelencek kimse yok\n");
            else
                azalarak_listele(liste_basi);
            break;
        case 4:
            if(!bos_mu(liste_basi))
            {
                printf("Not ortalamasini goruntulemek istediginiz ogrencinin numarasini giriniz:");
                scanf("%d",&aranan);

                bir_ogrenci=ara(liste_basi,aranan);

                if(bir_ogrenci)
                    printf("Ogrencinin not ortalamasi:%.2f\n",bir_ogrenci->ogrenci_not);
                else printf("Boyle bir ogrenci yok.\n");
            }
            else printf("Goruntulenecek ogrenci yok.\n");
            break;
        case 5:
            if(!bos_mu(liste_basi))
            {
                printf("Not ortalamasini guncellemek istediginiz ogrencinin numarasini giriniz:");
                scanf("%d",&aranan);

                bir_ogrenci=ara(liste_basi,aranan);

                if(bir_ogrenci)
                {
                    printf("Ogrencinin yeni not ortalamasini giriniz:");
                    scanf("%f",&bir_ogrenci->ogrenci_not);

                    if((bir_ogrenci->onceki!=liste_basi && bir_ogrenci->ogrenci_not<bir_ogrenci->onceki->ogrenci_not) ||
                            (bir_ogrenci->sonraki!=liste_basi && bir_ogrenci->ogrenci_not>bir_ogrenci->sonraki->ogrenci_not))
                    {
                        d_cift_cikar(bir_ogrenci);
                        d_cift_sirali_ekle(liste_basi,bir_ogrenci);
                    }
                    else
                    {
                        d_cift_cikar(bir_ogrenci);
                        d_cift_sirali_ekle(liste_basi,bir_ogrenci);
                    }

                    printf("Ogrencinin not ortalamasi basariyla guncellendi.\n");
                }
                else printf("Boyle bir ogrenci yok.\n");
            }
            else printf("Ogrenci listesi bos.\n");
            break;
        case 6:
            if(!bos_mu(liste_basi))
            {
                printf("Silmek istediginiz ogrencinin numarasini giriniz:");
                scanf("%d",&aranan);

                bir_ogrenci=ara(liste_basi,aranan);

                if(bir_ogrenci)
                {
                    d_cift_sil(bir_ogrenci);
                    printf("Ogrenci basariyla silindi.\n");
                }
                else printf("Boyle bir ogrenci yok.\n");
            }
            else printf("Ogrenci listesi bos.\n");
            break;
        case 7:
            if(!bos_mu(liste_basi))
            {
                printf("Sinir degerin altinda olan ogrencilerin silinmesi icin sinir not giriniz:");
                scanf("%f",&ogr_not);

                sayac=0;

                while(liste_basi->sonraki!=liste_basi && liste_basi->sonraki->ogrenci_not<ogr_not)
                {
                    d_cift_sil(liste_basi->sonraki);
                    sayac++;

                }
                if(sayac>0)
                    printf("Ogrenciler basariyla silindi.\n");
                else printf("Ortalamasi sinirin altinda ogrenci yoktur.\n");
            }
            else printf("Ogrenci listesi bos.\n");
            break;

        }

    }
    while(secim!=8);

    return 0;
}

void d_cift_sirali_ekle(struct ogrenci *liste_basi,struct ogrenci *yeni)
{
    struct ogrenci *gecici;

    gecici=liste_basi->sonraki;

    while(gecici!=liste_basi && gecici->ogrenci_not<yeni->ogrenci_not)
    {
        gecici=gecici->sonraki;
    }

    yeni->onceki=gecici->onceki;
    yeni->sonraki=gecici;
    gecici->onceki->sonraki=yeni;
    gecici->onceki=yeni;
}

int bos_mu(struct ogrenci *liste_basi)
{
    return liste_basi->sonraki==liste_basi;
}

void artarak_listele(struct ogrenci *liste_basi)
{
    struct ogrenci *gecici;

    gecici=liste_basi->sonraki;
    printf("\nNO  NOT\n");
    printf("--  ---\n");


    while(gecici!=liste_basi)
    {
        printf(" %d  %-15.2f\n",gecici->ogrenci_no,gecici->ogrenci_not);
        gecici=gecici->sonraki;
    }
}

void azalarak_listele(struct ogrenci *liste_basi)
{
    struct ogrenci *gecici;

    gecici=liste_basi->onceki;
    printf("\nNO  NOT\n");
    printf("--  ---\n");


    while(gecici!=liste_basi)
    {
        printf(" %d  %-15.2f\n",gecici->ogrenci_no,gecici->ogrenci_not);
        gecici=gecici->onceki;
    }
}

struct ogrenci *ara(struct ogrenci *liste_basi,int aranan)
{
    struct ogrenci *gecici;

    gecici=liste_basi->sonraki;

    while(gecici!=liste_basi)
    {
        if (gecici->ogrenci_no==aranan) return gecici;
        gecici=gecici->sonraki;
    }

    return NULL;
}

void d_cift_cikar(struct ogrenci *dugum)
{
    dugum->onceki->sonraki=dugum->sonraki;
    dugum->sonraki->onceki=dugum->onceki;
}

void d_cift_sil(struct ogrenci *dugum)
{
    dugum->onceki->sonraki=dugum->sonraki;
    dugum->sonraki->onceki=dugum->onceki;
    free(dugum);
}
