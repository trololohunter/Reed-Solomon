
#include <iostream>

 #include <cstdint>

#include "ReedSolomon.h"
#include <boost/compute/core.hpp>
#include <boost/compute/container/vector.hpp>


auto multy_matrix = multymatrix();
auto plus = plusminus();
auto zp_rev = Zp_reverse();
auto H = HH();
auto G = GG(H, zp_rev, multy_matrix, plus);



int main() {
    const::std::array<int, Qm> rev = Zp_reverse();
    int i;
    for ( i = 1; i < Qq; ++i)
        printf ("for %d reverse = %d \n", i, rev[i-1]);
    //auto mm = multymatrix();
    //auto hh = HH();
    //auto pm = plusminus();
    //auto gg = GG(hh, rev, mm, pm);
    std::array<int, QmS * Nn> in{};
    std::array<int, Qm * Nn> out{};

    for ( i = 0; i < QmS * Nn; i++) {
        in[i] = rand() % Qq;
        printf("%d \t", in[i]);
    }

    R_S_code(in, out);

    for (i = 0; i < Qm * Nn; ++i) {
        printf("%d \t", out[i]);
    }

    printf("\n");

    out[14] = 6;
    out[22] = 8;
    out[23] = 7;
    out[31] = 2;
    out[32] = 6;

    for (i = 0; i < Qm * Nn; ++i) {
        printf("%d \t", out[i]);
    }

    printf("\n");

    R_S_decode(out, in);

    for (i = 0; i < QmS * Nn; ++i) {
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