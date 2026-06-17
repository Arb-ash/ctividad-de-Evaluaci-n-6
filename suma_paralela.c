#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 500000000 // Tamaño masivo para notar la diferencia de rendimiento

int main() {
    // Asignación de memoria dinámica para evitar desbordamiento de pila (stack overflow)
    long long *arr = (long long*)malloc(N * sizeof(long long));
    long long sum = 0;

    if (arr == NULL) {
        printf("Error asignando memoria.\n");
        return 1;
    }

    // Parte Secuencial: Inicialización del arreglo
    for(long long i = 0; i < N; i++) {
        arr[i] = 1; 
    }

    int num_threads[] = {1, 2, 4, 8, 16};
    int num_tests = 5;

    printf("Hilos\tTiempo(s)\tSuma\n");
    printf("----------------------------------------\n");

    for(int t = 0; t < num_tests; t++) {
        int threads = num_threads[t];
        omp_set_num_threads(threads);
        sum = 0;

        // Iniciar medición de tiempo
        double start_time = omp_get_wtime();

        // Parte Paralela: Suma del arreglo usando reducción
        #pragma omp parallel for reduction(+:sum)
        for(long long i = 0; i < N; i++) {
            sum += arr[i];
        }

        // Finalizar medición de tiempo
        double end_time = omp_get_wtime();
        double time_taken = end_time - start_time;

        printf("%d\t%f\t%lld\n", threads, time_taken, sum);
    }

    free(arr);
    return 0;
}