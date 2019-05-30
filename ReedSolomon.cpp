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
    int i = 0, k = 0;
    for (; i < QmS; ++i)
        vec_out[i] = vec_in[i];

    for (; i < Qm; ++i)
        for (k = 0; k < QmS; ++k)
            vec_out[i] = plus[vec_out[i] * Q + multy_matrix[vec_in[k] * Q + G[k * S + i - QmS]]];

    return;
}


int R_S_code (std::array<int, QmS * N> &in, std::array<int, Qm * N> &out)
{
    std::array<int, QmS> vec_in{};
    std::array<int, Qm> vec_out{};
    int i, j, k;
    printf("\n");
    for (i = 0; i < N; ++i) {
        for (j = 0; j < QmS; ++j) {
            vec_in[j] = in[i * QmS + j];
            //printf("%d \t", vec_in[j]);
        }
        //printf("\n");
        vecmultG(vec_in, vec_out);
        for (j = 0; j < Qm; ++j) {
            out[i * Qm + j] = vec_out[j];
            //printf("%d \t", vec_out[j]);
            vec_out[j] = 0;
        }
        //printf("\n");
    }
}

int R_S_decode (std::array<int, Qm * N> &in, std::array<int, QmS * N> &out)
{

}