// computer-aided transmission line parameter calculator phase 2 - power and compensation analysis
// kocaeli university, electrical engineering department, 2026 spring semester
// electric power transmission course project - group 6
// by busra gulen, 220205029 

// NOTE: this code is overwritten from the "iletim1.c" code, meaning it's an extended and final version of the project.

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
typedef struct {
    char isim[50];
    double kesit_mm2;
    double dis_cap_mm;
    double gmr_mm;
    double r_ac_50hz;
} Iletken;

typedef struct {
    char isim[20];
    int tip; 
    double x[6]; 
    double y[6]; 
} Direk;

// faz 2 icin Z_birim ve Y_birim eklendi
typedef struct {
    double R;
    double L;
    double C_cap;
    double complex Z_birim;
    double complex Y_birim;
    double complex Zc;
    double complex gamma;
    double complex A;
    double complex B;
    double complex C_param;
    double complex D;
} HatParametreleri;

double mesafe_hesapla(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void print_polar(double complex z) {
    double genlik = cabs(z);
    double aci = carg(z) * (180.0 / PI);
    printf("%8.4f L %6.2f°", genlik, aci);
}

int main() {
    double omega = 2.0 * PI * f;

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

    Direk direkler[4] = {
        {"PA Tasiyici", 1, {-4.5, 0.0, 4.5, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        {"3PA1 Tasiyici", 2, {-11.9, 0.0, 11.9, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        {"154kV TA Tas.", 3, 
            {4.00, 4.00, 4.00, -4.00, -4.00, -4.00}, 
            {8.00, 4.00, 0.00, 8.00, 4.00, 0.00}},
        {"2A Cift 2'li", 4, 
            {8.60, 8.60, 8.60, -8.60, -8.60, -8.60}, 
            {12.0, 6.00, 0.00, 12.0, 6.00, 0.00}}
    };

    HatParametreleri sonuclar[4];

    // faz 1 user inputs
    int secim;
    double l;
    
    printf("--- Grup 6: Enerji Iletim Hatti Projesi KISIM 1 & 2 ---\n");
    printf("Kullanilabilir Iletkenler:\n");
    for(int i=0; i<8; i++) {
        printf("%d. %s\n", i+1, iletkenler[i].isim);
    }
    printf("Iletken seciniz (1-8): ");
    scanf("%d", &secim);
    secim--; 

    printf("Hat uzunlugunu giriniz (km): ");
    scanf("%lf", &l);

    Iletken secilen_iletken = iletkenler[secim];
    double r_iletken_m = (secilen_iletken.dis_cap_mm / 2.0) / 1000.0; 
    double gmr_iletken_m = secilen_iletken.gmr_mm / 1000.0; 

    // faz 1 calculations - hat parametreleri
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
            double D_AB_eq = pow(mesafe_hesapla(direkler[i].x[0], direkler[i].y[0], direkler[i].x[1], direkler[i].y[1]) * mesafe_hesapla(direkler[i].x[0], direkler[i].y[0], direkler[i].x[4], direkler[i].y[4]) * mesafe_hesapla(direkler[i].x[3], direkler[i].y[3], direkler[i].x[1], direkler[i].y[1]) * mesafe_hesapla(direkler[i].x[3], direkler[i].y[3], direkler[i].x[4], direkler[i].y[4]), 0.25);
            double D_BC_eq = pow(mesafe_hesapla(direkler[i].x[1], direkler[i].y[1], direkler[i].x[2], direkler[i].y[2]) * mesafe_hesapla(direkler[i].x[1], direkler[i].y[1], direkler[i].x[5], direkler[i].y[5]) * mesafe_hesapla(direkler[i].x[4], direkler[i].y[4], direkler[i].x[2], direkler[i].y[2]) * mesafe_hesapla(direkler[i].x[4], direkler[i].y[4], direkler[i].x[5], direkler[i].y[5]), 0.25);
            double D_CA_eq = pow(mesafe_hesapla(direkler[i].x[2], direkler[i].y[2], direkler[i].x[0], direkler[i].y[0]) * mesafe_hesapla(direkler[i].x[2], direkler[i].y[2], direkler[i].x[3], direkler[i].y[3]) * mesafe_hesapla(direkler[i].x[5], direkler[i].y[5], direkler[i].x[0], direkler[i].y[0]) * mesafe_hesapla(direkler[i].x[5], direkler[i].y[5], direkler[i].x[3], direkler[i].y[3]), 0.25);

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

        sonuclar[i].R = r_esdeger;
        sonuclar[i].L = L_mH;
        sonuclar[i].C_cap = C_nF;
        sonuclar[i].Z_birim = Z;
        sonuclar[i].Y_birim = Y;
        sonuclar[i].Zc = Zc;
        sonuclar[i].gamma = gamma;
        sonuclar[i].A = ccosh(gamma_l);
        sonuclar[i].B = Zc * csinh(gamma_l);
        sonuclar[i].C_param = csinh(gamma_l) / Zc;
        sonuclar[i].D = sonuclar[i].A;
    }

    // faz 2,user inputs
    printf("\n=== FAZ 2: GUC VE KOMPANZASYON ANALIZI ===\n");
    int direk_secim, pf_tip;
    double U2_kV, pf;

    printf("Hangi direk tipi uzerinden islem yapilsin? (1: PA, 2: 3PA1, 3: TA, 4: 2A): ");
    scanf("%d", &direk_secim);
    HatParametreleri hat = sonuclar[direk_secim - 1];

    printf("Hat Sonu Gerilimi U2 (kV) [Orn: 154 veya 400]: ");
    scanf("%lf", &U2_kV);

    printf("Guc Katsayisi (pf) [Orn: 0.85]: ");
    scanf("%lf", &pf);

    printf("Yuku Tipi (1: Kapasitif, 2: Enduktif): ");
    scanf("%d", &pf_tip);

    // temel elektriksel degerler
    double V2_genlik = (U2_kV * 1000.0) / sqrt(3.0);
    double complex V2 = V2_genlik + 0.0 * I;
    
    // S2 SIL (dogal guc) hesabi: S2 = U2^2 / Z_sil
    double Z_sil = sqrt((hat.L * 1e-3) / (hat.C_cap * 1e-9));
    double S2_VA = pow(U2_kV * 1000.0, 2) / Z_sil;
    
    double pf_aci_radyan = acos(pf);
    // kapasitif ise reaktif guc negatif, enduktif ise pozitif 
    double Q2_VA = S2_VA * sin(pf_aci_radyan) * (pf_tip == 1 ? -1.0 : 1.0); 
    double complex S2_kompleks = (S2_VA * pf) + Q2_VA * I;
    double complex I2 = conj(S2_kompleks / (3.0 * V2));

    printf("\n--- A SIKKI: Hat Basi Degerleri ---\n");
    double complex V1 = hat.A * V2 + hat.B * I2;
    double complex I1 = hat.C_param * V2 + hat.D * I2;
    double complex S1 = 3.0 * V1 * conj(I1);

    double U1_kV = sqrt(3.0) * cabs(V1) / 1000.0;
    double P1_MW = creal(S1) / 1e6;
    double Q1_MVAR = cimag(S1) / 1e6;
    double verim = (creal(S2_kompleks) / creal(S1)) * 100.0;
    double reg = ((cabs(V1) / cabs(hat.A) - V2_genlik) / V2_genlik) * 100.0;

    printf("S2 (Hat Sonu): %.2f MVA (P: %.2f MW, Q: %.2f MVAr)\n", S2_VA/1e6, creal(S2_kompleks)/1e6, cimag(S2_kompleks)/1e6);
    printf("U1 (Hat Basi Gerilimi): %.2f kV\n", U1_kV);
    printf("P1 (Hat Basi Aktif Guc): %.2f MW\n", P1_MW);
    printf("Q1 (Hat Basi Reaktif Guc): %.2f MVAr\n", Q1_MVAR);
    printf("Hat Verimi: %%.%.2f\n", verim);
    printf("Gerilim Regulasyonu: %%.%.2f\n", reg);

    printf("\n--- B SIKKI: Hat Profili  ---\n");
    printf("x(km),V_FazFaz(kV),P(MW),Q(MVAr)\n");
    for(int i = 0; i <= 10; i++) {
        double x = i * (l / 10.0);
        double complex gx = hat.gamma * x;
        double complex V_x = V2 * ccosh(gx) + I2 * hat.Zc * csinh(gx);
        double complex I_x = I2 * ccosh(gx) + (V2 / hat.Zc) * csinh(gx);
        double complex S_x = 3.0 * V_x * conj(I_x);
        
        printf("%.1f, %.3f, %.3f, %.3f\n", x, (sqrt(3.0)*cabs(V_x))/1000.0, creal(S_x)/1e6, cimag(S_x)/1e6);
    }

    printf("\n--- C SIKKI: P-V Egrisi ---\n");
    printf("k(Yük Carpanı),P1(MW),U1(kV)\n");
    for(double k = 0.1; k <= 1.55; k += 0.1) {
        double complex S2_k = k * S2_kompleks;
        double complex I2_k = conj(S2_k / (3.0 * V2));
        
        double complex V1_k = hat.A * V2 + hat.B * I2_k;
        double complex I1_k = hat.C_param * V2 + hat.D * I2_k;
        double complex S1_k = 3.0 * V1_k * conj(I1_k);
        
        printf("%.1f, %.3f, %.3f\n", k, creal(S1_k)/1e6, (sqrt(3.0)*cabs(V1_k))/1000.0);
    }

    printf("\n--- D SIKKI: Seri Kompanzasyon Analizi ---\n");
    double complex I2_asiri = I2 * 10.0; // 10S2 durumu
    
    // Pi devresi parametreleri
    double complex Z_pi = hat.Zc * csinh(hat.gamma * l);
    double complex Y_pi = (ccosh(hat.gamma * l) - 1.0) / (hat.Zc * csinh(hat.gamma * l));
    
    double comp_oranlari[] = {0.30, 0.50};
    
    for(int i = 0; i < 2; i++) {
        double k_comp = comp_oranlari[i];
        double complex Z_pi_yeni = creal(Z_pi) + (cimag(Z_pi) * (1.0 - k_comp)) * I;
        
        // yeni A, B, C, D degerleri
        double complex A_yeni = 1.0 + Z_pi_yeni * Y_pi;
        double complex B_yeni = Z_pi_yeni;
        double complex C_yeni = Y_pi * (2.0 + Z_pi_yeni * Y_pi);
        
        // normal yuk icin
        double complex V1_n = A_yeni * V2 + B_yeni * I2;
        double complex I1_n = C_yeni * V2 + A_yeni * I2;
        double P1_n = creal(3.0 * V1_n * conj(I1_n));
        double verim_n = (creal(S2_kompleks) / P1_n) * 100.0;
        double reg_n = ((cabs(V1_n)/cabs(A_yeni) - V2_genlik) / V2_genlik) * 100.0;
        
        // asiri yuk (10x) icin
        double complex V1_a = A_yeni * V2 + B_yeni * I2_asiri;
        double complex I1_a = C_yeni * V2 + A_yeni * I2_asiri;
        double P1_a = creal(3.0 * V1_a * conj(I1_a));
        double verim_a = (creal(S2_kompleks * 10.0) / P1_a) * 100.0;
        double reg_a = ((cabs(V1_a)/cabs(A_yeni) - V2_genlik) / V2_genlik) * 100.0;
        
        printf("%%%.0f Kompanzasyon Durumu:\n", k_comp * 100);
        printf("  > Nominal Yuk(S2):  U1 = %6.2f kV | Verim: %%%5.2f | Reg: %%%.2f\n", sqrt(3)*cabs(V1_n)/1000.0, verim_n, reg_n);
        printf("  > Asiri Yuk(10xS2): U1 = %6.2f kV | Verim: %%%5.2f | Reg: %%%.2f\n", sqrt(3)*cabs(V1_a)/1000.0, verim_a, reg_a);
    }

    printf("\n========================================================================================================\n\n");
    return 0;
}
