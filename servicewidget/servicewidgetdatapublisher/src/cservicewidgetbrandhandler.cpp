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
* Description:  brand status icons handler 
 *                
 *
*/

 
#include "cservicewidgetbrandhandler.h"

#include "servicewidgetcpglobals.h"
#include "swpdebugtrace.h"
// branding server
#include <cbsfactory.h>
#include <mbsaccess.h>
#include <cbsbitmap.h>
#include <fbs.h>
//icons
#include <gulicon.h>
#include <bautils.h>
#include <AknIconUtils.h>
#include <spsettings.h> 
#include <spentry.h>
#include <spproperty.h>

#include  "conversations.h"

_LIT8(KAppIdForBrand,"xsp");
_LIT8(KDefaultBrandIdForBrand,"xsp");

// Taken as conversation.h gile doesn't have it. 
_LIT8( KEMbmConversationsQgnDefaultImage,  "default_brand_image");

// ---------------------------------------------------------------------------
// CServiceWidgetBrandHandler::CServiceWidgetBrandHandler
// ---------------------------------------------------------------------------
// 
CServiceWidgetBrandHandler::CServiceWidgetBrandHandler(TInt aServiceId ):
	iAppId(KAppIdForBrand),
	iServiceId( aServiceId )
   {
   TRACE_SWP(TXT("CServiceWidgetBrandHandler::CServiceWidgetBrandHandler()") );
   }

// ---------------------------------------------------------------------------
// CServiceWidgetBrandHandler::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetBrandHandler* CServiceWidgetBrandHandler::NewL(TInt aServiceId )
    {
    TRACE_SWP(TXT("CServiceWidgetBrandHandler::NewL() start" ) );
    CServiceWidgetBrandHandler* self = NewLC(aServiceId);
    CleanupStack::Pop(self);
    TRACE_SWP(TXT("CServiceWidgetBrandHandler::NewL() end" ) );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetBrandHandler::NewLC
// ---------------------------------------------------------------------------
// 
CServiceWidgetBrandHandler* CServiceWidgetBrandHandler::NewLC(TInt aServiceId)
    {
    TRACE_SWP(TXT("CServiceWidgetBrandHandler::NewLC() start" ) );
    CServiceWidgetBrandHandler* self =
        new (ELeave) CServiceWidgetBrandHandler(aServiceId);
    CleanupStack::PushL(self);
    self->ConstructL();
    TRACE_SWP(TXT("CServiceWidgetBrandHandler::NewLC() end" ) );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetBrandHandler::ConstructL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetBrandHandler::ConstructL()
    {
	iBrandId = HBufC8::NewL( KSW_RESOURCE_MAX_LENGTH );
    TPtr8 brandIdPtr( iBrandId->Des() );    
    TInt version = 1 ; // default
    
    iLanguageId = ELangInternationalEnglish; 
    GetBrandInfoL(iLanguageId, version, brandIdPtr);	
   
   	iBrandingFactory = CBSFactory::NewL( KDefaultBrandIdForBrand, iAppId ); 
	
	// Leaves if no brand for service, therefore trap
    PrepareBrandingAccess();
    
    LoadBrandingBitmapsL();
    }
 
 // ---------------------------------------------------------
// CServiceWidgetBrandHandler::GetBrandInfo
// 
// ---------------------------------------------------------
TInt CServiceWidgetBrandHandler::GetBrandInfoL(TLanguage& aBrandLanguage, 
				TInt &aBrandVersion, TDes8& aBrandId)

	{
	CSPSettings* settings = CSPSettings::NewLC();
	 
	////////////// brand id/////////////////////
    CSPProperty* property = CSPProperty::NewLC();
    settings->FindPropertyL( iServiceId,
                                EPropertyBrandId,
                                *property );    
    if ( property )
        {
        HBufC* brandId = HBufC::NewLC( 100 );
    	TPtr brandIdPtr( brandId->Des() ); 
        property->GetValue( brandIdPtr );
        aBrandId.Copy(brandIdPtr);
        CleanupStack::PopAndDestroy( brandId );
        }
    CleanupStack::PopAndDestroy( property ); 
    
    /////////// langauge //////////////////
    TInt language = ELangInternationalEnglish; 
    property = CSPProperty::NewLC();
    settings->FindPropertyL( iServiceId,
                              EPropertyBrandLanguage,
                              *property );    
    if ( property )
        {
        property->GetValue( language );
        }
    CleanupStack::PopAndDestroy(property);
   	aBrandLanguage = ( TLanguage) language;
   	
   	/////////// version ////////////////
	property = CSPProperty::NewLC();
    settings->FindPropertyL( iServiceId,
                              EPropertyBrandVersion,
                              *property );    
    if ( property )
        {
        property->GetValue( aBrandVersion );
        }
    CleanupStack::PopAndDestroy( property );
   	//////////////////////////////////////
   	CleanupStack::PopAndDestroy( settings ); 
 	return KErrNone;
	}   

// ---------------------------------------------------------------------------
// CServiceWidgetBrandHandler::PrepareBrandingAccess()
// ---------------------------------------------------------------------------
// 
TInt CServiceWidgetBrandHandler::PrepareBrandingAccess()
    {
    TRAPD(err, iBrandingAccess = iBrandingFactory->CreateAccessL( *iBrandId, iLanguageId ));
    return err;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetBrandHandler::~CServiceWidgetBrandHandler
// ---------------------------------------------------------------------------
// 
CServiceWidgetBrandHandler::~CServiceWidgetBrandHandler()
    {
    TRACE_SWP(TXT("CServiceWidgetBrandHandler::~CServiceWidgetBrandHandler() start" ) );

    iBrandIcons.ResetAndDestroy();
    if(iBrandingAccess)
        {
        iBrandingAccess->Close();
        }
    delete iBrandingFactory;
    delete iBrandId;
    TRACE_SWP(TXT("CServiceWidgetBrandHandler::~CServiceWidgetBrandHandler() end" ) );
    }

// -----------------------------------------------------------------------------
// CServiceWidgetBrandHandler::LoadBrandBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void CServiceWidgetBrandHandler::LoadBrandingBitmapsL()
    {
    CGulIcon* gulIcon = NULL ;
    //Fetching bitmaps from the server. 
    if(!iBrandingAccess)
        {
        User::LeaveIfError(PrepareBrandingAccess());    
        }
    
    // EWSonline has to be at 0th in enums
    for(TInt index  = EWSOnline; index < EWSLast; index++ )
        {
        gulIcon = LoadBitmapL(index) ;
        iBrandIcons.Append(gulIcon) ;
        }
    }

CGulIcon* CServiceWidgetBrandHandler::LoadBitmapL(TInt aIndex)
    {
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    CGulIcon* gulIcon = NULL ;

    switch(aIndex)
        {
        case EWSOnline:
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimfriendon, bitmap, mask);
            break;
        case EWSAway:
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimfriendaway, bitmap, mask);
            break;
        case EWSbusy:
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimfriendbusy, bitmap, mask);
            break;
        case EWSDoNotDisturb:
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimfriendbusy, bitmap, mask);
            break;            
        case EWSOffline:    
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimfriendoff, bitmap, mask);
            break;
        case EWSOnPhone:
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimfriendonmobile, bitmap, mask);
            break;
        case EWSBlocked:
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimuserblocked, bitmap, mask);
            break;
        case EWSNewMessage:
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimimsgnew, bitmap, mask);
            break;
        case EWSServiceIcon:
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnDefaultImage, bitmap, mask);
            break;
        case EWSInvisible: // show offline icon for invisible state
            iBrandingAccess->GetBitmapL(KEMbmConversationsQgnpropimfriendoff, bitmap, mask);
            break;
        }
        
        if(bitmap && mask)
            {
            CleanupStack::PushL(bitmap);
            CleanupStack::PushL(mask);
            // to solve the handle zero issue
            TSize size ;
            AknIconUtils::GetContentDimensions(bitmap,  size);
            if ( aIndex == EWSServiceIcon )
                {
                size.iWidth = 200;
                size.iHeight = 200;
                }
            AknIconUtils::SetSize(bitmap,  size);
            gulIcon = CGulIcon::NewL(bitmap, mask);
            gulIcon->SetBitmapsOwnedExternally(EFalse) ;// give ownership to gulicon
            CleanupStack::Pop(mask);
            CleanupStack::Pop(bitmap);
            }
        return gulIcon ;
    }
// -----------------------------------------------------------------------------
// CServiceWidgetBrandHandler::GetStatusBitmapPath
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CServiceWidgetBrandHandler::GetStatusBitmapL(TWSPresenceStatus aIndex, TInt& aBitmapHandle, TInt& aMaskHandle)
    {
    CGulIcon *gulIcon = NULL ;
    
    if(aIndex < EWSOnline || aIndex >= EWSLast)
        return ;
    
    gulIcon = iBrandIcons[aIndex] ;
    if(gulIcon)
        {
            aBitmapHandle = gulIcon->Bitmap()->Handle();
            aMaskHandle = gulIcon->Mask()->Handle();
        }
    }
 // end of file
