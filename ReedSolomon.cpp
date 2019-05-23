//
// Created by vover on 5/17/19.
//
#include "ReedSolomon.h"

auto multy_matrix = multymatrix();
auto plus = plusminus();
/*   слева от нуля такая же матрица как и справа, надо этим как-то воспользоваться
если надо что-то вычесть, то можно вычетать из семерки и потом использовать таблицу сложения для результата
Z3   -2 -1 0 1 2
0    1  2  0 1 2
1    2  0  1 2 0
2    0  1  2 0 1
*/
auto zp_rev = Zp_reverse();
auto H = HH();
auto G = GG(H, zp_rev, multy_matrix, plus);

void vecmultG (std::array<int, QmS> &vec_in, std::array<int, Qm> &vec_out)
{
    int i = 0;

    

    return;
}


int R_S_code (std::array<int, QmS * N> &in, std::array<int, Qm * N> &out)
{
    std::array<int, Qm> vec_in;
    std::array<int, QmS> vec_out;




}

int R_S_decode (std::array<int, Qm * N> &in, std::array<int, QmS * N> &out)
{

}