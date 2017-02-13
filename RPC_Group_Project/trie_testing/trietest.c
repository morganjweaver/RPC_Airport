#include "trie-master/trie.h"
#include <stdio.h>
#include <string.h>

struct trie_info{
	char * state_city[70];
	float lat;
	float lon;
};

trie *
read_into_trie()
{
	const char * filename = "places2k.txt";
	FILE * file = fopen(filename, "r");
	char line[165];
	//char state[2];
    char city[64];
	char lat[10];
	char lon[10];

    while (fgets(line, sizeof(line), file)) {
         /*copy state into state array, copy city into city array, copy lat and long into their arrays*/
		//strncpy(state, line, sizeof(state));
		//strncpy(city, line + 9, sizeof(city));
		 //strncpy(lat, line)
		// printf("%s", city);
		printf("%s", line[0]);
    }
	//printf("%s", city);
	fclose(file);
}

int
main (int argc, char *argv[])
{
	read_into_trie();
	
	return 0;
}
