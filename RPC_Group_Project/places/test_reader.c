#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
//#include "kdtree.h"
#include <errno.h>
#include <dirent.h>


//signature: ./testreader filename
typedef char *string_type;
typedef struct airport_node *airport_list;
struct airport_node {
	string_type code;
	string_type name;
	string_type state;
	string_type distance;
	airport_list next;
};


int main(int argc, char **argv){


 FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    //get rid of first line:
    read = getline(&line, &len, fp);
    for(int i = 0; i<11; i++) {
    	read = getline(&line, &len, fp);
        printf("%s", line);
        while(&line){
        //load the kdTree nodes here with arrays of floats, and 
        //airport codes and city names in the extra node info space
        airport_node *new_node = malloc(sizeof(airport_node));

        }
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

}


