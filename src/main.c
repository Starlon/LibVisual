#include <stdlib.h>

#include <libvisual/libvisual.h>

#include "avs_parse.h"

int main (int argc, char *argv[])
{
	AVSTree *avstree;
	AVSContainer *container;
	char *file;

    visual_init(0, 0);

	if (argc < 1) {
		printf ("Usage: %s <source file>\n", argv[0]);
	
		exit (-1);
	}

	file = argv[1];

	avstree = avs_tree_new_from_preset ("/home/scott/Work/libvisual/branches/libvisual-avs/testpresets/superscope.avs");

	container = avstree->main;

	visual_object_unref (VISUAL_OBJECT (avstree));

	return 0;
}

