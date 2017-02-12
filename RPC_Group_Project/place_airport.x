
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
program DIRPROG2 {
	version DIR_VERS {
		airport_ret airport_lookup(lat_long_input ll_input) = 1;
	} = 1;
} = 0x31927172;
