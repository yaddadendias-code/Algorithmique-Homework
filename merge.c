
#include <stdio.h>
#include <stdlib.h>

void merge(int T[], int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;
    int k = 0;

    int *temp = malloc((right - left + 1) * sizeof(int));

    if (temp == NULL)
        return;

    while (i <= mid && j <= right)
    {
        if (T[i] <= T[j])
        {
            temp[k] = T[i];
            i++;
        }
        else
        {
            temp[k] = T[j];
            j++;
        }
        k++;
    }

    while (i <= mid)
    {
        temp[k] = T[i];
        i++;
        k++;
    }

    while (j <= right)
    {
        temp[k] = T[j];
        j++;
        k++;
    }

    for (i = left, k = 0; i <= right; i++, k++)
        T[i] = temp[k];

    free(temp);
}

void mergeSort(int T[], int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;

        mergeSort(T, left, mid);
        mergeSort(T, mid + 1, right);

        merge(T, left, mid, right);
    }
}

int main()
{
    int T[] = {8, 3, 5, 1, 9, 2, 7, 4};
    int n = sizeof(T) / sizeof(T[0]);

    mergeSort(T, 0, n - 1);

    printf("Sorted array: ");

    for (int i = 0; i < n; i++)
        printf("%d ", T[i]);

    return 0;
}
