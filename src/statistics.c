#include "statistics.h"

#include <stdlib.h>
#include <string.h>

unsigned int 
minimum (unsigned int values[], unsigned int size)
{
	unsigned int m = values[0];

	for (unsigned int i = 0; i < size; i++)
		if (values[i] < m)
			m = values[i];

	return m;
}

unsigned int
maximum (unsigned int values[], unsigned int size)
{
	unsigned int m = values[0];

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
median (unsigned int values[], unsigned int size)
{
	unsigned int* values_sorted;
	values_sorted = malloc(sizeof(unsigned int) * size);
	
	memcpy(values_sorted, values, sizeof(unsigned int) * size);
	qsort(values_sorted, size, sizeof(unsigned int), compare);

	if (size % 2)
		return (float)values_sorted[size/2];
	else
		return (float)(values_sorted[size/2 - 1] + values_sorted[size/2]) / 2.0f;
}

float
arithmetic_mean (unsigned int values[], unsigned int size)
{
	unsigned int sum = 0;

	for (unsigned int i = 0; i < size; i++)
		sum += values[i];
	
	return (float)sum/(float)size;
}

