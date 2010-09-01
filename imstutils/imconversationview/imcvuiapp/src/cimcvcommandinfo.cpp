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
* Description:   command info
*
*/

// INCLUDE FILES
#include <barsread.h>
#include "cimcvcommandinfo.h"


// ==================== MEMBER FUNCTIONS ====================
// ----------------------------------------------------------------------------
// CIMCVCommandInfo::CIMCVCommandInfo
// ----------------------------------------------------------------------------
//
CIMCVCommandInfo::CIMCVCommandInfo( TInt32 aPluginId ) : iPluginId( aPluginId )   
    {
    }
// ----------------------------------------------------------------------------
// CIMCVCommandInfo::~CIMCVCommandInfo
// ----------------------------------------------------------------------------
//
CIMCVCommandInfo::~CIMCVCommandInfo()
    { 
    }
// ----------------------------------------------------------------------------
// CIMCVCommandInfo::NewLC
// ----------------------------------------------------------------------------
//
CIMCVCommandInfo* CIMCVCommandInfo::NewLC( TResourceReader& aReader,
                                    TInt32 aPluginId,
        							TInt& aNewCommandIds )
    {
    CIMCVCommandInfo* self = new (ELeave) CIMCVCommandInfo( aPluginId );
    CleanupStack::PushL(self);
    self->ConstructL( aReader, aNewCommandIds );
    return self;
    }
// ----------------------------------------------------------------------------
// CIMCVCommandInfo::ConstructL
// ----------------------------------------------------------------------------
//    
void CIMCVCommandInfo::ConstructL( TResourceReader& aReader,
        						TInt& aNewCommandIds )
    {
    iOldCommandId = aReader.ReadInt32();
    iNewCommandId = aNewCommandIds++;    
    }
// ----------------------------------------------------------------------------
// CIMCVCommandInfo::PliginId
// ----------------------------------------------------------------------------
//   
TInt32 CIMCVCommandInfo::PluginId() const
	{
	return iPluginId;	
	}
// ----------------------------------------------------------------------------
// CIMCVCommandInfo::NewCommandId
// ----------------------------------------------------------------------------
//	
TInt32 CIMCVCommandInfo::NewCommandId() const
	{
	return iNewCommandId;	
	}
// ----------------------------------------------------------------------------
// CIMCVCommandInfo::OldCommandId
// ----------------------------------------------------------------------------
//
TInt32 CIMCVCommandInfo::OldCommandId() const
	{
	return iOldCommandId;	
	}
    
		       
// end of file
