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
 *  File Name     : DigitsGenericDigits.java
 *  Approver      : Jain Inap Edit Group
 *  Version       : 1.0
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
package javax.jain.ss7.inap.datatype;


import java.io.*;
import java.util.*;
import javax.jain.ss7.inap.constants.*;
/**
This Class defines the digits in the Generic Digits Format. (Encoded as in Q.763)
*/

public class DigitsGenericDigits implements java.io.Serializable 
{


    private int typeOfDigits;
    
    private int encodingScheme;
    
    private String digit;

/**
Constructor For DigitsGenericDigits
*/
	public DigitsGenericDigits(int typeOfDigits, int encodingScheme, java.lang.String digit)
	{
		setTypeOfDigits (typeOfDigits);
		setEncodingScheme (encodingScheme);
		setDigit (digit);
	}
	
//------------------------------------------	

/**
Gets  Type of Digits
*/

    public int getTypeOfDigits()
    {
        return typeOfDigits ;
    }

/**
Sets  Type of Digits
*/

    public void  setTypeOfDigits (int typeOfDigits)
    {
        this.typeOfDigits = typeOfDigits ;
    }

//------------------------------------------	

/**
Gets  Encoding Scheme
*/

    public int getEncodingScheme()
    {
        return encodingScheme ;
    }

/**
Sets  Encoding Scheme
*/

    public void  setEncodingScheme (int encodingScheme)
    {
        this.encodingScheme = encodingScheme ;
    }

//------------------------------------------	

/**
Gets  Digits
*/

    public java.lang.String getDigit()
    {
        return digit ;
    }

/**
Sets  Digits
*/

    public void  setDigit (java.lang.String digit)
    {
        this.digit = digit ;
    }

//------------------------------------------	

}