#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct urun {
    int urun_kodu;
    char urun_adi[16];
    int stok_miktari;
    float birim_fiyat;
    struct urun *koda_gore_sonraki;
    struct urun *ada_gore_sonraki;
};

void koda_gore_sirali_ekle(struct urun *liste_basi_ptr, struct urun *yeni);
void adin_ilk_harfine_gore_basa_ekle(struct urun **ilk_ptr, struct urun *yeni);
struct urun *koda_gore_ara(struct urun *liste_basi_ptr, int aranan_urun_kodu);
void ada_gore_ara_ve_listele(struct urun *ilk_ptr, char *aranan_urun_adi);
void adin_ilk_harfine_gore_bir_listeyi_listele(struct urun *ilk_ptr);
void stok_miktarina_gore_listele(struct urun *liste_basi_ptr, int stok_limiti);
struct urun *koda_gore_sil(struct urun *liste_basi_ptr, int silinecek_urun_kodu);
void ada_gore_sil(struct urun **ilk_ptr, struct urun *silinecek_ptr);

int main()
{
    struct urun koda_gore_hash_tablosu[100];
    struct urun *ada_gore_hash_tablosu[26]={NULL};
    struct urun *bir_urun;
    int i,secim;
    int urun_kodu,hash1,hash2,stok_limiti;
    char urun_adi[16],ilk_harf;

    for(i=0;i<100;i++)
        koda_gore_hash_tablosu[i].koda_gore_sonraki=&koda_gore_hash_tablosu[i];//liste basi dugumune sahip dairesel tek bagli listeler ilkleniyor

    do
    {
        printf("1.Yeni bir urun ekleme\n");
        printf("2.Kodu verilen bir urunun bilgilerini goruntuleme\n");
        printf("3.Verilen bir ada sahip urunlerin bilgilerini goruntuleme\n");
        printf("4.Adi, verilen bir harf ile baslayan urunlerin bilgilerini goruntuleme\n");
        printf("5.Stok miktari verilen bir degerin altina dusmus urunlerin bilgilerini goruntuleme\n");
        printf("6.Kodu verilen bir urunu silme\n");
        printf("7.Cikis\n");
        printf("Seciminizi giriniz:");
        scanf("%d",&secim);
        switch (secim)
        {
        case 1:
            printf("Urun kodunu giriniz:");
            scanf("%d",&urun_kodu);
            hash1=urun_kodu % 100;
            if(koda_gore_ara(&koda_gore_hash_tablosu[hash1],urun_kodu)==NULL) {//ayni koda sahip urun yoksa...
                bir_urun=malloc(sizeof(struct urun));//yeni urun icin bellekten yer ayriliyor
                bir_urun->urun_kodu=urun_kodu;
                printf("Urun adini giriniz:");
                fflush(stdin);
                scanf("%s",bir_urun->urun_adi);
                printf("Urun stok miktarini giriniz:");
                scanf("%d",&bir_urun->stok_miktari);
                printf("Urun birim fiyatini giriniz:");
                scanf("%f",&bir_urun->birim_fiyat);
                koda_gore_sirali_ekle(&koda_gore_hash_tablosu[hash1],bir_urun);//yeni urun, urun koduna gore ilgili listeye eklenir
                hash2=toupper(bir_urun->urun_adi[0])-'A';
                adin_ilk_harfine_gore_basa_ekle(&ada_gore_hash_tablosu[hash2],bir_urun);//yeni urun, urun adinin ilk harfine gore ilgili listeye eklenir
                printf("Urun eklendi\n");
            }
            else
                printf("Bu koda sahip bir urun zaten var!\n");
            break;
        case 2:
            printf("Urun kodunu giriniz:");
            scanf("%d",&urun_kodu);
            hash1=urun_kodu % 100;
            bir_urun=koda_gore_ara(&koda_gore_hash_tablosu[hash1],urun_kodu);
            if(bir_urun==NULL)
                printf("Bu koda sahip bir urun yok!\n");
            else {
                printf("Urun Kodu  Urun Adi         Stok  Fiyat\n");
                printf("---------  ---------------  ----  -----\n");
                printf("%-9d  %-15s  %-4d  %.2f\n",bir_urun->urun_kodu,bir_urun->urun_adi,bir_urun->stok_miktari,bir_urun->birim_fiyat);
            }
            break;
        case 3:
            printf("Urun adini giriniz:");
            fflush(stdin);
            scanf("%s",urun_adi);
            hash2=toupper(urun_adi[0])-'A';
            ada_gore_ara_ve_listele(ada_gore_hash_tablosu[hash2],urun_adi);
            break;
        case 4:
            printf("Urun adinin ilk harfini giriniz:");
            fflush(stdin);
            scanf("%c",&ilk_harf);
            hash2=toupper(ilk_harf)-'A';
            adin_ilk_harfine_gore_bir_listeyi_listele(ada_gore_hash_tablosu[hash2]);
            break;
        case 5:
            printf("Stok miktari icin ust limiti giriniz:");
            scanf("%d",&stok_limiti);

            printf("Urun Kodu  Urun Adi         Stok  Fiyat\n");
            printf("---------  ---------------  ----  -----\n");

            for(i=0;i<100;i++)//koda gore sirali olan tum liste basi dugumune sahip dairesel tek bagli listeler taraniyor
                stok_miktarina_gore_listele(&koda_gore_hash_tablosu[i],stok_limiti);

            break;
        case 6:
            printf("Urun kodunu giriniz:");
            scanf("%d",&urun_kodu);
            hash1=urun_kodu % 100;
            bir_urun=koda_gore_sil(&koda_gore_hash_tablosu[hash1],urun_kodu);//koda gore olan listeden cikar(urun varsa)
            if(bir_urun==NULL)
                printf("Bu koda sahip bir urun yok!\n");
            else {
                hash2=toupper(bir_urun->urun_adi[0])-'A';
                ada_gore_sil(&ada_gore_hash_tablosu[hash2],bir_urun);//urun varsa, ada gore olan listeden de cikar
                free(bir_urun);//urunu yok et
                printf("Urun silindi\n");
            }
            break;
        }
    }
    while (secim!=7);

    return 0;
}

void koda_gore_sirali_ekle(struct urun *liste_basi_ptr, struct urun *yeni)
{
    struct urun *onceki, *gecici;

    onceki=liste_basi_ptr;
    gecici=liste_basi_ptr->koda_gore_sonraki;
    while(gecici!=liste_basi_ptr && gecici->urun_kodu<yeni->urun_kodu)
    {
        onceki=gecici;
        gecici=gecici->koda_gore_sonraki;
    }

    yeni->koda_gore_sonraki=gecici;
    onceki->koda_gore_sonraki=yeni;
}

void adin_ilk_harfine_gore_basa_ekle(struct urun **ilk_ptr, struct urun *yeni)
{
    yeni->ada_gore_sonraki=*ilk_ptr;
    *ilk_ptr=yeni;
}

struct urun *koda_gore_ara(struct urun *liste_basi_ptr, int aranan_urun_kodu)//ogrenci listede bulunursa isaretcisi dondurulur, bulunamazsa NULL dondurulur
{
    struct urun *gecici;

    gecici=liste_basi_ptr->koda_gore_sonraki;
    while (gecici!=liste_basi_ptr && gecici->urun_kodu<=aranan_urun_kodu)
    {
        if (gecici->urun_kodu==aranan_urun_kodu) return gecici;
        gecici=gecici->koda_gore_sonraki;
    }

    return NULL;
}

void ada_gore_ara_ve_listele(struct urun *ilk_ptr, char *aranan_urun_adi)
{
    printf("Urun Kodu  Stok  Fiyat\n");
    printf("---------  ----  -----\n");

    while (ilk_ptr)//liste sirali olmadigi icin sonuna kadar taraniyor
    {
        if (strcmp(ilk_ptr->urun_adi,aranan_urun_adi)==0)
            printf("%-9d  %-4d  %.2f\n",ilk_ptr->urun_kodu,ilk_ptr->stok_miktari,ilk_ptr->birim_fiyat);

        ilk_ptr=ilk_ptr->ada_gore_sonraki;
    }
}

void adin_ilk_harfine_gore_bir_listeyi_listele(struct urun *ilk_ptr)
{
    printf("Urun Kodu  Urun Adi         Stok  Fiyat\n");
    printf("---------  ---------------  ----  -----\n");

    while (ilk_ptr)
    {
        printf("%-9d  %-15s  %-4d  %.2f\n",ilk_ptr->urun_kodu,ilk_ptr->urun_adi,ilk_ptr->stok_miktari,ilk_ptr->birim_fiyat);
        ilk_ptr=ilk_ptr->ada_gore_sonraki;
    }
}

void stok_miktarina_gore_listele(struct urun *liste_basi_ptr, int stok_limiti)
{
    struct urun *gecici;

    gecici=liste_basi_ptr->koda_gore_sonraki;
    while (gecici!=liste_basi_ptr)
    {
        if (gecici->stok_miktari<stok_limiti)
            printf("%-9d  %-15s  %-4d  %.2f\n",gecici->urun_kodu,gecici->urun_adi,gecici->stok_miktari,gecici->birim_fiyat);

        gecici=gecici->koda_gore_sonraki;
    }
}

struct urun *koda_gore_sil(struct urun *liste_basi_ptr, int silinecek_urun_kodu)
{
    struct urun *onceki, *gecici;

    onceki=liste_basi_ptr;
    gecici=liste_basi_ptr->koda_gore_sonraki;
    while(gecici!=liste_basi_ptr && gecici->urun_kodu<silinecek_urun_kodu)
    {
        onceki=gecici;
        gecici=gecici->koda_gore_sonraki;
    }

    if(gecici==liste_basi_ptr || gecici->urun_kodu>silinecek_urun_kodu)
        return NULL;//urun yoksa NULL dondur
    else {//urun varsa
        onceki->koda_gore_sonraki=gecici->koda_gore_sonraki;//urun koduna gore olan listeden cikar ama urunu yok etme
        return gecici;//ve cikarilan urunun isaretcisini dondur
    }
}

void ada_gore_sil(struct urun **ilk_ptr, struct urun *silinecek_ptr)
{
    struct urun *onceki, *gecici;

    if(*ilk_ptr==silinecek_ptr)//silinecek dugum, ilk dugum ise...
        *ilk_ptr=(*ilk_ptr)->ada_gore_sonraki;//ilk_ptr'yi guncelle
    else {
        onceki=*ilk_ptr;
        gecici=(*ilk_ptr)->ada_gore_sonraki;
        while(gecici!=silinecek_ptr)
        {
            onceki=gecici;
            gecici=gecici->ada_gore_sonraki;
        }

        onceki->ada_gore_sonraki=gecici->ada_gore_sonraki;
    }
}
