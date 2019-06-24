
#include <iostream>

 #include <cstdint>

#include "ReedSolomon.h"
//#include <boost/compute.hpp>

#include <boost/compute/core.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/utility/source.hpp>


auto multy_matrix = multymatrix();
auto plus = plusminus();
auto zp_rev = Zp_reverse();
auto H = HH();
auto G = GG(H, zp_rev, multy_matrix, plus);




boost::compute::kernel make_code_kernel (const boost::compute::context& context)
{
    const char source[] = BOOST_COMPUTE_STRINGIZE_SOURCE (
            __kernel void code (
                    const int *matrixG,
                    const int *matrixmultiply,
                    const int *matrixplus,
                    const int *vector_for_code,
                    int *result )
            {
                uint x = get_global_id(0);
                uint size = get_global_size(0);
                uint i = 0, k = 0, sum = 0, j = 0, y = x;

                for (j = 0; j*size + x < Nn; ++j)
                {
                    for (i = 0; i < QmS; ++i)
                        result[y * Qm + i] = vector_for_code[y * QmS + i];

                    for (i = QmS; i < Qm; ++i){
                        sum = 0;
                        for (k = 0; k < QmS; ++k)
                            sum = matrixplus[sum * Qq + matrixmultiply[vector_for_code[y * QmS + k] * Qq + matrixG[k * Ss + (i - QmS)]]];
                        result[y * Qm + i] = sum;
                    }

                    y += size;
                }

            }
            );

    //std::stringstream options;
    //options << "-DTILE_DIM=" << 20 << " -DBLOCK_ROWS=" << 4;

    boost::compute::program program = boost::compute::program::create_with_source(source, context);

    program.build();

    return program.create_kernel("code");
}


boost::compute::kernel make_decode_kernel (const boost::compute::context& context)
{
    const char source[] = BOOST_COMPUTE_STRINGIZE_SOURCE (
            __kernel void decode (__read_only int *matrixH,
                                __read_only int *matrixmultiply,
                                __read_only int *matrixplus,
                                __read_only int *vector_for_decode,
                                __write_only int *result)
            {
                uint x = get_global_id(0);
                uint size = get_global_size(0);
                uint i = 0, k = 0, sum = 0, j = 0, y = x;

                __local float sindrom[Ss];

                for (j = 0; j*size + x < Nn; ++j)
                {
                    sum = 0;
                    for (i = 0; i < Ss; ++i)
                    {
                        sindrom[i] = 0;
                        for (k = 0; k < Qm; ++k)
                            sindrom[i] = matrixplus[sindrom[i] * Qq + matrixmultiply[vector_for_decode[y * Qm + k] * Qq + matrixH[i * Qm + k]]];
                        sum += sindrom[i];

                    }

                    if (sindrom[i] == 0)
                        for (k = 0; k < QmS; ++k)
                            result[y * QmS + k] = vector_for_decode[y * Qm + k];
                    else
                        for (k = 0; k < QmS; ++k)
                            result[y * QmS + k] = -2 * Qq;

                    y += size;

                }

            }
    );
}

int boost_compute_R_S_code (std::array<int, QmS * Nn> &in, std::array<int, Qm * Nn> &out);
int boost_compute_R_S_decode (std::array<int, Qm * Nn> &in, std::array<int, QmS * Nn> &out);

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

    //R_S_code(in, out);

    boost_compute_R_S_code(in, out);

    for (i = 0; i < Qm * Nn; ++i) {
        printf("%d \t", out[i]);
    }

    printf("\n");
/*
    out[14] = 6;
    out[22] = 8;
    out[23] = 7;
    out[31] = 2;
    out[32] = 6;

    for (i = 0; i < Qm * Nn; ++i) {
        printf("%d \t", out[i]);
    }

    printf("\n");
*/
 //   R_S_decode(out, in);
    boost_compute_R_S_decode(out, in);

    for (i = 0; i < QmS * Nn; ++i) {
        printf("%d \t", in[i]);
    }

    printf("\n");


    std::cout << "Hello, World!" << std::endl;
    return 0;
}


int boost_compute_R_S_code (std::array<int, QmS * Nn> &in, std::array<int, Qm * Nn> &out)
{
    boost::compute::device gpu = boost::compute::system::default_device();
    boost::compute::context context(gpu);

    std::cout << "\n device: " << gpu.name() << std::endl;

    const char source[] =
            "__kernel void code ("
            "        const int *matrixG,"
            "        const int *matrixmultiply,"
            "        const int *matrixplus,"
            "        const int *vector_for_code,"
            "        int *result ) {"
            "    uint x = get_global_id(0);"
            "    uint size = get_global_size(0);"
            "    uint i = 0, k = 0, sum = 0, j = 0, y = x;"
            "    for (j = 0; j * size + x < Nn; ++j) {"
            "        for (i = 0; i < QmS; ++i)"
            "            result[y * Qm + i] = vector_for_code[y * QmS + i];"
            "        for (i = QmS; i < Qm; ++i) {"
            "            sum = 0;"
            "            for (k = 0; k < QmS; ++k)"
            "                sum = matrixplus[sum * Qq + matrixmultiply[vector_for_code[y * QmS + k] * Qq + matrixG[k * Ss + (i - QmS)]]];"
            "            result[y * Qm + i] = sum;"
            "        }"
            "        y += size;"
            "    }"
            "}";
    boost::compute::program program = boost::compute::program::create_with_source(source, context);

    program.build();

    boost::compute::kernel kernel(program, "add");

    boost::compute::command_queue queue(context, gpu);

    boost::compute::buffer buffer_matrix_G(context, QmS * Ss * sizeof(int));
    boost::compute::buffer buffer_matrix_multiply(context, Qq * Qq * sizeof(int));
    boost::compute::buffer buffer_matrix_plus(context, Qq * Qq * sizeof(int));
    boost::compute::buffer buffer_vec_for_code(context, QmS * Nn * sizeof(int));
    boost::compute::buffer buffer_result(context, Qm * Nn * sizeof(int));

    kernel.set_arg(0, buffer_matrix_G);
    kernel.set_arg(1, buffer_matrix_multiply);
    kernel.set_arg(2, buffer_matrix_plus);
    kernel.set_arg(3, buffer_vec_for_code);
    kernel.set_arg(4, buffer_result);

    queue.enqueue_write_buffer(buffer_matrix_G, 0, QmS * Ss * sizeof(int), &G);
    queue.enqueue_write_buffer(buffer_matrix_multiply, 0, Qq * Qq * sizeof(int), &multy_matrix);
    queue.enqueue_write_buffer(buffer_matrix_plus, 0, Qq * Qq * sizeof(int), &plus);
    queue.enqueue_write_buffer(buffer_vec_for_code, 0, QmS * Nn * sizeof(int), &in);

    queue.enqueue_1d_range_kernel(kernel, 0, NULL, NULL);

    queue.enqueue_read_buffer(buffer_result, 0, Qm * Nn * sizeof(int), &out);



    return 0;
}



int boost_compute_R_S_decode (std::array<int, Qm * Nn> &in, std::array<int, QmS * Nn> &out)
{



    return 0;
}



/*__read_only int *matrixG,
                                __read_only int *matrixmultiply,
                                __read_only int *matrixplus,
                                __read_only int *vector_for_code,
                                __write_only int *result */