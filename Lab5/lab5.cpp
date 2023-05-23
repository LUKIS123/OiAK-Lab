#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <time.h>
#include <chrono>
#include <math.h>
#include <random>
#include <bits/stdc++.h>
#include <limits>

#define OPERAIONS 4           // + - * /
#define REPEATS 10            // powtarzamy test 10 razy
#define DATA_VECTOR_SIZE 2048 // 2048, 4096 i 8192

struct Vector128bit
{
    float x, y, z, w;
};

void generateData(Vector128bit *vector1, Vector128bit *vector2)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    // std::uniform_real_distribution<float> distribution(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
    std::uniform_real_distribution<float> distribution(-10.0, 10.0);

    for (int i = 0; i < DATA_VECTOR_SIZE; i++)
    {
        vector1[i].x = distribution(generator);
        vector1[i].y = distribution(generator);
        vector1[i].z = distribution(generator);
        vector1[i].w = distribution(generator);

        vector2[i].x = distribution(generator);
        vector2[i].y = distribution(generator);
        vector2[i].z = distribution(generator);
        vector2[i].w = distribution(generator);
    }
}

void additionSIMD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "addps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=xm"(*resultVector)
        : "xm"(*vector1), "xm"(*vector2)
        : "xmm0", "xmm1");
}

void subtractionSIMD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "subps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=xm"(*resultVector)
        : "xm"(*vector1), "xm"(*vector2)
        : "xmm0", "xmm1");
}

void multiplicationSIMD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "mulps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=xm"(*resultVector)
        : "xm"(*vector1), "xm"(*vector2)
        : "xmm0", "xmm1");
}

void divisonSIMD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "divps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=xm"(*resultVector)
        : "xm"(*vector1), "xm"(*vector2)
        : "xmm0", "xmm1");
}

void additionSISD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    asm volatile(
        "flds %4\n"
        "flds %8\n"
        "faddp\n"
        "fstps %0\n"

        "flds %5\n"
        "flds %9\n"
        "faddp\n"
        "fstps %1\n"

        "flds %6\n"
        "flds %10\n"
        "faddp\n"
        "fstps %2\n"

        "flds %7\n"
        "flds %11\n"
        "faddp\n"
        "fstps %3\n"

        : "=m"(resultVector->x), "=m"(resultVector->y), "=m"(resultVector->z), "=m"(resultVector->w)
        : "m"(vector1->x), "m"(vector1->y), "m"(vector1->z), "m"(vector1->w),
          "m"(vector2->x), "m"(vector2->y), "m"(vector2->z), "m"(vector2->w));
}

void subtractionSISD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    asm volatile(
        "flds %4\n"
        "flds %8\n"
        "fsubp\n"
        "fstps %0\n"

        "flds %5\n"
        "flds %9\n"
        "fsubp\n"
        "fstps %1\n"

        "flds %6\n"
        "flds %10\n"
        "fsubp\n"
        "fstps %2\n"

        "flds %7\n"
        "flds %11\n"
        "fsubp\n"
        "fstps %3\n"

        : "=m"(resultVector->x), "=m"(resultVector->y), "=m"(resultVector->z), "=m"(resultVector->w)
        : "m"(vector1->x), "m"(vector1->y), "m"(vector1->z), "m"(vector1->w),
          "m"(vector2->x), "m"(vector2->y), "m"(vector2->z), "m"(vector2->w));
}

void multiplicationSISD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    asm volatile(
        "flds %4\n"
        "flds %8\n"
        "fmulp\n"
        "fstps %0\n"

        "flds %5\n"
        "flds %9\n"
        "fmulp\n"
        "fstps %1\n"

        "flds %6\n"
        "flds %10\n"
        "fmulp\n"
        "fstps %2\n"

        "flds %7\n"
        "flds %11\n"
        "fmulp\n"
        "fstps %3\n"

        : "=m"(resultVector->x), "=m"(resultVector->y), "=m"(resultVector->z), "=m"(resultVector->w)
        : "m"(vector1->x), "m"(vector1->y), "m"(vector1->z), "m"(vector1->w),
          "m"(vector2->x), "m"(vector2->y), "m"(vector2->z), "m"(vector2->w));
}

void divisionSISD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    asm volatile(
        "flds %4\n"
        "flds %8\n"
        "fdivp\n"
        "fstps %0\n"

        "flds %5\n"
        "flds %9\n"
        "fdivp\n"
        "fstps %1\n"

        "flds %6\n"
        "flds %10\n"
        "fdivp\n"
        "fstps %2\n"

        "flds %7\n"
        "flds %11\n"
        "fdivp\n"
        "fstps %3\n"

        : "=m"(resultVector->x), "=m"(resultVector->y), "=m"(resultVector->z), "=m"(resultVector->w)
        : "m"(vector1->x), "m"(vector1->y), "m"(vector1->z), "m"(vector1->w),
          "m"(vector2->x), "m"(vector2->y), "m"(vector2->z), "m"(vector2->w));
}

void testSIMD(Vector128bit *a, Vector128bit *b, Vector128bit resultVector[][DATA_VECTOR_SIZE], double *avgTime)
{
    // pusty niemiarodajny przebieg
    for (int i = 0; i < REPEATS * 5; i++)
    {
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            additionSIMD(&a[j], &b[j], &resultVector[0][j]);
            subtractionSIMD(&a[j], &b[j], &resultVector[1][j]);
            multiplicationSIMD(&a[j], &b[j], &resultVector[2][j]);
            divisonSIMD(&a[j], &b[j], &resultVector[3][j]);
        }
    }

    // tablica przechowujaca sume czasow dla kazdej operacji
    double singleResultTime[OPERAIONS][REPEATS];

    for (int i = 0; i < REPEATS; i++)
    {

        double additionTime = 0, subtractionTime = 0, multiplicationTime = 0, divisionTime = 0;
        // dodawanie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            additionSIMD(&a[j], &b[j], &resultVector[0][j]);

            auto end = std::chrono::high_resolution_clock::now();
            additionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        singleResultTime[0][i] = additionTime;

        // odejmowanie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            subtractionSIMD(&a[j], &b[j], &resultVector[1][j]);

            auto end = std::chrono::high_resolution_clock::now();
            subtractionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        singleResultTime[1][i] = subtractionTime;

        // mnozenie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            multiplicationSIMD(&a[j], &b[j], &resultVector[2][j]);

            auto end = std::chrono::high_resolution_clock::now();
            multiplicationTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        singleResultTime[2][i] = multiplicationTime;

        // dzielenie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            divisonSIMD(&a[j], &b[j], &resultVector[3][j]);

            auto end = std::chrono::high_resolution_clock::now();
            divisionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        singleResultTime[3][i] = divisionTime;
    }

    // zapisywanie sredniej czasow do tablicy wynikowej
    for (int i = 0; i < OPERAIONS; i++)
    {
        double timeSum = 0;
        for (int j = 0; j < REPEATS; j++)
        {
            timeSum += singleResultTime[i][j];
        }
        // dzielenie przez liczbe powtorzen oraz 1000 - zamiana ns na us
        avgTime[i] = timeSum / REPEATS / 1000;
    }
}

void testSSID(Vector128bit *a, Vector128bit *b, Vector128bit resultVector[][DATA_VECTOR_SIZE], double *avgTime)
{
    // additionSISD(&a[0], &b[0], &resultVector[0][0]);
    // Vector128bit *siema = &resultVector[0][0];
    // printf("xxx %f\n", siema->x);
    // printf("Siema %f\n", a[0].x);
    // printf("Siema %f\n", b[0].x);
    // printf("Siema %f\n", resultVector[0][0].x);
    // printf("Siema %f\n", a[0].z);
    // printf("Siema %f\n", b[0].z);
    // printf("Siema %f\n", resultVector[0][0].z);

    // pusty niemiarodajny przebieg
    for (int i = 0; i < REPEATS * 5; i++)
    {
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            additionSISD(&a[j], &b[j], &resultVector[0][j]);
            subtractionSISD(&a[j], &b[j], &resultVector[1][j]);
            multiplicationSISD(&a[j], &b[j], &resultVector[2][j]);
            divisionSISD(&a[j], &b[j], &resultVector[3][j]);
        }
    }

    // tablica przechowujaca sume czasow dla kazdej operacji
    double singleResultTime[OPERAIONS][REPEATS];

    for (int i = 0; i < REPEATS; i++)
    {

        double additionTime = 0, subtractionTime = 0, multiplicationTime = 0, divisionTime = 0;
        // dodawanie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            additionSISD(&a[j], &b[j], &resultVector[0][j]);

            auto end = std::chrono::high_resolution_clock::now();
            additionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        singleResultTime[0][i] = additionTime;

        // odejmowanie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            subtractionSISD(&a[j], &b[j], &resultVector[1][j]);

            auto end = std::chrono::high_resolution_clock::now();
            subtractionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        singleResultTime[1][i] = subtractionTime;

        // mnozenie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            multiplicationSISD(&a[j], &b[j], &resultVector[2][j]);

            auto end = std::chrono::high_resolution_clock::now();
            multiplicationTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        singleResultTime[2][i] = multiplicationTime;

        // dzielenie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            divisionSISD(&a[j], &b[j], &resultVector[3][j]);

            auto end = std::chrono::high_resolution_clock::now();
            divisionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        singleResultTime[3][i] = divisionTime;
    }

    // zapisywanie sredniej czasow do tablicy wynikowej
    for (int i = 0; i < OPERAIONS; i++)
    {
        double timeSum = 0;
        for (int j = 0; j < REPEATS; j++)
        {
            timeSum += singleResultTime[i][j];
        }
        // dzielenie przez liczbe powtorzen oraz 1000 - zamiana ns na us
        avgTime[i] = timeSum / REPEATS / 1000;
    }
}

void saveResults(double *testResults, std::string fileName, bool operationTypeSIMD)
{
    std::string operationType = "SIMD";
    if (!operationTypeSIMD)
    {
        operationType = "SISD";
    }

    std::ofstream file;
    file.open(fileName, std::ios_base::app);
    if (file.is_open())
    {
        file << "Typ obliczen: " << operationType
             << "\n";
        if (file.fail())
        {
            printf("File error - WRITE\n");
        }
        else
        {
            file << "Liczba liczb: " << DATA_VECTOR_SIZE << "\n";
            file << "Sredni czas [us]:\n";
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
}

int main()
{
    srand(time(NULL));
    double testResultsSIMD[OPERAIONS];
    double testResultsSISD[OPERAIONS];

    Vector128bit vector1[DATA_VECTOR_SIZE], vector2[DATA_VECTOR_SIZE];
    Vector128bit resultVector[OPERAIONS][DATA_VECTOR_SIZE];
    generateData(vector1, vector2);

    // test SIMD
    testSIMD(vector1, vector2, resultVector, testResultsSIMD);

    // test SSID
    testSSID(vector1, vector2, resultVector, testResultsSISD);

    // wyniki zapisywane do pliku - srednie czasy w mikrosekundach
    saveResults(testResultsSIMD, "wyniki_simd.txt", true);
    saveResults(testResultsSISD, "wyniki_sisd.txt", false);

    // wyniki wypisywane na ekran
    printf("SIMD Addition: %f\n", testResultsSIMD[0]);
    printf("SIMD Subtraction: %f\n", testResultsSIMD[1]);
    printf("SIMD Multiplication: %f\n", testResultsSIMD[2]);
    printf("SIMD Division: %f\n", testResultsSIMD[3]);

    printf("SISD Addition: %f\n", testResultsSISD[0]);
    printf("SISD Subtraction: %f\n", testResultsSISD[1]);
    printf("SISD Multiplication: %f\n", testResultsSISD[2]);
    printf("SISD Division: %f\n", testResultsSISD[3]);

    return 0;
}

// https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C
// https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm
// https://en.wikibooks.org/wiki/X86_Assembly/SSE
// http://students.mimuw.edu.pl/~zbyszek/asm/pl/instrukcje-sse.html
// https://www.codeproject.com/Articles/5298048/Using-SIMD-to-Optimize-x86-Assembly-Code-in-Array#sse

// komendy ggdb => i r xmm0 | i r sse | disass | x/f memory | x/2048 memory
