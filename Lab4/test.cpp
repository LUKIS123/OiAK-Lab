#include <iostream>
#include <unistd.h>
#include <time.h>
#include <chrono>

# define NANO_PER_SEC 1000000000
# define CPU_FREQ 4000000000.0

struct vector128bit
{
    float x, y, z, w;
};

vector128bit addSIMD(vector128bit *a, vector128bit *b, vector128bit *result)
{
    asm volatile(
        "movaps %1, %%xmm0;"
        "movaps %2, %%xmm1;"
        "addps %%xmm1, %%xmm0;"
        "movaps %%xmm0, %0;"
        : "=m" (*result)
        : "m" (*a), "m" (*b)
    );

    return *result;
}

// {
//     vector128bit result;

// 	asm volatile(
// 		"movaps %1 ,%%xmm0;"
// 		"movaps %2 ,%%xmm1;"
// 		"addps %%xmm1 ,%%xmm0;"
// 		"movaps %%xmm0 ,%0;"
// 		: "=m" (result)
// 		: "m" (a), "m" (b)
// 		);

//     return result;   
// }

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

int main()
{
    unsigned long long tstamp1, tstamp2;

    srand(time(0));
    
    tstamp1 = timestamp();
    sleep(1);
    tstamp2 = timestamp();

    printf("Cycles: %llu\n", tstamp2-tstamp1);
    printf("Seconds: %f\n", (tstamp2-tstamp1)/CPU_FREQ);
    printf("Nanoseconds: %f\n", ((tstamp2-tstamp1)/CPU_FREQ)*NANO_PER_SEC);


    float czas=0;
    auto start = std::chrono::high_resolution_clock::now();
    tstamp1 = timestamp();
    sleep(1);
    tstamp2 = timestamp();
    auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
    czas+=duration.count();
    printf("CZAS_CLOCK: %f[ns]\n",czas);

    printf("------------------------\n");
    
    vector128bit vector1, vector2, result;
    generateRandomData(&vector1);
    generateRandomData(&vector2);

    addSIMD(&vector1, &vector2, &result);
    printf("Result: %f %f %f %f\n", result.x, result.y, result.z, result.w);

    return 0;
}