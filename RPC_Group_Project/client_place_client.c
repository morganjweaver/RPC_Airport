/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "client_place.h"
#include <errno.h>

void
dirprog_1(char *host, string_type input, char* city, char * state)
{
	CLIENT *clnt;
	airport_ret  *result_1;
	string_type  lat_longt_lookup_1_arg = input;
	airport_list current;

#ifndef	DEBUG
	clnt = clnt_create (host, DIRPROG, DIR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = lat_longt_lookup_1(&lat_longt_lookup_1_arg, clnt);
	if (result_1 == (airport_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}else{
		printf("We did it!");
		errno = result_1->err;
		if (errno == 0){
			current = result_1->airport_ret_u.list;
			while (current != NULL){
				printf("%s, %s: %f, %f\n", city, state, current->latitude, current->longitude);
				printf("code: %s, name: %s, distance: %s", current->name, current->distance);
				current = current->next;
			}
		}
		else{
			perror("Error: ");
		}
	}
	
	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	printf("Are we at least entering main?\n");
	if (argc < 4) {
		printf ("Invalid input: program host city state");
		exit (1);
	}
	
	host = argv[1];
	char * city = argv[2];
	char * state = argv[3];
	
	printf("Making it into inputs\n");
	
	char * state_city = (char *) malloc(sizeof(city) + sizeof(state));
	strcpy(state_city, state);
	strcat(state_city, " ");
	strcat(state_city, city);
	string_type input = (string_type) state_city;
	printf("string to input: %s\n", input);
	dirprog_1 (host, input, city, state);
	exit (0);
}
