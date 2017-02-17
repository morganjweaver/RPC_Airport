Authors:
	Morgan Weaver
		built the airports server (place_airport_server.c), kd-tree library, bug fixing
	Bailey Strom-Pillar
		built the places server (place_airport_client.c), built trie, the client (client_place_client.c) 
	Both worked on and the build/make work

GitHub repo:
	https://github.com/morganjweaver/RPC_Airport 	

Compile in this order: 
	make -f Makefile.client_places
	make -f Makefile.places_aiport

To start servers:
	Start up ./place_airport_server (airports server)
	Start up ./place_airport_client (places server)

To run client:
	./client_place_client hostname city statecode 

External libraries used:
	kd-tree: https://github.com/jtsiomb/kdtree

Algorithms:
	places server: trie (insert and search functions implemented) for looking up lat/long given city/statecode
	airport server: kd-tree for looking up airports given lat/long 
	
Strengths:
	Designing algorithms and working with data structures

Weaknesses:
	Build work with makefiles, C 