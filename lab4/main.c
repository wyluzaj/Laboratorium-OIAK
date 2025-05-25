#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_NUMBERS     8192
#define PER_ARRAY_NUMBER  (TOTAL_NUMBERS/2)
#define NVECS             (PER_ARRAY_NUMBER/4)
#define NTESTS            10


typedef uint32_t Vec4 __attribute__((vector_size(16)));

// tablice dla SIMD
 Vec4 A_vec[NVECS];
 Vec4 B_vec[NVECS];
 Vec4 C_vec[NVECS];

// tablice dla SISD
 uint32_t A_sisd[PER_ARRAY_NUMBER];
 uint32_t B_sisd[PER_ARRAY_NUMBER];
 uint32_t C_sisd[PER_ARRAY_NUMBER];

// generator 32-bitowych liczb pseudolosowych
uint32_t rand32(void) {
    uint32_t high = rand() & 0xFFFF;       // 16 losowych bitów
    uint32_t low = rand() & 0xFFFF;       // kolejne 16 bitów
    return (high << 16) | low;              // sklejamy w 32-bitową wartość
}

// wypełnienie wektorów i tablic SISD
static void fill_data(void) {
    for (size_t i = 0; i < NVECS; ++i) {
        A_vec[i] = (Vec4){ rand32(), rand32(), rand32(), rand32() };
        B_vec[i] = (Vec4){ rand32(), rand32(), rand32(), rand32() };

    }
    for (size_t i = 0; i < PER_ARRAY_NUMBER; ++i) {
        A_sisd[i] = rand32();
        B_sisd[i] = rand32();
    }
}

// różnica czasu w nanosekundach
uint64_t diff_ns(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) * 1000000000ULL + (end->tv_nsec - start->tv_nsec);
}

//SIMD

uint64_t simd_add(void) {
    struct timespec t0,t1;
    Vec4 *pa=A_vec,*pb=B_vec,*pc=C_vec;
    clock_gettime(CLOCK_MONOTONIC,&t0);
    for (size_t i=0;i<NVECS;++i){
        __asm__ volatile(
                "movdqa (%[pa]),%%xmm0\n\t"
                "movdqa (%[pb]),%%xmm1\n\t"
                "paddd  %%xmm1,%%xmm0\n\t"
                "movdqa %%xmm0,(%[pc])\n\t"
                : : [pa]"r"(pa),[pb]"r"(pb),[pc]"r"(pc)
        : "xmm0","xmm1","memory");
        pa++; pb++; pc++;
    }
    clock_gettime(CLOCK_MONOTONIC,&t1);return diff_ns(&t0,&t1);
}
uint64_t simd_sub(void) {
    struct timespec t0,t1;
    Vec4 *pa=A_vec,*pb=B_vec,*pc=C_vec;
    clock_gettime(CLOCK_MONOTONIC,&t0);
    for(size_t i=0;i<NVECS;++i){
        __asm__ volatile(
                "movdqa (%[pa]),%%xmm0\n\t"
                "movdqa (%[pb]),%%xmm1\n\t"
                "psubd  %%xmm1,%%xmm0\n\t"
                "movdqa %%xmm0,(%[pc])\n\t"
                : : [pa]"r"(pa),[pb]"r"(pb),[pc]"r"(pc)
        : "xmm0","xmm1","memory");
        pa++; pb++; pc++;
    }
    clock_gettime(CLOCK_MONOTONIC,&t1);return diff_ns(&t0,&t1);
}
uint64_t simd_mul(void) {
    struct timespec t0,t1;
    Vec4 *pa=A_vec,*pb=B_vec,*pc=C_vec;
    clock_gettime(CLOCK_MONOTONIC,&t0);
    for(size_t i=0;i<NVECS;++i){
        __asm__ volatile(
                "movdqa (%[pa]),%%xmm0\n\t"
                "movdqa (%[pb]),%%xmm1\n\t"
                "pmulld %%xmm1,%%xmm0\n\t"
                "movdqa %%xmm0,(%[pc])\n\t"
                : : [pa]"r"(pa),[pb]"r"(pb),[pc]"r"(pc)
        : "xmm0","xmm1","memory");
        pa++; pb++; pc++;
    }
    clock_gettime(CLOCK_MONOTONIC,&t1);return diff_ns(&t0,&t1);
}
uint64_t simd_div(void) {
    struct timespec t0,t1;
    Vec4 *pa=A_vec,*pb=B_vec,*pc=C_vec;
    clock_gettime(CLOCK_MONOTONIC,&t0);
    for(size_t i=0;i<NVECS;++i){
        __asm__ volatile(
                "movdqa   (%[pa]),%%xmm0\n\t"
                "movdqa   (%[pb]),%%xmm1\n\t"
                "cvtdq2ps %%xmm0,%%xmm0\n\t"
                "cvtdq2ps %%xmm1,%%xmm1\n\t"
                "divps    %%xmm1,%%xmm0\n\t"
                "cvtps2dq %%xmm0,%%xmm0\n\t"
                "movdqa   %%xmm0,(%[pc])\n\t"
                : : [pa]"r"(pa),[pb]"r"(pb),[pc]"r"(pc)
        : "xmm0","xmm1","memory");
        pa++; pb++; pc++;
    }
    clock_gettime(CLOCK_MONOTONIC,&t1);return diff_ns(&t0,&t1);
}

//SISD

uint64_t sisd_add(void) {
    struct timespec t0,t1;
    clock_gettime(CLOCK_MONOTONIC,&t0);
    for (size_t i = 0; i < PER_ARRAY_NUMBER; i += 4) {
        __asm__ volatile(
                "mov  (%[a],%[i],4),%%eax\n\t"
                "add  (%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,(%[c],%[i],4)\n\t"

                "mov  4(%[a],%[i],4),%%eax\n\t"
                "add  4(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,4(%[c],%[i],4)\n\t"

                "mov  8(%[a],%[i],4),%%eax\n\t"
                "add  8(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,8(%[c],%[i],4)\n\t"

                "mov  12(%[a],%[i],4),%%eax\n\t"
                "add  12(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,12(%[c],%[i],4)\n\t"
                :
                : [a]"r"(A_sisd),[b]"r"(B_sisd),[c]"r"(C_sisd),[i]"r"(i)
        : "eax","memory","cc"
        );
    }
    clock_gettime(CLOCK_MONOTONIC,&t1);
    return diff_ns(&t0,&t1);
}
uint64_t sisd_sub(void) {
    struct timespec t0,t1;
    clock_gettime(CLOCK_MONOTONIC,&t0);
    for (size_t i = 0; i < PER_ARRAY_NUMBER; i += 4) {
        __asm__ volatile(
                "mov  (%[a],%[i],4),%%eax\n\t"
                "sub  (%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,(%[c],%[i],4)\n\t"

                "mov  4(%[a],%[i],4),%%eax\n\t"
                "sub  4(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,4(%[c],%[i],4)\n\t"

                "mov  8(%[a],%[i],4),%%eax\n\t"
                "sub  8(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,8(%[c],%[i],4)\n\t"

                "mov  12(%[a],%[i],4),%%eax\n\t"
                "sub  12(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,12(%[c],%[i],4)\n\t"
                :
                : [a]"r"(A_sisd),[b]"r"(B_sisd),[c]"r"(C_sisd),[i]"r"(i)
        : "eax","memory","cc"
        );
    }
    clock_gettime(CLOCK_MONOTONIC,&t1);
    return diff_ns(&t0,&t1);
}
uint64_t sisd_mul(void) {
    struct timespec t0,t1;
    clock_gettime(CLOCK_MONOTONIC,&t0);
    for (size_t i = 0; i < PER_ARRAY_NUMBER; i += 4) {
        __asm__ volatile(
                "mov  (%[a],%[i],4),%%eax\n\t"
                "imull (%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,(%[c],%[i],4)\n\t"

                "mov  4(%[a],%[i],4),%%eax\n\t"
                "imull 4(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,4(%[c],%[i],4)\n\t"

                "mov  8(%[a],%[i],4),%%eax\n\t"
                "imull 8(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,8(%[c],%[i],4)\n\t"

                "mov  12(%[a],%[i],4),%%eax\n\t"
                "imull 12(%[b],%[i],4),%%eax\n\t"
                "mov  %%eax,12(%[c],%[i],4)\n\t"
                :
                : [a]"r"(A_sisd),[b]"r"(B_sisd),[c]"r"(C_sisd),[i]"r"(i)
        : "eax","memory","cc"
        );
    }
    clock_gettime(CLOCK_MONOTONIC,&t1);
    return diff_ns(&t0,&t1);
}
uint64_t sisd_div(void) {
    struct timespec t0,t1;
    clock_gettime(CLOCK_MONOTONIC,&t0);
    for (size_t i = 0; i < PER_ARRAY_NUMBER; i += 4) {
        __asm__ volatile(
                "mov  (%[a],%[i],4),%%eax\n\t"
                "xor  %%edx,%%edx\n\t"
                "idivl (%[b],%[i],4)\n\t"
                "mov  %%eax,(%[c],%[i],4)\n\t"

                "mov  4(%[a],%[i],4),%%eax\n\t"
                "xor  %%edx,%%edx\n\t"
                "idivl 4(%[b],%[i],4)\n\t"
                "mov  %%eax,4(%[c],%[i],4)\n\t"

                "mov  8(%[a],%[i],4),%%eax\n\t"
                "xor  %%edx,%%edx\n\t"
                "idivl 8(%[b],%[i],4)\n\t"
                "mov  %%eax,8(%[c],%[i],4)\n\t"

                "mov  12(%[a],%[i],4),%%eax\n\t"
                "xor  %%edx,%%edx\n\t"
                "idivl 12(%[b],%[i],4)\n\t"
                "mov  %%eax,12(%[c],%[i],4)\n\t"
                :
                : [a]"r"(A_sisd),[b]"r"(B_sisd),[c]"r"(C_sisd),[i]"r"(i)
        : "eax","edx","memory","cc"
        );
    }
    clock_gettime(CLOCK_MONOTONIC,&t1);
    return diff_ns(&t0,&t1);
}



int main(void) {
    srand((unsigned)time(NULL));
    fill_data();

    uint64_t sum_simd_add=0, sum_simd_sub=0, sum_simd_mul=0, sum_simd_div=0;
    uint64_t sum_sisd_add=0, sum_sisd_sub=0, sum_sisd_mul=0, sum_sisd_div=0;

    for (int t = 0; t < NTESTS; ++t) {
        sum_simd_add += simd_add();
        sum_simd_sub += simd_sub();
        sum_simd_mul += simd_mul();
        sum_simd_div += simd_div();
        sum_sisd_add += sisd_add();
        sum_sisd_sub += sisd_sub();
        sum_sisd_mul += sisd_mul();
        sum_sisd_div += sisd_div();
    }

    double avg_simd_add = (double)sum_simd_add / NTESTS;
    double avg_simd_sub = (double)sum_simd_sub / NTESTS;
    double avg_simd_mul = (double)sum_simd_mul / NTESTS;
    double avg_simd_div = (double)sum_simd_div / NTESTS;
    double avg_sisd_add = (double)sum_sisd_add / NTESTS;
    double avg_sisd_sub = (double)sum_sisd_sub / NTESTS;
    double avg_sisd_mul = (double)sum_sisd_mul / NTESTS;
    double avg_sisd_div = (double)sum_sisd_div / NTESTS;

    printf("Typ obliczen: porownanie SIMD\n");
    printf("Liczba liczb lacznie: %d\n", TOTAL_NUMBERS);
    printf("Sredni czas [ns]:\n");
    printf("SIMD + : %.2f\n", avg_simd_add);
    printf("SIMD - : %.2f\n", avg_simd_sub);
    printf("SIMD * : %.2f\n", avg_simd_mul);
    printf("SIMD / : %.2f\n\n", avg_simd_div);

    printf("Typ obliczen: porownanie SISD\n");
    printf("Liczba liczb lacznie: %d\n", TOTAL_NUMBERS);
    printf("Sredni czas [ns]:\n");
    printf("SISD + : %.2f\n", avg_sisd_add);
    printf("SISD - : %.2f\n", avg_sisd_sub);
    printf("SISD * : %.2f\n", avg_sisd_mul);
    printf("SISD / : %.2f\n", avg_sisd_div);

    FILE *fp = fopen("SIMD_result_8192.txt","w");
    if (fp) {
        fprintf(fp, "Typ obliczen: porownanie SIMD\n");
        fprintf(fp, "Liczba liczb lacznie: %d\n", TOTAL_NUMBERS);
        fprintf(fp, "Sredni czas [ns]:\n");
        fprintf(fp, "SIMD + : %.2f\n", avg_simd_add);
        fprintf(fp, "SIMD - : %.2f\n", avg_simd_sub);
        fprintf(fp, "SIMD * : %.2f\n", avg_simd_mul);
        fprintf(fp, "SIMD / : %.2f\n", avg_simd_div);
        fclose(fp);
    }

    FILE *fp1 = fopen("SISD_result_8192.txt","w");
    if (fp1) {
        fprintf(fp, "Typ obliczen: porownanie SISD\n");
        fprintf(fp, "Liczba liczb lacznie: %d\n", TOTAL_NUMBERS);
        fprintf(fp, "Sredni czas [ns]:\n");
        fprintf(fp, "SISD + : %.2f\n", avg_sisd_add);
        fprintf(fp, "SISD - : %.2f\n", avg_sisd_sub);
        fprintf(fp, "SISD * : %.2f\n", avg_sisd_mul);
        fprintf(fp, "SISD / : %.2f\n", avg_sisd_div);
        fclose(fp);
    }

    return 0;
}

