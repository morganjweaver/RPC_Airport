#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <math.h>


#include "kdtree-master/kdtree.h"


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

static float dist_sq( float *a1, float *a2, int dims ) {
  double dist_sq = 0, diff;
  while( --dims >= 0 ) {
    diff = (a1[dims] - a2[dims]);
    dist_sq += diff*diff;
  }
  return dist_sq;
}

int main(int argc, char **argv){


 FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    void *kdtree = kd_create(2);

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    //get rid of first line:
    read = getline(&line, &len, fp);
    printf("%s", line);
    for(int i = 0; i<5; i++) {
    	read = getline(&line, &len, fp);
        printf("%s", line);
        //if(&line){
	        
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
        //}
    }
    printf("\nTree populated.  Time to search!");
    
    struct kdres *presults;
    const float pt[] = {33.0, -80.0};
    presults = kd_nearest_rangef( kdtree, pt, 6);
    printf( "found %d results:\n", kd_res_size(presults) );
    
    float farr[5];
    //NOW PRINT RESULTS:

    char *resptr;
   while( !kd_res_end( presults ) ) {
    /* get the data and position of the current result item */
    resptr = (char*)kd_res_itemf( presults, farr );

    /* compute the distance of the current result from the pt */
    float dist = sqrt( dist_sq( pt, farr, 2 ) );

    /* print out the retrieved data */
    printf( "node at (%.3f, %.3f) is %.3f away and has data=%s\n", 
	    farr[0], farr[1], dist, resptr );

    /* go to the next entry */
    kd_res_next( presults );
  }

  /* free our tree, results set, and other allocated memory */
  kd_res_free( presults );
  kd_free( kdtree );

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

}


