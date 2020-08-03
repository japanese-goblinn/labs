//
//  main.cpp
//  lab4
//
//  Created by Kirill Gorbachyonok on 5/8/19.
//  Copyright © 2019 saisuicied. All rights reserved.
//

#include <iostream>
#include <immintrin.h>

//sysctl -a | grep cpu.feat

#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef __GNUC__

void __cpuid(int* cpuinfo, int info)
{
    __asm__ __volatile__(
                         "xchg %%ebx, %%edi;"
                         "cpuid;"
                         "xchg %%ebx, %%edi;"
                         :"=a" (cpuinfo[0]), "=D" (cpuinfo[1]), "=c" (cpuinfo[2]), "=d" (cpuinfo[3])
                         :"0" (info)
                         );
}

unsigned long long _xgetbv(unsigned int index)
{
    unsigned int eax, edx;
    __asm__ __volatile__(
                         "xgetbv;"
                         : "=a" (eax), "=d"(edx)
                         : "c" (index)
                         );
    return ((unsigned long long)edx << 32) | eax;
}

#endif

bool is_avx_supported() {
    bool avxSupportted = false;
    int cpuinfo[4];
    __cpuid(cpuinfo, 1);
    avxSupportted = cpuinfo[2] & (1 << 28) || false;
    bool osxsaveSupported = cpuinfo[2] & (1 << 27) || false;
    if (osxsaveSupported && avxSupportted)
    {
        unsigned long long xcrFeatureMask = _xgetbv(0);
        avxSupportted = (xcrFeatureMask & 0x6) == 0x6;
    }
    return avxSupportted ? true: false;
}

union Double
{
    __m256d vector[2];
    double array[8];
};

union Float
{
    __m256 vector;
    float array[8];
};

Double* floatToDouble(Float from) {
    Double* to = new Double;
    for (int i = 0; i < 8; i++) {
        to->array[i] = (double)from.array[i];
    }
    return to;
}

int main(int argc, const char * argv[]) {
    
    //    F[i] = A[i] - B[i] + C[i] - D[i], i=1..8
    
    if (is_avx_supported()) {
        float a[] = { 2.0f, 4.0f, 1.0f, 5.0f, 6.0f, 9.0f, 10.0f, 11.0f };
        float b[] = { 1.0f, 2.0f, 3.0f, 4.0f, 7.0f, 16.0f, 9.0f, 8.0f };
        float c[] = { 11.0f, 23.0f, 24.0f, 13.0f, 26.0f, 89.0f, 3.0f, 5.0f };
        double d[] = { 1.0f, 2.0f, 3.0f, 4.0f, 7.0f, 16.0f, 9.0f, 8.0f };
        
        Float* A = (Float*)a;
        Float* B = (Float*)b;
        Float* C = (Float*)c;
        Double* D = (Double*)d;
        
        Float *a_sub_b = new Float;
        a_sub_b->vector = _mm256_sub_ps(A->vector, B->vector);
        Double* a_sub_b_to_double = floatToDouble(*a_sub_b);
        Double* c_to_double = floatToDouble(*C);
        Double *c_sub_d = new Double;
        c_sub_d->vector[0] = _mm256_sub_pd(c_to_double->vector[0], D->vector[0]);
        c_sub_d->vector[1] = _mm256_sub_pd(c_to_double->vector[1], D->vector[1]);
        Double *F = new Double;
        F->vector[0] = _mm256_add_pd(a_sub_b_to_double->vector[0], c_sub_d->vector[0]);
        F->vector[1] = _mm256_add_pd(a_sub_b_to_double->vector[1], c_sub_d->vector[1]);
        for (double x: F->array) {
            std::cout << x << " ";
        }
    } else {
        std::cout << "AVX is not supported on your CPU" << std::endl;
    }
}

