#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICTIONARY_FILE_PATH "dictionary.txt"

int main(int argc, char *argv[]) {
	char *target = argv[1];

	if (target == NULL)
		return EXIT_FAILURE;

	FILE *f = fopen(DICTIONARY_FILE_PATH, "r");

	if (f == NULL) {
		printf("Couldn't open file %s", DICTIONARY_FILE_PATH);
		return EXIT_FAILURE;
	}

	char l[255];
	char *curr;

	while (fgets(l, sizeof l, f) != NULL) {
		curr = strtok(l, ":");

		if (strcmp(target, curr) != 0)
			continue;

		printf("%s\n", target);

		curr = strtok(NULL, ",");

		while (curr != NULL) {
			printf("\t%s", curr);
			curr = strtok(NULL, ",");

			if (curr != NULL)
				printf("\n");
		}

		break;
	}

	return 0;
}
