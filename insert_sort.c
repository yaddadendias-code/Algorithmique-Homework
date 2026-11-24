#include <stdio.h>
void insertionSort(int T[], int n)
{
    int i, j, key;
    for (i = 1; i < n; i++)
    {
        key = T[i];
        j = i - 1;
        while (j >= 0 && T[j] > key)
        {
            T[j + 1] = T[j];
            j--;
        }
        T[j + 1] = key;
       }
}
int main()
{
    int T[] = {8, 3, 5, 1, 9, 2, 7, 4};
    int n = sizeof(T) / sizeof(T[0]);

    insertionSort(T, n);

    printf("Sorted array: ");

    for (int i = 0; i < n; i++)
        printf("%d ", T[i]);

    return 0;
}
