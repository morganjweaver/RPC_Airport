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
#define pi 3.14159265358979323846
void *kdtree;
bool kdpop = false;

// Helper functions:

double deg2rad(double deg) {
  return (deg * pi / 180);
}
double rad2deg(double rad) {
  return (rad * 180 / pi);
}

double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) *
  cos(deg2rad(lat2)) * cos(deg2rad(theta)); dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  switch(unit) {
    case 'M':
    break;
    case 'K':
    dist = dist * 1.609344;
    break;
    case 'N':
    dist = dist * 0.8684;
    break;
  }
  return (dist);
}

void populate_tree(){
	FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  kdtree = kd_create(2);

  fp = fopen("airport-locations.txt", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);
    //get rid of first line:
  read = getline(&line, &len, fp);
  while((read = getline(&line, &len, fp)) != -1) {
    float lat = 0; 
    float longi = 0;
    char *airpString = malloc(sizeof(char[50]));
    char *pch;
    pch = strtok (line," ");
    while (pch != NULL){
      if (isdigit(pch[0]) || pch[0] == '-'){
        if(lat != 0){
         longi = atof(pch);
       } else{
         lat = atof(pch);
       }
     } else{
      strcat(airpString, pch);
    }
    pch = strtok (NULL, " ");
  }    	
  float ll_arr[2] = {lat,longi};
  float *llptr = ll_arr;
  assert( 0 == kd_insertf( kdtree, llptr, &airpString[0] ));
}
fclose(fp);
//if (line)
  //free(line);
}

airport_ret *
airport_lookup_1_svc(lat_long_input *argp, struct svc_req *rqstp)
{
	float lat = argp->lat;
	float longitude = argp->longitude;
  errno = 0;
  static airport_ret  result;
  int counter = 0;
  float farr[2]; //holds kdtree values 
  char *pch;

  if (!kdpop){
   populate_tree();
   kdpop = false;
 }

 xdr_free((xdrproc_t)xdr_airport_ret, (char*)&result); 

 struct kdres *presults;
 const float pt[] = {lat, longitude};
 double  search_range = .15;
 presults = kd_nearest_rangef( kdtree, pt, search_range);
 airport_node* tail = malloc(sizeof(airport_node));
 airport_node* curr = tail;
 tail->next = NULL;
 curr = result.airport_ret_u.list;

 while ((kd_res_size(presults) < 5)){
   search_range *= 2;
   presults = kd_nearest_rangef( kdtree, pt, search_range);
 }
    while( counter <5 && !kd_res_end( presults )) {
	    // get the data and position of the current result item 
     pch = (char*)kd_res_itemf( presults, farr );
	    //compute the distance of the current result from the pt 
     double dist = distance((double)pt[0], (double)pt[1], (double)farr[0], (double)farr[1], 'M');
     printf( "Node at (%.3f, %.3f) is %.3f away and has data=%s\n", 
      farr[0], farr[1], dist, pch );
     airport_node* temp = malloc(sizeof(airport_node));
     temp->latitude = farr[0];
     temp->longitude = farr[1];
     char* output = (char*)calloc(15,1);
     snprintf(output, 15, "%f", dist);
     temp->distance = output;
     temp->code = (char*)calloc(6,1);
     strcpy(temp->code, pch);
     temp->name = (char*)calloc(64,1);
     strncpy(temp->name, pch+6, 63);
     int i = 0;
     while(isalpha(temp->name[i]) || ispunct(temp->name[i])) {
      i++;
    }
    temp->name[i] = '\0';
    temp->next = curr;
    curr = temp;
    counter++;
    kd_res_next( presults );
  }
  counter = 0;
  result.err = errno;
  result.airport_ret_u.list = curr;
  kd_res_free( presults );
  
  return &result;
}
