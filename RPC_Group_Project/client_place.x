/*node in the airport list*/
typedef struct airport_node* airport_list; 
struct airport_node{
	string code;
	string name;
	string state;
	string distance;

	airport_list next; 
};

/*this may be unnecessary if we store city and state as a single string*/
struct city_state{
	string city;
	string state; 
};

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
		airport_ret lat_longt_lookup(city_state cs_input) = 1;
	} = 1;
} = 0x31717171;
