/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for cvimpstuicontactselection ,a contact selection class from pbk
*
*/

// INCLUDE FILES
#include "cvimpstuicontactselection.h"
#include "cvimpstuitabbedview.h"
#include "uiservicetabtracer.h"

#include <e32base.h>

// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
// CVIMPSTUiContactSelection::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiContactSelection* CVIMPSTUiContactSelection::NewL(CVIMPSTUiTabbedView& iViewObserver )
	{
	CVIMPSTUiContactSelection* self = new (ELeave) CVIMPSTUiContactSelection( iViewObserver );
    return self;
 	}

// --------------------------------------------------------------------------
// CVIMPSTUiContactSelection::~CVIMPSTUiContactSelection
// --------------------------------------------------------------------------
//
CVIMPSTUiContactSelection::~CVIMPSTUiContactSelection()
	{
	Cancel();	
	}
	
// --------------------------------------------------------------------------
// CVIMPSTUiContactSelection::CVIMPSTUiContactSelection
// --------------------------------------------------------------------------
//
CVIMPSTUiContactSelection::CVIMPSTUiContactSelection(CVIMPSTUiTabbedView& iViewObserver )
: CActive( CActive::EPriorityStandard ),
	iViewObserver( iViewObserver )
	{
	CActiveScheduler::Add( this );	
	}
	
// --------------------------------------------------------------------------
// CVIMPSTUiContactSelection::IssueRequest
// --------------------------------------------------------------------------
//
void CVIMPSTUiContactSelection::IssueRequest()
	{
	TRACER_AUTO;
	if( IsActive() )
		{
		Cancel();	
		}
	if( !IsActive() )
		{
		TRequestStatus* status = &iStatus;
		User::RequestComplete( status, KErrNone );
		SetActive();	
		}
	}
	
// --------------------------------------------------------------------------
// CVIMPSTUiContactSelection::RunL
// --------------------------------------------------------------------------
//
void CVIMPSTUiContactSelection::RunL()
	{
	if( iStatus.Int() == 0 )
		{
		iViewObserver.HandleContactSelectionCompleteL();	
		}
	}
	
// --------------------------------------------------------------------------
// CVIMPSTUiContactSelection::DoCancel
// --------------------------------------------------------------------------
//
void CVIMPSTUiContactSelection::DoCancel()
	{
	if( IsActive() )
		{
		Cancel();	
		}
	}
	
// --------------------------------------------------------------------------
// CVIMPSTUiContactSelection::RunError
// --------------------------------------------------------------------------
//
TInt CVIMPSTUiContactSelection::RunError( TInt aError )
	{
	return aError;
	}
	
//end of file
