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
* Description:  Implementation of detailsview contact brand data
 *
*/


// INCLUDE FILES
#include	"cvimpstdetailsviewbrandhandler.h"

#include 	"vimpstdetailsview.hrh"
#include 	"vimpstui.h"
#include    "conversations.h"
#include 	"tvimpstconsts.h"
#include 	<vimpstui.mbg>
#include    <conversations.mbg>
// branding server
#include 	<cbsfactory.h>
#include 	<mbsaccess.h>
#include 	<mbselement.h>
// system includes
#include    <AknIconUtils.h>
// settings
#include 	<spsettings.h>
#include 	<spentry.h>
#include 	<spproperty.h>

#include    <cch.h>
#include "vimpstdebugtrace.h"

_LIT8(KAppIdForBrand,"xsp");
_LIT8(KDefaultBrandIdForBrand,"xsp");

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::CVIMPSTDetailsViewBrandHandler()
// ---------------------------------------------------------------------------
// Two-phased constructor.
CVIMPSTDetailsViewBrandHandler* CVIMPSTDetailsViewBrandHandler::NewL(TInt aServiceId )
    {
    CVIMPSTDetailsViewBrandHandler* self = new (ELeave) CVIMPSTDetailsViewBrandHandler( aServiceId);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::~CVIMPSTDetailsViewBrandHandler()
// ---------------------------------------------------------------------------
//  Destructor
CVIMPSTDetailsViewBrandHandler::~CVIMPSTDetailsViewBrandHandler()
    {
     if(iBrandingAccess)
    	{
        iBrandingAccess->Close();
    	}
    delete iBrandingFactory;
    iBrandingFactory = NULL; 
   	delete iBrandId;
   	delete iSettings;
    delete iServiceStoreUri;
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::CVIMPSTDetailsViewBrandHandler()
// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
//
CVIMPSTDetailsViewBrandHandler::CVIMPSTDetailsViewBrandHandler(TInt aServiceId) :
	iAppId(KAppIdForBrand),
	iServiceId( aServiceId)
    {
    iBrandingAccess = NULL;
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::ConstructL()
// ---------------------------------------------------------------------------
//  Symbian OS default constructor can leave.
void CVIMPSTDetailsViewBrandHandler::ConstructL()
	{
	
	iBrandId = HBufC8::NewL( KVIMPSTUISPSMaxPropertyLength );
    TPtr8 brandIdPtr( iBrandId->Des() );    
    
    TInt version = 1 ; // default
    
    iSettings = CSPSettings::NewL();
    
    // ELangInternationalEnglish is selected as Default Brand Language Id.
    iLanguageId = ELangInternationalEnglish;  
    	
    GetBrandInfoL(iLanguageId, version, brandIdPtr);	
    
	iBrandingFactory = CBSFactory::NewL( KDefaultBrandIdForBrand, iAppId ); 
	
	
	//Get the default MBM path
    Dll::FileName(iDefaultMbmPath); //get the location of the dll
    
    TDriveName drive( TParsePtrC( iDefaultMbmPath ).Drive( ) ); // solve drive

    // leave to stack, LC method
    HBufC* resourceFilePath = HBufC::NewLC( KMaxResourcePathLength );
    TPtr ptr( resourceFilePath->Des() );
    ptr.Append( KDefaultIconFile() ); // without drive
    ptr.Insert( 0, drive ); // insert drive

    // NearestLanguageFile takes only TFileName
    iDefaultMbmPath.Zero();
    iDefaultMbmPath.Append( *resourceFilePath );

	CleanupStack::PopAndDestroy();//resourceFilePath
	
	// Leaves if no brand for service, therefore trap
    PrepareBrandingAccess();
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::PrepareBrandingAccess()
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTDetailsViewBrandHandler::PrepareBrandingAccess()
    {
    
    TRAPD(err, iBrandingAccess = iBrandingFactory->CreateAccessL( *iBrandId, iLanguageId ));
    
    return err;
    }


// ---------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::LoadIconsLC
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTDetailsViewBrandHandler::LoadIconsLC( )
    {
    //Fetching bitmaps from the server.
    if( !iBrandingAccess )
	    {
	    if( KErrNone != PrepareBrandingAccess() )
	    //brand was not found so load the default data
		    {
		    return 	LoadDefaultIconsLC(iDefaultMbmPath);
		    }	    	    
	    }
	//brand data is available
	return LoadBrandIconsLC();		    
    }

// ---------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::LoadBitMapL
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTDetailsViewBrandHandler::LoadBitMapLC(CFbsBitmap*& aBitmap,
									            CFbsBitmap*& aMask,
									            TInt aBitmapId,
									            TInt aMaskId  )
    {
    if( !iBrandingAccess )
	    {
	    if( KErrNone != PrepareBrandingAccess() )
	    //brand was not found so load the default data
		    {
		    AknIconUtils::CreateIconLC( aBitmap,aMask, iDefaultMbmPath, aBitmapId, aMaskId  );
		    return;
		    }	    	    
	    }
	//brand data is available
	 CGulIcon* icon = NULL;
	 icon = LoadBrandBitmapL( KEMbmConversationsQgnpropservicedefaultavatar );	
     aBitmap = icon->Bitmap();
     aMask = icon->Mask();	
     // this is remove the memory leak
     icon->SetBitmapsOwnedExternally( ETrue );
     delete icon ;     
     CleanupStack::PushL( aBitmap );
     CleanupStack::PushL( aMask );
     }

// ---------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::LoadBrandIconsLC
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTDetailsViewBrandHandler::LoadBrandIconsLC( )
    {
    // array granularity is 12, because 12 icons are added to array
    CAknIconArray* icons = new( ELeave )CAknIconArray( 12);
    CleanupStack::PushL( icons );
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendoff) );//0
    		
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendon) );//1
    
  	icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendaway ) );//2
	
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendbusy ) );//3

   	icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendinvisible ) );//4
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimimsg ) );//5
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimimsgnew ) );//6
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropservicedefaultavatar ) );//7
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropservicefriendrequestreceived ) );//8
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropservicefriendrequestsent ) );//9
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropserviceofffriendrequestreceived ) );//10
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropserviceofffriendrequestsent ) );//11
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimuserblocked ) );//12
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendonmobile ) );//13
    
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimusercallforward ) );//14
    
    return icons;
    }

// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::LoadBrandBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CGulIcon* CVIMPSTDetailsViewBrandHandler::LoadBrandBitmapL( const TDesC8& aId )
	{
    
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    
    //Fetching bitmaps from the server.
    if(!iBrandingAccess)
	    {
	    PrepareBrandingAccess();	
	    }
    iBrandingAccess->GetBitmapL( aId, bitmap, mask );
    CleanupStack::PushL( bitmap );
    CleanupStack::PushL( mask );
    //	bitmap, mask ownership passed to gulIcon	
    CGulIcon* icon = CGulIcon::NewL(bitmap, mask);
    icon->SetBitmapsOwnedExternally( EFalse );
	CleanupStack::Pop(2); // bitmap, mask 
    return icon;
    }


// ---------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::LoadDefaultIconsLC
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTDetailsViewBrandHandler::LoadDefaultIconsLC( const TDesC& aMbmFile)
    {
    // array granularity is 5
    CAknIconArray* icons = new( ELeave )CAknIconArray( 5);
    CleanupStack::PushL( icons );
			
    icons->AppendL( LoadDefaultBitmapL( // 0
    	EMbmVimpstuiQgn_prop_im_friend_off,
		EMbmVimpstuiQgn_prop_im_friend_off_mask,
		aMbmFile ) );
    icons->AppendL( LoadDefaultBitmapL( // 1
		EMbmVimpstuiQgn_prop_im_friend_on,
		EMbmVimpstuiQgn_prop_im_friend_on_mask,
		aMbmFile ) );
		
	icons->AppendL( LoadDefaultBitmapL( // 2
		EMbmVimpstuiQgn_prop_im_friend_away,
		EMbmVimpstuiQgn_prop_im_friend_away_mask,
		aMbmFile ) );
	icons->AppendL( LoadDefaultBitmapL( // 3
		EMbmVimpstuiQgn_prop_im_friend_busy,
		EMbmVimpstuiQgn_prop_im_friend_busy_mask,
		aMbmFile ) );
	icons->AppendL( LoadDefaultBitmapL( // 4
		EMbmVimpstuiQgn_prop_im_friend_invisible,
		EMbmVimpstuiQgn_prop_im_friend_invisible_mask,
		aMbmFile ) );
	icons->AppendL( LoadDefaultBitmapL( // 5
		EMbmVimpstuiQgn_prop_im_imsg,
		EMbmVimpstuiQgn_prop_im_imsg_mask,
		aMbmFile ) );
	icons->AppendL( LoadDefaultBitmapL( // 6
		EMbmVimpstuiQgn_prop_im_imsg_new,
		EMbmVimpstuiQgn_prop_im_imsg_new_mask,
		aMbmFile ) );	
	icons->AppendL( LoadDefaultBitmapL( // 7
	    EMbmVimpstuiQgn_prop_service_default_avatar,
	    EMbmVimpstuiQgn_prop_service_default_avatar_mask,
        aMbmFile ) );  
    icons->AppendL( LoadDefaultBitmapL( // 8
            EMbmVimpstuiQgn_prop_service_friend_request_received,
            EMbmVimpstuiQgn_prop_service_friend_request_received_mask,
            aMbmFile ) );  
    icons->AppendL( LoadDefaultBitmapL( // 9
            EMbmVimpstuiQgn_prop_service_friend_request_sent,
            EMbmVimpstuiQgn_prop_service_friend_request_sent_mask,
               aMbmFile ) );  
    icons->AppendL( LoadDefaultBitmapL( // 10
            EMbmVimpstuiQgn_prop_service_off_friend_request_received,
            EMbmVimpstuiQgn_prop_service_off_friend_request_received_mask,
            aMbmFile ) );  
    icons->AppendL( LoadDefaultBitmapL( // 11
            EMbmVimpstuiQgn_prop_service_off_friend_request_sent,
            EMbmVimpstuiQgn_prop_service_off_friend_request_sent_mask,
               aMbmFile ) );    
    icons->AppendL( LoadDefaultBitmapL( // 12
            EMbmVimpstuiQgn_servtab_avatar_blocked,
            EMbmVimpstuiQgn_servtab_avatar_blocked_mask,
                   aMbmFile ) ); //12   

    icons->AppendL( LoadDefaultBitmapL( // 13
            EMbmVimpstuiQgn_prop_im_friend_on_mobile,
            EMbmVimpstuiQgn_prop_im_friend_on_mobile_mask,
               aMbmFile ) );    
    
    return icons;
    }

// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::LoadDefaultBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CGulIcon* CVIMPSTDetailsViewBrandHandler::LoadDefaultBitmapL( TInt aBitmapId, TInt aMaskId, 
                                      const TDesC& aFullPath )
	{
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    AknIconUtils::CreateIconLC( bitmap,mask, aFullPath,aBitmapId,aMaskId  ); 
    //	bitmap, mask ownership passed to gulIcon				  
    CGulIcon* gulIcon = CGulIcon::NewL( bitmap, mask );
    CleanupStack::Pop( 2 ); // bitmap, mask
    return gulIcon;
    }

 // ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::PropertyBrandVersionL()
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewBrandHandler::PropertyReadBrandL( TUint32 aServiceId ,TInt &aBrand, TServicePropertyName aPropertyName ) 
    {
    CSPProperty* property = CSPProperty::NewLC();
    iSettings->FindPropertyL( aServiceId,
                                aPropertyName,
                                *property );    
    if ( property )
        {
        property->GetValue( aBrand );
        }
    CleanupStack::PopAndDestroy( property );
   
    }

// ---------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::GetBrandInfo
// 
// ---------------------------------------------------------
TInt CVIMPSTDetailsViewBrandHandler::GetBrandInfoL(TLanguage& aBrandLanguage, 
				TInt &aBrandVersion, TDes8& aBrandId)

	{

    CSPProperty* property = CSPProperty::NewLC();
    iSettings->FindPropertyL( iServiceId,
                                EPropertyBrandId,
                                *property );    
    if ( property )
        {
        HBufC* brandId = HBufC::NewLC( KVIMPSTUISPSMaxPropertyLength );
    	TPtr brandIdPtr( brandId->Des() ); 
        property->GetValue( brandIdPtr );
        aBrandId.Copy(brandIdPtr);
        CleanupStack::PopAndDestroy( brandId );
        }
    CleanupStack::PopAndDestroy( property ); 
    
    TInt language = ELangInternationalEnglish; 
   	PropertyReadBrandL( iServiceId, language , EPropertyBrandLanguage );
   	aBrandLanguage = ( TLanguage) language;
   	
	PropertyReadBrandL( iServiceId, aBrandVersion , EPropertyBrandVersion) ;
	
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::GetServiceNameL()
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewBrandHandler::GetServiceNameL( TDes& aServiceName )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    TRAPD( err, iSettings->FindEntryL( iServiceId, *entry ) );
    if ( !err )
        {
        aServiceName = entry->GetServiceName();
        }
    CleanupStack::PopAndDestroy( entry );    
    }
 
 // ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::ServiceStoreUriL()
// ---------------------------------------------------------------------------
//
const TDesC& CVIMPSTDetailsViewBrandHandler::ServiceStoreUriL()
    {
    if( !iServiceStoreUri )
        {       
        iServiceStoreUri = HBufC::NewL( KVIMPST_MAX_LENGTH );       
        TPtr  serviceStorePtr( iServiceStoreUri->Des() );

        CSPProperty* property = CSPProperty::NewLC();
        iSettings->FindPropertyL( iServiceId,
                EPropertyContactStoreId,
                *property ); 

        property->GetValue( serviceStorePtr );
        CleanupStack::PopAndDestroy( property );
         }
    return *iServiceStoreUri;
    } 
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewBrandHandler::ServiceSupportedL( )
// ---------------------------------------------------------------------------
//
TBool CVIMPSTDetailsViewBrandHandler::ServiceSupportedL( )
    {
    TBool ret = EFalse;
    CCch* cchClient = CCch::NewLC();
    if (cchClient)
        {       
        CCchService* service = cchClient->GetService( iServiceId ); 
        TCchServiceStatus status;
        status.SetState(ECCHUninitialized);
        TInt err = service->GetStatus( TCCHSubserviceType(ECCHPresenceSub), status );
        if (KErrNone == err)
           {
           ret = ETrue; 
           }
        CleanupStack::PopAndDestroy(); // cchClient
        }
    return ret;
    }
//  End of File
