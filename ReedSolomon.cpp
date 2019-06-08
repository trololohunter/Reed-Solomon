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


    for (i = QmS; i < Qm; ++i) {
        vec_out[i] = 0;
        for (k = 0; k < QmS; ++k)
            vec_out[i] = plus[vec_out[i] * Q + multy_matrix[vec_in[k] * Q + G[k * S + i - QmS]]];
    }

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
            //vec_out[j] = 0;
        }
        //printf("\n");
    }

    return 0;
}

int Hmultvec (std::array<int, Qm> &vec_in, std::array<int, S> &vec_out)
{
    int i, k;
    int sum = 0;
   for (i = 0; i < S; ++i) {
       vec_out[i] = 0;
       for (k = 0; k < Qm; ++k)
           vec_out[i] = plus[vec_out[i] * Q + multy_matrix[vec_in[k] * Q + H[i * Qm + k]]];
       sum += vec_out[i];
   }

   return sum;
}

int search_errors (std::array<int, QmS> &vec_in, std::array<int, T> &pos_of_er)
{

    return 0;
}

int fix_errors (std::array<int, QmS> &vec_in, std::array<int, T> &pos_of_er)
{


    return 0;
}

int search_and_fix_errors (std::array<int, QmS> &vec_in)
{
    std::array<int, T> positions_of_errors{};
    //int error;

    if(search_errors(vec_in, positions_of_errors) != 0) {
        printf("все очень плохо");
        return 1;
    }
    if (fix_errors(vec_in, positions_of_errors) != 0) {
        printf("все совсем плохо");
        return 2;
    }
    
    return 0;
}

int R_S_decode (std::array<int, Qm * N> &in, std::array<int, QmS * N> &out)
{
    std::array<int, Qm> vec_in{};
    std::array<int, S> vec_out{};
    int i, j, k;
    printf("\n");
    for (i = 0; i < N; ++i) {
        for (j = 0; j < Qm; ++j) {
            vec_in[j] = in[i * Qm + j];
            //printf("%d \t", vec_in[j]);
        }
        //printf("\n");
        if (Hmultvec(vec_in, vec_out) == 0)
        for (j = 0; j < QmS; ++j) {
            out[i * QmS + j] = vec_in[j];
            //printf("%d \t", vec_out[j]);
            //vec_out[j] = 0;
        }
        else {
            printf ("bad vector");
        }
        //printf("\n");
    }
}