#include <stdio.h>
#include <stdlib.h>

struct ogrenciler
{
    int ogr_no;
    char ad_soyad[31];
    int sinif;
    int donem_kredi;
    float agirlik_not_ort;
    struct ogrenciler *no_sonraki;
};
struct ders_dugumu
{
    int ders_kod;
    char ders_adi[26];
    int ders_kredi;
    int ders_alan_ogr_say;
    float ders_not_ort;
    struct notlar *not_durumu;//notlarin bilgilerini gostericek olan gosterici
};
struct notlar
{
    int ogr_no;
    int ogr_not;
    struct notlar *no_sonraki;
    struct notlar *ogr_not_onceki;
    struct notlar *ogr_not_sonraki;
};

typedef struct notlar Notlar;
typedef struct ders_dugumu Ders_dugumu;
typedef struct ogrenciler Ogrenciler;

int sayi_al(int alt,int ust);
int menu_secim();
void noya_gore_sirali_ekle(struct ogrenciler **ilk, struct ogrenciler *yeni);
void noya_gore_dairesel_ekle(struct notlar *liste_basi_ptr,struct notlar *yeni);
void ortalamaya_gore_cif_dairesel_ekle(struct notlar *liste_basi_ptr,struct notlar *yeni);
struct ogrenciler *noya_gore_ara(struct ogrenciler *ilk,int aranan_ogr_no);
struct notlar *noya_gore_dairesel_liste_ara(struct notlar *liste_basi_ptr, int aranan_no);
void yeni_ders_ekle(struct ders_dugumu *dersler[]);
void dersi_alan_ogrenci_not_ekle(struct ders_dugumu *dersler[],struct ogrenciler *koda_gore_hash_tablosu[]);
struct ogrenciler *noya_gore_listeden_cikar(struct ogrenciler **ilk,int aranan_ogr_no);
struct notlar *noya_gore_dairesel_cikar(struct notlar *liste_basi_ptr, int cikarilacak_ogr_no);
void nota_gore_dairesel_cift_baglidan_dugum_cikar(struct notlar *cikarilacak);
void ogr_ders_sil(struct ders_dugumu *dersler[],int silinecek_ogr_no);
void dugum_tasi(struct notlar *liste_basi_ptr,struct notlar *yeni);
void ogrenci_not_guncelle(struct ders_dugumu *dersler[],struct ogrenciler *koda_gore_hash_tablosu[]);
void bir_dersi_alan_ogrenciler_listele(struct ders_dugumu *dersler[],struct ogrenciler *koda_gore_hash_tablosu[]);
void bir_dersi_alan_notu_yuksek_olanlar(struct ders_dugumu *dersler[],struct ogrenciler *koda_gore_hash_tablosu[]);
void bir_dersi_alan_notu_altinda_olanlar(struct ders_dugumu *dersler[],struct ogrenciler *koda_gore_hash_tablosu[]);
void bir_donem_ders_listele(struct ders_dugumu *dersler[]);
int bir_ogrenci_listele(struct ogrenciler *koda_gore_hash_tablosu[]);
void bir_ogrenci_aldigi_ders_listele(struct ders_dugumu *dersler[],struct ogrenciler *koda_gore_hash_tablosu[]);
void ogrenci_sinif_listele(struct ogrenciler *koda_gore_hash_tablosu[]);

int main()
{
    Ogrenciler *koda_gore_hash_tablosu[100]= {NULL};
    Ders_dugumu *dersler[80]= {NULL};
    Ogrenciler *bir_ogrenci;
    int ogr_no,hash,secenek;

    do
    {
        secenek=menu_secim();
        switch(secenek)
        {
        case 1:
            printf("Eklemek istediginiz ogrenci numarasini giriniz(1-10000):");
            ogr_no=sayi_al(1,10000);
            hash=(ogr_no-1)/100;
            if(noya_gore_ara(koda_gore_hash_tablosu[hash],ogr_no)==NULL)
            {
                bir_ogrenci=malloc(sizeof(Ogrenciler));
                bir_ogrenci->ogr_no=ogr_no;
                bir_ogrenci->agirlik_not_ort=0;
                bir_ogrenci->donem_kredi=0;

                printf("Ogrencinin adini ve soyadini giriniz:");
                fflush(stdin);
                gets(bir_ogrenci->ad_soyad);

                printf("Ogrencinin sinifini giriniz:");
                scanf("%d",&bir_ogrenci->sinif);

                noya_gore_sirali_ekle(&koda_gore_hash_tablosu[hash],bir_ogrenci);
                printf("Yeni Ogrenci basariyla eklendi\n");
            }
            else
                printf("Bu ogrenci numarasina sahip ogrenci vardir.\n");
            break;
        case 2:
            yeni_ders_ekle(dersler);
            break;
        case 3:
            dersi_alan_ogrenci_not_ekle(dersler,koda_gore_hash_tablosu);
            break;
        case 4:
            ogrenci_not_guncelle(dersler,koda_gore_hash_tablosu);
            break;
        case 5:
            printf("Silinecek ogrencinin numarasini giriniz:");
            ogr_no=sayi_al(1,10000);
            hash=(ogr_no-1)/100;
            bir_ogrenci=noya_gore_listeden_cikar(&koda_gore_hash_tablosu[hash],ogr_no);
            if(bir_ogrenci==NULL)
                printf("%d ogrenci numarasina sahip ogrenci yoktur\n",ogr_no);
            else
            {
                free(bir_ogrenci);//isaretci ogrenci dizisindeki ogrencinin bilgileri siliniyor
                ogr_ders_sil(dersler,ogr_no);
                printf("Ogrenci basariyla silindi.\n");
            }
            break;
        case 6:
            bir_dersi_alan_ogrenciler_listele(dersler,koda_gore_hash_tablosu);
            break;
        case 7:
            bir_dersi_alan_notu_yuksek_olanlar(dersler,koda_gore_hash_tablosu);
            break;
        case 8:
            bir_dersi_alan_notu_altinda_olanlar(dersler,koda_gore_hash_tablosu);
            break;
        case 9:
            bir_donem_ders_listele(dersler);
            break;
        case 10:
            bir_ogrenci_listele(koda_gore_hash_tablosu);
            break;
        case 11:
            bir_ogrenci_aldigi_ders_listele(dersler,koda_gore_hash_tablosu);
            break;
        case 12:
            ogrenci_sinif_listele(koda_gore_hash_tablosu);
            break;
        }
    }
    while(secenek!=13);

    printf("\nOgrenci Takip Sistemi Programindan Ciktiniz.\n");

    return 0;
}

int sayi_al(int alt,int ust)
{
    int sayi;

    scanf("%d",&sayi);
    while(sayi<alt || sayi>ust)
    {
        printf("Hatali giris yaptiniz,uygun aralikta tekrar giriniz:");
        scanf("%d",&sayi);
    }
    return sayi;
}

int menu_secim()
{
    int secim;

    printf("\n  1. Yeni ogrencinin eklenmesi\n");
    printf("  2. Yeni bir dersin eklenmesi\n");
    printf("  3. Bir dersi alan ogrencilerin notlarinin eklenmesi\n");
    printf("  4. Bir dersi alan ogrencinin notunun guncellenmesi\n");
    printf("  5. Bir ogrencinin silinmesi\n");
    printf("  6. Bir dersin bilgilerinin ve o dersi alan ogrencilerin listelenmesi\n");
    printf("  7. Bir dersi alan ogrencilerden notu belirli bir notun ustunde olan ogrencilerin listelenmesi\n");
    printf("  8. Bir dersi alan ogrencilerden notu belirli bir notun altinda olan ogrencilerin listelenmesi\n");
    printf("  9. Bir donem derslerinin listelenmesi\n");
    printf(" 10. Bir ogrencinin bilgilerinin listelenmesi\n");
    printf(" 11. Bir ogrencinin bilgilerinin ve aldigi derslerin listelenmesi\n");
    printf(" 12. Bir sinifta okuyan ogrencilerin listelenmesi\n");
    printf(" 13. Cikis\n\n");

    printf("\nLutfen Seciminizi Giriniz:");
    secim=sayi_al(1,13);

    return secim;
}

void noya_gore_sirali_ekle(Ogrenciler **ilk,Ogrenciler *yeni)//ogrenci numarasina gore tek bagli listeye ogrenci ekliyecek olan fonksiyon
{
    Ogrenciler *gecici,*onceki;

    if (*ilk==NULL)//liste bossa basa ekleme
    {
        yeni->no_sonraki=NULL;
        *ilk=yeni;
    }
    else if (yeni->ogr_no<(*ilk)->ogr_no)//ilk siraya ekleme
    {
        yeni->no_sonraki=*ilk;
        *ilk=yeni;
    }
    else
    {
        //araya veya sona sirali olarak ekleme
        onceki=*ilk;
        gecici=(*ilk)->no_sonraki;
        while ((gecici!=NULL) && (gecici->ogr_no<yeni->ogr_no))
        {
            onceki=gecici;
            gecici=gecici->no_sonraki;
        }
        yeni->no_sonraki=gecici;
        onceki->no_sonraki=yeni;
    }
}

Ogrenciler *noya_gore_ara(Ogrenciler *ilk,int aranan_ogr_no)//girilen numarali ogrencinin olup olmadigini tek bagli listede arayan fonksiyon
{
    while (ilk!=NULL && ilk->ogr_no<=aranan_ogr_no)
    {
        if (ilk->ogr_no==aranan_ogr_no) return ilk;//ogrenciyi bulmussa ogrencinin tek bagli listedeki adresini dondurur
        ilk=ilk->no_sonraki;
    }
    return NULL;//ogrenci yoksa NULL dondurur
}

Notlar *noya_gore_dairesel_liste_ara(Notlar *liste_basi_ptr, int aranan_no)//numaraya gore dairesel tek bagli listede tutan ogrenci numarasini arayan fonksiyon
{
    Notlar *gecici;

    gecici=liste_basi_ptr->no_sonraki;
    while (gecici!=liste_basi_ptr && aranan_no>=gecici->ogr_no)
    {
        if (gecici->ogr_no==aranan_no) return gecici;
        gecici=gecici->no_sonraki;
    }
    return NULL;
}

void noya_gore_dairesel_ekle(Notlar *liste_basi_ptr,Notlar *yeni)//ogrenci numarasina gore dairesel tek bagli listeye ekleyen fonksiyon
{
    Notlar *onceki,*gecici;

    onceki=liste_basi_ptr;
    gecici=liste_basi_ptr->no_sonraki;
    while(gecici!=liste_basi_ptr && gecici->ogr_no<yeni->ogr_no)
    {
        onceki=gecici;
        gecici=gecici->no_sonraki;
    }
    yeni->no_sonraki=gecici;
    onceki->no_sonraki=yeni;
}

void ortalamaya_gore_cif_dairesel_ekle(Notlar *liste_basi_ptr,Notlar *yeni)//ogrencinin not ortalamasina gore dairesel cift bagli listeye ekleyen fonksiyon
{
    Notlar *gecici;

    gecici=liste_basi_ptr->ogr_not_sonraki;
    while (gecici!=liste_basi_ptr && gecici->ogr_not<yeni->ogr_not)
        gecici=gecici->ogr_not_sonraki;

    yeni->ogr_not_sonraki=gecici;
    yeni->ogr_not_onceki=gecici->ogr_not_onceki;
    gecici->ogr_not_onceki->ogr_not_sonraki=yeni;
    gecici->ogr_not_onceki=yeni;
}

void yeni_ders_ekle(Ders_dugumu *dersler[])//yeni dersin yaratildigi ve eklendigi fonksiyon
{
    int donem,sayac=0;
    Ders_dugumu *yeni;

    printf("Eklemek istediginiz dersin donemini giriniz(1-8):");
    donem=(sayi_al(1,8)*10)-10;

    while(dersler[donem+sayac]!=NULL && sayac!=10)//isaretci dersler dizisinde bos gordugu ilk yere kadar ya da bir donem en cok 10 kez ders verilebildiginden 10 kez doner.
        sayac++;

    if(sayac!=10)
    {
        yeni=malloc(sizeof(Ders_dugumu));//yeni ders icin hafizadan mallocla yer aliniyor.
        yeni->ders_kod=donem+sayac+10;

        printf("Eklenecek dersin kodu:%d",yeni->ders_kod);

        printf("\nYeni dersin adini giriniz:");
        fflush(stdin);
        gets(yeni->ders_adi);

        printf("Yeni dersin kredisini giriniz:");
        scanf("%d",&yeni->ders_kredi);

        yeni->ders_alan_ogr_say=0;
        yeni->ders_not_ort=0;

        yeni->not_durumu=malloc(sizeof(Notlar));//ogrencinin nosuna ve ortalamasina gore tutulacak dugum icin mallocla hafizadan yer alinarak liste basi dugumu yaratiliyor.
        yeni->not_durumu->no_sonraki=yeni->not_durumu;//liste basi dugumu icin ilklemeler yapiliyor.
        yeni->not_durumu->ogr_not_onceki=yeni->not_durumu;
        yeni->not_durumu->ogr_not_sonraki=yeni->not_durumu;

        dersler[yeni->ders_kod-10]=yeni;//yeni ders isaretci dersler dizisine atiliyor.
        printf("Yeni Ders basariyla eklendi\n");
    }
    else
        printf("Bu doneme ders eklemek icin yer yoktur.Bir donemde en cok 10 ders verilebilir.\n");
}

void dersi_alan_ogrenci_not_ekle(Ders_dugumu *dersler[],Ogrenciler *koda_gore_hash_tablosu[])// bir dersi alan ogrencilerin notlarini alan fonksiyon
{
    Notlar *bir_not;
    Ogrenciler *bir_ogrenci;
    int kod,kod2,hash,ders_say=0;
    float top_not,top_not2,eski_ders_top,ders_not_top=0;
    char cevap;

    printf("Derse ogrenci eklemek icin ders kodunu giriniz[(10-89):");
    kod=sayi_al(10,89)-10;
    if(dersler[kod]==NULL)
        printf("Girdiginiz %d ders koduna sahip ders yoktur.\n",kod+10);
    else
    {
        do
        {
            printf("Ekleme yapmak istediginiz ogrencinin numarasini giriniz:");
            kod2=sayi_al(1,10000);
            hash=(kod2-1)/100;
            bir_ogrenci=noya_gore_ara(koda_gore_hash_tablosu[hash],kod2);
            if(bir_ogrenci==NULL)
                printf("Ekleme yapmak istediginiz %d nolu ogrenci bulunmamaktadir.\n",kod2);
            else
            {
                bir_not=malloc(sizeof(Notlar));//notlar dugumu icin hafizadan yer aliniyor
                printf("Ogrencinin bu dersten aldigi notu giriniz:");
                scanf("%d",&bir_not->ogr_not);
                bir_not->ogr_no=kod2;

                ders_not_top+=bir_not->ogr_not;//bir dersi alan ogrencilerin notlarinin toplami
                ders_say++;//bir dersi alan ogrenci sayisi

                top_not=bir_ogrenci->agirlik_not_ort*bir_ogrenci->donem_kredi;// ogrencinin donemlik agirlikli not ortalamasi hesabi islemleri yapiliyor
                top_not2=bir_not->ogr_not*dersler[kod]->ders_kredi;
                bir_ogrenci->donem_kredi+=dersler[kod]->ders_kredi;
                bir_ogrenci->agirlik_not_ort=(top_not+top_not2)/bir_ogrenci->donem_kredi;

                noya_gore_dairesel_ekle(dersler[kod]->not_durumu,bir_not);//ogrenci numarasina gore notlar dugumune dairesel tek bagli olarak ekleniyor
                ortalamaya_gore_cif_dairesel_ekle(dersler[kod]->not_durumu,bir_not);//ogrencinin notuna gore yine ayni ilgili notlar dugumune dairesel cift bagli olarak ekleniyor
            }
            do
            {
                printf("Yeni ogrenci numarasi girecekmisiniz?(E/e/H/h):");
                fflush(stdin);
                cevap=getchar();
            }
            while(cevap!='E' && cevap!='e' && cevap!='H' && cevap!='h');
        }
        while(cevap=='E' || cevap=='e');
        eski_ders_top=dersler[kod]->ders_not_ort*dersler[kod]->ders_alan_ogr_say;
        dersler[kod]->ders_alan_ogr_say+=ders_say;//bir dersi alan ogrenci sayisi
        dersler[kod]->ders_not_ort=(eski_ders_top+ders_not_top)/dersler[kod]->ders_alan_ogr_say;//dersin genel not ortalamasi hesabi islemleri
    }
}

void dugum_tasi(Notlar *liste_basi_ptr,Notlar *yeni)//ogrencinin notuna gore dugumun uygun yere tasinmasi
{
    Notlar *gecici;

    if(yeni->ogr_not_onceki!=liste_basi_ptr && yeni->ogr_not_onceki->ogr_not>yeni->ogr_not)// dugumun nereye tasinicaginin saptanmasi
    {
        gecici=yeni->ogr_not_onceki;
        while(gecici->ogr_not_onceki!=liste_basi_ptr && yeni->ogr_not<gecici->ogr_not_onceki->ogr_not)
            gecici=gecici->ogr_not_onceki;

        yeni->ogr_not_onceki->ogr_not_sonraki=yeni->ogr_not_sonraki;//yeni baglarin kurulmasi
        yeni->ogr_not_sonraki->ogr_not_onceki=yeni->ogr_not_onceki;

        yeni->ogr_not_sonraki=gecici;//yeni baglarin kurulmasi
        yeni->ogr_not_onceki=gecici->ogr_not_onceki;
        gecici->ogr_not_onceki->ogr_not_sonraki=yeni;
        gecici->ogr_not_onceki=yeni;
    }
    else if(yeni->ogr_not_sonraki!=liste_basi_ptr && yeni->ogr_not_sonraki->ogr_not<yeni->ogr_not)
    {
        gecici=yeni->ogr_not_sonraki;
        while(yeni->ogr_not_sonraki!=liste_basi_ptr && yeni->ogr_not>gecici->ogr_not_sonraki->ogr_not)
            gecici=gecici->ogr_not_sonraki;

        yeni->ogr_not_onceki->ogr_not_sonraki=yeni->ogr_not_sonraki;//yeni baglarin kurulmasi
        yeni->ogr_not_sonraki->ogr_not_onceki=yeni->ogr_not_onceki;

        yeni->ogr_not_sonraki=gecici->ogr_not_sonraki;//yeni baglarin kurulmasi
        yeni->ogr_not_onceki=gecici;
        gecici->ogr_not_sonraki->ogr_not_onceki=yeni;
        gecici->ogr_not_sonraki=yeni;
    }
}

void ogrenci_not_guncelle(Ders_dugumu *dersler[],Ogrenciler *koda_gore_hash_tablosu[])//ogrencinin ilgili dersten aldigi notunun guncellenmesi
{
    Notlar *gecici;
    Ogrenciler *bir_ogrenci;
    int kod,ogr_no,hash;
    float eski_ders_not;

    printf("Notunu guncellemek istediginiz dersin kodunu giriniz:");
    kod=sayi_al(10,89)-10;
    if(dersler[kod]!=NULL)
    {
        printf("Notunu Guncellemek istediginiz ogrencinin numarasini giriniz(1-10000):");
        ogr_no=sayi_al(1,10000);
        hash=(ogr_no-1)/100;
        bir_ogrenci=noya_gore_ara(koda_gore_hash_tablosu[hash],ogr_no);
        if(bir_ogrenci==NULL)
            printf("Girdiginiz %d nolu ogrenci yoktur\n",ogr_no);
        else
        {
            gecici=noya_gore_dairesel_liste_ara(dersler[kod]->not_durumu,ogr_no);//notlar dugumunde ogrenci numarasina gore dairesel tek bagli listede araniyor
            if(gecici!=NULL)
            {
                eski_ders_not=gecici->ogr_not;
                printf("Bu ogrencinin yeni notunu giriniz:");
                scanf("%d",&gecici->ogr_not);

                dersler[kod]->ders_not_ort+=(gecici->ogr_not-eski_ders_not)/dersler[kod]->ders_alan_ogr_say;//dersin yeni genel not ortalamasi hesaplaniyor
                bir_ogrenci->agirlik_not_ort+=((gecici->ogr_not-eski_ders_not)*dersler[kod]->ders_kredi)/bir_ogrenci->donem_kredi;//ogrencinin yeni donemlik agirlikli not ortalamasi hesaplaniyor

                dugum_tasi(dersler[kod]->not_durumu,gecici);//ogrencinin guncellenmis yeni notu notlar dugumunde uygun yere tasiniyor
                printf("Ogrencinin notu guncellendi\n");
            }
            else printf("Bu ogrenci bu dersi almamaktadir\n");
        }
    }
    else
        printf("Girdiginiz %d ders koduna sahip ders yoktur\n",kod+10);
}

Ogrenciler *noya_gore_listeden_cikar(Ogrenciler **ilk,int aranan_ogr_no)//ogrencinin dugumu tek bagli listeden cikartiliyor
{
    Ogrenciler *gecici,*onceki;

    if(*ilk==NULL) return NULL;
    else if((*ilk)->ogr_no==aranan_ogr_no)
    {
        gecici=*ilk;
        *ilk=(*ilk)->no_sonraki;
        return gecici;
    }
    else
    {
        onceki=*ilk;
        gecici=(*ilk)->no_sonraki;
        while(gecici!=NULL && aranan_ogr_no>gecici->ogr_no)
        {
            onceki=gecici;
            gecici=gecici->no_sonraki;
        }
        if(gecici!=NULL && gecici->ogr_no==aranan_ogr_no)
        {
            onceki->no_sonraki=gecici->no_sonraki;
            return gecici;
        }
        else return NULL;
    }
}

Notlar *noya_gore_dairesel_cikar(Notlar *liste_basi_ptr,int cikarilacak_ogr_no)//ogrencinin numaraya gore notlar dugumundeki yeri dairesel tek bagli listeden cikartiliyor
{
    Notlar *onceki, *gecici;

    onceki=liste_basi_ptr;
    gecici=liste_basi_ptr->no_sonraki;
    while(gecici!=liste_basi_ptr && gecici->ogr_no<cikarilacak_ogr_no)
    {
        onceki=gecici;
        gecici=gecici->no_sonraki;
    }
    if(gecici==liste_basi_ptr || gecici->ogr_no>cikarilacak_ogr_no)
        return NULL;
    else
    {
        onceki->no_sonraki=gecici->no_sonraki;
        return gecici;
    }
}

void nota_gore_dairesel_cift_baglidan_dugum_cikar(Notlar *cikarilacak)//ogrencinin notuna gore notlar dugumundeki yeri dairesel cift bagli listeden cikartiliyor
{
    cikarilacak->ogr_not_onceki->ogr_not_sonraki=cikarilacak->ogr_not_sonraki;
    cikarilacak->ogr_not_sonraki->ogr_not_onceki=cikarilacak->ogr_not_onceki;
}

void ogr_ders_sil(Ders_dugumu *dersler[],int silinecek_ogr_no)//ogrencinin silinmesini saglayan fonksiyon
{
    int i,bulundu=0;
    float yeni_ders_top;
    Notlar *bir_ogr_not;

    for(i=0; i<80; i++)//ogrencinin aldigi derslerin silinmesi icin butun isaretci dersler dizisi dolasiliyor
    {
        if(dersler[i]!=NULL)
        {
            bir_ogr_not=noya_gore_dairesel_cikar(dersler[i]->not_durumu,silinecek_ogr_no);
            if(bir_ogr_not!=NULL)
            {
                bulundu=1;
                dersler[i]->ders_alan_ogr_say--;//dersi alan ogrenci sayisi hesaplaniyor
                if(dersler[i]->ders_alan_ogr_say!=0)
                {
                    yeni_ders_top=(dersler[i]->ders_not_ort*(dersler[i]->ders_alan_ogr_say+1))-bir_ogr_not->ogr_not;
                    dersler[i]->ders_not_ort=yeni_ders_top/dersler[i]->ders_alan_ogr_say;// dersin yeni not ortalamasi hesaplaniyor
                }
                else dersler[i]->ders_not_ort=0;

                nota_gore_dairesel_cift_baglidan_dugum_cikar(bir_ogr_not);
                free(bir_ogr_not);// ogrencinin aldigi ders ve notlari siliniyor.
            }
        }
    }
    if(bulundu==0) printf("%d nolu ogrencinin silinecek dersi yoktur\n",silinecek_ogr_no);
}

void bir_dersi_alan_ogrenciler_listele(Ders_dugumu *dersler[],Ogrenciler *koda_gore_hash_tablosu[])//bir dersi alan ogrencileri listeleyen fonksiyon
{
    Notlar *gecici;
    Ogrenciler *bir_ogrenci;
    int kod,hash;

    printf("Goruntulemek istediginiz dersin kodunu giriniz(10,89):");
    kod=sayi_al(10,89)-10;
    if(dersler[kod]==NULL)
        printf("Girdiginiz %d ders koduna sahip ders yoktur.\n",kod+10);
    else
    {
        printf("\nDers Kodu Ders Adi       Kredi          Ogrenci Say         Not Ort\n");
        printf("--------- -------------- -------------- -----------------   -------\n");
        printf("   %d      %-15s %-12d  %-15d     %-15.2f\n",dersler[kod]->ders_kod,dersler[kod]->ders_adi,dersler[kod]->ders_kredi,dersler[kod]->ders_alan_ogr_say,dersler[kod]->ders_not_ort);

        if(dersler[kod]->not_durumu->no_sonraki!=dersler[kod]->not_durumu)//bu dersi alan ogrenci ya da ogrenciler varsa
        {
            printf("\nDersi Alan Ogrenciler:");
            printf("\nOgr No  Ad Soyad     Sinif         Notu\n");
            printf("------  ------------ ------------- -----\n");

            gecici=dersler[kod]->not_durumu->no_sonraki;
            while(gecici!=dersler[kod]->not_durumu)//numaraya gore sirali dairesel tek bagli liste olan notlar dugumu dolasiliyor
            {
                hash=(gecici->ogr_no-1)/100;
                bir_ogrenci=noya_gore_ara(koda_gore_hash_tablosu[hash],gecici->ogr_no);
                printf("%d     %-15s %-12d %-15d\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,gecici->ogr_not);
                gecici=gecici->no_sonraki;
            }
        }
        else
            printf("\nBu dersi alan ogrenci yoktur.\n");
    }
}

void bir_dersi_alan_notu_yuksek_olanlar(Ders_dugumu *dersler[],Ogrenciler *koda_gore_hash_tablosu[])
{
    Ogrenciler *bir_ogrenci;
    Notlar *gecici;
    int kod,hash,not_limit,limit_sayac=0,yaz_kontrol=0;

    printf("Goruntulemek istediginiz dersin kodunu giriniz(10,89):");
    kod=sayi_al(10,89)-10;
    if(dersler[kod]==NULL)
        printf("Girdiginiz %d ders koduna sahip ders yoktur.\n",kod+10);
    else
    {
        printf("Not limitini giriniz:");
        scanf("%d",&not_limit);
        if(dersler[kod]->not_durumu->no_sonraki!=dersler[kod]->not_durumu)// bu dersi alan ogrenci ya da ogrenciler varsa
        {
            gecici=dersler[kod]->not_durumu->ogr_not_onceki;
            while(gecici!=dersler[kod]->not_durumu && gecici->ogr_not>not_limit)//ogrenci notuna gore sirali dairesel cift bagli notlar dugumu not limitine gore oncekisinden baslanarak dolasiliyor
            {
                if(yaz_kontrol==0)
                {
                    printf("\nNotu Yuksek Olan Ogrenciler:");
                    printf("\nOgr No  Ad Soyad     Sinif         Notu\n");
                    printf("------  ------------ ------------- -----\n");
                    yaz_kontrol=1;
                }
                hash=(gecici->ogr_no-1)/100;
                bir_ogrenci=noya_gore_ara(koda_gore_hash_tablosu[hash],gecici->ogr_no);
                printf("%d     %-15s %-12d %-15d\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,gecici->ogr_not);
                limit_sayac++;

                gecici=gecici->ogr_not_onceki;
            }
            printf("\nNotu %d nin ustunde olan ogr sayisi %d ve yuzdesi %%%.2f\n",not_limit,limit_sayac,(float)limit_sayac*100/dersler[kod]->ders_alan_ogr_say);
        }
        else
            printf("Bu dersi alan ogrenci yoktur.\n");
    }
}

void bir_dersi_alan_notu_altinda_olanlar(Ders_dugumu *dersler[],Ogrenciler *koda_gore_hash_tablosu[])
{
    Ogrenciler *bir_ogrenci;
    Notlar *gecici;
    int kod,hash,not_limit,limit_sayac=0,yaz_kontrol=0;

    printf("Goruntulemek istediginiz dersin kodunu giriniz(10,89):");
    kod=sayi_al(10,89)-10;
    if(dersler[kod]==NULL)
        printf("Girdiginiz %d ders koduna sahip ders yoktur.\n",kod+10);
    else
    {
        printf("Not limitini giriniz:");
        scanf("%d",&not_limit);
        if(dersler[kod]->not_durumu->no_sonraki!=dersler[kod]->not_durumu)
        {
            gecici=dersler[kod]->not_durumu->ogr_not_sonraki;
            while(gecici!=dersler[kod]->not_durumu && gecici->ogr_not<not_limit)//ogrenci notuna gore sirali dairesel cift bagli notlar dugumu not limitine gore sonrakisinden baslanarak dolasiliyor
            {
                if(yaz_kontrol==0)
                {
                    printf("\nNotu Dusuk Olan Ogrenciler:");
                    printf("\nOgr No  Ad Soyad     Sinif         Notu\n");
                    printf("------  ------------ ------------- -----\n");
                    yaz_kontrol=1;
                }
                hash=(gecici->ogr_no-1)/100;
                bir_ogrenci=noya_gore_ara(koda_gore_hash_tablosu[hash],gecici->ogr_no);
                printf("%d     %-15s %-12d %-15d\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,gecici->ogr_not);
                limit_sayac++;

                gecici=gecici->ogr_not_sonraki;
            }
            printf("\nNotu %d nin altinda olan ogr sayisi %d ve yuzdesi %%%.2f\n",not_limit,limit_sayac,(float)limit_sayac*100/dersler[kod]->ders_alan_ogr_say);
        }
        else
            printf("Bu dersi alan ogrenci yoktur.\n");
    }
}

void bir_donem_ders_listele(Ders_dugumu *dersler[])//bir donemdeki dersleri listeleyen fonksiyonlar
{
    int donem,yaz_kontrol=0,sayac=0,ders_kredi_top=0;
    Ders_dugumu *bir_ders;

    printf("Derslerini gormek istediginiz donemi giriniz(1-8):");
    donem=(sayi_al(1,8)*10)-10;
    while(dersler[donem+sayac]!=NULL && sayac!=10)
    {
        if(yaz_kontrol==0)
        {
            printf("\nDers Kodu Ders Adi       Kredi          Ogrenci Say         Not Ort\n");
            printf("--------- -------------- -------------- -----------------   -------\n");
            yaz_kontrol=1;
        }
        bir_ders=dersler[donem+sayac];
        printf("   %d      %-15s %-12d  %-15d     %-15.2f\n",bir_ders->ders_kod,bir_ders->ders_adi,bir_ders->ders_kredi,bir_ders->ders_alan_ogr_say,bir_ders->ders_not_ort);
        ders_kredi_top+=bir_ders->ders_kredi;
        sayac++;
    }
    if(sayac!=0)
        printf("\nDerslerin kredileri toplami:%d\n",ders_kredi_top);
    else
        printf("Bu donem listelencek ders bulunmamaktadir.\n");
}

int bir_ogrenci_listele(Ogrenciler *koda_gore_hash_tablosu[])//bir ogrencinin bilgilerini listeleyen fonksiyon ogrenci yoksa 0 varsa ogrencinin kodunu dondurur
{
    int ogr_no,hash;
    Ogrenciler *bir_ogrenci;

    printf("Goruntulemek istediginiz Ogrenci numarasini giriniz(1-10000):");
    ogr_no=sayi_al(1,10000);
    hash=(ogr_no-1)/100;
    bir_ogrenci=noya_gore_ara(koda_gore_hash_tablosu[hash],ogr_no);
    if(bir_ogrenci==NULL)
    {
        printf("Bu numaraya sahip ogrenci yok\n");
        return 0;
    }
    else
    {
        printf("\nOgr No Ad Soyad       Sinif    Kredi Say           Not Ort\n");
        printf("------ -------------- -----    ---------           -------\n");
        printf("  %d  %-15s %-12d %-15d %-15.2f\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,bir_ogrenci->donem_kredi,bir_ogrenci->agirlik_not_ort);
        return ogr_no;
    }
}

void bir_ogrenci_aldigi_ders_listele(Ders_dugumu *dersler[],Ogrenciler *koda_gore_hash_tablosu[])// bir ogrencinin aldigi dersleri listeleyen fonksiyon
{
    Notlar *gecici;
    int kod,i,yaz_kontrol=0,bulundu=0;

    kod=bir_ogrenci_listele(koda_gore_hash_tablosu);
    if(kod!=0)
    {
        for(i=0; i<80; i++)//ogrencinin aldigi dersleri listelemek icin isaretci dersler dizisi dolasiliyor
        {
            if(dersler[i]!=NULL)
            {
                gecici=noya_gore_dairesel_liste_ara(dersler[i]->not_durumu,kod);
                if(gecici!=NULL)
                {
                    if(yaz_kontrol==0)
                    {
                        printf("\nBu Donem Aldigi Dersler:\n");
                        printf("Ders Kodu  Ders Adi      Kredi         Notu\n");
                        printf("---------  ------------- -----         ----\n");
                        yaz_kontrol=1;
                    }
                    bulundu=1;
                    printf("   %d      %-15s %-12d %-15d\n",dersler[i]->ders_kod,dersler[i]->ders_adi,dersler[i]->ders_kredi,gecici->ogr_not);
                }
            }
        }
        if (bulundu==0) printf("\n%d nolu ogrencinin bu donem aldigi ders yoktur.\n",kod);
    }
}

void ogrenci_sinif_listele(Ogrenciler *koda_gore_hash_tablosu[])//sinifa gore ogrencileri listeleyen fonksiyon
{
    int sinif,i,sinif_ogr_say=0,sinif_ogr_say2=0,yaz_kontrol=0;
    float sinif_top_ort=0;
    Ogrenciler *gecici;

    printf("Ogrencilerini gormek istediginiz sinifin numarasini giriniz:");
    scanf("%d",&sinif);
    for(i=0; i<100; i++)//isaretci ogrenciler dizisi dolasiliyor
    {
        gecici=koda_gore_hash_tablosu[i];
        while(gecici!=NULL)
        {
            if(gecici->sinif==sinif)
            {
                if(yaz_kontrol==0)
                {
                    printf("\n Ogr No        Ad Soyad      Sinif          Kredi Say          Not Ort\n");
                    printf(" ------------- ------------  -------------  -----------------  -------\n");
                    yaz_kontrol=1;
                }
                printf("  %-15d%-12s  %-15d  %-15d %-15.2f\n",gecici->ogr_no,gecici->ad_soyad,gecici->sinif,gecici->donem_kredi,gecici->agirlik_not_ort);
                sinif_top_ort+=gecici->agirlik_not_ort;

                if(gecici->agirlik_not_ort<60) sinif_ogr_say2++;

                sinif_ogr_say++;
            }
            gecici=gecici->no_sonraki;
        }
    }
    if(sinif_ogr_say!=0)
    {
        printf("\nSiniftaki ogrenci sayisi:%d",sinif_ogr_say);
        printf("\nSinifin genel not ortalamasi:%.2f\n",sinif_top_ort/sinif_ogr_say);
        printf("Donemlik agirlikli not ortalamasi 60 in altinda ogrenci sayisi %d ve yuzdesi %%%.2f\n",sinif_ogr_say2,(float)sinif_ogr_say2*100/sinif_ogr_say);
    }
    else
        printf("Bu sinifta listelencek ogrenci yoktur.\n");
}
