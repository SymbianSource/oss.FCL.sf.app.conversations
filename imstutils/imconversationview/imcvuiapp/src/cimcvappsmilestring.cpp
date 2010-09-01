/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Class for storing one smiley with index to icon array.
*
*/


// INCLUDE FILES
#include "cimcvappsmilestring.h"
#include "imcvlogger.h"

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CIMCVAppSmileString* CIMCVAppSmileString::NewL()
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileString::NewL() start") );
    CIMCVAppSmileString* self = new (ELeave) CIMCVAppSmileString();
    IM_CV_LOGS(TXT("CIMCVAppSmileString::NewL() end") );
    return self;
    }

// Destructor
CIMCVAppSmileString::~CIMCVAppSmileString()
    {
    if(iSmiley)
	    {
	    delete iSmiley;
	    iSmiley = NULL;
	    }
	if(iShortestSmileyString)
		{
	    delete iShortestSmileyString;
	    iShortestSmileyString = NULL;
		}
	}

// C++ constructor can NOT contain any code, that
// might leave.
//
CIMCVAppSmileString::CIMCVAppSmileString()
    {
    }

// ---------------------------------------------------------
// CIMCVAppSmileString::SmileIconIndex
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMCVAppSmileString::SmileIconIndex() const
	{
	return iIndex;
	}

// ---------------------------------------------------------
// CIMCVAppSmileString::SmileString
// (other items were commented in a header).
// ---------------------------------------------------------
//
const TDesC& CIMCVAppSmileString::SmileString() const
    {
	return *iSmiley;
	}

// ---------------------------------------------------------
// CIMCVAppSmileString::Set
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileString::Set( const HBufC* aSmileyString, TInt aIndex )
	{
	IM_CV_LOGS(TXT("CIMCVAppSmileString::Set() start") );
	// detect (ignore) double assignment
	if( aSmileyString != iSmiley )
	    {
    	delete iSmiley;
    	iSmiley = aSmileyString;
	    }
    iIndex = aIndex;
    IM_CV_LOGS(TXT("CIMCVAppSmileString::Set() end") );
	}

// ---------------------------------------------------------
// CIMCVAppSmileString::SetShortestSmileStringL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileString::SetShortestSmileStringL( const TDesC& aSmileyString )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileString::SetShortestSmileStringL() start") );
    HBufC* temp = aSmileyString.AllocL();
    delete iShortestSmileyString;
    iShortestSmileyString = temp;
    IM_CV_LOGS(TXT("CIMCVAppSmileString::SetShortestSmileStringL() end") );
    }

// ---------------------------------------------------------
// CIMCVAppSmileString::HasShortestString
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CIMCVAppSmileString::HasShortestString() const
    {
    return iShortestSmileyString ? ETrue : EFalse;
    }

// ---------------------------------------------------------
// CIMCVAppSmileString::ShortestSmileString
// (other items were commented in a header).
// ---------------------------------------------------------
//    
const TDesC& CIMCVAppSmileString::ShortestSmileString() const
    {
    return *iShortestSmileyString;
    }
// End of File
