#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <time.h>
#include <chrono>
#include <math.h>
#include <random>

// #define NANO_PER_SEC 1000000000
// #define MICRO_PER_SEC 1000000
// #define CPU_FREQ 3300000000.0 // 3.3 GHz

#define VECTOR_SIZE 4096 // 2048 4096 8192
#define TEST_ITERATIONS 10
#define OPERATIONS 4

struct vector128bit
{
    float x, y, z, w;
};

// unsigned long long timestamp()
// {
//     unsigned long long tsc;

//     asm volatile(
//         "xor %%rax, %%rax\n"
//         "cpuid\n"
//         "rdtsc\n"
//         "shl $32, %%rdx\n"
//         "or %%rdx, %%rax"
//         : "=A"(tsc)
//         :
//         : "rbx", "rcx");

//     return tsc;
// }

void addSIMD(vector128bit *a, vector128bit *b, vector128bit *result, int index)
{
    vector128bit *output = &result[index];

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "addps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=xm"(*output)
        : "xm"(*a), "xm"(*b)
        : "xmm0", "xmm1");
}

void subSIMD(vector128bit *a, vector128bit *b, vector128bit *result, int index)
{
    vector128bit *output = &result[index];

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "subps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=xm"(*output)
        : "xm"(*a), "xm"(*b)
        : "xmm0", "xmm1");
}

void mulSIMD(vector128bit *a, vector128bit *b, vector128bit *result, int index)
{
    vector128bit *output = &result[index];

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "mulps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=xm"(*output)
        : "xm"(*a), "xm"(*b)
        : "xmm0", "xmm1");
}

void divSIMD(vector128bit *a, vector128bit *b, vector128bit *result, int index)
{
    vector128bit *output = &result[index];

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "divps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=xm"(*output)
        : "xm"(*a), "xm"(*b)
        : "xmm0", "xmm1");
}

void generateRandomData(vector128bit *vector)
{
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dist(__FLT_MIN__, __FLT_MAX__);

    vector->x = dist(gen);
    vector->y = dist(gen);
    vector->z = dist(gen);
    vector->w = dist(gen);
}

void testSIMD(vector128bit *a, vector128bit *b, vector128bit *result, double *avgTime)
{
    // pusty przebieg - aby wyniki byly bardziej wiarygodne
    for (int i = 0; i < TEST_ITERATIONS; i++)
    {
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            addSIMD(&a[j], &b[j], result, j);
            subSIMD(&a[j], &b[j], result, j);
            mulSIMD(&a[j], &b[j], result, j);
            divSIMD(&a[j], &b[j], result, j);
        }
    }

    // tablica przechowujaca sume czasow dla kazdej operacji
    double resultTime[OPERATIONS][TEST_ITERATIONS];

    for (int i = 0; i < TEST_ITERATIONS; i++)
    {
        // dodawanie
        double timeADD = 0;
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            unsigned long long tstamp1, tstamp2;

            auto start = std::chrono::high_resolution_clock::now();
            addSIMD(&a[j], &b[j], result, j);
            auto finish = std::chrono::high_resolution_clock::now();

            timeADD += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
        }
        resultTime[0][i] = timeADD;

        // odejmowanie
        double timeSUB = 0;
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            unsigned long long tstamp1, tstamp2;

            auto start = std::chrono::high_resolution_clock::now();
            subSIMD(&a[j], &b[j], result, j);
            auto finish = std::chrono::high_resolution_clock::now();

            timeSUB += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
        }
        resultTime[1][i] = timeSUB;

        // mnozenie
        double timeMUL = 0;
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            unsigned long long tstamp1, tstamp2;

            auto start = std::chrono::high_resolution_clock::now();
            mulSIMD(&a[j], &b[j], result, j);
            auto finish = std::chrono::high_resolution_clock::now();

            timeMUL += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
        }
        resultTime[2][i] = timeMUL;

        // dzielenie
        double timeDIV = 0;
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            unsigned long long tstamp1, tstamp2;

            auto start = std::chrono::high_resolution_clock::now();
            divSIMD(&a[j], &b[j], result, j);
            auto finish = std::chrono::high_resolution_clock::now();

            timeDIV += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
        }
        resultTime[3][i] = timeDIV;
    }

    // zapisywanie sredniej czasow do tablicy wynikowej
    for (int i = 0; i < OPERATIONS; i++)
    {
        double timeSum = 0;
        for (int j = 0; j < TEST_ITERATIONS; j++)
        {
            timeSum += resultTime[i][j];
        }
        avgTime[i] = timeSum / TEST_ITERATIONS;
    }
}

int main()
{
    vector128bit vector1[VECTOR_SIZE], vector2[VECTOR_SIZE], outputVector[VECTOR_SIZE];

    srand(time(NULL));
    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        generateRandomData(&vector1[i]);
        generateRandomData(&vector2[i]);
    }

    double testResults[OPERATIONS];

    testSIMD(vector1, vector2, outputVector, testResults);

    printf("SIMD Addition: %f\n", testResults[0]);
    printf("SIMD Subtraction: %f\n", testResults[1]);
    printf("SIMD Multiplication: %f\n", testResults[2]);
    printf("SIMD Division: %f\n", testResults[3]);

    std::ofstream file;
    file.open("wyniki_simd.txt", std::ios_base::app);
    if (file.is_open())
    {
        file << "Typ obliczen: SIMD"
             << "\n";
        if (file.fail())
        {
            printf("File error - WRITE\n");
        }
        else
        {
            file << "Liczba liczb: " << VECTOR_SIZE << "\n";
            file << "Sredni czas [ns]:\n";
            file << "+ : " << testResults[0] << "\n";
            file << "- : " << testResults[1] << "\n";
            file << "* : " << testResults[2] << "\n";
            file << "/ : " << testResults[3] << "\n\n";

            file.close();
        }
    }
    else
    {
        printf("File error - OPEN\n");
    }

    return 0;
}

// https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C"
// "https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm"
// "https://en.wikibooks.org/wiki/X86_Assembly/SSE"
// "http://students.mimuw.edu.pl/~zbyszek/asm/pl/instrukcje-sse.html"
// https://www.codeproject.com/Articles/5298048/Using-SIMD-to-Optimize-x86-Assembly-Code-in-Array#sse

// koimendy ggdb => i r xmm0 | i r sse | disass | x/f memory | x/2048 memory
