#include "../libctf/src/libctf.h"

#include "statistics.h"

#include <stdio.h>
#include <stdlib.h>

static void
usage ()
{
	fprintf(stderr, "Usage: ctfstats file\n");
}

static int 
print_label_count (struct ctf_file* file)
{
	unsigned int label_count = 0;
	struct ctf_label* label = NULL;
	int retval; 

	while ((retval = ctf_file_get_next_label(file, label, &label)) == CTF_OK)
		label_count++;

	if (retval != CTF_END && retval != CTF_EMPTY)
	{
		fprintf(stderr, "ERROR: reading labels: %s\n", ctf_get_error_string(retval));
		return retval;
	}

	printf("Number of labels: %d\n", label_count);
	return CTF_OK;
}

static int 
print_type_count (struct ctf_file* file)
{
	unsigned int type_count = 0;
	struct ctf_type* type = NULL;
	int retval; 

	while ((retval = ctf_file_get_next_type(file, type, &type)) == CTF_OK)
		type_count++;

	if (retval != CTF_END && retval != CTF_EMPTY)
	{
		fprintf(stderr, "ERROR: reading types: %s\n", ctf_get_error_string(retval));
		return retval;
	}

	printf("Number of types: %d\n", type_count);
	return CTF_OK;
}

static int
print_data_object_count (struct ctf_file* file)
{
	unsigned int data_object_count = 0;
	struct ctf_data_object* data_object = NULL;
	int retval; 

	while ((retval = ctf_file_get_next_data_object(file, data_object,
	    &data_object)) == CTF_OK)
		data_object_count++;

	if (retval != CTF_END && retval != CTF_EMPTY)
	{
		fprintf(stderr, "ERROR: reading data objects: %s\n", 
		    ctf_get_error_string(retval));
		return retval;
	}

	printf("Number of data objects: %d\n", data_object_count);
	return CTF_OK;
}

static int
print_function_count (struct ctf_file* file)
{
	unsigned int function_count = 0;
	struct ctf_function* function = NULL;
	int retval; 

	while ((retval = ctf_file_get_next_function(file, function, &function)) 
	    == CTF_OK)
		function_count++;

	if (retval != CTF_END && retval != CTF_EMPTY)
	{
		fprintf(stderr, "ERROR: reading functions: %s\n", 
		    ctf_get_error_string(retval));
		return retval;
	}

	printf("Number of functions: %d\n", function_count);
	return CTF_OK;
}

static void
print_global_counts (struct ctf_file* file)
{
	printf("-- Global Counts -----\n");
	print_label_count(file);
	print_type_count(file);
	print_data_object_count(file);
	print_function_count(file);
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

	print_global_counts(file);
	/* print_function_stats(); */
	/* print_kind_counts(); */
	/* print_struct_stats(); */
	/* print_union_stats(); */
	/* print_enum_stats(); */
}

