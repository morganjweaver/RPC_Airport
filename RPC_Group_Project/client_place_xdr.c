/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "client_place.h"

bool_t
xdr_airport_list (XDR *xdrs, airport_list *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct airport_node), (xdrproc_t) xdr_airport_node))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_string_type (XDR *xdrs, string_type *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, MAXLEN))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airport_node (XDR *xdrs, airport_node *objp)
{
	register int32_t *buf;

	 if (!xdr_string_type (xdrs, &objp->code))
		 return FALSE;
	 if (!xdr_string_type (xdrs, &objp->name))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->latitude))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->longitude))
		 return FALSE;
	 if (!xdr_string_type (xdrs, &objp->distance))
		 return FALSE;
	 if (!xdr_airport_list (xdrs, &objp->next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airport_ret (XDR *xdrs, airport_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case 0:
		 if (!xdr_airport_list (xdrs, &objp->airport_ret_u.list))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
