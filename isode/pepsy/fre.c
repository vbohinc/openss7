/*****************************************************************************

 @(#) $RCSfile$ $Name$($Revision$) $Date$

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2007  OpenSS7 Corporation <http://www.openss7.com/>
 Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@openss7.org>

 All Rights Reserved.

 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, version 3 of the license.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>, or write to the
 Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 -----------------------------------------------------------------------------

 U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on
 behalf of the U.S. Government ("Government"), the following provisions apply
 to you.  If the Software is supplied by the Department of Defense ("DoD"), it
 is classified as "Commercial Computer Software" under paragraph 252.227-7014
 of the DoD Supplement to the Federal Acquisition Regulations ("DFARS") (or any
 successor regulations) and the Government is acquiring only the license rights
 granted herein (the license rights customarily provided to non-Government
 users).  If the Software is supplied to any unit or agency of the Government
 other than DoD, it is classified as "Restricted Computer Software" and the
 Government's rights in the Software are defined in paragraph 52.227-19 of the
 Federal Acquisition Regulations ("FAR") (or any successor regulations) or, in
 the cases of NASA, in paragraph 18.52.227-86 of the NASA Supplement to the FAR
 (or any successor regulations).

 -----------------------------------------------------------------------------

 Commercial licensing and support of this software is available from OpenSS7
 Corporation at a fee.  See http://www.openss7.com/

 -----------------------------------------------------------------------------

 Last Modified $Date$ by $Author$

 -----------------------------------------------------------------------------

 $Log$
 *****************************************************************************/

#ident "@(#) $RCSfile$ $Name$($Revision$) $Date$"

static char const ident[] = "$RCSfile$ $Name$($Revision$) $Date$";

/* fre.c */

#ifndef	lint
static char *rcsid =
    "Header: /xtel/isode/isode/pepsy/RCS/fre.c,v 9.0 1992/06/16 12:24:03 isode Rel";
#endif

/* 
 * Header: /xtel/isode/isode/pepsy/RCS/fre.c,v 9.0 1992/06/16 12:24:03 isode Rel
 *
 *
 * Log: fre.c,v
 * Revision 9.0  1992/06/16  12:24:03  isode
 * Release 8.0
 *
 */

/*
 *				  NOTICE
 *
 *    Acquisition, use, and distribution of this module and related
 *    materials are subject to the restrictions of a license agreement.
 *    Consult the Preface in the User's Manual for the full terms of
 *    this agreement.
 *
 */

/* LINTLIBRARY */

/* LINTLIBRARY */

/*
 * These routines are the driving routines for freeing of the data
 */
#include	<stdio.h>
#include	"pepsy-driver.h"	/* for PEPSY_VERSION defn */
#include	"psap.h"
#include	"pepsy.h"

extern ptpe *next_tpe();
extern int pepsylose();

#define NEXT_TPE(p)	p = next_tpe(p)
#define CHKTAG(mod, p, pe)	ismatch(p, mod, pe->pe_class, pe->pe_id)

static fre_seq();
static fre_seqof();
static fre_choice();

/*
 * free an objects data. Basic algorithm is to walk through it twice
 * first time freeing all the "children" of the data structure - then
 * the second time free the structure itself
 */
fre_obj(parm, p, mod, dofree)
	modtyp *mod;
	ptpe *p;
	char *parm;
	int dofree;
{
	char *malptr = NULL;		/* Have we seen a malloc */
	int ndofree = dofree;		/* Does the function below deallocate space */

	if (parm == 0)
		return (OK);

	if (p->pe_type != PE_START) {
		(void) pepsylose(mod, p, NULLPE, "fre_obj: missing PE_START\n");
		return (NOTOK);
	}

	for (p++; p->pe_type != PE_END; NEXT_TPE(p)) {

		/* 
		 * we have to have all these cases here because it is different to the
		 * situation when the entry is not the main entry of the typereference.
		 */
		switch (p->pe_type) {
		case MEMALLOC:
			if (dofree) {
				malptr = parm;
				ndofree = 0;	/* we are deallocating space on this level */
			}
			break;

		default:
			if (fre_type(parm, p, mod, ndofree) != OK)
				return (NOTOK);
			break;
		}
	}

	if (malptr && dofree) {	/* If we saw a malloc free item */
		free(malptr);
		malptr = NULLCP;
	}

	return (OK);
}

/*
 * Handle freeing of single type field. All the more general routines
 * fall back to this so we can put the code to free something just
 * here once and it will handle all the cases else where
 */
fre_type(parm, p, mod, dofree)
	char *parm;
	ptpe *p;
	modtyp *mod;			/* Module it is from */
	int dofree;
{

	if (parm == 0)
		return OK;

      again:
	switch (p->pe_type) {
	case MEMALLOC:
		break;

	case PE_END:
	case PE_START:
	case UCODE:
		break;

	case BOPTIONAL:
	case FREE_ONLY:	/* this next entry is for us */
	case DFLT_F:
		p++;
		goto again;

	case ETAG:
		switch (p->pe_ucode) {

		default:
			p++;
			if (fre_type(parm, p, mod, dofree) != OK)
				return (NOTOK);
		}
		break;

	case SEQ_START:
	case SET_START:
		if (fre_seq(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
			return (NOTOK);
		break;

	case SEQOF_START:
	case SETOF_START:
		if (fre_seqof(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
			return (NOTOK);
		break;

	case SSEQ_START:
	case SSET_START:
		if (fre_seq((char *) parm + p->pe_ucode, p, mod, dofree) != OK)
			return (NOTOK);
		break;

	case SSEQOF_START:
	case SSETOF_START:
		if (fre_seqof((char *) parm + p->pe_ucode, p, mod, dofree) != OK)
			return (NOTOK);
		break;

	case IMP_OBJ:
		p++;
		if (p->pe_type == EXTOBJ) {
			if (fre_obj(*(char **) (parm + p->pe_ucode),
				    (EXT2MOD(mod, (p + 1)))->md_dtab[p->pe_tag],
				    EXT2MOD(mod, (p + 1)), 1) != OK)
				return (NOTOK);
		} else if (p->pe_type == SEXTOBJ) {
			if (fre_obj((parm + p->pe_ucode),
				    (EXT2MOD(mod, (p + 1)))->md_dtab[p->pe_tag],
				    EXT2MOD(mod, (p + 1)), dofree) != OK)
				return (NOTOK);
		} else if (p->pe_type == SOBJECT) {
			if (fre_obj
			    ((char *) parm + p->pe_ucode, mod->md_dtab[p->pe_tag], mod,
			     dofree) != OK)
				return (NOTOK);
		} else
		    if (fre_obj(*(char **) (parm + p->pe_ucode),
				mod->md_dtab[p->pe_tag], mod, 1) != OK)
			return (NOTOK);
		break;

	case SOBJECT:
		if (fre_obj((char *) parm + p->pe_ucode, mod->md_dtab[p->pe_tag], mod, dofree) !=
		    OK)
			return (NOTOK);
		break;

	case OBJECT:
		if (fre_obj(*(char **) (parm + p->pe_ucode), mod->md_dtab[p->pe_tag], mod, 1) != OK)
			return (NOTOK);
		break;

	case SCHOICE_START:
		if (fre_choice((char *) parm + p->pe_ucode, p, mod, dofree) != OK)
			return (NOTOK);
		break;

	case CHOICE_START:
		if (fre_choice(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
			return (NOTOK);
		break;

	case SEXTOBJ:
		if (p[1].pe_type != EXTMOD) {
			(void) pepsylose(mod, p, NULLPE, "fre_type:missing EXTMOD");
			return (NOTOK);
		}
		if (fre_obj(parm + p->pe_ucode, (EXT2MOD(mod, (p + 1)))->md_dtab[p->pe_tag],
			    EXT2MOD(mod, (p + 1)), dofree) != OK)
			return (NOTOK);
		break;

	case EXTOBJ:
		if (p[1].pe_type != EXTMOD) {
			(void) pepsylose(mod, p, NULLPE, "fre_type:missing EXTMOD");
			return (NOTOK);
		}
		if (fre_obj(*(char **) (parm + p->pe_ucode),
			    (EXT2MOD(mod, (p + 1)))->md_dtab[p->pe_tag],
			    EXT2MOD(mod, (p + 1)), 1) != OK)
			return (NOTOK);
		break;

	case INTEGER:
	case BOOLEAN:
	case T_NULL:
	case REALTYPE:
		break;

	case SANY:
		/* 
		 * These tests of the pointer don't appear necessary from the
		 * definition of encoding and decoding but ISODE generates
		 * freeing code that does these checks and ISODE's ps layer
		 * definitely requires it
		 */
		if (parm != NULL) {
			pe_free((PE) parm);
			parm = NULL;
		}
		break;

	case ANY:
		if (*(char **) (parm + p->pe_ucode) != NULL) {
			pe_free(*(PE *) (parm + p->pe_ucode));
			*((PE *) (parm + p->pe_ucode)) = NULLPE;
		}
		break;

	case SOCTETSTRING:
		if (parm != NULL) {
			qb_free((struct qbuf *) parm);
			parm = NULL;
		}
		break;

	case T_STRING:
	case OCTET_PTR:
	case BITSTR_PTR:
		if (*(char **) (parm + p->pe_ucode) != NULL) {
			free(*(char **) (parm + p->pe_ucode));
			*(char **) (parm + p->pe_ucode) = NULLCP;
		}
		break;

	case OCTETSTRING:
		if (*(char **) (parm + p->pe_ucode) != NULL) {
			qb_free(*(struct qbuf **) (parm + p->pe_ucode));
			*(struct qbuf **) (parm + p->pe_ucode) = (struct qbuf *) 0;
		}
		break;

	case SBITSTRING:
		if (parm != NULL) {
			pe_free((PE) parm);
			parm = NULL;
		}
		break;

	case BITSTRING:
		if (*(char **) (parm + p->pe_ucode) != NULL) {
			pe_free(*(PE *) (parm + p->pe_ucode));
			*(PE *) (parm + p->pe_ucode) = NULLPE;
		}
		break;

	case SOBJID:
		if (parm != NULL) {
			oid_free((OID) parm);
			parm = NULL;
		}
		break;

	case OBJID:
		if (*(char **) (parm + p->pe_ucode) != NULL) {
			oid_free(*(OID *) (parm + p->pe_ucode));
			*(OID *) (parm + p->pe_ucode) = NULLOID;
		}
		break;

	case FN_CALL:
		break;		/* can't do anything with this */

	case FFN_CALL:
		if ((FN_PTR(mod, p)) (parm) == NOTOK)
			return pepsylose(mod, p, NULLPE, "fre_type:FN_CALL:call failed");
		break;

	default:
		(void) pepsylose(mod, p, NULLPE, "fre_type: %d not implemented\n", p->pe_type);
		return (NOTOK);
	}

	return (OK);
}

/*
 * free elements of a sequential type. e.g. sequence or set
 */
static
fre_seq(parm, p, mod, dofree)
	char *parm;
	ptpe *p;
	modtyp *mod;			/* Module it is from */
	int dofree;
{
/*    int    *popt = NULL;	Pointer to optional field */
	char *malptr = NULL;		/* Have we seen a malloc */
	int ndofree = dofree;		/* Does the function below deallocate space */

	if (parm == 0)
		return OK;

	if (p->pe_type != SEQ_START && p->pe_type != SET_START
	    && p->pe_type != SSEQ_START && p->pe_type != SSET_START) {
		(void) pepsylose(mod, p, NULLPE, "fre_seq: bad starting item %d\n", p->pe_type);
		return (NOTOK);
	}
	p++;

	if (p->pe_type == DFLT_B)
		p++;

	while (p->pe_type != PE_END) {

		switch (p->pe_type) {
		case MEMALLOC:
			if (dofree) {
				malptr = parm;
				ndofree = 0;	/* we are deallocating space on this level */
			}
			break;

		case OPTL:
/*	    popt = (int *) (parm + p->pe_ucode); */
			break;

		case ETAG:
			p++;
			continue;

		case UCODE:
			break;

		case SET_START:
		case SEQ_START:
			if (fre_seq(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
				return (NOTOK);
			break;

		case SETOF_START:
		case SEQOF_START:
			if (fre_seqof(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
				return (NOTOK);
			break;

		case SSEQ_START:
		case SSET_START:
			if (fre_seq((char *) parm + p->pe_ucode, p, mod, ndofree) != OK)
				return (NOTOK);
			break;

		case SSEQOF_START:
		case SSETOF_START:
			if (fre_seqof((char *) parm + p->pe_ucode, p, mod, ndofree) != OK)
				return (NOTOK);
			break;

		case IMP_OBJ:
			p++;
			continue;

		case SOBJECT:
			if (fre_obj((char *) parm + p->pe_ucode,
				    mod->md_dtab[p->pe_tag], mod, ndofree) != OK)
				return (NOTOK);
			break;

		case OBJECT:
			if (fre_obj(*(char **) (parm + p->pe_ucode),
				    mod->md_dtab[p->pe_tag], mod, 1) != OK)
				return (NOTOK);
			break;

		case SCHOICE_START:
			if (fre_choice((char *) parm + p->pe_ucode, p, mod, ndofree) != OK)
				return (NOTOK);
			break;

		case CHOICE_START:
			if (fre_choice(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
				return (NOTOK);
			break;

		case SEXTOBJ:
			if (p[1].pe_type != EXTMOD) {
				(void) pepsylose(mod, p, NULLPE, "fre_seq:missing EXTMOD");
				return (NOTOK);
			}
			if (fre_obj(parm + p->pe_ucode, (EXT2MOD(mod, (p + 1)))->md_dtab[p->pe_tag],
				    EXT2MOD(mod, (p + 1)), ndofree) != OK)
				return (NOTOK);
			break;

		case EXTOBJ:
			if (p[1].pe_type != EXTMOD) {
				(void) pepsylose(mod, p, NULLPE, "fre_seq:missing EXTMOD");
				return (NOTOK);
			}
			if (fre_obj(*(char **) (parm + p->pe_ucode),
				    (EXT2MOD(mod, (p + 1)))->md_dtab[p->pe_tag],
				    EXT2MOD(mod, (p + 1)), 1) != OK)
				return (NOTOK);
			break;

		default:
			if (fre_type(parm, p, mod, ndofree) != OK)
				return (NOTOK);
			break;
		}

		NEXT_TPE(p);
	}
	if (malptr && dofree) {	/* If we saw a malloc free item */
		free(malptr);
		malptr = NULLCP;
	}

	return (OK);

}

/*
 * free all the fields in a SET OF/SEQUENCE OF type structure. We
 * must follow the linked list until the end
 */
static
fre_seqof(parm, p, mod, dofree)
	char *parm;
	ptpe *p;
	modtyp *mod;			/* Module it is from */
	int dofree;
{
	ptpe *start;			/* first entry in list */
	char *oparm;

	if (parm == 0)
		return OK;

	if (p->pe_type != SEQOF_START && p->pe_type != SETOF_START
	    && p->pe_type != SSEQOF_START && p->pe_type != SSETOF_START) {
		(void) pepsylose(mod, p, NULLPE, "fre_seqof: illegal field");
		return (NOTOK);
	}
	for (start = p; (char *) parm != NULL; p = start) {
		p++;

		if (p->pe_type == DFLT_B)
			p++;

		while (p->pe_type != PE_END) {

			switch (p->pe_type) {
			case MEMALLOC:
				break;

			case ETAG:
				p++;
				continue;

			case UCODE:
				break;

			case SEQ_START:
			case SET_START:
				if (fre_seq(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
					return (NOTOK);
				break;

			case SEQOF_START:
			case SETOF_START:
				if (fre_seqof(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
					return (NOTOK);
				break;

			case SSEQ_START:
			case SSET_START:
				if (fre_seq((char *) parm + p->pe_ucode, p, mod, dofree) != OK)
					return (NOTOK);
				break;

			case SSEQOF_START:
			case SSETOF_START:
				if (fre_seqof((char *) parm + p->pe_ucode, p, mod, dofree) != OK)
					return (NOTOK);
				break;

			case IMP_OBJ:
				p++;
				continue;

			case SOBJECT:
				if (fre_obj(parm + p->pe_ucode, mod->md_dtab[p->pe_tag], mod, 0) !=
				    OK)
					return (NOTOK);
				break;

			case OBJECT:
				if (fre_obj(*(char **) (parm + p->pe_ucode),
					    mod->md_dtab[p->pe_tag], mod, 1) != OK)
					return (NOTOK);
				break;

			case SCHOICE_START:
				if (fre_choice((char *) parm + p->pe_ucode, p, mod, 0) != OK)
					return (NOTOK);
				break;

			case CHOICE_START:
				if (fre_choice(*(char **) (parm + p->pe_ucode), p, mod, 1) != OK)
					return (NOTOK);
				break;

			case SEXTOBJ:
				if (p[1].pe_type != EXTMOD) {
					(void) pepsylose(mod, p, NULLPE,
							 "fre_seqof: missing EXTMOD");
					return (NOTOK);
				}
				if (fre_obj
				    (parm + p->pe_ucode,
				     (EXT2MOD(mod, (p + 1)))->md_dtab[p->pe_tag], EXT2MOD(mod,
											  (p + 1)),
				     0) != OK)
					return (NOTOK);
				break;

			case EXTOBJ:
				if (p[1].pe_type != EXTMOD) {
					(void) pepsylose(mod, p, NULLPE,
							 "fre_seqof: missing EXTMOD");
					return (NOTOK);
				}
				if (fre_obj(*(char **) (parm + p->pe_ucode),
					    (EXT2MOD(mod, (p + 1)))->md_dtab[p->pe_tag],
					    EXT2MOD(mod, (p + 1)), 1) != OK)
					return (NOTOK);
				break;

			default:
				if (fre_type(parm, p, mod, 1) != OK)
					return (NOTOK);
				break;
			}

			NEXT_TPE(p);
		}
		oparm = parm;
		parm = *(char **) (parm + p->pe_ucode);	/* Any more ? */
		if (dofree) {
			free(oparm);
			oparm = NULLCP;
		}
	}

	return (OK);

}

/*
 * free the item of the choice. Use the SCTRL field to determine
 * which item is present and then call the appropriate routine to
 * free it
 */
static
fre_choice(parm, p, mod, dofree)
	char *parm;
	ptpe *p;
	modtyp *mod;			/* Module it is from */
	int dofree;
{
	int cnt;
	char *malptr = NULL;		/* Have we seen a malloc */
	int ndofree = dofree;		/* Does the function below deallocate space */

	if (parm == 0)
		return OK;

	if (p->pe_type != CHOICE_START && p->pe_type != SCHOICE_START) {
		(void) pepsylose(mod, p, NULLPE,
				 "fre_choice:CHOICE_START missing found %d\n", p->pe_type);
	}
	p++;

	if (p->pe_type == DFLT_B)
		p++;

	if (p->pe_type == MEMALLOC) {
		if (dofree) {
			malptr = parm;
			ndofree = 0;	/* we are deallocating space on this level */
		}
		p++;
	}
	if (p->pe_type != SCTRL) {
		(void) pepsylose(mod, p, NULLPE, "fre_choice: missing SCTRL information\n");
		return (NOTOK);
	}
	cnt = *(int *) (parm + p->pe_ucode);
	if (cnt != 0)
		cnt--;
	if (cnt < 0) {
		(void) pepsylose(mod, p, NULLPE, "fre_choice:offset negative %d", cnt);
		return (NOTOK);
	}
	for (p++; p->pe_type != PE_END; NEXT_TPE(p)) {
		if (ISDTYPE(p)) {
			if (cnt == 0) {
				if (fre_type(parm, p, mod, ndofree) != OK)
					return (NOTOK);
				if (malptr && dofree) {	/* If we saw a malloc free item */
					free(malptr);
					malptr = NULLCP;
				}
				return (OK);
			}
			cnt--;
		}
	}

	(void) pepsylose(mod, p, NULLPE, "fre_choice: no choice taken");
	return (NOTOK);
}

/*
 * look out for FN_CALL - if this entry is really a FN_CALL return non zero
 * else return 0
 * Basically we have to stop FN_CALL being tested by hasdata which will call
 * the decoding function which is illegal and gives rubbish.
 */
callsfn(p, mod)
	ptpe *p;
	modtyp *mod;
{

	while (p->pe_type != PE_END) {
		switch (p->pe_type) {
		case ETAG:
			p++;
			continue;

		case FN_CALL:
			return (1);

		default:
		case INTEGER:
		case REALTYPE:
		case BOOLEAN:
		case T_NULL:
		case IMP_OBJ:
		case OBJECT:
			return (0);
		}
	}

	(void) pepsylose(mod, p, NULLPE, "callsfn:Corrupted tables:PE_END found\n");
	ferr(1, "callsfn:Mangled tables\n");
 /*NOTREACHED*/}