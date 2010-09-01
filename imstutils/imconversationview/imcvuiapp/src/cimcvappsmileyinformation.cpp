/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Storage to save smiley information
*
*/


// INCLUDES
#include "cimcvappsmileyinformation.h"
#include "imcvlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::CIMCVAppSmileyInformation()
// ---------------------------------------------------------------------------
//
CIMCVAppSmileyInformation::CIMCVAppSmileyInformation()
    {
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::NewL()
// ---------------------------------------------------------------------------
//
CIMCVAppSmileyInformation* CIMCVAppSmileyInformation::NewL()
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyInformation::NewL() start") );
    CIMCVAppSmileyInformation* self = new (ELeave) CIMCVAppSmileyInformation;
	IM_CV_LOGS(TXT("CIMCVAppSmileyInformation::NewL() end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::~CIMCVAppSmileyInformation()
// ---------------------------------------------------------------------------
//
CIMCVAppSmileyInformation::~CIMCVAppSmileyInformation()
    {
    if(iSmileyString)
	    {
	    delete iSmileyString;
	    iSmileyString = NULL;
	    }
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::Position()
// ---------------------------------------------------------------------------
//
TInt CIMCVAppSmileyInformation::Position() const
    {
    return iPosition;
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::SetPosition()
// ---------------------------------------------------------------------------
//
void CIMCVAppSmileyInformation::SetPosition( TInt aPosition )
    {
    iPosition = aPosition;
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::Index()
// ---------------------------------------------------------------------------
//
TInt CIMCVAppSmileyInformation::Index() const
    {
    return iIndex;
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::SetIndex()
// ---------------------------------------------------------------------------
//
void CIMCVAppSmileyInformation::SetIndex( TInt aIndex )
    {
    iIndex = aIndex;
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::SmileyString()
// ---------------------------------------------------------------------------
//
const TDesC& CIMCVAppSmileyInformation::SmileyString() const
    {
    return *iSmileyString;
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::SetSmileyStringL()
// ---------------------------------------------------------------------------
//
void CIMCVAppSmileyInformation::SetSmileyStringL( const TDesC& aSmiley )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyInformation::SetSmileyStringL() start") );
    HBufC* smile = aSmiley.AllocL();
    delete iSmileyString;
    iSmileyString = smile;
    IM_CV_LOGS(TXT("CIMCVAppSmileyInformation::SetSmileyStringL() end") );
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::IsIcon()
// ---------------------------------------------------------------------------
//
TBool CIMCVAppSmileyInformation::IsIcon() const
    {
    return iIsIcon;
    }

// ---------------------------------------------------------------------------
// CIMCVAppSmileyInformation::SetIcon()
// ---------------------------------------------------------------------------
//
void CIMCVAppSmileyInformation::SetIcon( TBool aIsIcon )
    {
    iIsIcon = aIsIcon;
    }
