/*input struct*/
struct lat_long_input{
	float lat;
	float longitude;
};

/*IDL program*/ 
program DIRPROG {
	version DIR_VERS {
		airport_ret airport_lookup(lat_long_input ll_input) = 1;
	} = 1;
} = 0x31927172;
