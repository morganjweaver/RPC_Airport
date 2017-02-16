Authors:
Morgan Weaver
Bailey Strom-Pillar

Compile in this order: 
make -f Makefile.client_places
make -f Makefile.places_aiport

To start servers:
Start up ./places_airport_server (airports server)
Start up ./client_places_client (places server)

To run client:
./client_places_client hostname "State name" statecode 

External libraries used:
kd-tree: 