#include <stdio.h>
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
int partition(int T[], int low, int high)
{
    int pivot = T[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (T[j] <= pivot)
        {
            i++;
            swap(&T[i], &T[j]);
        }
    }
    swap(&T[i + 1], &T[high]);
    return i + 1;
}
void quickSort(int T[], int low, int high)
{
    if (low < high)
    {
        int p = partition(T, low, high);

        quickSort(T, low, p - 1);
        quickSort(T, p + 1, high);
    }
}

int main()
{
    int T[] = {8, 3, 5, 1, 9, 2, 7, 4};
    int n = sizeof(T) / sizeof(T[0]);

    quickSort(T, 0, n - 1);

    printf("Sorted array: ");

    for (int i = 0; i < n; i++)
        printf("%d ", T[i]);

    return 0;
}
