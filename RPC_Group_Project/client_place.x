/*node in the airport list*/
typedef struct airport_node* airport_list; 

const MAXLEN = 255;
typedef string string_type<MAXLEN>;

struct airport_node{
	string_type code;
	string_type name;
	double latitude;
	double longitude;
	string_type distance;

	airport_list next; 
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
		airport_ret lat_longt_lookup(string_type city_state) = 1;
	} = 1;
} = 0x31717171;
