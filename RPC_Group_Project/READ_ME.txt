Authors:
	Morgan Weaver
		built the airports server
	Bailey Strom-Pillar
		built the planes server 
	Both worked on the client and the build/make work 	

Compile in this order: 
	make -f Makefile.client_places
	make -f Makefile.places_aiport

To start servers:
	Start up ./places_airport_server (airports server)
	Start up ./client_places_client (places server)

To run client:
	./client_places_client hostname city statecode 

External libraries used:
	kd-tree: 

Algorithms:
	places server: trie (insert and search functions implemented) for looking up lat/long given city/statecode
	airport server: kd-tree for looking up airports given lat/long 
	
Strengths/weaknesses: