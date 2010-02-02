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
* Description: Extension view info for VIMPSTUI
*
*/


#include "cvimpstuiextensionviewinfo.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionViewInfo::
//      CVIMPSTUIExtensionViewInfo
// ---------------------------------------------------------------------------
//
CVIMPSTUIExtensionViewInfo::CVIMPSTUIExtensionViewInfo(
    TUint32 aViewId,
    TBool aIsTabbedView,
    TUint aIconId,
    TUint aMaskId ):
    iViewId( aViewId ),
    iIsTabbedView( aIsTabbedView ),
    iIconId( aIconId ),
    iMaskId( aMaskId )
    {
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionViewInfo::ConstructL
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIExtensionViewInfo::ConstructL( const TDesC& aIconFile,
    const TDesC& aServiceName )
    {
    iIconFile = aIconFile.AllocL();
    iServiceName = aServiceName.AllocL();
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionViewInfo::NewL
// ---------------------------------------------------------------------------
// 
CVIMPSTUIExtensionViewInfo* CVIMPSTUIExtensionViewInfo::NewL(
    TUint32 aViewId,
    TBool aIsTabbedView,
    const TDesC& aIconFile,
    TUint aIconId,
    TUint aMaskId,
    const TDesC& aServiceName )
    {
    
    CVIMPSTUIExtensionViewInfo* self = NewLC(
        aViewId, aIsTabbedView, aIconFile, aIconId, aMaskId, aServiceName );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionViewInfo::NewLC
// ---------------------------------------------------------------------------
// 
CVIMPSTUIExtensionViewInfo* CVIMPSTUIExtensionViewInfo::NewLC(
    TUint32 aViewId,
    TBool aIsTabbedView,
    const TDesC& aIconFile,
    TUint aIconId,
    TUint aMaskId,
    const TDesC& aServiceName )
    {
    CVIMPSTUIExtensionViewInfo* self =
        new (ELeave) CVIMPSTUIExtensionViewInfo(
            aViewId, aIsTabbedView, aIconId, aMaskId );
    CleanupStack::PushL(self);
    self->ConstructL( aIconFile, aServiceName );
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionViewInfo::
//      ~CVIMPSTUIExtensionViewInfo
// ---------------------------------------------------------------------------
// 
CVIMPSTUIExtensionViewInfo::~CVIMPSTUIExtensionViewInfo()
    {
    delete iIconFile;
    iIconFile = NULL;
    
    
    delete iServiceName;
    iServiceName = NULL;
    
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionViewInfo::OldViewId
// ---------------------------------------------------------------------------
//     
TInt32 CVIMPSTUIExtensionViewInfo::OldViewId() const
    {
    return iViewId;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionViewInfo::TabView
// ---------------------------------------------------------------------------
//     
TBool CVIMPSTUIExtensionViewInfo::TabView() const
    {
    return iIsTabbedView;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::TabIconId
// ---------------------------------------------------------------------------
//     
TInt32 CVIMPSTUIExtensionViewInfo::TabIconId() const
    {
    return iIconId;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::TabMaskId
// ---------------------------------------------------------------------------
//     
TInt32 CVIMPSTUIExtensionViewInfo::TabMaskId() const
    {
    return iMaskId;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::TabIconFile
// ---------------------------------------------------------------------------
//     
const TDesC& CVIMPSTUIExtensionViewInfo::TabIconFile() const
    {
    return *iIconFile;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::Name
// ---------------------------------------------------------------------------
//     
const TDesC& CVIMPSTUIExtensionViewInfo::Name() const
    {
    return *iServiceName;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SortIconId
// ---------------------------------------------------------------------------
//     
TInt32 CVIMPSTUIExtensionViewInfo::SortIconId() const
    {
    return iIconId;    
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SortMaskId
// ---------------------------------------------------------------------------
//     
TInt32 CVIMPSTUIExtensionViewInfo::SortMaskId() const
    {
    return iMaskId;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionViewInfo::SortIconFile
// ---------------------------------------------------------------------------
//     
const TDesC& CVIMPSTUIExtensionViewInfo::SortIconFile() const
    {
    return *iIconFile;
    }    
// End of file
