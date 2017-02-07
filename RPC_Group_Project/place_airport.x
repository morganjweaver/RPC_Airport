/* a node in the airport list*/ 
typedef struct airport_node* airport_list;

struct airport_node{
string code;
string name;
string state;
string distance;

airport_list next; };
// If user inputs invalid city/state,
// return NULLptr and interpret on client end.
union airport_ret switch (int err){
	case 0:
		airport_list list;
	default:
		void;	
}


/*IDL program*/ 
program DIRPROG {
	version DIR_VERS {
		airport_ret airport_lookup(float lat, float long ) = 1;
	} = 1;
} = 0x31717172;
