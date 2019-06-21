//
// Created by vover on 5/17/19.
//

#include "ReedSolomon.h"

extern std::array<int, Qq * Qq> multy_matrix; // = multymatrix();
extern std::array<int, Qq * Qq> plus;// = plusminus();
/*   слева от нуля такая же матрица как и справа, надо этим как-то воспользоваться
если надо что-то вычесть, то можно вычетать из семерки и потом использовать таблицу сложения для результата
Z3   -2 -1 0 1 2
0    1  2  0 1 2
1    2  0  1 2 0
2    0  1  2 0 1
*/
extern std::array<int, Qm> zp_rev; // = Zp_reverse();
extern std::array<int, Qm * Ss> H; // = HH();
extern std::array<int, QmS * Ss> G;// = GG(H, zp_rev, multy_matrix, plus);

void vecmultG (std::array<int, QmS> &vec_in, std::array<int, Qm> &vec_out)
{
    int i = 0, k = 0;
    for (; i < QmS; ++i)
        vec_out[i] = vec_in[i];


    for (i = QmS; i < Qm; ++i) {
        vec_out[i] = 0;
        for (k = 0; k < QmS; ++k)
            vec_out[i] = plus[vec_out[i] * Qq + multy_matrix[vec_in[k] * Qq + G[k * Ss + (i - QmS)]]];
    }

    return;
}


int R_S_code (std::array<int, QmS * Nn> &in, std::array<int, Qm * Nn> &out)
{
    std::array<int, QmS> vec_in{};
    std::array<int, Qm> vec_out{};
    int i, j, k;
    printf("\n");
    for (i = 0; i < Nn; ++i) {
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

int Hmultvec (std::array<int, Qm> &vec_in, std::array<int, Ss> &vec_out)
{
    int i, k;
    int sum = 0;
   for (i = 0; i < Ss; ++i) {
       vec_out[i] = 0;
       for (k = 0; k < Qm; ++k)
           vec_out[i] = plus[vec_out[i] * Qq + multy_matrix[vec_in[k] * Qq + H[i * Qm + k]]];
       sum += vec_out[i];
   }

   return sum;
}

int Gorner (std::array<int, Tt> &Lambda, int h, int x)
{
    int i;
    int ans = multy_matrix[Lambda[0] * Qq + x];

    for (i = 1; i < h; ++i)
    {
        ans = multy_matrix[plus[ans * Qq + Lambda[i]] + x * Qq];
    }
    return plus[ans * Qq + 1];
}

int Gauss_in_Zq (std::array<int, Tt * Tt> &matrix, std::array<int, Tt> &vec_b, int h)
{
    int max_ind, max, coef;
    int i, j, k;

    for (i = 0; i < h; ++i)
    {
        for (j = 0; j < h; ++j)
            printf ("%3d \t", matrix[i * h + j]);
        printf ("%3d \n", vec_b[i]);
    }
    printf("\n\n");




    for (i = 0; i < h; ++i)
    {
        max = -1;
        max_ind = -1;
        for (j = i; j < h; ++j)
        {
            if (max < matrix[j*h + i]) {
                max = matrix[j*h + i];
                max_ind = j;
            }
        }
        if (max < 1) return -1;

        for (k = i; k < h; ++k)
        {
            coef = matrix[i*h + k];
            matrix[i*h + k] = matrix[max_ind*h + k];
            matrix[max_ind*h + k] = coef;
        }
        coef = vec_b[i];
        vec_b[i] = vec_b[max_ind];
        vec_b[max_ind] = coef;

        coef = zp_rev[matrix[i * h + i] - 1];
        for (k = i; k < h; ++k)
            matrix[i * h + k] = multy_matrix[coef * Qq + matrix[i * h + k]];

        vec_b[i] = multy_matrix[coef * Qq + vec_b[i]];



        for (j = 0; j < h; ++j) {
            if (i == j) continue;
            coef = matrix[j * h + i];
            for (k = 0; k < h; k++) {
                matrix[j * h + k] = plus[matrix[j * h + k] * Qq - multy_matrix[coef * Qq + matrix[i * h + k]] + Qq * ((matrix[i * h + k] == 0) ? 0 : 1)];
            }
            vec_b[j] = plus[vec_b[j] * Qq - multy_matrix[coef * Qq + vec_b[i]] + Qq * ((vec_b[i] == 0) ? 0 : 1)];
        }

        for (int ii = 0; ii < h; ++ii)
        {
            for (int jj = 0; jj < h; ++jj)
                printf ("%3d \t", matrix[ii * h + jj]);
            printf ("%3d \n", vec_b[ii]);
        }
        printf("\n\n");

    }


    return 0;
}

int search_errors (std::array<int, Ss> &vec_in, std::array<int, Tt> &pos_of_er) {
    int h, i, j;
    std::array<int, Tt * Tt> matrix{};
    std::array<int, Tt> vec_b{};


    for (h = Tt; h > 0; --h) {
        for (i = 0; i < h; ++i) {
            for (j = 0; j < h; ++j)
                matrix[i * h + j] = vec_in[i + j];
            vec_b[i] = (Qq - vec_in[h + i]) % Qq;
        }
        if (Gauss_in_Zq(matrix, vec_b, h) == 0) break;
    }

    if (h == 0) return -1;

    j = h;
    for (i = 1; i < Qq; ++i)
        if (Gorner(vec_b, h, i) == 0)
            pos_of_er[--j] = zp_rev[i - 1];

    return h;
}

void inc_z2_vec (std::array<int, Tt> &vec)
{
    int i;
    for (i = Tt-1; i > 0; --i)
        if (vec[i] == 0) {
            vec[i] = 1;
            return;
        } else {
            vec[i] = 1;
        }

}

int check_vec (std::array<int, Qm> &vec_in, std::array<int, Tt> &pos_of_er, std::array<int, Tt> &vec_b, int h)
{
    int i, j, k = 1;
    std::array<int, Qm> vec{};
    std::array<int, Ss> vec_{};
    std::array<int, Tt> z2_vec{};

    for (i = 1; i < h; ++i)
        k *= 2;

    for (int l = 0; l < k; ++l) {
        for (j = 0; j < Qm; ++j)
            vec[j] = vec_in[j];
        for (i = 0; i < h; ++i)
            vec[pos_of_er[i]-1] = (z2_vec[Tt-1 - i]) ? plus[vec[pos_of_er[i]-1] * Qq + vec_b[i]] : plus[vec[pos_of_er[i]-1] * Qq - vec_b[i] + Qq * ((vec_b[i] == 0) ? 0 : 1)];
        if (Hmultvec(vec, vec_) == 0) {
            for (j = 0; j < Qm; ++j)
                vec_in[j] = vec[j];
            return 0;
        }
        inc_z2_vec(z2_vec);
    }
}

int fix_errors (std::array<int, Qm> &vec_in, std::array<int, Tt> &pos_of_er, int h)
{
    int i, j;
    std::array<int, Tt * Tt> matrix{};
    std::array<int, Tt> vec_b{};
    std::array<int, Qm> vec = vec_in;
    std::array<int, Ss> vec_{};

    Hmultvec(vec, vec_);

    for (j = 0; j < h; ++j)
        matrix[j] = pos_of_er[j];
    vec_b[0] = vec_[1];

    for (i = 1; i < h; ++i) {
        for (j = 0; j < h; ++j)
            matrix[i * h + j] = multy_matrix[pos_of_er[j] * Qq + matrix[(i - 1) * h + j]];

        vec_b[i] = vec_[i * 2 + 1];
    }

    printf("исправление \n");
    if (Gauss_in_Zq(matrix, vec_b, h) != 0) {
        printf("не удалось определить отклонение \n");
        return -1;
    }

    check_vec(vec_in, pos_of_er, vec_b, h);


    return 0;
}

int search_and_fix_errors (std::array<int, Qm> &v)
{
    std::array<int, Tt> positions_of_errors{};
    std::array<int, Ss> vec{};

    int error;

    Hmultvec(v, vec);

    for (int i = 0; i < Ss; ++i)
        printf ("%3d \t", vec[i]);
    printf("\n");

    if((error = search_errors(vec, positions_of_errors)) < 0) {
        printf("все очень плохо\n");
        return 1;
    }
    if (fix_errors(v, positions_of_errors, error) != 0) {
        printf("все совсем плохо\n");
        return 2;
    }

    return 0;
}

int R_S_decode (std::array<int, Qm * Nn> &in, std::array<int, QmS * Nn> &out)
{
    std::array<int, Qm> vec_in{};
    std::array<int, Ss> vec_out{};
    int i, j, k;
    printf("\n");
    for (i = 0; i < Nn; ++i) {
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
            printf ("bad vector\n");
            if (search_and_fix_errors(vec_in) == 0)
            {
                for (j = 0; j < QmS; ++j) {
                    out[i * QmS + j] = vec_in[j];
                    //printf("%d \t", vec_out[j]);
                    //vec_out[j] = 0;
                }
            }
            else printf ("не удалось исправить ошибки\n");
        }
        //printf("\n");
    }
}