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
    std::uniform_real_distribution<float> distribution(-1000000.0, 1000000.0);
    // std::uniform_real_distribution<float> distribution(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());

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

double additionSIMD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    auto start = std::chrono::high_resolution_clock::now();

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "addps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=m"(*resultVector)
        : "m"(*vector1), "m"(*vector2)
        : "xmm0", "xmm1");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

double subtractionSIMD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    auto start = std::chrono::high_resolution_clock::now();

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "subps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=m"(*resultVector)
        : "m"(*vector1), "m"(*vector2)
        : "xmm0", "xmm1");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

double multiplicationSIMD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    auto start = std::chrono::high_resolution_clock::now();

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "mulps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=m"(*resultVector)
        : "m"(*vector1), "m"(*vector2)
        : "xmm0", "xmm1");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

double divisonSIMD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    auto start = std::chrono::high_resolution_clock::now();

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "divps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=m"(*resultVector)
        : "m"(*vector1), "m"(*vector2)
        : "xmm0", "xmm1");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

double additionSISD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    auto start = std::chrono::high_resolution_clock::now();

    asm volatile(
        "flds %8\n"
        "flds %4\n"
        "faddp\n"
        "fstps %0\n"

        "flds %9\n"
        "flds %5\n"
        "faddp\n"
        "fstps %1\n"

        "flds %10\n"
        "flds %6\n"
        "faddp\n"
        "fstps %2\n"

        "flds %11\n"
        "flds %7\n"
        "faddp\n"
        "fstps %3\n"

        : "=m"(resultVector->x), "=m"(resultVector->y), "=m"(resultVector->z), "=m"(resultVector->w)
        : "m"(vector1->x), "m"(vector1->y), "m"(vector1->z), "m"(vector1->w),
          "m"(vector2->x), "m"(vector2->y), "m"(vector2->z), "m"(vector2->w));

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

double subtractionSISD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    auto start = std::chrono::high_resolution_clock::now();

    asm volatile(
        "flds %8\n"
        "flds %4\n"
        "fsubp\n"
        "fstps %0\n"

        "flds %9\n"
        "flds %5\n"
        "fsubp\n"
        "fstps %1\n"

        "flds %10\n"
        "flds %6\n"
        "fsubp\n"
        "fstps %2\n"

        "flds %11\n"
        "flds %7\n"
        "fsubp\n"
        "fstps %3\n"

        : "=m"(resultVector->x), "=m"(resultVector->y), "=m"(resultVector->z), "=m"(resultVector->w)
        : "m"(vector1->x), "m"(vector1->y), "m"(vector1->z), "m"(vector1->w),
          "m"(vector2->x), "m"(vector2->y), "m"(vector2->z), "m"(vector2->w));

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

double multiplicationSISD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    auto start = std::chrono::high_resolution_clock::now();

    asm volatile(
        "flds %8\n"
        "flds %4\n"
        "fmulp\n"
        "fstps %0\n"

        "flds %9\n"
        "flds %5\n"
        "fmulp\n"
        "fstps %1\n"

        "flds %10\n"
        "flds %6\n"
        "fmulp\n"
        "fstps %2\n"

        "flds %11\n"
        "flds %7\n"
        "fmulp\n"
        "fstps %3\n"

        : "=m"(resultVector->x), "=m"(resultVector->y), "=m"(resultVector->z), "=m"(resultVector->w)
        : "m"(vector1->x), "m"(vector1->y), "m"(vector1->z), "m"(vector1->w),
          "m"(vector2->x), "m"(vector2->y), "m"(vector2->z), "m"(vector2->w));

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

double divisionSISD(Vector128bit *vector1, Vector128bit *vector2, Vector128bit *resultVector)
{
    auto start = std::chrono::high_resolution_clock::now();

    asm volatile(
        "flds %8\n"
        "flds %4\n"
        "fdivp\n"
        "fstps %0\n"

        "flds %9\n"
        "flds %5\n"
        "fdivp\n"
        "fstps %1\n"

        "flds %10\n"
        "flds %6\n"
        "fdivp\n"
        "fstps %2\n"

        "flds %11\n"
        "flds %7\n"
        "fdivp\n"
        "fstps %3\n"

        : "=m"(resultVector->x), "=m"(resultVector->y), "=m"(resultVector->z), "=m"(resultVector->w)
        : "m"(vector1->x), "m"(vector1->y), "m"(vector1->z), "m"(vector1->w),
          "m"(vector2->x), "m"(vector2->y), "m"(vector2->z), "m"(vector2->w));

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

void testSIMD(Vector128bit *a, Vector128bit *b, Vector128bit resultVector[][DATA_VECTOR_SIZE], double *avgTime)
{
    // pusty niemiarodajny przebieg 5x10
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
            double timeElapsed = additionSIMD(&a[j], &b[j], &resultVector[0][j]);
            additionTime += timeElapsed;
        }
        singleResultTime[0][i] = additionTime;

        // odejmowanie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            double timeElapsed = subtractionSIMD(&a[j], &b[j], &resultVector[1][j]);
            subtractionTime += timeElapsed;
        }
        singleResultTime[1][i] = subtractionTime;

        // mnozenie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            double timeElapsed = multiplicationSIMD(&a[j], &b[j], &resultVector[2][j]);
            multiplicationTime += timeElapsed;
        }
        singleResultTime[2][i] = multiplicationTime;

        // dzielenie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            double timeElapsed = divisonSIMD(&a[j], &b[j], &resultVector[3][j]);
            divisionTime += timeElapsed;
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
        avgTime[i] = timeSum / REPEATS;
    }
}

void testSSID(Vector128bit *a, Vector128bit *b, Vector128bit resultVector[][DATA_VECTOR_SIZE], double *avgTime)
{
    // pusty niemiarodajny przebieg 10x5
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
            double timeElapsed = additionSISD(&a[j], &b[j], &resultVector[0][j]);
            additionTime += timeElapsed;
        }
        singleResultTime[0][i] = additionTime;

        // odejmowanie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            double timeElapsed = subtractionSISD(&a[j], &b[j], &resultVector[1][j]);
            subtractionTime += timeElapsed;
        }
        singleResultTime[1][i] = subtractionTime;

        // mnozenie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            double timeElapsed = multiplicationSISD(&a[j], &b[j], &resultVector[2][j]);
            multiplicationTime += timeElapsed;
        }
        singleResultTime[2][i] = multiplicationTime;

        // dzielenie
        for (int j = 0; j < DATA_VECTOR_SIZE; j++)
        {
            double timeElapsed = divisionSISD(&a[j], &b[j], &resultVector[3][j]);
            divisionTime += timeElapsed;
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
        avgTime[i] = timeSum / REPEATS;
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
}

int main()
{
    srand(time(NULL));
    double testResultsSIMD[OPERAIONS];
    double testResultsSISD[OPERAIONS];

    Vector128bit vector1[DATA_VECTOR_SIZE], vector2[DATA_VECTOR_SIZE];
    Vector128bit resultVector[OPERAIONS][DATA_VECTOR_SIZE];

    // generowanie danych i zapisywanie ich do tablic struktur
    generateData(vector1, vector2);

    // test SIMD
    testSIMD(vector1, vector2, resultVector, testResultsSIMD);

    // sprawdzanie poprawnosci obliczen SIMD
    for (int j = 0; j < DATA_VECTOR_SIZE; j++)
    {
        if (vector1[j].x + vector2[j].x != resultVector[0][j].x || vector1[j].y + vector2[j].y != resultVector[0][j].y || vector1[j].z + vector2[j].z != resultVector[0][j].z || vector1[j].w + vector2[j].w != resultVector[0][j].w)
        {
            printf("Error in additionSIMD at index %d\n", j);
        }
        if (vector1[j].x - vector2[j].x != resultVector[1][j].x || vector1[j].y - vector2[j].y != resultVector[1][j].y || vector1[j].z - vector2[j].z != resultVector[1][j].z || vector1[j].w - vector2[j].w != resultVector[1][j].w)
        {
            printf("Error in subtractionSIMD at index %d\n", j);
        }
        if (vector1[j].x * vector2[j].x != resultVector[2][j].x || vector1[j].y * vector2[j].y != resultVector[2][j].y || vector1[j].z * vector2[j].z != resultVector[2][j].z || vector1[j].w * vector2[j].w != resultVector[2][j].w)
        {
            printf("Error in multiplicationSIMD at index %d\n", j);
        }
        if (vector1[j].x / vector2[j].x != resultVector[3][j].x || vector1[j].y / vector2[j].y != resultVector[3][j].y || vector1[j].z / vector2[j].z != resultVector[3][j].z || vector1[j].w / vector2[j].w != resultVector[3][j].w)
        {
            printf("Error in divisionSIMD at index %d\n", j);
        }
    }

    // test SISD
    testSSID(vector1, vector2, resultVector, testResultsSISD);

    // sprawdzanie poprawnosci obliczen SISD
    for (int j = 0; j < DATA_VECTOR_SIZE; j++)
    {
        if (vector1[j].x + vector2[j].x != resultVector[0][j].x || vector1[j].y + vector2[j].y != resultVector[0][j].y || vector1[j].z + vector2[j].z != resultVector[0][j].z || vector1[j].w + vector2[j].w != resultVector[0][j].w)
        {
            printf("Error in additionSISD at index %d\n", j);
        }
        if (vector1[j].x - vector2[j].x != resultVector[1][j].x || vector1[j].y - vector2[j].y != resultVector[1][j].y || vector1[j].z - vector2[j].z != resultVector[1][j].z || vector1[j].w - vector2[j].w != resultVector[1][j].w)
        {
            printf("Error in subtractionSISD at index %d\n", j);
        }
        if (vector1[j].x * vector2[j].x != resultVector[2][j].x || vector1[j].y * vector2[j].y != resultVector[2][j].y || vector1[j].z * vector2[j].z != resultVector[2][j].z || vector1[j].w * vector2[j].w != resultVector[2][j].w)
        {
            printf("Error in multiplicationSISD at index %d\n", j);
        }
        if (vector1[j].x / vector2[j].x != resultVector[3][j].x || vector1[j].y / vector2[j].y != resultVector[3][j].y || vector1[j].z / vector2[j].z != resultVector[3][j].z || vector1[j].w / vector2[j].w != resultVector[3][j].w)
        {
            printf("Error in divisionSISD at index %d\n", j);
        }
    }

    // wyniki zapisywane do pliku - srednie czasy w mikrosekundach
    saveResults(testResultsSIMD, "wyniki_simd.txt", true);
    saveResults(testResultsSISD, "wyniki_sisd.txt", false);

    // wyniki wypisywane na ekran
    printf("SIMD Addition: %f\n", testResultsSIMD[0]);
    printf("SIMD Subtraction: %f\n", testResultsSIMD[1]);
    printf("SIMD Multiplication: %f\n", testResultsSIMD[2]);
    printf("SIMD Division: %f\n", testResultsSIMD[3]);
    printf("--------------------------------\n");
    printf("SISD Addition: %f\n", testResultsSISD[0]);
    printf("SISD Subtraction: %f\n", testResultsSISD[1]);
    printf("SISD Multiplication: %f\n", testResultsSISD[2]);
    printf("SISD Division: %f\n", testResultsSISD[3]);

    return 0;
}
