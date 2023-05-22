#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

int M;
int N;
// zajednicke varijable
atomic_int A;
atomic_int *ulaz;
atomic_int *broj;

// proces koji koristi zajedničke varijable
void *proces(int i)
{
    for (int t = 0; t < M; t++)
    {
        // ulaz_ko(i);
        int max = -1;
        for (int j = 0; j < N; j++)
        {
            if (max < broj[j])
            {
                max = broj[j];
            }
        }

        ulaz[i] = 1;
        broj[i] = max + 1;
        ulaz[i] = 0;

        for (int j = 0; j < N; j++)
        {
            while (ulaz[j] != 0)
            {
            }
            while (broj[j] != 0 && (broj[j] < broj[i] || (broj[j] == broj[i] && j < i)))
            {
            }
        }

        // krit_odsj();
        A++;
        // printf("%d\n", A);
        //     printf("\n");

        // izlaz(i);
        broj[i] = 0;
    }
}

int main()
{
    printf("Unesi broj dretvi N: ");
    scanf("%d", &N);
    printf("Unesi broj ponavljanja M: ");
    scanf("%d", &M);

    ulaz = malloc(N * sizeof(atomic_int));
    broj = malloc(N * sizeof(atomic_int));
    for (int i = 0; i < N; i++)
    {
        ulaz[i] = 0;
        broj[i] = 0;
    }

    A = 0;
    pthread_t thr_id[N];
    for (int j = 0; j < N; j++)
    {
        broj[j] = j;
        if (pthread_create(&thr_id[j], NULL, proces, j) != 0)
        {
            printf("Greska pri stvaranju dretve!\n");
            exit(1);
        }
    }

    for (int j = 0; j < N; j++)
    {
        pthread_join(thr_id[j], NULL);
    }
    free(ulaz);
    free(broj);

    printf("\n%d\n", A);
}