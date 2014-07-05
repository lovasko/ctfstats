#include "../libctf/src/libctf.h"

#include <stdio.h>
#include <stdlib.h>

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

int
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

void
usage ()
{
	fprintf(stderr, "Usage: ctfstats file\n");
}

int
main (int argc, char* argv[])
{
	if (argc < 2 || argc > 2)
	{
		usage();
		return EXIT_FAILURE;
	}

	struct ctf_file* file;
	int retval;
	if ((retval = ctf_read_file(argv[1], &file)) != CTF_OK)
	{
		fprintf(stderr, "ERROR: unable to read the file: %s\n",
		    ctf_get_error_string(retval));
		return EXIT_FAILURE;
	}

	print_global_counts();
	print_function_stats();
	print_type_counts();
	print_struct_stats();
	print_union_stats();
	print_enum_stats();
}

