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
* Description:  utils class
*
*/


// INCLUDES
#include "servicewidgetutils.h"
#include "servicewidgetcpglobals.h"
#include <coemain.h>
#include <StringLoader.h>
// -----------------------------------------------------------------------------
// ServiceWidgetUtils::DisplayId
// -----------------------------------------------------------------------------
//
TPtrC ServiceWidgetUtils::DisplayId( const TDesC& aId, 
								TBool aDomainHiding )
	{
	TPtrC ret( aId );
	// locate ":" for userid, groupid.
	// locate "/" for list id.
	TInt pos = aId.FindC( aDomainHiding ? KSlash : KColon );
	if ( ( pos != KErrNotFound) && ( pos != aId.Length()-1) )
		{
		// contains the special character, and it is not the last char
		// remove everything before the special char (including the char)
		ret.Set( aId.Mid( pos + 1 ) );
		}
	if( aDomainHiding)
		{
		// remove also the domain part
		TInt domainPos = ret.FindC( KAt );
		if ( ( domainPos != KErrNotFound ) && ( domainPos != 0 ) )
			 {
			 ret.Set( ret.Mid( 0, domainPos ) );
			 }
		} 
	return ret;
	}

// -----------------------------------------------------------------------------
// ServiceWidgetUtils::LoadResourceL
// give ownership to caller
// -----------------------------------------------------------------------------
// 
HBufC* ServiceWidgetUtils::LoadResourceLC( TInt aResourceId ,
									CCoeEnv* aEnv /*= NULL*/)
    {
    HBufC* ret = NULL;
    if(aEnv)
        {
        ret = StringLoader::LoadLC( aResourceId );
        }
     return ret;
     }	
    
// -----------------------------------------------------------------------------
// ServiceWidgetUtils::LoadResourceL
// give ownership to caller
// -----------------------------------------------------------------------------
// 
HBufC* ServiceWidgetUtils::LoadResourceLC( TInt aResourceId ,
										const TDesC& aText ,
										CCoeEnv* aEnv /*= NULL*/ )
    {
    HBufC* ret = NULL;
    if(aEnv)
        {
        ret = StringLoader::LoadLC( aResourceId, aText );
        }
     return ret;
	}		
 
 // -----------------------------------------------------------------------------
// ServiceWidgetUtils::LoadResourceL
// give ownership to caller
// -----------------------------------------------------------------------------
// 
HBufC* ServiceWidgetUtils::LoadResourceLC( 
								TInt aResourceId ,
								TInt aInt ,
								CCoeEnv* aEnv /*= NULL*/)
    {
    HBufC* ret = NULL;
    if(aEnv)
        {
        ret = StringLoader::LoadLC( aResourceId, aInt );
        }
     return ret;
	}	
 // -----------------------------------------------------------------------------
// ServiceWidgetUtils::LoadResourceL
// give ownership to caller
// -----------------------------------------------------------------------------
// 
HBufC* ServiceWidgetUtils::LoadResourceLC( 
						TInt aResourceId ,
						const CArrayFix<TInt>& aInts ,
						CCoeEnv* aEnv /*= NULL*/ )
    {
    HBufC* ret = NULL;
    if(aEnv)
        {
        ret = StringLoader::LoadLC( aResourceId, aInts );
        }
     return ret;
     }
     			
// End of file
