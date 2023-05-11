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
#define DATA_VECTOR_SIZE 8192 // 2048, 4096 i 8192
#define REPEATS 10            // potarzamy test 10 razy

struct Vector128bit
{
    float x, y, z, w;
};

void generateData(Vector128bit *vector1, Vector128bit *vector2)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-1000000.0, 1000000.0);

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

void testSIMD(Vector128bit *a, Vector128bit *b, Vector128bit resultVector[][DATA_VECTOR_SIZE], double *avgTime)
{
    // pusty niemiarodajny przebieg
    for (int i = 0; i < REPEATS * 3; i++)
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

void saveResults(double *testResults, std::string fileName)
{
    std::ofstream file;
    file.open(fileName, std::ios_base::app);
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
    Vector128bit vector1[DATA_VECTOR_SIZE], vector2[DATA_VECTOR_SIZE];
    Vector128bit resultVector[OPERAIONS][DATA_VECTOR_SIZE];

    double testResults[OPERAIONS];
    srand(time(NULL));

    // test SIMD
    generateData(vector1, vector2);
    testSIMD(vector1, vector2, resultVector, testResults);

    // wyniki zapisywane do pliku - srednie czasy w mikrosekundach
    saveResults(testResults, "wyniki_simd_new.txt");

    // print test
    printf("Wektor 1: %f %f %f %f\n", vector1[0].x, vector1[0].y, vector1[0].z, vector1[0].w);
    printf("Wektor 2: %f %f %f %f\n", vector2[0].x, vector2[0].y, vector2[0].z, vector2[0].w);
    printf("Wynik: %f %f %f %f\n\n\n", resultVector[0][0].x, resultVector[0][0].y, resultVector[0][0].z, resultVector[0][0].w);

    printf("Wektor 1: %f %f %f %f\n", vector1[DATA_VECTOR_SIZE - 1].x, vector1[DATA_VECTOR_SIZE - 1].y, vector1[DATA_VECTOR_SIZE - 1].z, vector1[DATA_VECTOR_SIZE - 1].w);
    printf("Wektor 2: %f %f %f %f\n", vector2[DATA_VECTOR_SIZE - 1].x, vector2[DATA_VECTOR_SIZE - 1].y, vector2[DATA_VECTOR_SIZE - 1].z, vector2[DATA_VECTOR_SIZE - 1].w);
    printf("Wynik: %f %f %f %f\n\n\n", resultVector[0][DATA_VECTOR_SIZE - 1].x, resultVector[0][DATA_VECTOR_SIZE - 1].y, resultVector[0][DATA_VECTOR_SIZE - 1].z, resultVector[0][DATA_VECTOR_SIZE - 1].w);
    // print

    printf("SIMD Addition: %f\n", testResults[0]);
    printf("SIMD Subtraction: %f\n", testResults[1]);
    printf("SIMD Multiplication: %f\n", testResults[2]);
    printf("SIMD Division: %f\n", testResults[3]);

    return 0;
}

// https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C
// https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm
// https://en.wikibooks.org/wiki/X86_Assembly/SSE
// http://students.mimuw.edu.pl/~zbyszek/asm/pl/instrukcje-sse.html
// https://www.codeproject.com/Articles/5298048/Using-SIMD-to-Optimize-x86-Assembly-Code-in-Array#sse

// komendy ggdb => i r xmm0 | i r sse | disass | x/f memory | x/2048 memory