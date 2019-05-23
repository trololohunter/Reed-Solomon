
#include <iostream>

 #include <cstdint>

#include "ReedSolomon.h"

//std::array<int, > GG(std::array<std::array<int, P>, S> Hh);


int main() {
    const::std::array<int, Qm> rev = Zp_reverse();
    for (int i = 1; i < Q; ++i)
        printf ("for %d reverse = %d \n", i, rev[i-1]);
    auto mm = multymatrix();
    auto hh = HH();
    auto pm = plusminus();
    auto gg = GG(hh, rev, mm, pm);

    //printf ("%d \t %d \t %d", 3 % 7, (-3) % 7, -3 % 7);

    printf("\n");printf("\n");
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < Qm; ++j)
            printf("%3d \t", hh[i * Qm + j]);
        printf("\n");
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}



/*
std::array<std::array<int, P>, P - S> GG(std::array<std::array<int, P>, S> Hh){
    std::array<std::array<int, P>, P - S> Gg;




    return Gg;
}

*/