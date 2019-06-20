//
// Created by vover on 5/17/19.
//

#ifndef UNTITLED5_REEDSOLOMON_H
#define UNTITLED5_REEDSOLOMON_H

#include<array>

#define Q 13
#define Qm 12    // Q - 1
#define T 2 // number of errors that can be corrected
#define S 4 // number of errors that can be detected (number of verification characters)
#define QmS 8   // Q - 1 - S
#define N 4



constexpr std::array<int, Qm> Zp_reverse() {
    std::array<int, Qm> rev{};
    rev[0] = 1;
    for (int i = 1; i < Qm; ++i)
        for (int j = 2; j < Q; ++j)
            if (((i + 1) * j) % Q == 1)
            {
                rev[i] = j;
                break;
            }

    return rev;
}

constexpr std::array<int, Q * Q> multymatrix() {
    std::array<int, Q * Q> mm{};

    printf("\n");
    printf ("multymatrix \n");
    for (int i = 0; i < Q; ++i)
    {
        //mm[i].resize(P-1);
        for (int j = 0; j < Q; ++j) {
            mm[i * Q + j] = (i * j) % Q;
            printf ("%d \t", mm[i * Q + j]);
        }
        printf("\n");
    }

    return mm;
}

constexpr std::array<int, Q * Q> plusminus () {
    std::array<int, Q * Q> pm{};
    printf("\n");
    printf ("plusminus \n");
    for (int i = 0; i < Q; ++i)
    {
        for (int j = 0; j < Q; ++j)
        {
            pm[i * Q + j] = (j + i) % Q;
            printf ("%d \t", pm[i * Q + j]);
        }
        printf("\n");

    }

    return pm;
}


constexpr std::array<int, Qm * S> HH(){
    std::array<int, Qm * S> Hh{};
    int i = 0, j = 0;
    printf("\n");
    printf("HH\n");
    for (j = 0; j < Qm; ++j) {
        Hh[j] = 1;
        Hh[Qm + j] = j+1;
    }
    for (i = 2; i < S; ++i)
        for (j = 0; j < Qm; ++j)
            Hh[i * Qm + j] = (Hh[(i-1) * Qm + j] * Hh[Qm + j]) % Q;

    for (i = 0; i < S; i++) {
        for (j = 0; j < Qm; ++j)
            printf("%3d \t", Hh[i * Qm + j]);
        printf("\n");
    }



    return Hh;
}

constexpr std::array<int, QmS * S> GG(std::array<int, Qm * S> hh, const std::array<int, Qm> zp_rev, const std::array<int, Q * Q> multym, const std::array<int, Q * Q> pm)
{
    std::array<int, QmS * S> Gg{};
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
    for (i = 0; i < S; ++i)
    {
        coef = zp_rev[hh[i * Qm + i + QmS] - 1];
        for (k = 0; k < Qm; ++k)
            hh[i * Qm + k] = multym[coef * Q + hh[i * Qm + k]];

        for (j = 0; j < S; ++j) {
            if ((i == j)) continue;
            if (hh[i * Qm + i + QmS] == 0) printf ("hh[i * Qm + i + QmS] ==== 0 \n");
            coef = hh[j * Qm + i + QmS];
            for (k = 0; k < Qm; ++k) {
                hh[j * Qm + k] = pm[hh[j * Qm + k] * Q - multym[coef * Q + hh[i * Qm + k]] + Q * ((hh[i * Qm + k] == 0) ? 0 : 1)];
                //if (hh[j * Qm + k] < 0) hh[j * Qm + k] += Q;
            }
        }
    }

    printf("\n");
    for (i = 0; i < S; i++) {
        for (j = 0; j < Qm; ++j)
            printf("%3d \t", hh[i * Qm + j]);
        printf("\n");
    }

//    for (i = 0; i < QmS; ++i)
//        Gg[i * Qm + i] = 1;

    for (i = 0; i < QmS; ++i)
        for (j = 0; j < S; ++j)
            Gg[i * S + j] = (Q - hh[j * Qm + i]) % Q;

    printf("\n");
    for (i = 0; i < QmS; i++) {
        for (j = 0; j < S; ++j)
            printf("%3d \t", Gg[i * S + j]);
        printf("\n");
    }
    return Gg;
};

int R_S_code (std::array<int, QmS * N> &in, std::array<int, Qm * N> &out);      // returns error value
int R_S_decode (std::array<int, Qm * N> &in, std::array<int, QmS * N> &out);    // returns error value

#endif //UNTITLED5_REEDSOLOMON_H
