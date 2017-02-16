/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "kdtree-master/kdtree.h"
#include "place_airport.h"

void *kdtree;
bool kdpop = false;

// Helper functions:

double spherical_miles(float c_lat, float c_long, float res_lat, float res_long){
	return ((60*acos((sin(c_lat)*sin(res_lat)) + 
		(cos(c_lat)*cos(res_lat)*cos(res_long-c_long))))*1.1507794);
}

void populate_tree(void *treeptr){
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    treeptr = kd_create(2);

    fp = fopen("places/airport-locations.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    //get rid of first line:
    read = getline(&line, &len, fp);
    //printf("%s", line);
    while((read = getline(&line, &len, fp)) != -1) {
    	
        printf("%s", line);
	        
        float lat = 0; 
        float longi = 0;
        char *airpString = malloc(sizeof(char[50]));
        char *pch;
        pch = strtok (line," ");
        printf ("%s\n",pch);
		
		while (pch != NULL){
		    printf ("\nThis token: %s\n",pch);
		    if (isdigit(pch[0]) || pch[0] == '-'){
        		if(lat != 0){
        			longi = atof(pch);
        		} else{
        			lat = atof(pch);
        		}
        	} else{
        		strcat(airpString, pch);
        		printf("\nCurrent string: %s", airpString);
        	}
		    pch = strtok (NULL, " ");
		  }
    	
    	float ll_arr[2] = {lat,longi};
    	float *llptr = ll_arr;
    	assert( 0 == kd_insertf( kdtree, llptr, &airpString[0] ));
       
    }
    //printf("\nTree populated. Time to search!");
    //struct kdres *presults;
  //kd_free( kdtree );
  fclose(fp);
  if (line)
    free(line);
}

airport_ret *
airport_lookup_1_svc(lat_long_input *argp, struct svc_req *rqstp)
{
	float lat = argp->lat;
	float longitude = argp->longitude;
    errno = 0;
	static airport_ret  result;

    if (!kdpop)
    	populate_tree(kdtree);
	
    struct kdres *presults;
    const float pt[] = {lat, longitude};
    int search_range = 5;
    presults = kd_nearest_rangef( kdtree, pt, search_range);
    
    airport_node* tail = malloc(sizeof(airport_node));
    airport_node* curr = tail;
    tail->next = NULL;

    //printf( "found %d results:\n", kd_res_size(presults) );
    curr = result.airport_ret_u.list;

    while ((kd_res_size(presults) < 5)){
    	search_range *= 2;
    	presults = kd_nearest_rangef( kdtree, pt, search_range);
    }
    
    float farr[3]; //holds kdtree values 
    
    char *resptr;
    while( !kd_res_end( presults ) ) {
	    /* get the data and position of the current result item */
	    resptr = (char*)kd_res_itemf( presults, farr );

	    /* compute the distance of the current result from the pt */
	    double dist = spherical_miles(pt[0], pt[1], farr[0], farr[1]);
	    airport_node* n = malloc(sizeof(airport_node));
      	n->latitude = farr[0];
      	n->longitude = farr[1];
      	char* output = (char*)calloc(15,1);
		snprintf(output, 15, "%f", dist);
      	n->distance = output;
      	n->code = (char*)calloc(6,1);
      	strncpy(resptr, n->code, 5);
      	printf("airpoort code: %s", n->code);
      	n->name = (char*)calloc(50,1);
      	n->name = resptr;
      	printf("\nAirport name: %s\n", n->name);
      	n->next = curr;
      	curr = n;
	    /* print out the retrieved data */
	    printf( "node at (%.3f, %.3f) is %.3f away and has data=%s\n", 
		    farr[0], farr[1], dist, resptr );

	    /* go to the next entry */
	    kd_res_next( presults );
  }
    result.err = errno;
    result.airport_ret_u.list = curr;

  /* free our tree, results set, and other allocated memory */
  kd_res_free( presults );

	return &result;
}
