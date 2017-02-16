/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "place_airport.h"
#include "client_place.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

//information stored in the trie 
//used in both client and server code 
struct trie_info{
	double lat;
	double lon;
};
typedef struct trie_info trie_info;

/*
*
PLACES CLIENT CODE
*
*/
//global so that server can access to send back to user-end client 
airport_ret  *result_1;
void
dirprog2_1(char *host, trie_info * data)
{
	CLIENT *clnt;
	
	lat_long_input  airport_lookup_1_arg;
	
	airport_lookup_1_arg.lat = data->lat;
	airport_lookup_1_arg.longitude = data->lon;

#ifndef	DEBUG
	clnt = clnt_create (host, DIRPROG2, DIR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = airport_lookup_1(&airport_lookup_1_arg, clnt);
	if (result_1 == (airport_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

/*
*
PLACES SERVER CODE
*
*/
//constants
const int ascii_vals = 224;
const char FILENAME[] = "places2k.txt";

//root, initially null
struct trie_node * root = NULL;

//trie nodes 
struct trie_node{
	//ascii values, starting at value 32
	struct trie_node * characters[224];
	struct trie_info * data; 
};
typedef struct trie_node trie_node;

//pass a root, the c string containing the string, and the data the string references 
//returns 1 on successful insert, 0 on failed insert 
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
			int j;
			for(j = 0; j < ascii_vals; j++){
				new_node->characters[j] = NULL;
			}
			current->characters[c] = new_node;
		}
		current = current->characters[c];	
	}
	current->data = data; 
	
	return 1;
}

//takes root and key, returns trie_info struct on successful lookup, null on failed 
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
		for (j = 0; j < ascii_vals; j++){
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

//called once; reads file into trie 
trie_node *
read_into_trie()
{
	//for reading in the file 
	const char * filename = FILENAME;
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
	int j;
	for(j = 0; j < ascii_vals; j++){
		root->characters[j] = NULL;
	}
	
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
	free(line);
	free(state);
	free(city);
	free(lat);
	free(lon);
	return root; 
}

//server function, called by client_place_client.c 
airport_ret *
lat_longt_lookup_1_svc(string_type *argp, struct svc_req *rqstp)
{
	static airport_ret  result; 
	
	xdr_free((xdrproc_t) xdr_airport_ret, (char*) &result_1);
	
	//if the root is empty, read in the file. 
	if(!root){
			printf("Creating trie\n");
			root = read_into_trie();
	}

	//call search 
	char * input = (char *) malloc(68 * sizeof(char));
	struct trie_info * lat_long = NULL; 
	strcpy(input, *argp);
	lat_long = search_trie(root, input);
	
	if(lat_long){
		printf("lat: %f, long: %f\n", lat_long->lat, lat_long->lon);
		dirprog2_1("localhost", lat_long);
		if(result_1){
			result = *result_1;
		}
	}else{
		result.err = errno;
	}

	free(lat_long);
	return &result;
}

