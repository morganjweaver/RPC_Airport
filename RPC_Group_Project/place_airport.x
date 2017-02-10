/* a node in the airport list*/ 
typedef struct airport_node* airport_list;

const MAXLEN = 255;
typedef string string_type<MAXLEN>;

struct airport_node{
	string_type code;
	string_type name;
	string_type state;
	string_type distance;

	airport_list next; 
};

struct lat_long_input{
	float lat;
	float longitude;
};


/*If user inputs invalid city/state,
return NULLptr and interpret on client end.*/
union airport_ret switch (int err){
	case 0:
		airport_list list;
	default:
		void;	
};


/*IDL program*/ 
program DIRPROG {
	version DIR_VERS {
		airport_ret airport_lookup(lat_long_input ll_input) = 1;
	} = 1;
} = 0x31717172;
