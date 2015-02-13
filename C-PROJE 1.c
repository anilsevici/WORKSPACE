#include <stdio.h>
#include <stdlib.h>

#define BOS_KAYIT_KODU 0
#define MAX_KAYIT_SAY 900
#define MAX_OGRENCI_SAY 150

struct ders
{
    int ders_kodu;
    char ders_adi[26];
    int ders_kredi;
    char ogretim_uye[31];
    char derslik_adi[21];
};

struct ogrenci
{
    int ogrenci_no;
    char ogrenci_ad[31];
    int sinif;
    int donem_kredi;
    float agirlik_not_ort;
};

struct notlar
{
    int ders_kodu;
    int ogrenci_no;
    int ogrenci_not;
};

typedef struct ders Ders;
typedef struct ogrenci Ogrenci;
typedef struct notlar Notlar;

int ogrenci_goruntule(FILE *);
void donem_aldigi_ders(FILE *,FILE *);
void tum_ogrenci_listele(FILE *);
void donem_basari_liste(FILE *);
int ders_listele(FILE *);
void dersi_alan(FILE *,FILE *);
void yeni_ogrenci_ekle(FILE *);
void ogretim_uye_derslik_degistir(FILE *);
void ogrenci_sil(FILE *);
void bir_donem_ders_listele(FILE *);
void yeni_ders_ekle(FILE *);
void ogrenci_not_gir(FILE *,FILE *);
int menu_secim();
int sayi_al(int,int);

int main()
{
    FILE *fptr;
    FILE *cfptr;
    int secenek;

    if((fptr=fopen("dersler.dat","rb+"))==NULL)
        printf("Dersler dosyasi acilamadi.\n");
    else if((cfptr=fopen("ogrenciler.dat","rb+"))==NULL)
        printf("Ogrenciler dosyasi acilamadi.\n");
    else
    {
        do
        {
            secenek=menu_secim();
            switch(secenek)
            {
            case 1:
                ogrenci_goruntule(cfptr);
                break;
            case 2:
                donem_aldigi_ders(cfptr,fptr);
                break;
            case 3:
                tum_ogrenci_listele(cfptr);
                break;
            case 4:
                donem_basari_liste(cfptr);
                break;
            case 5:
                ders_listele(fptr);
                break;
            case 6:
                dersi_alan(cfptr,fptr);
                break;
            case 7:
                bir_donem_ders_listele(fptr);
                break;
            case 8:
                yeni_ogrenci_ekle(cfptr);
                break;
            case 9:
                yeni_ders_ekle(fptr);
                break;
            case 10:
                ogretim_uye_derslik_degistir(fptr);
                break;
            case 11:
                ogrenci_not_gir(cfptr,fptr);
                break;
            case 12:
                ogrenci_sil(cfptr);
                break;
            }
        }
        while(secenek!=13);
    }
    printf("\nOgrenci Takip Sistemi Programindan Ciktiniz.\n");
    fclose(fptr);//Dosyalar kapaniyor.
    fclose(cfptr);

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

    printf("\n  1. Bir ogrencinin bilgilerinin listelenmesi\n");
    printf("  2. Bir ogrencinin bilgilerinin ve bu donem aldigi derslerin listelenmesi\n");
    printf("  3. Bir sinifta okuyan ogrencilerin listelenmesi\n");
    printf("  4. Bolumun bu doneme ait basari istatistiklerinin listelenmesi\n");
    printf("  5. Bir dersin bilgilerinin listelenmesi\n");
    printf("  6. Bir dersin bilgilerinin ve bu donem alan ogrencilerin listelenmesi\n");
    printf("  7. Bir donemde verilen derslerin bilgilerinin listelenmesi\n");
    printf("  8. Yeni bir ogrencinin eklenmesi\n");
    printf("  9. Yeni bir dersin eklenmesi\n");
    printf(" 10. Bir dersin ogretim uyesinin ve dersliginin degistirilmesi\n");
    printf(" 11. Bir dersi alan ogrencilerin notlarinin girilmesi\n");
    printf(" 12. Bir ogrencinin kaydinin silinmesi\n");
    printf(" 13. Cikis\n\n");

    printf("\nLutfen Seciminizi Giriniz:");
    secim=sayi_al(1,13);

    return secim;
}

int ogrenci_goruntule(FILE *cfptr)
{
    Ogrenci ogrenciler;
    int kod;

    printf("Goruntulemek istediginiz ogrencinin numarasini giriniz(100-999):");
    kod=sayi_al(100,999);

    fseek(cfptr,(kod-100)*sizeof(Ogrenci),SEEK_SET);//dosya pozisyon gostericisi anahtar degere göre konumlaniyor.
    fread(&ogrenciler,sizeof(Ogrenci),1,cfptr);//Kayit Ogrenci yapisina aktariliyor.
    if(ogrenciler.ogrenci_no==BOS_KAYIT_KODU)//Kaydin bos olup olmadigi kontrol ediliyor.
    {
        printf("Girdiginiz %d nolu ogrenci numarasina sahip ogrenci bulunmamaktadir.\n",kod);
        return BOS_KAYIT_KODU;//Ogrenci yoksa 0 dondurur.
    }
    else
    {
        printf("\nOgr No Ad Soyad       Sinif    Kredi Say           Not Ort\n");
        printf("------ -------------- -----    ---------           -------\n");
        printf("  %d  %-15s %-12d %-15d %-15.2f\n",ogrenciler.ogrenci_no,ogrenciler.ogrenci_ad,ogrenciler.sinif,ogrenciler.donem_kredi,ogrenciler.agirlik_not_ort);
        return kod;//Ogrenci varsa ogrenci numarasi dondurur.
    }
}

void donem_aldigi_ders(FILE *cfptr,FILE *fptr)
{
    FILE *dosya;
    Ders dersler;
    Notlar ogrenci_notlar;
    int kod,bulundu=0,yaz_kontrol=0;

    dosya=fopen("notlar.txt","r");
    kod=ogrenci_goruntule(cfptr);//ogrenci goruntule fonksiyonu cagrilip ogrenci numarasi ya da 0 dondurur.Ogrenci varsa o ogrencinin bilgilerini yazdirir.

    if(kod!=BOS_KAYIT_KODU)
    {
        fscanf (dosya, "%d %d %d", &ogrenci_notlar.ders_kodu,&ogrenci_notlar.ogrenci_no,&ogrenci_notlar.ogrenci_not);
        while(!feof(dosya))//Notlar.txt dosyasi sonuna kadar okunuyor.
        {
            if(kod==ogrenci_notlar.ogrenci_no)//Ogrenci numarasi notlar.txt dosyasindaki ogrenci numarasiyla ayni olup olmadigi kontrol ediliyor.
            {
                if(yaz_kontrol==0)
                {
                    printf("\nBu Donem Aldigi Dersler:\n");
                    printf("Ders Kodu  Ders Adi      Kredi         Notu\n");
                    printf("---------  ------------- -----         ----\n");
                    yaz_kontrol=1;
                }
                fseek(fptr,(ogrenci_notlar.ders_kodu-1)*sizeof(Ders),SEEK_SET);//notlar.txt dosyasindaki ogrencinin ders kodu dersler.dat dosyasindaki ders kodunun oldugu yere konumlaniyor.
                fread(&dersler,sizeof(Ders),1,fptr);//Kayit Ders yapisina aktariliyor.
                bulundu=1;

                printf("   %d      %-15s %-12d %-15d\n",dersler.ders_kodu,dersler.ders_adi,dersler.ders_kredi,ogrenci_notlar.ogrenci_not);
            }
            fscanf (dosya, "%d %d %d", &ogrenci_notlar.ders_kodu,&ogrenci_notlar.ogrenci_no,&ogrenci_notlar.ogrenci_not);
        }
        if (bulundu==0) printf("\n%d nolu ogrencinin bu donem aldigi ders yoktur.\n",kod);
    }
    fclose(dosya);
}

void tum_ogrenci_listele(FILE *cfptr)
{
    Ogrenci gecici;
    Ogrenci ogrenciler[MAX_OGRENCI_SAY];
    int i,j,sinif,sayac=0,sayac2=0,yer_degisti;
    float top_sinif=0;

    printf("Ogrencilerini gormek istediginiz sinifin numarasini giriniz:");
    scanf("%d",&sinif);

    rewind(cfptr);//Ogrenciler.dat dosyasi basa konumlaniyor.

    for(i=0; i<MAX_KAYIT_SAY; i++)
    {
        fread(&ogrenciler[sayac],sizeof(Ogrenci),1,cfptr);//Ogrenci.dat dosyasi en bastan okunuyor.

        if(ogrenciler[sayac].ogrenci_no!=BOS_KAYIT_KODU && ogrenciler[sayac].sinif==sinif)
        {
            top_sinif+=ogrenciler[sayac].agirlik_not_ort;

            if(ogrenciler[sayac].agirlik_not_ort<60)   sayac2++;

            sayac++;
        }
    }
    if(sayac!=0)
    {
        for(i=0; i<sayac-1; i++)//Kabarcik siralama yapiliyor.
        {
            yer_degisti=1;
            for(j=0; j<sayac-1; j++)
            {
                if(ogrenciler[j].agirlik_not_ort<ogrenciler[j+1].agirlik_not_ort)
                {
                    gecici=ogrenciler[j];
                    ogrenciler[j]=ogrenciler[j+1];
                    ogrenciler[j+1]=gecici;
                    yer_degisti=0;
                }
                else if(ogrenciler[j].agirlik_not_ort==ogrenciler[j+1].agirlik_not_ort && ogrenciler[j].donem_kredi<ogrenciler[j+1].donem_kredi)
                {
                    gecici=ogrenciler[j];
                    ogrenciler[j]=ogrenciler[j+1];
                    ogrenciler[j+1]=gecici;
                    yer_degisti=0;
                }
            }
            if(yer_degisti==1)  break;
        }

        printf("\nSira No Ogr No            Ad Soyad    Sinif          Kredi Say           Not Ort\n");
        printf("------- ----------------- --------    -----          ---------           -------\n");

        for(i=0; i<sayac; i++)
            printf("    %d     %-15d %-12s  %-15d  %-15d %-15.2f\n",i+1,ogrenciler[i].ogrenci_no,ogrenciler[i].ogrenci_ad,ogrenciler[i].sinif,ogrenciler[i].donem_kredi,ogrenciler[i].agirlik_not_ort);

        printf("\nSinifin genel not ortalamasi:%.2f\n",top_sinif/sayac);
        printf("Donemlik agirlikli not ortalamasi 60 in altinda ogrenci sayisi %d ve yuzdesi %%%.2f\n",sayac2,(float)sayac2*100/sayac);
    }
    else
        printf("Bu sinifta ogrenci yoktur.\n");
}

void donem_basari_liste(FILE *cfptr)
{
    Ogrenci ogrenciler;
    int i,indeks,k=80,sayac=0,donem_notlar[10]= {0};

    rewind(cfptr);

    for(i=0; i<MAX_KAYIT_SAY; i++)
    {
        fread(&ogrenciler,sizeof(Ogrenci),1,cfptr);

        if(ogrenciler.ogrenci_no!=BOS_KAYIT_KODU)
        {
            indeks=ogrenciler.agirlik_not_ort/10;//Not araliklarina gore notlarin indeksleri bulunuyor.

            if(indeks==10)   indeks=9;

            donem_notlar[indeks]++;
            sayac++;
        }
    }
    printf("\nNot Araligi  Ogrenci Sayisi  Ogrenci Yuzdesi\n");
    printf("-----------  --------------  ---------------\n");
    printf("   90-100            %-13d%%%-13.2f\n",donem_notlar[9],(float)donem_notlar[9]*100/sayac);

    for(i=8; i>=0; i--)
    {
        printf("%5d-%3d            %-13d%%%-13.2f\n",k,k+9,donem_notlar[i],(float)donem_notlar[i]*100/sayac);
        k=k-10;
    }
}

int ders_listele(FILE *fptr)
{
    Ders dersler;
    int kod;

    printf("Goruntulemek istediginiz dersin kodunu giriniz(10-89):");
    kod=sayi_al(10,89);

    fseek(fptr,(kod-1)*sizeof(Ders),SEEK_SET);//Dosya pozisyon gostericisi dersler.dat dosyasinda ders kodua göre konumlaniyor.
    fread(&dersler,sizeof(Ders),1,fptr);//Kayit Ders yapisina aktariliyor.
    if(dersler.ders_kodu==BOS_KAYIT_KODU)
    {
        printf("Girdiginiz %d nolu ders koduna sahip ders yoktur\n",kod);
        return BOS_KAYIT_KODU;//Ders yoksa 0 dondurur.
    }
    else
    {
        printf("\nDers Kodu Ders Adi       Kredi          Ogretim Uyesi   Derslik Adi\n");
        printf("--------- -------------- -------------- -------------   -----------\n");
        printf("   %d      %-15s %-12d  %-15s      %-15s\n",dersler.ders_kodu,dersler.ders_adi,dersler.ders_kredi,dersler.ogretim_uye,dersler.derslik_adi);
        return kod;//Ders varsa ders kodunu dondurur.
    }
}

void dersi_alan(FILE *cfptr,FILE *fptr)
{
    FILE *dosya;
    Ogrenci ogrenciler;
    Notlar ogrenci_notlar;
    int kod,ders_sayac=0,ders_sayac2=0,bulundu=0,yaz_kontrol=0;
    float top_ders=0;

    dosya=fopen("notlar.txt","r");
    kod=ders_listele(fptr);//ders listele fonksiyonu cagrilip ders varsa ders kodunu yoksa 0 dondurur.Ders varsa o dersin bilgilerini yazdirir.

    if(kod!=BOS_KAYIT_KODU)
    {
        fscanf (dosya, "%d %d %d", &ogrenci_notlar.ders_kodu,&ogrenci_notlar.ogrenci_no,&ogrenci_notlar.ogrenci_not);
        while(!feof(dosya))//notlar.txt dosyasi sonuna kadar okunuyor.
        {
            if(kod==ogrenci_notlar.ders_kodu)
            {
                if(yaz_kontrol==0)
                {
                    printf("\nBu donem Alan Ogrenciler:");
                    printf("\nOgr No  Ad Soyad     Sinif         Notu\n");
                    printf("------  ------------ ------------- -----\n");
                    yaz_kontrol=1;
                }
                fseek(cfptr,(ogrenci_notlar.ogrenci_no-100)*sizeof(Ogrenci),SEEK_SET);//notlar.txt dosyasindaki ogrenci numarasi ogrenciler.dat dosyasindaki ogrencinin numarasinin oldugu yere konumlaniyor.
                fread(&ogrenciler,sizeof(Ogrenci),1,cfptr);//Kayit Ogrenci yapisina aktariliyor.
                bulundu=1;

                printf("%d     %-15s %-12d %-15d\n",ogrenciler.ogrenci_no,ogrenciler.ogrenci_ad,ogrenciler.sinif,ogrenci_notlar.ogrenci_not);
                top_ders+=ogrenci_notlar.ogrenci_not;

                if(ogrenci_notlar.ogrenci_not<60)  ders_sayac2++;

                ders_sayac++;
            }
            fscanf (dosya, "%d %d %d", &ogrenci_notlar.ders_kodu,&ogrenci_notlar.ogrenci_no,&ogrenci_notlar.ogrenci_not);
        }

        if (bulundu==0)
            printf("\n%d nolu dersi bu donem alan ogrenci yoktur.\n",kod);
        else
        {
            printf("\nDersi alan ogrenci sayisi:%d\n",ders_sayac);
            printf("Dersin genel not ortalamasi:%.2f\n",top_ders/ders_sayac);
            printf("Notu 60 in altinda olan ogrenci sayisi  %d  ve yuzdesi %%%.2f\n",ders_sayac2,(float)ders_sayac2*100/ders_sayac);
        }
    }
    fclose(dosya);
}

void bir_donem_ders_listele(FILE *fptr)
{
    Ders dersler;
    int i,donem,yaz_kontrol=0,bulundu=0,ders_kredi_top=0;

    printf("Derslerini gormek istediginiz donemi giriniz(1-8):");
    donem=sayi_al(1,8);

    fseek(fptr,(donem*10-1)*sizeof(Ders),SEEK_SET);//Donem alindiktan sonra dersler.dattaki ilgili yere konumlaniyor.

    for(i=0; i<10; i++)//Bir donemde en fazla 10 ders verildiginden dongu 10 kere donuyor.
    {
        fread(&dersler,sizeof(Ders),1,fptr);

        if(dersler.ders_kodu!=BOS_KAYIT_KODU)
        {
            if(yaz_kontrol==0)
            {
                printf("\nDers Kodu Ders Adi       Kredi          Ogretim Uyesi   Derslik Adi\n");
                printf("--------- -------------- -------------- -------------   -----------\n");
                yaz_kontrol=1;
            }
            bulundu=1;
            printf("   %d      %-15s %-12d  %-15s      %-15s\n",dersler.ders_kodu,dersler.ders_adi,dersler.ders_kredi,dersler.ogretim_uye,dersler.derslik_adi);
            ders_kredi_top+=dersler.ders_kredi;
        }
    }
    if(bulundu==1)
        printf("\nDerslerin kredileri toplami:%d\n",ders_kredi_top);
    else
        printf("Bu donem listelencek ders bulunmamaktadir.\n");
}

void yeni_ogrenci_ekle(FILE *cfptr)
{
    Ogrenci ogrenciler;
    int kod;

    printf("Eklemek istediginiz ogrencinin numarasini giriniz:");
    kod=sayi_al(100,999);

    fseek(cfptr,(kod-100)*sizeof(Ogrenci),SEEK_SET);
    fread(&ogrenciler,sizeof(Ogrenci),1,cfptr);
    if(ogrenciler.ogrenci_no==BOS_KAYIT_KODU)//Bos kayit olan yer bulunduktan sonra ogrenciye iliskin bilgiler aliniyor.
    {
        ogrenciler.ogrenci_no=kod;
        ogrenciler.agirlik_not_ort=0;
        ogrenciler.donem_kredi=0;

        printf("Ogrencinin adini soyadini giriniz:");
        fflush(stdin);
        gets(ogrenciler.ogrenci_ad);
        printf("Ogrencinin sinifini giriniz:");
        scanf("%d",&ogrenciler.sinif);

        fseek(cfptr,(kod-100)*sizeof(Ogrenci),SEEK_SET);
        fwrite(&ogrenciler,sizeof(Ogrenci),1,cfptr);
        printf("\nYeni ogrenci ekleme basari ile gerceklesti\n");
    }
    else
        printf("Girdiginiz %d nolu ogrenci numarasina sahip ogrenci bulunmaktadir.\n",kod);
}

void yeni_ders_ekle(FILE *fptr)
{
    Ders dersler;
    int i,donem,bulundu=0;

    printf("Eklemek istediginiz dersin donemini giriniz(1-8):");
    donem=sayi_al(1,8);

    fseek(fptr,(donem*10-1)*sizeof(Ders),SEEK_SET);

    for(i=0; i<10; i++)
    {
        fread(&dersler,sizeof(Ders),1,fptr);

        if(dersler.ders_kodu==BOS_KAYIT_KODU)
        {
            dersler.ders_kodu=i+(donem*10);//ilk bos kayida ders kodu ataniyor
            bulundu=1;
            break;//ilk bos yer bulunduktan sonra donguden cikiliyor
        }
    }
    if(bulundu==1)//Yeni ders icin bilgiler aliniyor
    {
        printf("\nEklenecek dersin kodu:%d\n",dersler.ders_kodu);

        printf("\nYeni dersin adini giriniz:");
        fflush(stdin);
        gets(dersler.ders_adi);

        printf("Yeni dersin kredisini giriniz:");
        scanf("%d",&dersler.ders_kredi);

        printf("Yeni dersin ogretim uyesini giriniz:");
        fflush(stdin);
        gets(dersler.ogretim_uye);

        printf("Yeni dersin dersligini giriniz:");
        fflush(stdin);
        gets(dersler.derslik_adi);

        fseek(fptr,(donem*10+i-1)*sizeof(Ders),SEEK_SET);
        fwrite(&dersler,sizeof(Ders),1,fptr);

        printf("\nYeni ders ekleme islemi basari ile gerceklestirildi.\n");
    }
    else
        printf("Bu doneme ders eklemek icin yer yoktur.Bir donemde en cok 10 ders verilebilir.\n");
}

void ogretim_uye_derslik_degistir(FILE *fptr)
{
    Ders dersler;
    int kod;

    printf("Guncellemek istediginiz dersin bilgilerinin kodunu giriniz(10-89):");
    kod=sayi_al(10,89);

    fseek(fptr,(kod-1)*sizeof(Ders),SEEK_SET);
    fread(&dersler,sizeof(Ders),1,fptr);
    if(dersler.ders_kodu==BOS_KAYIT_KODU)
        printf("Girdiginiz %d nolu kod numarasina sahip ders bulunmamaktadir.\n",kod);
    else
    {
        printf("Bu dersin yeni ogretim uyesinin adini soyadini giriniz:");
        fflush(stdin);
        gets(dersler.ogretim_uye);
        printf("Bu dersin yeni dersligini giriniz:");
        fflush(stdin);
        gets(dersler.derslik_adi);

        fseek(fptr,(kod-1)*sizeof(Ders),SEEK_SET);
        fwrite(&dersler,sizeof(Ders),1,fptr);//Degistirilmek istenen dersin bilgileri alinip dersler.data yazdiriliyor.

        printf("\nDersin bilgileri basari ile guncellendi.\n");
    }
}

void ogrenci_not_gir(FILE *cfptr,FILE *fptr)
{
    FILE *dosya;
    Ogrenci ogrenciler;
    Ders dersler;
    Notlar ogrenci_notlar;
    int kod;
    float top_not,top_not2;
    char cevap;

    dosya=fopen("notlar.txt","a+");//Notlar.txtin sonuna yazdirmak icin a+ ile aciliyor.

    printf("Goruntulemek istediginiz dersin kodunu giriniz(10-89):");
    kod=sayi_al(10,89);

    fseek(fptr,(kod-1)*sizeof(Ders),SEEK_SET);
    fread(&dersler,sizeof(Ders),1,fptr);
    if(dersler.ders_kodu==BOS_KAYIT_KODU)
        printf("Girdiginiz %d nolu kod numarasina sahip ders bulunmamaktadir.\n",kod);
    else
    {
        do
        {
            printf("Ekleme yapmak istediginiz ogrencinin numarasini giriniz:");
            kod=sayi_al(100,999);

            fseek(cfptr,(kod-100)*sizeof(Ogrenci),SEEK_SET);
            fread(&ogrenciler,sizeof(Ogrenci),1,cfptr);
            if(ogrenciler.ogrenci_no==BOS_KAYIT_KODU)
            {
                printf("Ekleme yapmak istediginiz %d nolu ogrenci bulunmamaktadir.\n",kod);

                do
                {
                    printf("Yeni ogrenci numarasi girecekmisiniz?(E/e/H/h):");
                    fflush(stdin);
                    cevap=getchar();
                }
                while(cevap!='E' && cevap!='e' && cevap!='H' && cevap!='h');

                if(cevap=='H' || cevap=='h') break;
            }
            else
            {
                ogrenci_notlar.ders_kodu=dersler.ders_kodu;
                ogrenci_notlar.ogrenci_no=ogrenciler.ogrenci_no;

                printf("Ogrencinin dersten aldigi notu giriniz:");
                scanf("%d",&ogrenci_notlar.ogrenci_not);

                fprintf(dosya,"%d %d %d\n",ogrenci_notlar.ders_kodu,ogrenci_notlar.ogrenci_no,ogrenci_notlar.ogrenci_not);//Ogrencinin dersten aldigi not notlar.txtin sonuna yazdiriliyor.

                fseek(cfptr,(kod-100)*sizeof(Ogrenci),SEEK_SET);

                top_not=ogrenciler.agirlik_not_ort*ogrenciler.donem_kredi;//Agirlikli not ortalamasi hesabi islemleri yapiliyor.
                top_not2=ogrenci_notlar.ogrenci_not*dersler.ders_kredi;
                ogrenciler.donem_kredi+=dersler.ders_kredi;
                ogrenciler.agirlik_not_ort=(top_not+top_not2)/ogrenciler.donem_kredi;

                fwrite(&ogrenciler,sizeof(Ogrenci),1,cfptr);//Agirlikli not ortalamasi guncellenip yazdiriliyor.

                do
                {
                    printf("Yeni ogrenci numarasi girecekmisiniz?(E/e/H/h):");
                    fflush(stdin);
                    cevap=getchar();
                }
                while(cevap!='E' && cevap!='e' && cevap!='H' && cevap!='h');
            }
        }
        while(cevap=='E' || cevap=='e');
    }
    fclose(dosya);
}

void ogrenci_sil(FILE *cfptr)
{
    FILE *dosya;
    FILE *dosya2;
    Ogrenci ogrenciler;
    Notlar ogrenci_notlar;
    int kod;

    dosya=fopen("notlar.txt","r");

    printf("Silmek istediginiz ogrencinin numarasini giriniz(100-999):");
    kod=sayi_al(100,999);

    fseek(cfptr,(kod-100)*sizeof(Ogrenci),SEEK_SET);
    fread(&ogrenciler,sizeof(Ogrenci),1,cfptr);
    if(ogrenciler.ogrenci_no==BOS_KAYIT_KODU)
    {
        printf("Silmek istediginiz %d nolu ogrenci bulunmamaktadir.\n",kod);
        fclose(dosya);
    }
    else
    {
        dosya2=fopen("gecici.txt","w");//gecici.txt dosyasi yazdirmak icin yaratiliyor.

        fscanf (dosya, "%d %d %d", &ogrenci_notlar.ders_kodu,&ogrenci_notlar.ogrenci_no,&ogrenci_notlar.ogrenci_not);
        while(!feof(dosya))
        {
            if(ogrenciler.ogrenci_no!=ogrenci_notlar.ogrenci_no)//Ogrencinin bilgileri notlar.txtden dosyasindan siliniyor.
                fprintf(dosya2, "%d %d %d\n",ogrenci_notlar.ders_kodu,ogrenci_notlar.ogrenci_no,ogrenci_notlar.ogrenci_not);

            fscanf (dosya, "%d %d %d", &ogrenci_notlar.ders_kodu,&ogrenci_notlar.ogrenci_no,&ogrenci_notlar.ogrenci_not);
        }

        fclose(dosya2);
        fclose(dosya);

        remove("notlar.txt");
        rename("gecici.txt","notlar.txt");

        fseek(cfptr,(kod-100)*sizeof(Ogrenci),SEEK_SET);
        ogrenciler.ogrenci_no=BOS_KAYIT_KODU;//Bos kayit anlamina gelen sayi ogrenci numarasina ataniyor.
        fwrite(&ogrenciler,sizeof(Ogrenci),1,cfptr);//Ogrencinin bilgileri silinip ogrenciler.data yazdiriliyor.

        printf("\nOgrenci basariyla silindi.\n");
    }
}
