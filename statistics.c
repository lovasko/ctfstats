#include "statistics.h"

#include <stdlib.h>
#include <string.h>

int 
minimum (int values[], unsigned int size)
{
	int m = values[0];

	for (unsigned int i = 0; i < size; i++)
		if (values[i] < m)
			m = values[i];

	return m;
}

int
maximum (int values[], unsigned int size)
{
	int m = values[0];

	for (unsigned int i = 0; i < size; i++)
		if (values[i] > m)
			m = values[i];

	return m;
}

static int
compare (const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

float
median (int values[], unsigned int size)
{
	int* values_sorted;
	values_sorted = malloc(sizeof(int) * size);
	
	memcpy(values_sorted, values, sizeof(int) * size);
	qsort(values_sorted, size, sizeof(int), compare);

	if (size % 2)
		return (float)values_sorted[size/2];
	else
		return (float)(values_sorted[size/2 - 1] + values_sorted[size/2]) / 2.0f;
}

float
arithmetic_mean (int values[], unsigned int size)
{
	int sum = 0;

	for (unsigned int i = 0; i < size; i++)
		sum += values[i];
	
	return (float)sum/(float)size;
}

