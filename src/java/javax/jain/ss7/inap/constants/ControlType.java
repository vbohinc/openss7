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
 *  File Name     : ControlType.java
 *  Approver      : Jain Inap Edit Group
 *  Version       : 1.0
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

package javax.jain.ss7.inap.constants;


import java.io.*;
import java.util.*;

/**
This class ControlType indicates the reason for activating call gapping.
*/

public class ControlType {

    public static final int M_SCP_OVERLOADED=0;
    public static final int M_MANUALLY_INITIATED=1;
    public static final int M_DESTINATION_OVERLOAD=2;
   
   //internal variable to store the constant value
   
   private int controlType;
   
     
    
/**
Constructor for ControlType datatype
*/
    
   private ControlType(int controlType) 
   {
   		this.controlType=controlType;
   }
   
   
/*
* ControlType :SCP_OVERLODED
*
*/

  
  public static final ControlType SCP_OVERLOADED =new ControlType (M_SCP_OVERLOADED);
  
  
  
   
/*
* ControlType :MANUALLY_INITIATED
*
*/

  
  public static final ControlType MANUALLY_INITIATED =new ControlType (M_MANUALLY_INITIATED);
  
  
   
/*
* ControlType :DESTINATION_OVERLOAD
*
*/

  
  public static final ControlType DESTINATION_OVERLOAD =new ControlType (M_DESTINATION_OVERLOAD);
  


	/**
	* Gets the integer String representation of the Constant class 
	*
	* @ return  Integer provides value of Constant
	*
	*/
	
	public int getControlType ()
	{
	
		return controlType;
		
	}




}