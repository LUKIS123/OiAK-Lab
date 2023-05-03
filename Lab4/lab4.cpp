#include <iostream>
#include <unistd.h>
#include <fstream>


# define NANO_PER_SEC 1000000000
# define MICRO_PER_SEC 1000000
# define CPU_FREQ 4000000000.0

# define VECTOR_SIZE 2048 // 4096 8192
# define TEST_ITERATIONS 10
# define OPERATIONS 4


struct vector128bit
{
    float x, y, z, w;
};

void addSIMD(vector128bit *a, vector128bit *b, vector128bit *result, int index)
{
    vector128bit *output = &result[index];

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "addps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=m" (*output)
        : "m" (*a), "m" (*b)
        : "xmm0", "xmm1"
    );
}

void subSIMD(vector128bit *a, vector128bit *b, vector128bit *result, int index)
{
    vector128bit *output = &result[index];

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "subps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=m" (*output)
        : "m" (*a), "m" (*b)
        : "xmm0", "xmm1"
    );
}

void mulSIMD(vector128bit *a, vector128bit *b, vector128bit *result, int index)
{
    vector128bit *output = &result[index];

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "mulps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=m" (*output)
        : "m" (*a), "m" (*b)
        : "xmm0", "xmm1"
    );
}

void divSIMD(vector128bit *a, vector128bit *b, vector128bit *result, int index)
{
    vector128bit *output = &result[index];

    asm volatile(
        "movaps %1, %%xmm0\n"
        "movaps %2, %%xmm1\n"
        "divps %%xmm1, %%xmm0\n"
        "movaps %%xmm0, %0"
        : "=m" (*output)
        : "m" (*a), "m" (*b)
        : "xmm0", "xmm1"
    );
}

unsigned long long timestamp()
{
    unsigned long long tsc;

    asm volatile(
        "xor %%rax, %%rax\n"
        "cpuid\n"
        "rdtsc\n"
        "shl $32, %%rdx\n"
        "or %%rdx, %%rax"
        : "=A" (tsc)
        :
        : "rbx", "rcx"
    );

    return tsc;
}

void generateRandomData(vector128bit *vector){
    vector->x = (float)(rand()) / (float)(rand());
    vector->y = (float)(rand()) / (float)(rand());
    vector->z = (float)(rand()) / (float)(rand());
    vector->w = (float)(rand()) / (float)(rand());
}

void testSIMD(vector128bit *a, vector128bit *b, vector128bit *result, double *avgTime){

    // tablica przechowujaca sume czasow dla kazdej operacji
    double resultTime[OPERATIONS][TEST_ITERATIONS];

    for (int i = 0; i < TEST_ITERATIONS; i++)
    {       
        // dodawanie
        double timeADD = 0;
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            unsigned long long tstamp1, tstamp2;

            tstamp1 = timestamp();
            addSIMD(&a[j], &b[j], result, j);
            tstamp2 = timestamp();

            timeADD += ((tstamp2-tstamp1)/CPU_FREQ)*NANO_PER_SEC;
        }
        resultTime[0][i] = timeADD/VECTOR_SIZE;


        // odejmowanie
        double timeSUB = 0;
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            unsigned long long tstamp1, tstamp2;

            tstamp1 = timestamp();
            subSIMD(&a[j], &b[j], result, j);
            tstamp2 = timestamp();

            timeSUB += ((tstamp2-tstamp1)/CPU_FREQ)*NANO_PER_SEC;
        }
        resultTime[1][i] = timeSUB/VECTOR_SIZE;
        

        // mnozenie 
        double timeMUL = 0;
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            unsigned long long tstamp1, tstamp2;

            tstamp1 = timestamp();
            mulSIMD(&a[j], &b[j], result, j);
            tstamp2 = timestamp();

            timeMUL += ((tstamp2-tstamp1)/CPU_FREQ)*NANO_PER_SEC;
        }
        resultTime[2][i] = timeMUL/VECTOR_SIZE;

        // dzielenie
        double timeDIV = 0;
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            unsigned long long tstamp1, tstamp2;

            tstamp1 = timestamp();
            divSIMD(&a[j], &b[j], result, j);
            tstamp2 = timestamp();

            timeDIV += ((tstamp2-tstamp1)/CPU_FREQ)*NANO_PER_SEC;
        }
        resultTime[3][i] = timeDIV/VECTOR_SIZE;
    }


    // zapisywanie sredniej czasow do tablicy wynikowej
    for (int i = 0; i < OPERATIONS; i++)
    {
        double timeSum = 0;
        for (int j = 0; j < TEST_ITERATIONS; j++)
        {
            timeSum += resultTime[i][j];
            
        }
        avgTime[i] = timeSum/TEST_ITERATIONS;
    }

}

int main()
{
    srand(time(0));

    vector128bit vector1[VECTOR_SIZE], vector2[VECTOR_SIZE], outputVector[VECTOR_SIZE];

    for(int i=0; i<VECTOR_SIZE; i++){
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
    file.open("wyniki_simd.txt", std::ios::out);
      if (file.is_open()) {
        file << "Typ obliczen: SIMD" << "\n";
        if (file.fail()) {
            printf("File error - WRITE\n");
        } else {
            file << "Liczba liczb: " << VECTOR_SIZE << "\n";
            file << "Sredni czas [ns]:\n";
            file << "+ : " << testResults[0] << "\n";
            file << "- : " << testResults[1] << "\n";
            file << "* : " << testResults[2] << "\n";
            file << "/ : " << testResults[3] << "\n";

            file.close();
        }
    } else {
       printf("File error - OPEN\n");
    }

    return 0;
}