#include "../libctf/src/libctf.h"

#include "statistics.h"

#include <stdio.h>
#include <stdlib.h>

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

