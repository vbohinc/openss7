/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Copyrights:
 *
 *  Copyright - 1999 Sun Microsystems, Inc. All rights reserved.
 *  901 San Antonio Road, Palo Alto, California 94043, U.S.A.
 *
 *  This product and related documentation are protected by copyright and
 *  distributed under licenses restricting its use, copying, distribution, and
 *  decompilation. No part of this product or related documentation may be
 *  reproduced in any form by any means without prior written authorization of
 *  Sun and its licensors, if any.
 *
 *  RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the United
 *  States Government is subject to the restrictions set forth in DFARS
 *  252.227-7013 (c)(1)(ii) and FAR 52.227-19.
 *
 *  The product described in this manual may be protected by one or more U.S.
 *  patents, foreign patents, or pending applications.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Author:
 *
 *  Mahindra British Telecom
 *  155 , Bombay - Pune Road 
 *  Pimpri ,
 *  Pune - 411 018.
 *
 *  Module Name   : JAIN INAP API
 *  File Name     : CGEncountered.java
 *  Approver      : Jain Inap Edit Group
 *  Version       : 1.0
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
package javax.jain.ss7.inap.constants;


import java.io.*;
import java.util.*;

/**
This class indicates the type of automatic call gapping encountered, if any.
*/

public class CGEncountered  {

    public static final int M_NO_CG_ENCOUNTERED=0;
    public static final int M_MANUAL_CG_ENCOUNTERED=1;
    public static final int M_SCP_OVERLOAD=2;
    
	//Internal variable to store constant value
	
	private int cGEncountered ;
	
	
/**
Constructor for CGEncountered datatype
*/
    
    private CGEncountered(int cGEncountered) 
	{
    	this.cGEncountered=cGEncountered;	
		
	}    

/**
* CGEncountered :NO_CG_ENCOUNTERED
*
*/

public static final CGEncountered NO_CG_ENCOUNTERED=new CGEncountered(M_NO_CG_ENCOUNTERED);



/**
* CGEncountered :MANUAL_CG_ENCOUNTERED
*
*/

public static final CGEncountered MANUAL_CG_ENCOUNTERED=new CGEncountered(M_MANUAL_CG_ENCOUNTERED);



/**
* CGEncountered :SCP_OVERLOAD
*
*/

public static final CGEncountered SCP_OVERLOAD=new CGEncountered(M_SCP_OVERLOAD);

	/**
	* Gets the integer String representation of the Constant class 
	*
	* @ return  Integer provides value of Constant
	*
	*/
	
	public int getCGEncountered()
	{
	
		return cGEncountered;
	
	}

}

