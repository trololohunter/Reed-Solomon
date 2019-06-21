//
// Created by vover on 5/17/19.
//

#ifndef UNTITLED5_REEDSOLOMON_H
#define UNTITLED5_REEDSOLOMON_H

#include<array>

#define Qq 13
#define Qm 12    // Q - 1
#define Tt 2 // number of errors that can be corrected
#define Ss 4 // number of errors that can be detected (number of verification characters)
#define QmS 8   // Q - 1 - S
#define Nn 4



constexpr std::array<int, Qm> Zp_reverse() {
    std::array<int, Qm> rev{};
    rev[0] = 1;
    for (int i = 1; i < Qm; ++i)
        for (int j = 2; j < Qq; ++j)
            if (((i + 1) * j) % Qq == 1)
            {
                rev[i] = j;
                break;
            }

    return rev;
}

constexpr std::array<int, Qq * Qq> multymatrix() {
    std::array<int, Qq * Qq> mm{};

    printf("\n");
    printf ("multymatrix \n");
    for (int i = 0; i < Qq; ++i)
    {
        //mm[i].resize(P-1);
        for (int j = 0; j < Qq; ++j) {
            mm[i * Qq + j] = (i * j) % Qq;
            printf ("%d \t", mm[i * Qq + j]);
        }
        printf("\n");
    }

    return mm;
}

constexpr std::array<int, Qq * Qq> plusminus () {
    std::array<int, Qq * Qq> pm{};
    printf("\n");
    printf ("plusminus \n");
    for (int i = 0; i < Qq; ++i)
    {
        for (int j = 0; j < Qq; ++j)
        {
            pm[i * Qq + j] = (j + i) % Qq;
            printf ("%d \t", pm[i * Qq + j]);
        }
        printf("\n");

    }

    return pm;
}


constexpr std::array<int, Qm * Ss> HH(){
    std::array<int, Qm * Ss> Hh{};
    int i = 0, j = 0;
    printf("\n");
    printf("HH\n");
    for (j = 0; j < Qm; ++j) {
        Hh[j] = 1;
        Hh[Qm + j] = j+1;
    }
    for (i = 2; i < Ss; ++i)
        for (j = 0; j < Qm; ++j)
            Hh[i * Qm + j] = (Hh[(i-1) * Qm + j] * Hh[Qm + j]) % Qq;

    for (i = 0; i < Ss; i++) {
        for (j = 0; j < Qm; ++j)
            printf("%3d \t", Hh[i * Qm + j]);
        printf("\n");
    }



    return Hh;
}

constexpr std::array<int, QmS * Ss> GG(std::array<int, Qm * Ss> hh, const std::array<int, Qm> zp_rev, const std::array<int, Qq * Qq> multym, const std::array<int, Qq * Qq> pm)
{
    std::array<int, QmS * Ss> Gg{};
    int i = 0, j = 0, k = 0;
    int coef = 0;
    printf("\n");
    printf("\n");
/*
    for (i = 1; i < S; ++i)
    {
        for (j = 0; j < Qm; ++j)
            --hh[i * Qm + j];
    }
    printf("\n");
    for (i = 0; i < S; i++) {
        for (j = 0; j < Qm; ++j)
            printf("%3d \t", hh[i * Qm + j]);
        printf("\n");
    }
*/
    for (i = 0; i < Ss; ++i)
    {
        coef = zp_rev[hh[i * Qm + i + QmS] - 1];
        for (k = 0; k < Qm; ++k)
            hh[i * Qm + k] = multym[coef * Qq + hh[i * Qm + k]];

        for (j = 0; j < Ss; ++j) {
            if ((i == j)) continue;
            if (hh[i * Qm + i + QmS] == 0) printf ("hh[i * Qm + i + QmS] ==== 0 \n");
            coef = hh[j * Qm + i + QmS];
            for (k = 0; k < Qm; ++k) {
                hh[j * Qm + k] = pm[hh[j * Qm + k] * Qq - multym[coef * Qq + hh[i * Qm + k]] + Qq * ((hh[i * Qm + k] == 0) ? 0 : 1)];
                //if (hh[j * Qm + k] < 0) hh[j * Qm + k] += Q;
            }
        }
    }

    printf("\n");
    for (i = 0; i < Ss; i++) {
        for (j = 0; j < Qm; ++j)
            printf("%3d \t", hh[i * Qm + j]);
        printf("\n");
    }

//    for (i = 0; i < QmS; ++i)
//        Gg[i * Qm + i] = 1;

    for (i = 0; i < QmS; ++i)
        for (j = 0; j < Ss; ++j)
            Gg[i * Ss + j] = (Qq - hh[j * Qm + i]) % Qq;

    printf("\n");
    for (i = 0; i < QmS; i++) {
        for (j = 0; j < Ss; ++j)
            printf("%3d \t", Gg[i * Ss + j]);
        printf("\n");
    }
    return Gg;
};

int R_S_code (std::array<int, QmS * Nn> &in, std::array<int, Qm * Nn> &out);      // returns error value
int R_S_decode (std::array<int, Qm * Nn> &in, std::array<int, QmS * Nn> &out);    // returns error value

#endif //UNTITLED5_REEDSOLOMON_H
