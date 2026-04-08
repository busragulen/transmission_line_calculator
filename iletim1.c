// computer-aided transmission line parameter calculator for different pole types and conductor types.
// kocaeli university, electrical engineering department, 2026 spring semester
// electric power transmission course project
// written by busra gulen, 220205029 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <string.h>

// sabit degerler
#define PI 3.14159265358979323846
#define f 50.0
#define d 0.40 // demet araligi

// veriler
// iletken yapisi
typedef struct {
    char isim[50];
    double kesit_mm2;
    double dis_cap_mm;
    double gmr_mm;
    double r_ac_50hz; // ohm/km (50hz ac direnc)
} Iletken;

// koordinatli direk yapisi sistemi
typedef struct {
    char isim[20];
    int tip; // 1: tek devre demetsiz, 2: tek devre 3'lu demet, 3: cift devre demetsiz, 4: cift devre 2'li demet
    // koordinatlar: tek devre için ilk 3 indeks (A, B, C) 
    // cift devre için 6 indeks (A, B, C, A', B', C') 
    double x[6]; 
    double y[6]; 
} Direk;

// karmasik sayi formati
typedef struct {
    double R;
    double L;
    double C_cap;
    double complex Z;
    double complex Y;
    double complex A;
    double complex B;
    double complex C_param;
    double complex D;
} HatParametreleri;

// iki nokta arasındaki mesafeyi hesaplayan func
double mesafe_hesapla(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// kutupsal formatta yazdirma func fazor
void print_polar(double complex z) {
    double genlik = cabs(z);
    double aci = carg(z) * (180.0 / PI);
    printf("%8.4f L %6.2f°", genlik, aci);
}

int main() {
    // acisal frrekans
    double omega = 2.0 * PI * f;

    // iletkenler 
    Iletken iletkenler[8] = {
        {"795 MCM Drake", 402.8, 28.14, 11.37, 0.0853},
        {"795 MCM Tern", 402.8, 27.00, 10.73, 0.08907},
        {"954 MCM RAIL", 483.4, 29.59, 11.99, 0.0595},
        {"1192.5 MCM BUNTING", 604.2, 33.07, 13.41, 0.0470},
        {"477 MCM Hawk", 241.7, 21.79, 8.814, 0.1196}, 
        {"336.4 MCM Linnet", 170.5, 18.28, 7.402, 0.1692}, 
        {"954 MCM Cardinal", 483.4, 30.38, 12.25, 0.0590},
        {"1272 MCM Pheasant", 645.0, 35.10, 14.17, 0.0430}
    };

    // direkler koordinatli 
    Direk direkler[4] = {
        // PA tipi tasiyici - 1. direk isterine gore koordinatlar: A(4.5), B(4.5), C(9) GMD=5.67m
        {"PA Tasiyici", 1, {-4.5, 0.0, 4.5, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        
        // 3PA1 (tek dvre 3'lu demet - yatay dizilim) D = 11.9m
        {"3PA1 Tasiyici", 2, {-11.9, 0.0, 11.9, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        
        // TA (Cift devre demetsiz - Drake serisi standart tasiyici)
        // Sag: A(4.0, 8.0), B(4.0, 4.0), C(4.0, 0) | Sol: A'(-4.0, 8.0), B'(-4.0, 4.0), C'(-4.0, 0)
        {"154kV TA Tas.", 3, 
            {4.00, 4.00, 4.00, -4.00, -4.00, -4.00}, 
            {8.00, 4.00, 0.00, 8.00, 4.00, 0.00}},
            
        // 2A (cift devre 2'li demet )
        {"2A Cift 2'li", 4, 
            {8.60, 8.60, 8.60, -8.60, -8.60, -8.60}, 
            {12.0, 6.00, 0.00, 12.0, 6.00, 0.00}}
    };

    HatParametreleri sonuclar[4];

    // user inputs
    int secim;
    double l;
    
    printf("--- Grup 6: Enerji Iletim Hatti Parametre Hesaplama ---\n");
    printf("Kullanilabilir Iletkenler:\n");
    for(int i=0; i<8; i++) {
        printf("%d. %s\n", i+1, iletkenler[i].isim);
    }
    printf("Iletken seciniz (1-8): ");
    scanf("%d", &secim);
    secim--; 

    if (secim < 0 || secim > 7) {
        printf("Hatali secim! Lutfen 1-8 arasi bir deger giriniz.\n");
        return 1;
    }

    printf("Hat uzunlugunu giriniz (km) [Varsayilan icin 200 girin]: ");
    scanf("%lf", &l);

    Iletken secilen_iletken = iletkenler[secim];
    double r_iletken_m = (secilen_iletken.dis_cap_mm / 2.0) / 1000.0; 
    double gmr_iletken_m = secilen_iletken.gmr_mm / 1000.0; 

    for(int i = 0; i < 4; i++) {
        int tip = direkler[i].tip;
        double gmd, gmr_L, gmr_C, r_esdeger;

        if(tip == 1 || tip == 2) {
            double D_AB = mesafe_hesapla(direkler[i].x[0], direkler[i].y[0], direkler[i].x[1], direkler[i].y[1]);
            double D_BC = mesafe_hesapla(direkler[i].x[1], direkler[i].y[1], direkler[i].x[2], direkler[i].y[2]);
            double D_CA = mesafe_hesapla(direkler[i].x[2], direkler[i].y[2], direkler[i].x[0], direkler[i].y[0]);
            
            gmd = cbrt(D_AB * D_BC * D_CA);

            if(tip == 1) { 
                gmr_L = gmr_iletken_m; 
                gmr_C = r_iletken_m; 
                r_esdeger = secilen_iletken.r_ac_50hz;
            } else { 
                gmr_L = cbrt(gmr_iletken_m * d * d);
                gmr_C = cbrt(r_iletken_m * d * d);
                r_esdeger = secilen_iletken.r_ac_50hz / 3.0;
            }
        } 
        else {
            double D_AB_eq = pow(
                mesafe_hesapla(direkler[i].x[0], direkler[i].y[0], direkler[i].x[1], direkler[i].y[1]) * mesafe_hesapla(direkler[i].x[0], direkler[i].y[0], direkler[i].x[4], direkler[i].y[4]) * mesafe_hesapla(direkler[i].x[3], direkler[i].y[3], direkler[i].x[1], direkler[i].y[1]) * mesafe_hesapla(direkler[i].x[3], direkler[i].y[3], direkler[i].x[4], direkler[i].y[4]), 0.25);
                
            double D_BC_eq = pow(
                mesafe_hesapla(direkler[i].x[1], direkler[i].y[1], direkler[i].x[2], direkler[i].y[2]) * mesafe_hesapla(direkler[i].x[1], direkler[i].y[1], direkler[i].x[5], direkler[i].y[5]) * mesafe_hesapla(direkler[i].x[4], direkler[i].y[4], direkler[i].x[2], direkler[i].y[2]) * mesafe_hesapla(direkler[i].x[4], direkler[i].y[4], direkler[i].x[5], direkler[i].y[5]), 0.25);
                
            double D_CA_eq = pow(
                mesafe_hesapla(direkler[i].x[2], direkler[i].y[2], direkler[i].x[0], direkler[i].y[0]) * mesafe_hesapla(direkler[i].x[2], direkler[i].y[2], direkler[i].x[3], direkler[i].y[3]) * mesafe_hesapla(direkler[i].x[5], direkler[i].y[5], direkler[i].x[0], direkler[i].y[0]) * mesafe_hesapla(direkler[i].x[5], direkler[i].y[5], direkler[i].x[3], direkler[i].y[3]), 0.25);

            gmd = cbrt(D_AB_eq * D_BC_eq * D_CA_eq);

            double D_AA = mesafe_hesapla(direkler[i].x[0], direkler[i].y[0], direkler[i].x[3], direkler[i].y[3]);
            double D_BB = mesafe_hesapla(direkler[i].x[1], direkler[i].y[1], direkler[i].x[4], direkler[i].y[4]);
            double D_CC = mesafe_hesapla(direkler[i].x[2], direkler[i].y[2], direkler[i].x[5], direkler[i].y[5]);

            double baz_gmr_L, baz_gmr_C;

            if(tip == 3) { 
                baz_gmr_L = gmr_iletken_m;
                baz_gmr_C = r_iletken_m;
                r_esdeger = secilen_iletken.r_ac_50hz / 2.0;
            } else { 
                baz_gmr_L = sqrt(gmr_iletken_m * d);
                baz_gmr_C = sqrt(r_iletken_m * d);
                r_esdeger = secilen_iletken.r_ac_50hz / 4.0;
            }

            double gmr_A_eq_L = sqrt(baz_gmr_L * D_AA);
            double gmr_B_eq_L = sqrt(baz_gmr_L * D_BB);
            double gmr_C_eq_L = sqrt(baz_gmr_L * D_CC);
            gmr_L = cbrt(gmr_A_eq_L * gmr_B_eq_L * gmr_C_eq_L);

            double gmr_A_eq_C = sqrt(baz_gmr_C * D_AA);
            double gmr_B_eq_C = sqrt(baz_gmr_C * D_BB);
            double gmr_C_eq_C = sqrt(baz_gmr_C * D_CC);
            gmr_C = cbrt(gmr_A_eq_C * gmr_B_eq_C * gmr_C_eq_C);
        }

        double L_mH = 0.2 * log(gmd / gmr_L); 
        double C_nF = 55.6 / log(gmd / gmr_C); 

        double complex Z = r_esdeger + I * (omega * L_mH * 1e-3); 
        double complex Y = 0 + I * (omega * C_nF * 1e-9); 

        double complex gamma = csqrt(Z * Y);
        double complex Zc = csqrt(Z / Y);
        double complex gamma_l = gamma * l;

        double complex A = ccosh(gamma_l);
        double complex B = Zc * csinh(gamma_l);
        double complex C_param = csinh(gamma_l) / Zc;
        double complex D = A;

        sonuclar[i].R = r_esdeger;
        sonuclar[i].L = L_mH;
        sonuclar[i].C_cap = C_nF;
        sonuclar[i].Z = Z;
        sonuclar[i].Y = Y;
        sonuclar[i].A = A;
        sonuclar[i].B = B;
        sonuclar[i].C_param = C_param;
        sonuclar[i].D = D;
    }

    printf("\n========================================================================================================\n");
    printf("Secilen Iletken: %s\n", secilen_iletken.isim);
    printf("Hat Uzunlugu: %.2f km\n", l);
    printf("Secilen Direk Tipleri ve Ebatlari:\n");
    for(int i=0; i<4; i++) {
        if(direkler[i].tip == 1 || direkler[i].tip == 2)
            printf("  - %s: Tek Devre -> A(%.2f, %.2f) B(%.2f, %.2f) C(%.2f, %.2f)\n", 
            direkler[i].isim, direkler[i].x[0], direkler[i].y[0], direkler[i].x[1], direkler[i].y[1], direkler[i].x[2], direkler[i].y[2]);
        else
            printf("  - %s: Cift Devre -> A(%.2f, %.2f) B(%.2f, %.2f) C(%.2f, %.2f) A'(%.2f, %.2f) B'(%.2f, %.2f) C'(%.2f, %.2f)\n", 
            direkler[i].isim, 
            direkler[i].x[0], direkler[i].y[0], direkler[i].x[1], direkler[i].y[1], direkler[i].x[2], direkler[i].y[2],
            direkler[i].x[3], direkler[i].y[3], direkler[i].x[4], direkler[i].y[4], direkler[i].x[5], direkler[i].y[5]);
    }
    printf("========================================================================================================\n\n");

    printf("%-15s | %-19s | %-19s | %-19s | %-19s\n", "Parametre", direkler[0].isim, direkler[1].isim, direkler[2].isim, direkler[3].isim);
    printf("--------------------------------------------------------------------------------------------------------\n");
    
    printf("%-15s | %-19.5f | %-19.5f | %-19.5f | %-19.5f\n", "R (Ohm/km)", sonuclar[0].R, sonuclar[1].R, sonuclar[2].R, sonuclar[3].R);
    printf("%-15s | %-19.5f | %-19.5f | %-19.5f | %-19.5f\n", "L (mH/km)", sonuclar[0].L, sonuclar[1].L, sonuclar[2].L, sonuclar[3].L);
    printf("%-15s | %-19.5f | %-19.5f | %-19.5f | %-19.5f\n", "C (nF/km)", sonuclar[0].C_cap, sonuclar[1].C_cap, sonuclar[2].C_cap, sonuclar[3].C_cap);
    
    printf("%-15s | %.4f+j%.4f | %.4f+j%.4f | %.4f+j%.4f | %.4f+j%.4f\n", "Z (Ohm/km)", 
        creal(sonuclar[0].Z), cimag(sonuclar[0].Z), creal(sonuclar[1].Z), cimag(sonuclar[1].Z), 
        creal(sonuclar[2].Z), cimag(sonuclar[2].Z), creal(sonuclar[3].Z), cimag(sonuclar[3].Z));
    
    printf("%-15s | ", "Y (S/km)"); print_polar(sonuclar[0].Y); printf(" | "); print_polar(sonuclar[1].Y); printf(" | "); 
    print_polar(sonuclar[2].Y); printf(" | "); print_polar(sonuclar[3].Y); printf("\n");

    printf("%-15s | ", "A Parametresi"); print_polar(sonuclar[0].A); printf(" | "); print_polar(sonuclar[1].A); printf(" | "); 
    print_polar(sonuclar[2].A); printf(" | "); print_polar(sonuclar[3].A); printf("\n");

    printf("%-15s | ", "B (Ohm)"); print_polar(sonuclar[0].B); printf(" | "); print_polar(sonuclar[1].B); printf(" | "); 
    print_polar(sonuclar[2].B); printf(" | "); print_polar(sonuclar[3].B); printf("\n");

    printf("%-15s | ", "C (S)"); print_polar(sonuclar[0].C_param); printf(" | "); print_polar(sonuclar[1].C_param); printf(" | "); 
    print_polar(sonuclar[2].C_param); printf(" | "); print_polar(sonuclar[3].C_param); printf("\n");

    printf("%-15s | ", "D Parametresi"); print_polar(sonuclar[0].D); printf(" | "); print_polar(sonuclar[1].D); printf(" | "); 
    print_polar(sonuclar[2].D); printf(" | "); print_polar(sonuclar[3].D); printf("\n");

    printf("========================================================================================================\n\n");

    return 0;
}
