#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct trie_info{
	double lat;
	double lon;
};
typedef struct trie_info trie_info;

struct trie_node{
	//ascii values, starting at value 32
	struct trie_node * characters[224];
	//lat and long values stored at end of strings 
	struct trie_info * data; 
};
typedef struct trie_node trie_node;

//pass a root, the c string containing the string, and the data the string references 
int
insert_trie(struct trie_node * root, char * key, struct trie_info * data){
	int length = strlen(key);
	struct trie_node * current = root; 
	
	int i;
	for (i = 0; i < length; i++){
		int c = (int) key[i] - 32;
		if (c < 0 || c > 223){
			return 0; //invalid character entered 
		}
		//if next node does not exist, create a new one
		if (!current->characters[c]){
			struct trie_node * new_node = (trie_node*) malloc(sizeof(trie_node));
			current->characters[c] = new_node;
		}
		current = current->characters[c];	
	}
	current->data = data; 
	
	//printf("data for %s inserted at: %c. City len: %d\n", key, key[i-1], length);
	return 1;
}

trie_info * 
search_trie(struct trie_node *root, char * key){
	int length = strlen(key);
	struct trie_node * current = root;
	
	int i;
	for (i = 0; i < length; i++){
		int c = (int) key[i] - 32;
		if (c < 0 || c > 223){
			return NULL;
		}
		
		if (!current->characters[c]){
			return NULL; 
		}
		current = current->characters[c];
	}

	//if length of string is reached and data is at the node, return 
	if (current->data){
		return current->data; 
	}
	
	//if length of string is reachd and no data, then continue searching so long as there is only one child
	int one_child = 1;
	while (one_child){
		int count = 0;
		int j;
		int val; 
		for (j = 0; j < 224; j++){
			if(current->characters[j]){
				count++;
				val = j; 
			}
		}
		
		//if the count is greater than one, multiple auto completes 
		if (count > 1){
			return NULL; 
		}
		
		//if no children, return any data at that node 
		if(count == 0){
			if(current->data){
				return current->data;
			}
			else{
				return NULL; 
			}
		}
		
		//if exactly one child, return any data at that node or keep going if none 
		if (count == 1){
			if(current->data){
				return current->data;
			}
			else{
				current = current->characters[val];
			}
		}
	}
	return NULL;
	
}

trie_node *
read_into_trie()
{
	//for reading in the file 
	const char * filename = "test.txt";
	FILE * file = fopen(filename, "r");
	char * line = (char *) malloc(168 * sizeof(char));
	char * state = (char *) malloc(3* sizeof(char));
	char * city = (char *) malloc(64 * sizeof(char));
	char * lat = (char *) malloc(11 * sizeof(char));
	char * lon = (char *) malloc(11 * sizeof(char));
	
	//for creating the trie 
	char * city_state = (char *) malloc(68 * sizeof(char));
	double latitude, longitude; 
	struct trie_node * root = (trie_node *) malloc(sizeof(trie_node));
	
	//will insert city_state c string and trie_info struct 
    while (fgets(line, 167, file)) {
		//get city and state info
		strncpy(state, line, 2);
		state[2] = '\0';
		strncpy(city, line + 9, 63);	
	
		int i = 1;
		int end = 0;
		while (i < 62 && !end){
			if(city[i] == ' ' && city[i+1] == ' '){
				end = 1;
			}
			i++;
		}
		city[i-1] = '\0';	
		
		strcpy(city_state, state);
		strcat(city_state, " ");
		strcat(city_state, city);
		
		//getting lat and long info 
		strncpy(lat, line+143, 10);
		lat[10] = '\0';
		latitude = atof(lat);
		
		strncpy(lon, line+154, 10);
		lon[10] = '\0';
		longitude = atof(lon);
		
		//insert info into trie 
		struct trie_info * lat_long = (trie_info *) malloc(sizeof(trie_info));
		lat_long->lat = latitude;
		lat_long->lon = longitude; 
		insert_trie(root, city_state, lat_long);
    }

	fclose(file);
	return root; 
}

int
main (int argc, char *argv[])
{
	struct trie_node * root = read_into_trie();
	char p[] = "AL Akron twwn";
	char * ptr = p;
	//printf("%s\n", ptr);
	
	struct trie_info * result;
	result = search_trie(root, ptr);
	if (result){
		printf("lat: %f, long: %f\n", result->lat, result->lon);
	}else{
		printf("Failed search\n");
	}
	
	return 0;
}
