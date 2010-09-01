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
* Description:  wrapper for message container
*
*/




// INCLUDE FILES
#include    "cimcvappmessagewrapper.h"
#include    "mimcvenginemessage.h"
#include    "mimcvenginemessagecontainer.h"
#include 	"imcvlogger.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::CIMCVAppMessageWrapper
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppMessageWrapper::CIMCVAppMessageWrapper( MIMCVEngineMessage& aWrappedMessage
                                       )
    : iMessage( aWrappedMessage ),
     iInserted( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppMessageWrapper* CIMCVAppMessageWrapper::NewL( MIMCVEngineMessage& aWrappedMessage )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageWrapper::NewL()	start" ));
    CIMCVAppMessageWrapper* self = 
        new( ELeave ) CIMCVAppMessageWrapper( aWrappedMessage );
    IM_CV_LOGS(TXT("CIMCVAppMessageWrapper::NewL()	end" ));
    return self;
    }


// Destructor
CIMCVAppMessageWrapper::~CIMCVAppMessageWrapper()
    {
       
    iHighlights.Reset();
    iTypes.Reset();
    }



// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::ThumbPos
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection& CIMCVAppMessageWrapper::ThumbPos()
    {
    return iThumbPosition;
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::Selection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection& CIMCVAppMessageWrapper::Selection()
    {
    return iSelection;
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::WholeSelection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection& CIMCVAppMessageWrapper::WholeSelection()
    {
    return iWholeSelection;
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::WholeSelection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection& CIMCVAppMessageWrapper::MessageSelection()
    {
    return iMessageSelection;
    }
        
// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::Highlights
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RArray<TCursorSelection>& CIMCVAppMessageWrapper::Highlights()
    {
    return iHighlights;
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::HighlightTypes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RArray<TInt>& CIMCVAppMessageWrapper::HighlightTypes()
    {
    return iTypes;
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::Message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MIMCVEngineMessage& CIMCVAppMessageWrapper::Message()
    {
    return iMessage;    
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::SetInserted
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageWrapper::SetInserted( TBool aInserted )
    {
    iInserted = aInserted;
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageWrapper::IsInserted
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppMessageWrapper::IsInserted() const
    {
    return iInserted;
    }
    
//  End of File
