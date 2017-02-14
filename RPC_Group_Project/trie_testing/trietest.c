#include <stdio.h>
#include <string.h>

struct trie_info{
	//char * state_city[70];
	float lat;
	float lon;
};

trie *
read_into_trie()
{
	const char * filename = "test.txt";
	FILE * file = fopen(filename, "r");
	char line[165];
	char state[3];
    char city[64];
	char city_state[67];
	//char lat[10];
	//char lon[10];
	int count = 0;

    while (fgets(line, sizeof(line), file)) {
         /*copy state into state array, copy city into city array, copy lat and long into their arrays*/
		strncpy(state, line, 2);
		state[2] = '\0';
		strncpy(city, line + 9, 63);
		city[63] = '\0';
		
		count++;
		printf("%d %s\n", count, city);
		
	//	strcat(city_state, state);
		//strcat(city_state, " ");
		//strcat(city_state, city);
    }

	fclose(file);
}

int
main (int argc, char *argv[])
{
	//read_into_trie();
	
	return 0;
}
