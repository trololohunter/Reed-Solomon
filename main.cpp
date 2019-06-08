
#include <iostream>

 #include <cstdint>

#include "ReedSolomon.h"

//std::array<int, > GG(std::array<std::array<int, P>, S> Hh);

//тут будет типа версия с использованием оупенсиэль

int main() {
    const::std::array<int, Qm> rev = Zp_reverse();
    int i;
    for ( i = 1; i < Q; ++i)
        printf ("for %d reverse = %d \n", i, rev[i-1]);
    auto mm = multymatrix();
    auto hh = HH();
    auto pm = plusminus();
    auto gg = GG(hh, rev, mm, pm);
    std::array<int, QmS * N> in{};
    std::array<int, Qm * N> out{};

    for ( i = 0; i < QmS * N; i++) {
        in[i] = rand() % Q;
        printf("%d \t", in[i]);
    }

    R_S_code(in, out);

    for (i = 0; i < Qm * N; ++i) {
        printf("%d \t", out[i]);
    }

    printf("\n");

    out[16] = 4;

    R_S_decode(out, in);

    for (i = 0; i < QmS * N; ++i) {
        printf("%d \t", in[i]);
    }

    printf("\n");


    std::cout << "Hello, World!" << std::endl;
    return 0;
}



/*
std::array<std::array<int, P>, P - S> GG(std::array<std::array<int, P>, S> Hh){
    std::array<std::array<int, P>, P - S> Gg;




    return Gg;
}

*/