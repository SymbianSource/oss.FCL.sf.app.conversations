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
* Description:  vimpstui.dll
 *
*/

// INCLUDE FILES
#include <barsread.h>
#include "ccommandinfo.h"


// CONSTANTS

// ==================== MEMBER FUNCTIONS ====================
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::CCommandInfo
// ----------------------------------------------------------------------------
//
CCommandInfo::CCommandInfo( TInt32 aPluginId ) : iPluginId( aPluginId )   
    {
    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::~CCommandInfo
// ----------------------------------------------------------------------------
//
CCommandInfo::~CCommandInfo()
    { 
    }
// ----------------------------------------------------------------------------
// CCommandInfo::NewLC
// ----------------------------------------------------------------------------
//
CCommandInfo* CCommandInfo::NewLC( TResourceReader& aReader,
                                    TInt32 aPluginId,
        							TInt& aNewCommandIds )
    {
    CCommandInfo* self = new (ELeave) CCommandInfo( aPluginId );
    CleanupStack::PushL(self);
    self->ConstructL( aReader, aNewCommandIds );
    return self;
    }
// ----------------------------------------------------------------------------
// CCommandInfo::ConstructL
// ----------------------------------------------------------------------------
//    
void CCommandInfo::ConstructL( TResourceReader& aReader,
        						TInt& aNewCommandIds )
    {
    iOldCommandId = aReader.ReadInt32();
    iNewCommandId = aNewCommandIds++;    
    }
// ----------------------------------------------------------------------------
// CCommandInfo::Id
// ----------------------------------------------------------------------------
//   
TInt32 CCommandInfo::PliginId() const
	{
	return iPluginId;	
	}
// ----------------------------------------------------------------------------
// CCommandInfo::NewCommandId
// ----------------------------------------------------------------------------
//	
TInt32 CCommandInfo::NewCommandId() const
	{
	return iNewCommandId;	
	}
// ----------------------------------------------------------------------------
// CCommandInfo::OldCommandId
// ----------------------------------------------------------------------------
//
TInt32 CCommandInfo::OldCommandId() const
	{
	return iOldCommandId;	
	}
    
		       
// end of file
