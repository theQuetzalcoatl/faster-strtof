#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SUCCESS (0u)
#define FAILURE (1u)

#define TRUE (1u)
#define FALSE (0u)

#ifdef __LITTLE_ENDIAN
    #define NaN_STRING_CODE (0x004e614eul)
    #define nan_STRING_CODE (0x006e616eul)

    #define Inf_STRING_CODE (0x00666E49ul)
    #define inf_STRING_CODE (0x00666E69ul)
#elif defined(__BIG_ENDIAN)
    #define NaN_STRING_CODE (0x4e614e00ul)
    #define nan_STRING_CODE (0x6e616e00ul)

    #define Inf_STRING_CODE (0x496E6600ul)
    #define inf_STRING_CODE (0x696E6600ul)
#endif

int32_t str2float(char *s, float *n)
{
    *n = 0.0f;
    uint32_t is_negative = FALSE;
    if(*s == '-'){
        is_negative = TRUE;
        ++s;
    }
    else if(*s == '+') ++s;
    else if(*(uint32_t *)s == NaN_STRING_CODE){
        *n = NAN;
        return SUCCESS;
    }
    else if(*(uint32_t *)s == nan_STRING_CODE){
        *n = NAN;
        return SUCCESS;
    }
    else if(*(uint32_t *)s == Inf_STRING_CODE){
        *n = INFINITY;
        return SUCCESS;
    }
    else if(*(uint32_t *)s == inf_STRING_CODE){
        *n = INFINITY;
        return SUCCESS;
    }
    else if(*s > '9' || *s < '0'){
        *n = 0.0f;
        return FAILURE;
    }
    else{
        *n += *s - '0';
        ++s;
    } 

    while(*s){
        if(*s <= '9' && *s >= '0'){
            *n *= 10;
            *n += *s - '0';
        } 
        else if(*s == '.'){
            ++s;
            float f_temp = 0.0f;
            uint32_t i = 0;
            for(i = 0; *s <= '9' && *s >= '0'; ++i, ++s){
                f_temp += *s - '0';
                f_temp *= 10;
            }
            if(*s != 0 && *s != 'e' && *s != 'E'){
                *n = 0.0f;
                return FAILURE;
            }
            f_temp /= 10;
            for(; i; --i) f_temp /= 10;
            *n += f_temp;
            --s;
        } 
        else if(*s == 'e' || *s == 'E'){
            uint32_t tmp = 0;
            ++s;
            if(*s <= '9' && *s >= '0'){
                while(*s <= '9' && *s >= '0'){
                    tmp += *s -'0';
                    tmp *= 10;
                    ++s;
                }
                tmp /= 10;
                if(*s == 0){
                    for(uint32_t iteration = tmp/4; iteration; --iteration) *n *= 10000;
                    for(uint32_t iteration = tmp%4; iteration; --iteration) *n *= 10;
                }
                else{
                    *n = 0.0f;
                    return FAILURE;
                }
                --s;
            }
            else if(*s == '-'){
                ++s;
                while(*s <= '9' && *s >= '0'){
                    tmp += *s -'0';
                    tmp *= 10;
                    ++s;
                }
                tmp /= 10;
                if(*s == 0){
                    for(uint32_t iteration = tmp/4; iteration; --iteration) *n /= 10000;
                    for(uint32_t iteration = tmp%4; iteration; --iteration) *n /= 10;
                }
                else{
                    *n = 0.0f;
                    return FAILURE;
                }
                --s;
            }
            else{
                *n = 0.0f;
                return FAILURE;
            }
        }
        else{
            *n = 0.0f;
            return FAILURE;
        }
        ++s;
    }
    if(is_negative) *n = *n*(-1);
    return SUCCESS;
}

int main(void)
{
    float f = 8.5e2f;

    struct timespec before = {0};
    struct timespec after = {0};

    uint32_t my_avg = 0;
    uint32_t sys_avg = 0;
    uint32_t n_runs = 0;
    char * endptr;

    char *test_inputs[] = {"1.2",
                          "-0.1",
                          "4.3e9",
                          "-8e2",
                          "+5",
                          "0",
                          "inf",
                          "nan",
                          "5000000000000000000",
                          "999999999999",
                          "-12e-39",
                          "0005.666",
                          "22E39"
                         };

    for(unsigned int i = 0; i < sizeof(test_inputs)/sizeof(test_inputs[0]); ++i){
        printf("INPUT: %s\n", test_inputs[i]);
        clock_gettime(CLOCK_MONOTONIC ,&before);
        str2float(test_inputs[i], &f);
        clock_gettime(CLOCK_MONOTONIC, &after);
        printf("my:  %ld ns", after.tv_nsec-before.tv_nsec);printf("\t%.15f\n", f);
        my_avg += after.tv_nsec-before.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC ,&before);
        f = strtof(test_inputs[i], &endptr);
        clock_gettime(CLOCK_MONOTONIC ,&after);
        printf("sys: %ld ns", after.tv_nsec-before.tv_nsec);printf("\t%.15f\n", f);
        sys_avg += after.tv_nsec-before.tv_nsec;
        ++n_runs;
        printf("\n");
    }

    printf("my average:     %.2f ns\n", (float)my_avg/n_runs);
    printf("system average: %.2f ns\n", (float)sys_avg/n_runs);

    return 0;
}

/*
 * main.c
 * This file is part of faster-strtof
 *
 * Copyright (C) 2021 - theQuetzalcoatl
 *
 * faster-strtof is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * faster-strtof is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with faster-strtof. If not, see <http://www.gnu.org/licenses/>.
 */