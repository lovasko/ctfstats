#include "../lctf/src/libctf.h"

#include "statistics.h"
#include "conversion.h"

#include <stdio.h>
#include <stdlib.h>

static void
usage ()
{
	fprintf(stderr, "Usage: ctfstats file\n");
}

static int 
print_label_count (ctf_file file)
{
	unsigned int label_count = 0;
	ctf_label label = NULL;
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
print_type_count (ctf_file file)
{
	unsigned int type_count = 0;
	ctf_type type = NULL;
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
print_data_object_count (ctf_file file)
{
	unsigned int data_object_count = 0;
	ctf_data_object data_object = NULL;
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
print_function_count (ctf_file file)
{
	unsigned int function_count = 0;
	ctf_function function = NULL;
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
print_global_counts (ctf_file file)
{
	printf("-- Global Counts -----\n");
	print_label_count(file);
	print_type_count(file);
	print_data_object_count(file);
	print_function_count(file);
}

static int
print_kind_counts (ctf_file file)
{
	ctf_type type = NULL;
	unsigned int counts[CTF_KIND_MAX+1];
	int retval;

	for (unsigned int i = 0; i < CTF_KIND_MAX+1; i++)
		counts[i] = 0;

	while ((retval = ctf_file_get_next_type(file, type, &type)) == CTF_OK)
	{
		ctf_kind kind;
		ctf_type_get_kind(type, &kind);

		if (kind >= CTF_KIND_MIN && kind <= CTF_KIND_MAX)
			counts[kind]++;
		else
			return CTF_E_KIND_INVALID;
	}

	if (retval != CTF_END && retval != CTF_EMPTY)
	{
		fprintf(stderr, "ERROR: reading types: %s\n", ctf_get_error_string(retval));
		return retval;
	}

	printf("-- Kind Counts -----\n");
	for (unsigned int i = 0; i < CTF_KIND_MAX; i++)
		printf("Number of %ss: %d\n", kind_to_string(i), counts[i]);

	return CTF_OK;
}

static int
print_struct_stats (ctf_file file)
{
	ctf_type type = NULL;
	int retval;

	unsigned int total_member_count = 0;
	unsigned int total_struct_count = 0;
	unsigned int* member_counts = NULL;

	while ((retval = ctf_file_get_next_type(file, type, &type)) == CTF_OK)
	{
		ctf_kind kind;
		ctf_type_get_kind(type, &kind);

		if (kind == CTF_KIND_STRUCT)
		{
			total_struct_count++;
			member_counts = realloc(member_counts, total_struct_count *
			    sizeof(unsigned int));

			ctf_struct_union struct_union;
			ctf_struct_union_init(type, &struct_union);

			ctf_member member = NULL;
			unsigned int member_count = 0;		
			while ((retval = ctf_struct_union_get_next_member(struct_union, member, 
			    &member)) == CTF_OK)
				member_count++;

			member_counts[total_struct_count-1] = member_count;
			total_member_count += member_count;
		}
	}

	printf("-- Struct Stats -----\n");
	printf("Number of members: %d\n", total_member_count);
	printf("Minimal member count: %d\n", 
	    minimum(member_counts, total_struct_count));
	printf("Maximal member count: %d\n", 
	    maximum(member_counts, total_struct_count));
	printf("Average member count: %f\n",
	    arithmetic_mean(member_counts, total_struct_count));
	printf("Median member count: %f\n",
	    median(member_counts, total_struct_count));

	return CTF_OK;
}

static int
print_union_stats (ctf_file file)
{
	ctf_type type = NULL;
	int retval;

	unsigned int total_member_count = 0;
	unsigned int total_union_count = 0;
	unsigned int* member_counts = NULL;

	while ((retval = ctf_file_get_next_type(file, type, &type)) == CTF_OK)
	{
		ctf_kind kind;
		ctf_type_get_kind(type, &kind);

		if (kind == CTF_KIND_UNION)
		{
			total_union_count++;
			member_counts = realloc(member_counts, total_union_count *
			    sizeof(unsigned int));

			ctf_struct_union struct_union;
			ctf_struct_union_init(type, &struct_union);

			ctf_member member = NULL;
			unsigned int member_count = 0;		
			while ((retval = ctf_struct_union_get_next_member(struct_union, member, 
			    &member)) == CTF_OK)
				member_count++;

			member_counts[total_union_count-1] = member_count;
			total_member_count += member_count;
		}
	}

	printf("-- Union Stats -----\n");
	printf("Number of members: %d\n", total_member_count);
	printf("Minimal member count: %d\n", 
	    minimum(member_counts, total_union_count));
	printf("Maximal member count: %d\n", 
	    maximum(member_counts, total_union_count));
	printf("Average member count: %f\n",
	    arithmetic_mean(member_counts, total_union_count));
	printf("Median member count: %f\n",
	    median(member_counts, total_union_count));

	return CTF_OK;
}

static int
print_enum_stats (ctf_file file)
{
	ctf_type type = NULL;
	int retval;

	unsigned int total_entry_count = 0;
	unsigned int total_enum_count = 0;
	unsigned int* entry_counts = NULL;

	while ((retval = ctf_file_get_next_type(file, type, &type)) == CTF_OK)
	{
		ctf_kind kind;
		ctf_type_get_kind(type, &kind);

		if (kind == CTF_KIND_ENUM)
		{
			total_enum_count++;
			entry_counts = realloc(entry_counts, total_enum_count *
			    sizeof(unsigned int));

			ctf_enum _enum;
			ctf_enum_init(type, &_enum);

			ctf_enum_entry enum_entry = NULL;
			unsigned int entry_count = 0;		
			while ((retval = ctf_enum_get_next_enum_entry(_enum, enum_entry, 
			    &enum_entry)) == CTF_OK)
				entry_count++;

			entry_counts[total_enum_count-1] = entry_count;
			total_entry_count += entry_count;
		}
	}

	printf("-- Enum Stats -----\n");
	printf("Number of entries: %d\n", total_enum_count);
	printf("Minimal entry count: %d\n", 
	    minimum(entry_counts, total_enum_count));
	printf("Maximal entry count: %d\n", 
	    maximum(entry_counts, total_enum_count));
	printf("Average entry count: %f\n",
	    arithmetic_mean(entry_counts, total_enum_count));
	printf("Median entry count: %f\n",
	    median(entry_counts, total_enum_count));

	return CTF_OK;
}

int
main (int argc, char* argv[])
{
	if (argc != 2)
	{
		usage();
		return EXIT_FAILURE;
	}

	ctf_file file;
	int retval;
	if ((retval = ctf_file_read(argv[1], &file)) != CTF_OK)
	{
		fprintf(stderr, "ERROR: unable to read the file: %s\n",
		    ctf_get_error_string(retval));
		return EXIT_FAILURE;
	}

	print_global_counts(file);
	/* print_function_stats(); */
	print_kind_counts(file);
	print_struct_stats(file);
	print_union_stats(file);
	print_enum_stats(file);
}

