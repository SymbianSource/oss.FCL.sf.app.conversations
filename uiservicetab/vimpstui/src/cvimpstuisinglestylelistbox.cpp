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
* Description:  Custom listbox component for the contact list
*
*/


#include "cvimpstuisinglestylelistbox.h"
#include "cvimpstuilistboxmodel.h"
#include <aknlists.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CVIMPSTUiListBox::CreateModelL
// Here we change the listbox model
// ---------------------------------------------------------------------------
//
void CVIMPSTUiSingleStyleListBox::CreateModelL()
    {
    // we have to make sure that the model is ours,
    // so we can leave if the model creation fails.
    // (just make sure that the pointer is NULL)
    // in column style listbox , listboxes consturctL() calls this method
    delete iModel;
    iModel = NULL;
    iModel = new ( ELeave ) CVIMPSTUiListBoxModel;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBox::SetContactListModel
// ConstructL has to be called before calling this
// ---------------------------------------------------------------------------
//
void CVIMPSTUiSingleStyleListBox::SetContactListModelL(MVIMPSTProcessArray& aContactListModel )
    {
    if ( iModel )
        {
        static_cast<CVIMPSTUiListBoxModel*>(iModel)->SetContactListModel( aContactListModel );
        }
    }
    

