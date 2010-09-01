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
* Description:  Brand handler
*
*/


// INCLUDE FILES
#include	"cvimpstuibranddata.h"
#include    "conversations.h"
#include    <AknIconArray.h>
#include 	<cbsfactory.h>
#include 	<mbsaccess.h>
#include 	<mbselement.h>
#include 	<cbsbitmap.h>
#include "uiservicetabtracer.h"

#include    <AknIconUtils.h>
#include    <AknIconArray.h>
#include    <eikclbd.h>
#include    <bautils.h>

#include    <AknIconUtils.h>
#include    <AknsUtils.h> 

#include 	"mvimpstengine.h"
#include 	"tvimpstconsts.h"
#include 	<vimpstui.mbg>
#include    <conversations.mbg>
#include    <avkon.mbg>
#include    <StringLoader.h>
 

//App Id is used by MeCo and ece to access branding icons and this name should be generic to both applications, so it needs to be changed later.
_LIT8(KAppIdForBrand,"xsp");
_LIT8(KDefaultBrandIdForBrand,"xsp");
_LIT(KZDrive,"Z:\\resource\\");
_LIT(KCDrive,"C:\\resource\\");
_LIT(KTOU,"TOU.r");
_LIT(KUnderScore,"_");

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CVIMPSTUiBrandData* CVIMPSTUiBrandData::NewL(MVIMPSTEngine& aEngine)
    {
	TRACER_AUTO;
    CVIMPSTUiBrandData* self = new (ELeave) CVIMPSTUiBrandData;
    CleanupStack::PushL( self );
    self->ConstructL(aEngine);
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CVIMPSTUiBrandData::~CVIMPSTUiBrandData()
    {
    delete iIconsArray;
    if(iBrandingAccess)
    	{
        iBrandingAccess->Close();
    	}
    
    delete iBrandingFactory;
    iBrandingFactory = NULL; 
   
   	delete iBrandId;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CVIMPSTUiBrandData::CVIMPSTUiBrandData() :
iAppId(KAppIdForBrand)
    {
    }

// Symbian OS default constructor can leave.
void CVIMPSTUiBrandData::ConstructL(MVIMPSTEngine& aEngine)
	{
	TRACER_AUTO;
	
	iBrandId = HBufC8::NewL( 100 );
    TPtr8 brandIdPtr( iBrandId->Des() );    
    
    TInt version;
    
    aEngine.GetBrandInfoL(iLanguageId, version, brandIdPtr);	
    
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

	CleanupStack::PopAndDestroy();
	
	TRACE(" prepare branding access" );
	// Leaves if no brand for service, therefore trap
    PrepareBrandingAccess();

    
    LoadIconsL();// load the existing icons
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiBrandData::PrepareBrandingAccess()
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTUiBrandData::PrepareBrandingAccess()
    {
	TRACER_AUTO;
    TRAPD(err, iBrandingAccess = iBrandingFactory->CreateAccessL( *iBrandId, iLanguageId ));
    
    return err;
    }


// ---------------------------------------------------------
// CVIMPSTUiBrandData::LoadIconsLC
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTUiBrandData::LoadIconsLC( )
    {
	TRACER_AUTO;  
    CAknIconArray* copyArray =  new( ELeave )CAknIconArray( 4); 
    CleanupStack::PushL( copyArray );
    
    TInt count = iIconsArray->Count();
    CGulIcon* newIcon = NULL;
    CGulIcon* icon = NULL;
    
    for( TInt i=0; i<count;i++)
        {
        icon = (*iIconsArray)[i];
        // bitmap and mask are still valid in iIconsArray array 
        // because of SetBitmapsOwnedExternally
        newIcon = CGulIcon::NewL( icon->Bitmap(),icon->Mask() );
        newIcon->SetBitmapsOwnedExternally( ETrue );
        copyArray->AppendL( newIcon ); // takes the ownership of newIcon
        }
  
    return copyArray;	 // on cleanup stack	    
    }

// ---------------------------------------------------------
// CVIMPSTUiBrandData::LoadIconsL
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTUiBrandData::LoadIconsL()
    {
	TRACER_AUTO;     
    //brand data is available
    TRACE("do load" );     
    return LoadBrandIconsL();          
    }

// ---------------------------------------------------------
// CVIMPSTUiBrandData::LoadBrandIconsLC
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTUiBrandData::LoadBrandIconsL( )
    {
	TRACER_AUTO;    
    delete iIconsArray;
    iIconsArray = NULL;
    
    iIconsArray  = new( ELeave )CAknIconArray( 4);
    // array granularity is 3, because 3 icons are added to array
    
    TRACE(" KEMbmConversationsQgnpropimfriendoff" );	    	
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendoff) );//0
    		
    TRACE("KEMbmConversationsQgnpropimfriendon" );	    			
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendon) );//1
    
    TRACE(" KEMbmConversationsQgnpropimfriendaway" );	    			
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendaway ) );//2
	
    TRACE("KEMbmConversationsQgnpropimfriendbusy" );	    			
	iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendbusy ) );//3

	TRACE(" KEMbmConversationsQgnpropimfriendinvisible" );	    			
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendinvisible ) );//4
    
    TRACE(" KEMbmConversationsQgnpropimimsg" );	    			
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimimsg ) );//5
    
    TRACE("  KEMbmConversationsQgnpropimimsgnew" );	    			
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimimsgnew ) );//6
    
    TRACE("  KEMbmConversationsQgnpropservicedefaultavatar" );	    			
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropservicedefaultavatar ) );//7
    
    TRACE("  KEMbmConversationsQgnpropservicefriendrequestreceived" );	    			
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropservicefriendrequestreceived ) );//8
    
    TRACE("  KEMbmConversationsQgnpropservicefriendrequestsent" );	    			
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropservicefriendrequestsent ) );//9
    
    TRACE(" KEMbmConversationsQgnpropserviceofffriendrequestreceived" );                  
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropserviceofffriendrequestreceived ) );//10
    
    TRACE("  KEMbmConversationsQgnpropserviceofffriendrequestsent" );                  
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropserviceofffriendrequestsent ) );//11

    TRACE("  KEMbmConversationsQgnpropimfriendonmobile" );                  
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendonmobile ) );//12
    
    TRACE(" KEMbmConversationsQgnservtabavatarblocked" );                  
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimuserblocked ) );//13
    
    TRACE("  KEMbmConversationsQgnservtabavatarblocked" );                  
    iIconsArray->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimusercallforward ) );//14

        
    return iIconsArray;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiBrandData::LoadBrandBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CGulIcon* CVIMPSTUiBrandData::LoadBrandBitmapL( const TDesC8& aId )
	{
	TRACER_AUTO;    	    
    
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    
    //EASY WAY TO DO THINGS.. :) 
    //Fetching bitmaps from the server.
    if(!iBrandingAccess)
	    {
    TRACE("create access" );	    
	    PrepareBrandingAccess();	
	    }
    TRAPD(err_bitmap, iBrandingAccess->GetBitmapL( aId, bitmap, mask ));
    if(KErrNone != err_bitmap)
    	{
    TRACE(" err" );	    
        User::Leave( err_bitmap );
    	}
    TRACE("create icon" );	    	
    CGulIcon* icon = CGulIcon::NewL(bitmap, mask);
    icon->SetBitmapsOwnedExternally( EFalse );
    TRACE(" return icon" );	    	
    return icon;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiBrandData::LoadDefaultBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CGulIcon* CVIMPSTUiBrandData::LoadDefaultBitmapL( TInt aBitmapId, TInt aMaskId, 
                                      const TDesC& aFullPath )
	{
	TRACER_AUTO;       
    
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    AknIconUtils::CreateIconLC( bitmap,mask, aFullPath,aBitmapId,aMaskId  ); 					  
    CGulIcon* gulIcon = CGulIcon::NewL( bitmap, mask );
    CleanupStack::Pop( 2 ); // bitmap, mask
    return gulIcon;
    }
    
// ---------------------------------------------------------
// CVIMPSTUiBrandData::LoadOwnStatusIconsLC
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTUiBrandData::LoadOwnStatusIconsLC( )
    {
	TRACER_AUTO;   
    //EASY WAY TO DO THINGS.. :) 
    //Fetching bitmaps from the server.
    if(!iBrandingAccess)
	    {
	    if (KErrNone != PrepareBrandingAccess())
	    //brand was not found so load the default data
		    {
		    return 	LoadDefaultOwnStatusIconsLC(iDefaultMbmPath);
		    }	    	    
	    }
	//brand data is available
	return LoadBrandOwnStatusIconsLC();		    
    }
// ---------------------------------------------------------
// CVIMPSTUiBrandData::LoadBrandOwnStatusIconsLC
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTUiBrandData::LoadBrandOwnStatusIconsLC( )
    {
	TRACER_AUTO;   
    // array granularity is 3, because 3 icons are added to array
    CAknIconArray* icons = new( ELeave )CAknIconArray( 5);
    CleanupStack::PushL( icons );
	
	// online
	icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendon) );//0
	
	//away		
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendaway) );//1
    
     //busy
    icons->AppendL( LoadBrandBitmapL( KEMbmConversationsQgnpropimfriendbusy) );//2

    //invisible	
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendinvisible) );//3

     //invisible	
    icons->AppendL( LoadBrandBitmapL(KEMbmConversationsQgnpropimfriendoff) );//4
   
    return icons;
    }
    
 // ---------------------------------------------------------
// CVIMPSTUiBrandData::LoadDefaultOwnStatusIconsLC
// Loads necessary icons
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTUiBrandData::LoadDefaultOwnStatusIconsLC( const TDesC& aMbmFile)
    {
	TRACER_AUTO;   
    // array granularity is 4, because 4 icons are added to array
    CAknIconArray* icons = new( ELeave )CAknIconArray( 5);
    CleanupStack::PushL( icons );
			
   
    icons->AppendL( LoadDefaultBitmapL( // 0
		EMbmVimpstuiQgn_prop_im_friend_on,
		EMbmVimpstuiQgn_prop_im_friend_on_mask,
		aMbmFile ) );
	icons->AppendL( LoadDefaultBitmapL( // 1
		EMbmVimpstuiQgn_prop_im_friend_away,
		EMbmVimpstuiQgn_prop_im_friend_away_mask,
		aMbmFile ) );
	icons->AppendL( LoadDefaultBitmapL( // 2
		EMbmVimpstuiQgn_prop_im_friend_busy,
		EMbmVimpstuiQgn_prop_im_friend_busy_mask,
		aMbmFile ) );
	icons->AppendL( LoadDefaultBitmapL( // 3
		EMbmVimpstuiQgn_prop_im_friend_invisible,
		EMbmVimpstuiQgn_prop_im_friend_invisible_mask,
		aMbmFile ) );
	 icons->AppendL( LoadDefaultBitmapL( // 4
    	EMbmVimpstuiQgn_prop_im_friend_off,
		EMbmVimpstuiQgn_prop_im_friend_off_mask,
		aMbmFile ) );
    return icons;
    }
// ---------------------------------------------------------
// CVIMPSTUiBrandData::GetTitlePaneBrandBitmapL
// (other items were commented in a header).
// ---------------------------------------------------------
//
   
TInt CVIMPSTUiBrandData::GetBrandBitmapsL( CFbsBitmap **aBitmap, CFbsBitmap **aMask)
    {
	TRACER_AUTO;   
    	    
    TInt error =KErrNone;
    if ( !iBrandingAccess && KErrNone != PrepareBrandingAccess())
        {
          //load default icon              
          AknIconUtils::CreateIconLC( *aBitmap, *aMask,
                  iDefaultMbmPath, EMbmVimpstuiQgn_prop_im_friend_off, 
                  EMbmVimpstuiQgn_prop_im_friend_off_mask );         
          CleanupStack::Pop(2);//aBitmap,aMask.
          }
      else
          {
          TRAP(error, iBrandingAccess->GetBitmapL( KEMbmConversationsQgnpropimfriendon, *aBitmap, *aMask ));
          }
    TRACE("returns: %d", error );	          
    return error;
    }
    
// ---------------------------------------------------------
// ---------------------------------------------------------
// CVIMPSTUiBrandData::GetToUBrandedStringL
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC* CVIMPSTUiBrandData::GetToUBrandedStringL()
	{
	TRACER_AUTO;   
	HBufC* prompt= NULL;
	if(iBrandingAccess || KErrNone == PrepareBrandingAccess())
		{
		TRAP_IGNORE(prompt = iBrandingAccess->GetTextL( KToumessage ));
		}
	return prompt;
	}

// ---------------------------------------------------------
// CVIMPSTUiBrandData::IsBrandingSupported
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CVIMPSTUiBrandData::IsBrandingSupported()
    {
    TBool ret = EFalse;
    if(iBrandingAccess)
        {
        ret = ETrue;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiBrandData::GetToUStringL()
// -----------------------------------------------------------------------------
//
HBufC* CVIMPSTUiBrandData::GetToUStringL()
    {
	TRACER_AUTO;   
    HBufC* TouDes = NULL; 
    const TInt KLangBufLength = 2;
    const TInt KLeadingZero = 0;
    TBuf<KLangBufLength> langBuf;

    // To get two or more digit language id.
    if( iLanguageId < 10 )
        {
        langBuf.AppendNum( KLeadingZero );
        }
    langBuf.AppendNum( iLanguageId );
   
    // Number 2 is used in the below equation because
    // it is multiplied with the length of the underscore
    // to calculate the over all length of the string.
    TInt length = KCDrive().Length() + iAppId.Length() + iBrandId->Length()
                    + KTOU().Length() + langBuf.Length() + 2 * KUnderScore().Length();

    HBufC* fileName = HBufC::NewLC( length );
    TPtr brandFileName( fileName->Des() );
    
    // Converting the HBufC8 iAppId to HBufC appId
    HBufC* appId = HBufC::NewL( iAppId.Length() );
    appId->Des().Copy( iAppId );
    
    // Converting the HBufC8 iBrandId to HBufC brandId
    HBufC* brandId = HBufC::NewL( iBrandId->Length() );
    brandId->Des().Copy( *iBrandId );
    
    //Formation of filename.
    brandFileName.Append( *appId );
    brandFileName.Append( KUnderScore );
    brandFileName.Append( *brandId );
    brandFileName.Append( KUnderScore );
    brandFileName.Append( KTOU );
    brandFileName.Append( langBuf );

    TFileName temp(KCDrive);
    
    HBufC* targetFile = ConcatLC( temp, brandFileName );
    
    // for getting the access to the file.
    RFs fsSession; 
    RFile file; 
    User::LeaveIfError(fsSession.Connect()); 
    
    TFileName fullPtr( targetFile->Des() );
    BaflUtils::NearestLanguageFile( fsSession, fullPtr );
    TInt err = file.Open(fsSession, fullPtr, EFileRead); 
    CleanupStack::PopAndDestroy(); // targetFile
    
    if( KErrNone != err )
        {
        temp = KZDrive;
        targetFile = ConcatLC( temp, brandFileName );
        //brandFileName = targetFile->Des();
        fullPtr = targetFile->Des();
        BaflUtils::NearestLanguageFile( fsSession, fullPtr );
        err = file.Open(fsSession, fullPtr, EFileRead);
        CleanupStack::PopAndDestroy(); // targetFile
        }

    // Calculates the size of the file.
    if( !err )
        {
        err = file.Size( length );
        }

    if( ! err )
        {
        HBufC8* readBuf = HBufC8::NewL( length );
        TPtr8 readBufPtr = readBuf->Des();
        file.Read( readBufPtr );
        TouDes = HBufC::NewL( readBuf->Length() );
        TouDes->Des().Copy( *readBuf );
        delete readBuf;
        }
    file.Close();
    fsSession.Close();
    CleanupStack::PopAndDestroy(); // fileName
    delete appId;
    delete brandId;
    return TouDes;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiBrandData::GetToUStringL()
// -----------------------------------------------------------------------------
//
HBufC* CVIMPSTUiBrandData::ConcatLC( TDesC& aDesc1, TDesC& aDesc2 )
   {
	TRACER_AUTO;   
   HBufC* resultDesc = HBufC::NewLC( aDesc1.Length() + aDesc2.Length() );
   resultDesc->Des().Append( aDesc1 );
   resultDesc->Des().Append( aDesc2 );
   return resultDesc;
   }

//  End of File
