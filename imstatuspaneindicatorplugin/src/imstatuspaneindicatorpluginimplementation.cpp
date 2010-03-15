/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is the plugin implementation
*
*/


// INCLUDE FILES
#include <imstatuspaneindicatorpluginrss.rsg>
#include <StringLoader.h>
#include <bautils.h>
#include <f32file.h>
#include <e32base.h>
#include <barsread.h>
#include <coemain.h>
#include <eikenv.h>
#include <apgcli.h>
#include <avkon.hrh>

#include <xSPViewServices.h>
// imlauncher
#include <imcvlauncher.h>

//sp settings
#include <spsettings.h> 
#include <spentry.h>
#include <spproperty.h>

#include "imstatuspaneindicator.h"
#include "imstatuspaneindicatorpluginimplementation.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicatorPluginImplementation::CIMStatusPaneIndicatorPluginImplementation
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CIMStatusPaneIndicatorPluginImplementation::CIMStatusPaneIndicatorPluginImplementation()
: iCoeEnv( *CCoeEnv::Static() )
    {
    }

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicatorPluginImplementation::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CIMStatusPaneIndicatorPluginImplementation::ConstructL()
    {
    TFileName resPathWithoutDrive;
    resPathWithoutDrive.Zero();
    resPathWithoutDrive.Append( KResourceDir() );
    resPathWithoutDrive.Append( KIndicatorResourceFile() );
    
    TFileName path;    
    Dll::FileName(path); //get the location of the dll
    
    TDriveName drive( TParsePtrC( path ).Drive( ) ); // solve drive

    // leave to stack, LC method
    HBufC* resourceFilePath = HBufC::NewLC( KMaxResourcePathLength );
    TPtr ptr( resourceFilePath->Des() );
    ptr.Append( resPathWithoutDrive ); // without drive
    ptr.Insert( 0, drive ); // insert drive

    // NearestLanguageFile takes only TFileName
    path.Zero();
    path.Append( *resourceFilePath );

    if ( path.Length() != 0 )
        {
        BaflUtils::NearestLanguageFile( iCoeEnv.FsSession(), path );
        }
    
    CleanupStack::PopAndDestroy(resourceFilePath);

    iResourceOffset = iCoeEnv.AddResourceFileL(path);
     
    iPopupTextOne =  StringLoader::LoadLC( R_QTN_SERVTAB_UNIPANE_ONENEWMSG );
    CleanupStack::Pop();
    iPopupTextMultiple = StringLoader::LoadLC( R_QTN_SERVTAB_UNIPANE_NEWMSGS );
    CleanupStack::Pop();
    iIMStatusPaneIndicator = CIMStatusPaneIndicator::NewL();
    }

// -----------------------------------------------------------------------------
// CIMStatusPaneIndicatorPluginImplementation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMStatusPaneIndicatorPluginImplementation* CIMStatusPaneIndicatorPluginImplementation::NewL()
    {
    CIMStatusPaneIndicatorPluginImplementation* self = 
        new( ELeave ) CIMStatusPaneIndicatorPluginImplementation();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// Destructor
CIMStatusPaneIndicatorPluginImplementation::~CIMStatusPaneIndicatorPluginImplementation()
    {
    delete iIMStatusPaneIndicator;
    iCoeEnv.DeleteResourceFile( iResourceOffset );
    delete iPopupTextMultiple;
    delete iPopupTextOne;  
    iViewServices.Close();
    }


// ---------------------------------------------------------------------------
// CIMStatusPaneIndicatorPluginImplementation::HandleIndicatorTapL
// ---------------------------------------------------------------------------
//
void CIMStatusPaneIndicatorPluginImplementation::HandleIndicatorTapL( const TInt aUid)
    {
   if(EAknIndicatorMecoServiceTab == aUid)
       {
       ShowMessageL();
       }
    }

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicatorPluginImplementation::CreatePopupContentMessageL()
// ---------------------------------------------------------------------------
//
HBufC* CIMStatusPaneIndicatorPluginImplementation::CreatePopupContentMessageL()
    {
    if( !iPopupTextMultiple || !iPopupTextOne )
       {
       // do not allocate KNullDesC>AllocL() memory here, as the avakon component
       // donot test for the lenght of the text, instead it tests if
       // the text is valid or not.
       // Load resource strings before calling this method!
       return NULL;
       }
    
    HBufC* dynStringBuf = NULL;
    if(iIMStatusPaneIndicator)
        {
        TInt Count = iIMStatusPaneIndicator->MsgCount();
        if( 1 == Count )
           {
           dynStringBuf =  iPopupTextOne->AllocL();
           }
       else if(Count > 1)
           {
           TInt reqDynLen = iPopupTextMultiple->Length() + 
                                    KMaxMessageNumberLength;
           dynStringBuf = HBufC::NewL( reqDynLen );
           TPtr dynString = dynStringBuf->Des();
           StringLoader::Format( dynString,
                                iPopupTextMultiple->Des(),
                                -1,             //No index code in source string
                                Count );
           }
        }
   
      return dynStringBuf;
    }
 
// ---------------------------------------------------------------------------
// CIMStatusPaneIndicatorPluginImplementation::TextL()
// ---------------------------------------------------------------------------
//    
HBufC* CIMStatusPaneIndicatorPluginImplementation::TextL( const TInt aUid, TInt& aTextType )
	{
	if(EAknIndicatorMecoServiceTab == aUid)
	    {
	    // check the uid, if its only my uid then call the functionality below.
        aTextType = CAknIndicatorPlugin::EAknIndicatorPluginLinkText;
        return CreatePopupContentMessageL();
	    }
	return NULL;
	}
// ---------------------------------------------------------------------------
// CIMStatusPaneIndicatorPluginImplementation::ShowMessageL()
// ---------------------------------------------------------------------------
// 
void CIMStatusPaneIndicatorPluginImplementation::ShowMessageL()
	{
    if(iIMStatusPaneIndicator)
        {
        // conversation view should be launched if is 1 message or if more than
        // one message, but the sender is same of all the messages.
        if(iIMStatusPaneIndicator->MsgCount() >= 1 && iIMStatusPaneIndicator->IsMultipleSenders() == 1 )
            {
            TVwsViewId activeViewId;
            CCoeEnv::Static()->AppUi()->GetActiveViewId( activeViewId );
            // launch the conversation view
            // imlauncher call for luanching the conversationview with seletced user id
            IMCVLauncher::LaunchImConversationViewL(activeViewId, iIMStatusPaneIndicator->ServiceId(), iIMStatusPaneIndicator->SenderId());  
            }
        // servie tab is launched when there are more than one messages received from different
        // senders.
        else if(iIMStatusPaneIndicator->MsgCount() > 1 )
            {
            //launch service tab based on the service id.
            TInt serviceviewId = 0 ;
            TInt seriveId = iIMStatusPaneIndicator->ServiceId();
            if(  seriveId != KErrNotFound)
                {
                CSPSettings* spSettings = CSPSettings::NewL();
                CleanupStack::PushL(spSettings);
                //get the tabview id from settings
                CSPProperty* property = CSPProperty::NewLC();
                spSettings->FindPropertyL( seriveId,
                        EPropertyContactViewId,
                        *property );    
                property->GetValue(serviceviewId);
                CleanupStack::PopAndDestroy( property );  
                CleanupStack::PopAndDestroy( spSettings );
                }
            // Launched from service tab, go back there
            TInt err = iViewServices.Open();
            if ( KErrServerTerminated == err || KErrNotFound == err )
                {
                iViewServices.Close();
                err = iViewServices.Open();
                }
            // this error comes when the phonebook is already open, hence we should directly launch the 
            // service tab.
            if(KErrAlreadyExists != err)
                {
                User::LeaveIfError( err );
                }
            // Activate the service tab in phonebook.
            User::LeaveIfError( iViewServices.Activate( 
                    KMECOIMPLEMENTATIONUID,
                    serviceviewId ) );
            
            }
        }
	}



//  End of File
