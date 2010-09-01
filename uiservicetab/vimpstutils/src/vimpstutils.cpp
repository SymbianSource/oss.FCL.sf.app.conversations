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
* Description:  Utils for IMUiServiceTab modules.
*
*/


// INCLUDES
#include	"vimpstutils.h"
#include "StringLoader.h"
//#include "vimpstdebugprint.h" //TODO: for later use
#include <coemain.h>
#include "uiservicetabtracer.h"

// CONSTANTS
// TODO: For later use
// general colon needed in various places
//_LIT( KColon, ":" );
// general slash needed in various places
//_LIT( KSlash, "/" );
// general slash needed in various places
_LIT( KAt, "@" );



// -----------------------------------------------------------------------------
// VIMPSTUtils::DisplayId
// -----------------------------------------------------------------------------
//
EXPORT_C TPtrC VIMPSTUtils::DisplayId( const TDesC& aId, TBool /*aListHiding*/ )
    {
	TRACER_AUTO;
    TPtrC ret( aId );
    
    //TODO: Modify the function body when it is required later 
   
    
    /*MCAStoredContacts* contacts = NULL;
    TRAPD( err, contacts = CCAStorageManagerFactory::ContactListInterfaceL() );
    if ( err != KErrNone || !contacts )
        {
        // some error, don't modify
        CHAT_DP_FUNC_DP("DisplayId", "Got some error, not hiding");
        return ret;
        }

    // if we've branded the feature out, don't modify anything
    if ( contacts->WVHiding() )
        {
        // locate ":" for userid, groupid.
        // locate "/" for list id.
        TInt pos = aId.F//TODO : Modify the function body when required laterindC( aListHiding ? KSlash : KColon );

        if ( ( pos != KErrNotFound) && ( pos != aId.Length()-1) )
            {
            // contains the special character, and it is not the last char
            // remove everything before the special char (including the char)
            ret.Set( aId.Mid( pos + 1 ) );
            }
         */
        // remove also the domain part
        TInt domainPos = ret.FindC( KAt );
        if ( ( domainPos != KErrNotFound ) && ( domainPos != 0 ) )
            {
            ret.Set( ret.Mid( 0, domainPos ) );
            }
       // }
    
 
    return ret;
    }
 

// -----------------------------------------------------------------------------
// VIMPSTUtils::LoadResourceL
// give ownership to caller
// -----------------------------------------------------------------------------
// 
EXPORT_C HBufC* VIMPSTUtils::LoadResourceL( TInt aResourceId )
	{
	TRACER_AUTO;
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

// -----------------------------------------------------------------------------
// VIMPSTUtils::LoadResourceL
// give ownership to caller
// -----------------------------------------------------------------------------
// 
EXPORT_C HBufC* VIMPSTUtils::LoadResourceL( TInt aResourceId , const TDesC& aPrompt)
    {
	TRACER_AUTO;
    CCoeEnv* env = CCoeEnv::Static();
    HBufC* ret = NULL;
    if(env)
        {
        ret = StringLoader::LoadL(aResourceId,aPrompt);
        return ret;
        }
    else
        {
        return ret;
        }

    }

// ---------------------------------------------------------
// VIMPSTUtils::CombineStringFromResourceLC()
// See header for details.
// ---------------------------------------------------------
//
EXPORT_C HBufC* VIMPSTUtils::CombineStringFromResourceLC( TInt aResourceId1, TInt aResourceId2 )
	{
	TRACER_AUTO;
	// empty texts
    _LIT ( KPSUIEmptyStringHeader, "%S\n%S" );
    HBufC* emptyText = CCoeEnv::Static()->AllocReadResourceLC( 
        aResourceId1 );
    
    HBufC* emptyText2 = CCoeEnv::Static()->AllocReadResourceLC( 
        aResourceId2 );  
    
    HBufC* emptyFinal = HBufC::NewLC( 
        emptyText->Length() + emptyText2->Length() + 2 );// 2 is length of "\n" 
        
    CleanupStack::Pop(emptyFinal);
        
    emptyFinal->Des().Format( KPSUIEmptyStringHeader, emptyText, emptyText2 );
    
    CleanupStack::PopAndDestroy( 2 );// emptyText, emptyText2
    
    CleanupStack::PushL(emptyFinal);
    
    return emptyFinal; 
          	
	}


// End of file
