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
* Description:  
:                
*
*/



// INCLUDES
#include	"vimpstutils.h"
#include "stringloader.h"
#include <coemain.h>


// CONSTANTS
// TODO: For later use
// general colon needed in various places
//_LIT( KColon, ":" );
// general slash needed in various places
//_LIT( KSlash, "/" );
// general slash needed in various places
_LIT( KAt, "@" );

// -----------------------------------------------------------------------------
// VIMPSTUtils::LoadResourceL
// give ownership to caller
// -----------------------------------------------------------------------------
// 
 HBufC* VIMPSTUtils::LoadResourceL( TInt aResourceId )
	{
	CCoeEnv* env = CCoeEnv::Static();
	HBufC* ret = NULL;
	if(env)
		{
		ret = env->AllocReadResourceL(aResourceId);
		return ret;
		}
	else
		{
		return ret;
		}

	}


// End of file
