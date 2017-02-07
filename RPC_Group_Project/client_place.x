/* a node in the directory list*/

typedef struct airport_node* airport_list; 

struct airport_node{
string code;
string name;
string state;
string distance;

airport_list next; 
};
// If user inputs invalid city/state, 
// return NULLptr and interpret on client end.

/*IDL program*/ 
program DIRPROG {
version DIR_VERS {
airport_list* lat_longt_lookup(string city, string state ) = 1;
} = 1;
} = 0x31717171;
