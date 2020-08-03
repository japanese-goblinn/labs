//
//  main.cpp
//  avs
//
//  Created by Kirill Gorbachyonok on 5/1/19.
//  Copyright © 2019 saisuicied. All rights reserved.
//

#include <iostream>
#include <immintrin.h>
#include <xmmintrin.h>


int main(int argc, const char * argv[]) {
    bool is_complex = false;
    float A, B, C;
    float root, dis;
    float res[4] = {0, 0, 0, 0};
    __m128 reg_one, reg_two;
    std::cin >> A >> B >> C;
    if (!A) {
        if (!B) {
            if (!C) {
                std::cout << "Solution is any number" << std::endl;
                return 0;
            }
            std::cout << "No solution" << std::endl;
            return 0;
        }
        reg_one = _mm_setzero_ps();
        reg_two = _mm_set_ss(C);
        reg_one = _mm_sub_ss(reg_one, reg_two);
        reg_two = _mm_set_ss(B);
        reg_one = _mm_div_ss(reg_one, reg_two);
        _mm_store_ss(&root, reg_one);
        std::cout << "Lineral equation" << std::endl;
        std::cout << "Root: " << root << std::endl;
        return 0;
    } else {
        reg_one = _mm_setr_ps(A, B, A, A);
        reg_two = _mm_setr_ps(C, B, -2, 2);
        reg_one = _mm_mul_ps(reg_one, reg_two);
        reg_two = _mm_set_ss(4);
        reg_one = _mm_mul_ss(reg_one, reg_two); // [4ac, b^2, -2a, 2a]
        _mm_store_ps(res, reg_one);
        reg_one = _mm_set_ss(res[1]);
        reg_two = _mm_set_ss(res[0]);
        reg_one = _mm_sub_ss(reg_one, reg_two); // [b^2-4ac, 0, 0, 0]
        _mm_store_ss(&dis, reg_one);
        if (dis < 0) {
            is_complex = true;
            reg_two = _mm_set_ss(-1);
            reg_one = _mm_mul_ss(reg_one, reg_two); // [-dis, 0, 0, 0]
        }
    }
    reg_one = _mm_sqrt_ss(reg_one);
    _mm_store_ss(&dis, reg_one);
    reg_one = _mm_setr_ps(-B, dis, -dis, 0);
    reg_two = _mm_setr_ps(res[3], res[3], res[3], 1);
    reg_one = _mm_div_ps(reg_one, reg_two); // [-b/2a, dis/2a, -dis/2a, 0]
    _mm_store_ps(res, reg_one);
    if (!dis) {
        std::cout << "Roots are real and the same" << std::endl;
        std::cout << "Root: " << res[0] << std::endl;
        return 0;
    }
    if (is_complex) {
        std::cout << "Roots are complex" << std::endl;
        std::cout << res[0] << "+" << res[1] << "i" << std::endl;
        std::cout << res[0] << "-" << res[1] << "i" << std::endl;
        return 0;
    }
    reg_two = _mm_set_ps1(res[0]);
    reg_one = _mm_add_ps(reg_one, reg_two);
    _mm_store_ps(res, reg_one);
    std::cout << "Roots are real " << std::endl;
    std::cout << "Roots: " << res[1] << " " << res[2] << std::endl;
}
