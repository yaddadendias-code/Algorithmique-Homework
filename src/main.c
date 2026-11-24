#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define RUNS 5
#define BIG_SIZE 100000
#define MAX_MANUAL_SIZE 100
#define NUMBER_OF_SIZES 5
#define MAX_QUADRATIC_SIZE 5000

static const int comparisonSizes[NUMBER_OF_SIZES] = {1000, 5000, 10000, 50000, 100000};
static int originalArray[BIG_SIZE];
static int workArray[BIG_SIZE];

typedef void (*SortFunction)(int[], int);

static size_t currentTrackedBytes = 0;
static size_t peakTrackedBytes = 0;
static int maximumRecursionDepth = 0;
static bool allocationFailed = false;

static void resetMeasurements(void)
{
    currentTrackedBytes = 0;
    peakTrackedBytes = 0;
    maximumRecursionDepth = 0;
    allocationFailed = false;
}

static void recordRecursionDepth(int depth)
{
    if (depth > maximumRecursionDepth) {
        maximumRecursionDepth = depth;
    }
}

static void *trackedMalloc(size_t bytes)
{
    void *pointer = malloc(bytes);

    if (pointer == NULL) {
        allocationFailed = true;
        return NULL;
    }

    currentTrackedBytes += bytes;
    if (currentTrackedBytes > peakTrackedBytes) {
        peakTrackedBytes = currentTrackedBytes;
    }

    return pointer;
}

static void trackedFree(void *pointer, size_t bytes)
{
    if (pointer != NULL) {
        if (bytes <= currentTrackedBytes) {
            currentTrackedBytes -= bytes;
        } else {
            currentTrackedBytes = 0;
        }
        free(pointer);
    }
}

static void swap(int *first, int *second)
{
    int temporary = *first;
    *first = *second;
    *second = temporary;
}

void bubbleSort(int array[], int size)
{
    for (int pass = 0; pass < size - 1; pass++) {
        bool swapped = false;

        for (int j = 0; j < size - 1 - pass; j++) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

void selectionSort(int array[], int size)
{
    for (int i = 0; i < size - 1; i++) {
        int minimumIndex = i;

        for (int j = i + 1; j < size; j++) {
            if (array[j] < array[minimumIndex]) {
                minimumIndex = j;
            }
        }

        if (minimumIndex != i) {
            swap(&array[i], &array[minimumIndex]);
        }
    }
}

void insertionSort(int array[], int size)
{
    for (int i = 1; i < size; i++) {
        int key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }
}

static void merge(int array[], int temporary[], int left, int middle, int right)
{
    int i = left;
    int j = middle + 1;
    int k = left;

    while (i <= middle && j <= right) {
        if (array[i] <= array[j]) {
            temporary[k++] = array[i++];
        } else {
            temporary[k++] = array[j++];
        }
    }

    while (i <= middle) {
        temporary[k++] = array[i++];
    }

    while (j <= right) {
        temporary[k++] = array[j++];
    }

    for (i = left; i <= right; i++) {
        array[i] = temporary[i];
    }
}

static void mergeSortRecursive(int array[], int temporary[], int left, int right, int depth)
{
    recordRecursionDepth(depth);

    if (left >= right) {
        return;
    }

    int middle = left + (right - left) / 2;
    mergeSortRecursive(array, temporary, left, middle, depth + 1);
    mergeSortRecursive(array, temporary, middle + 1, right, depth + 1);
    merge(array, temporary, left, middle, right);
}

void mergeSort(int array[], int size)
{
    if (size < 2) {
        return;
    }

    size_t bytes = (size_t)size * sizeof(int);
    int *temporary = trackedMalloc(bytes);

    if (temporary == NULL) {
        return;
    }

    mergeSortRecursive(array, temporary, 0, size - 1, 1);
    trackedFree(temporary, bytes);
}

static int partition(int array[], int low, int high)
{
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
         swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[high]);
    return i + 1;
}

static void quickSortRecursive(int array[], int low, int high, int depth)
{
    recordRecursionDepth(depth);

    while (low < high) {
        int pivotIndex = partition(array, low, high);
        int leftSize = pivotIndex - low;
        int rightSize = high - pivotIndex;

        if (leftSize < rightSize) {
            quickSortRecursive(array, low, pivotIndex - 1, depth + 1);
            low = pivotIndex + 1;
        } else {
            quickSortRecursive(array, pivotIndex + 1, high, depth + 1);
            high = pivotIndex - 1;
        }
    }
}

void quickSort(int array[], int size)
{
    if (size > 1) {
        quickSortRecursive(array, 0, size - 1, 1);
    }
}

static const char *sortNames[5] = {
    "Bubble Sort",
    "Selection Sort",
    "Insertion Sort",
    "Merge Sort",
    "Quick Sort"
};

static SortFunction sortFunctions[5] = {
    bubbleSort,
    selectionSort,
    insertionSort,
    mergeSort,
    quickSort
};

static bool isSorted(const int array[], int size)
{
    for (int i = 1; i < size; i++) {
        if (array[i - 1] > array[i]) {
            return false;
        }
    }

    return true;
}

static bool readPositiveInt(const char *prompt, int *value, int maximum)
{
    char line[128];
    char extraCharacter;
    long number;

    while (true) {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            return false;
        }

        if (sscanf(line, " %ld %c", &number, &extraCharacter) == 1 &&
            number >= 1 && number <= maximum) {
            *value = (int)number;
            return true;
        }

        printf("Invalid input. Enter an integer between 1 and %d.\n", maximum);
    }
}

static bool readArrayElements(int array[], int size)
{
    char line[128];
    char extraCharacter;
    int value;

    printf("\nEnter the %d elements of the array:\n", size);

    for (int i = 0; i < size; i++) {
        while (true) {
            printf("Element [%d]: ", i + 1);

            if (fgets(line, sizeof(line), stdin) == NULL) {
                return false;
            }

            if (sscanf(line, " %d %c", &value, &extraCharacter) == 1) {
                array[i] = value;
                break;
            }

            printf("Invalid element. Please enter an integer.\n");
        }
    }

    return true;
}

static void printArray(const int array[], int size)
{
    printf("[ ");
    for (int i = 0; i < size; i++) {
        printf("%d", array[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf(" ]\n");
}

static void printArraySample(const int array[], int size)
{
    int sampleSize = size < 10 ? size : 10;

    printf("First values: [ ");
    for (int i = 0; i < sampleSize; i++) {
        printf("%d", array[i]);
        if (i < sampleSize - 1) {
            printf(", ");
        }
    }
    printf(" ]\n");

    printf("Last values:  [ ");
    for (int i = size - sampleSize; i < size; i++) {
        printf("%d", array[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf(" ]\n");
}

static bool measureOne(SortFunction sortFunction,
                       const int original[],
                       int size,
                       double *averageTimeMs,
                       double *peakExtraHeapKB,
                       int *maxRecursionDepth)
{
    double totalMilliseconds = 0.0;
    size_t maximumExtraBytes = 0;
    int maximumDepth = 0;

    for (int run = 0; run < RUNS; run++) {
        memcpy(workArray, original, (size_t)size * sizeof(int));
        resetMeasurements();

        clock_t start = clock();
        sortFunction(workArray, size);
        clock_t end = clock();

        if (start == (clock_t)-1 || end == (clock_t)-1 || end < start) {
            fprintf(stderr, "clock() could not measure this run.\n");
            return false;
        }

        if (allocationFailed) {
            fprintf(stderr, "Memory allocation failed during sorting.\n");
            return false;
        }

        totalMilliseconds +=
            1000.0 * (double)(end - start) / (double)CLOCKS_PER_SEC;

        if (peakTrackedBytes > maximumExtraBytes) {
            maximumExtraBytes = peakTrackedBytes;
        }

        if (maximumRecursionDepth > maximumDepth) {
            maximumDepth = maximumRecursionDepth;
        }

        if (!isSorted(workArray, size)) {
            fprintf(stderr, "ERROR: sorting result is incorrect.\n");
            return false;
        }
    }

    *averageTimeMs = totalMilliseconds / RUNS;
    *peakExtraHeapKB = (double)maximumExtraBytes / 1024.0;
    *maxRecursionDepth = maximumDepth;
    return true;
}

static void runSingleAlgorithm(int choice)
{
    int size;

    if (!readPositiveInt("Array size (1-100): ", &size, MAX_MANUAL_SIZE)) {
        return;
    }

    if (!readArrayElements(originalArray, size)) {
        return;
    }

    printf("\nOriginal array: ");
    printArray(originalArray, size);

    double averageTimeMs = 0.0;
    double peakExtraHeapKB = 0.0;
    int maxRecursionDepth = 0;

    if (measureOne(sortFunctions[choice], originalArray, size,
                   &averageTimeMs, &peakExtraHeapKB, &maxRecursionDepth)) {
        printf("Sorted array:   ");
        printArray(workArray, size);
        printf("\n%s | average CPU time (%d runs): %.3f ms\n",
               sortNames[choice], RUNS, averageTimeMs);
        printf("Extra heap peak tracked: %.2f KB\n", peakExtraHeapKB);
        printf("Maximum recursive depth: %d\n", maxRecursionDepth);
    }
}

static void generateRandomArray(int array[], int size)
{
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 200001 - 100000;
    }
}

static void writeCsvRow(FILE *csv,
                         const char *algorithm,
                         int size,
                         double averageTimeMs,
                         double peakExtraHeapKB,
                         int maxRecursionDepth,
                         const char *status)
{
    if (csv != NULL) {
        fprintf(csv, "%s,%d,%.3f,%.2f,%d,%s\n",
                algorithm, size, averageTimeMs, peakExtraHeapKB,
                maxRecursionDepth, status);
    }
}

static void compareAllAlgorithms(void)
{
    FILE *csv = fopen("results/comparison.csv", "w");

    if (csv != NULL) {
        fprintf(csv, "Algorithm,ArraySize,AverageCpuTimeMs,ExtraHeapPeakKB,MaxRecursionDepth,Status\n");
    }

    srand(2026);

    printf("\nComparison uses fixed seed 2026 and %d runs per measurement.\n", RUNS);
    printf("Quadratic algorithms are limited to sizes up to %d.\n", MAX_QUADRATIC_SIZE);
    printf("\n%-17s %10s %14s %16s %12s %10s\n",
           "Algorithm", "Array size", "Average ms", "Extra heap KB", "Depth", "Status");
    printf("-----------------------------------------------------------------------------------\n");

    for (int sizeIndex = 0; sizeIndex < NUMBER_OF_SIZES; sizeIndex++) {
        int size = comparisonSizes[sizeIndex];
        generateRandomArray(originalArray, size);

        for (int algorithm = 0; algorithm < 5; algorithm++) {
            bool isQuadratic = algorithm <= 2;

            if (isQuadratic && size > MAX_QUADRATIC_SIZE) {
                printf("%-17s %10d %14s %16s %12s %10s\n",
                       sortNames[algorithm], size, "--", "--", "--", "SKIPPED");
                writeCsvRow(csv, sortNames[algorithm], size, 0.0, 0.0, 0, "SKIPPED");
                continue;
            }

            double averageTimeMs = 0.0;
            double peakExtraHeapKB = 0.0;
            int maxRecursionDepth = 0;
            bool success = measureOne(sortFunctions[algorithm], originalArray, size,
                                      &averageTimeMs, &peakExtraHeapKB,
                                      &maxRecursionDepth);

            if (success) {
                printf("%-17s %10d %14.3f %16.2f %12d %10s\n",
                       sortNames[algorithm], size, averageTimeMs,
                       peakExtraHeapKB, maxRecursionDepth, "OK");
                writeCsvRow(csv, sortNames[algorithm], size, averageTimeMs,
                            peakExtraHeapKB, maxRecursionDepth, "OK");
            } else {
                printf("%-17s %10d %14s %16s %12s %10s\n",
                       sortNames[algorithm], size, "--", "--", "--", "FAILED");
                writeCsvRow(csv, sortNames[algorithm], size, 0.0, 0.0, 0, "FAILED");
            }
        }
        printf("--------------------------------------------------------------------------------\n");
    }

    if (csv != NULL) {
        fclose(csv);
        printf("Results saved to results/comparison.csv\n");
    } else {
        printf("Could not write results/comparison.csv. Run the program from the project root.\n");
    }
}

static void runBigArrayExample(void)
{
    FILE *csv = fopen("results/big_array.csv", "w");

    if (csv != NULL) {
        fprintf(csv, "Algorithm,ArraySize,AverageCpuTimeMs,ExtraHeapPeakKB,MaxRecursionDepth,Status\n");
    }

    srand(2026);
    generateRandomArray(originalArray, BIG_SIZE);

    printf("\nBig array test: %d random integers (fixed seed = 2026).\n", BIG_SIZE);
    printf("Bubble Sort, Selection Sort and Insertion Sort are skipped because they are O(n^2).\n\n");

    for (int algorithm = 3; algorithm <= 4; algorithm++) {
        double averageTimeMs = 0.0;
        double peakExtraHeapKB = 0.0;
        int maxRecursionDepth = 0;
        bool success = measureOne(sortFunctions[algorithm], originalArray, BIG_SIZE,
                                  &averageTimeMs, &peakExtraHeapKB,
                                  &maxRecursionDepth);

        if (success) {
            printf("%s: average CPU time = %.3f ms | extra heap peak = %.2f KB | recursion depth = %d\n",
                   sortNames[algorithm], averageTimeMs,
                   peakExtraHeapKB, maxRecursionDepth);
            writeCsvRow(csv, sortNames[algorithm], BIG_SIZE, averageTimeMs,
                        peakExtraHeapKB, maxRecursionDepth, "OK");
            printf("Sorted correctly: %s\n", isSorted(workArray, BIG_SIZE) ? "YES" : "NO");
            printArraySample(workArray, BIG_SIZE);
        } else {
            printf("%s failed.\n", sortNames[algorithm]);
            writeCsvRow(csv, sortNames[algorithm], BIG_SIZE, 0.0, 0.0, 0, "FAILED");
        }
        printf("\n");
    }

    if (csv != NULL) {
        fclose(csv);
        printf("Big-array results saved to results/big_array.csv\n");
    }
}

int main(void)
{
    int choice;

    while (true) {
        char line[64];
        char extraCharacter;

        printf("\n========== SORTING ALGORITHMS ==========\n");
        printf("1. Bubble Sort\n");
        printf("2. Selection Sort\n");
        printf("3. Insertion Sort\n");
        printf("4. Merge Sort\n");
        printf("5. Quick Sort\n");
        printf("6. Compare all algorithms on several sizes\n");
        printf("7. Big array example (100,000 elements)\n");
        printf("0. Exit\n");
        printf("Your choice: ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        if (sscanf(line, " %d %c", &choice, &extraCharacter) != 1) {
            printf("Invalid choice.\n");
            continue;
        }

        if (choice == 0) {
            printf("Goodbye!\n");
            break;
        } else if (choice >= 1 && choice <= 5) {
            runSingleAlgorithm(choice - 1);
        } else if (choice == 6) {
            compareAllAlgorithms();
        } else if (choice == 7) {
            runBigArrayExample();
        } else {
            printf("Invalid choice. Choose between 0 and 7.\n");
        }
    }

    return 0;
}
