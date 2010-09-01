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
* Description:  avatar selection handler
 *
*/




// INCLUDE FILES
#include    "cvimpstuiavatarselectionhandler.h"
#include    "vimpstui.hrh"
#include    "vimpstutilsdialog.h"
#include    "tvimpstconsts.h"

#include    <StringLoader.h>
#include   <vimpstuires.rsg>
#include    <NewFileServiceClient.h>
#include    <AiwGenericParam.h>
#include    <MGFetch.h>
#include    <DocumentHandler.h>
#include    <eikenv.h>
#include    <imageconversion.h>
#include	<aknnavi.h>
#include	<aknappui.h>
#include    <avkon.rsg>
#include    <caf/content.h>
#include    <caf/manager.h> // content access
#include "uiservicetabtracer.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAvatarSelectionHandler::CAvatarSelectionHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAvatarSelectionHandler::CAvatarSelectionHandler()
: CActive( CActive::EPriorityLow )
            {
            CActiveScheduler::Add( this );
            }

// -----------------------------------------------------------------------------
// CAvatarSelectionHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAvatarSelectionHandler::ConstructL()
    {
	TRACER_AUTO;
    User::LeaveIfError(iApaSession.Connect() );

    // create dochandler
    iDocHandler = CDocumentHandler::NewL();
    iDocHandler->SetExitObserver( this);
    }

// -----------------------------------------------------------------------------
// CAvatarSelectionHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAvatarSelectionHandler* CAvatarSelectionHandler::NewLC()

    {
	TRACER_AUTO;
    CAvatarSelectionHandler* self = new( ELeave ) CAvatarSelectionHandler();

    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }

// Destructor
CAvatarSelectionHandler::~CAvatarSelectionHandler()
    {
    Cancel();

    delete iDecoder;
    iDecoder = NULL;

    delete iDocHandler;
    iDocHandler = NULL;

    iApaSession.Close();
    if (iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    delete iMimeType;
    }

//-----------------------------------------------------------------------------
// CAvatarSelectionHandler::RunL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CAvatarSelectionHandler::RunL()
    {
    if (iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    }

//-----------------------------------------------------------------------------
// CAvatarSelectionHandler::DoCancel
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CAvatarSelectionHandler::DoCancel()
    {
    iDecoder->Cancel();
    }

//-----------------------------------------------------------------------------
// CAvatarSelectionHandler::RunError
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CAvatarSelectionHandler::RunError(TInt aError)
    {
    if (IsActive() )
        {
        iStatus = aError;
        Cancel();
        }

    if (aError == KErrNoMemory)
        {
        CActiveScheduler::Current()->Error(KErrNoMemory);
        }

    return KErrNone;
    }

// ---------------------------------------------------------
// CAvatarSelectionHandler::HandleServerAppExit
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CAvatarSelectionHandler::HandleServerAppExit(TInt aReason)
    {
	TRACER_AUTO;
    iServerAppExitReason = aReason;
    if (iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    }

// ---------------------------------------------------------
// CAvatarSelectionHandler::VerifySelectedFilesL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CAvatarSelectionHandler::VerifySelectedFilesL( const MDesCArray* aSelectedFiles )

    {
	TRACER_AUTO;
    if( aSelectedFiles && aSelectedFiles->MdcaCount() )
        {
        TPtrC filename( aSelectedFiles->MdcaPoint(0) ); // since muliselection is not allowed
        //take the only file in the array 
        TInt value( KErrNone ); 
        ContentAccess::CContent* content = ContentAccess::CContent::NewL( filename );
        // check for  protetected image 
        TInt err( content->GetAttribute( ContentAccess::EIsProtected, value ) );
        delete content;

        if( err == KErrNone && value ) // protected 
            {
            // show note
            ShowErrorL(EProtected);
            }
        else  // not protected 
            {
            delete iDecoder; // delete decoder if there is any 
            iDecoder = NULL;

            TInt params(CImageDecoder::EPreferFastDecode);
            TRAPD( err, iDecoder = CImageDecoder::FileNewL(
                    CEikonEnv::Static()->FsSession(),filename, 
                    static_cast<CImageDecoder::TOptions>(params) ));
            if( err == KErrNone )
                {
                // file seems ok, try to decode image
                CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
                CleanupStack::PushL( bitmap );

                TFrameInfo frame =  iDecoder->FrameInfo();
                TSize decodeSize( 60,60 );

                TInt bitmaperr = bitmap->Create( decodeSize, frame.iFrameDisplayMode );      

                if( bitmaperr )
                    { // any bit mapp error throw error and return EFalse
                    CActiveScheduler::Current()->Error( bitmaperr );
                    CleanupStack::PopAndDestroy( bitmap );
                    return EFalse;
                    }
                // start converting
                iDecoder->Convert( &iStatus, *bitmap );
                if( !IsActive() )
                    {
                    SetActive();
                    }

                // and wait until it's finished
                if( !iWait.IsStarted() )
                    {
                    iWait.Start();  //  iWait is not an active object
                    }

                TInt statuscode = iStatus.Int();
                CleanupStack::PopAndDestroy( bitmap );

                if( statuscode == KErrCorrupt ) // image corrupted 
                    {
                    // show note
                    ShowErrorL( ECorrupted );
                    return  EFalse; 
                    }

                return ETrue; // selected file is ok ,return ETrue 
                }
            else
                {
                ShowErrorL( ECorrupted );
                }

            }

        }
    return EFalse;  // no files or protected file return EFalse
    }

// ---------------------------------------------------------
// CAvatarSelectionHandler::DecodeSize
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSize CAvatarSelectionHandler::DecodeSize(const TSize& aSize)
    {
	TRACER_AUTO;
    // 1:1 is always valid ratio for decode scaling
    TInt lastValidRatio( 1);
    for (TInt ratio(KDecodeScaleRatioMin); ratio <= KDecodeScaleRatioMax; ratio<<= 1)
        {
        if (aSize.iWidth % ratio + aSize.iHeight % ratio == 0)
            {
            // this ratio is valid
            lastValidRatio = ratio;
            }
        }

    // return the size scaled with correct ratio
    return TSize(aSize.iWidth / lastValidRatio, aSize.iHeight
            / lastValidRatio);
    }

// ---------------------------------------------------------
// CAvatarSelectionHandler::HandleAvatarSelectionL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC*  CAvatarSelectionHandler::HandleAvatarSelectionL(TInt aCommand )
    {
	TRACER_AUTO;
    HBufC* selectedFile = NULL;
    switch (aCommand)
        {
        case ECmdAvatarGallery:
            {
            CDesCArray* files = new( ELeave )CDesCArrayFlat(1);
            CleanupStack::PushL(files);
            CDesCArray* mimeTypes = new ( ELeave ) CDesCArrayFlat(1 );                                           
            CleanupStack::PushL( mimeTypes );

            mimeTypes->AppendL( KJPegType );
            mimeTypes->AppendL( KGifType );
            mimeTypes->AppendL( KPngType );

            HBufC* softkey = CCoeEnv::Static()->AllocReadResourceLC(
                    R_QTN_SERVTAB_GALLERY_LSK_SELECT);

            HBufC* title = CCoeEnv::Static()->AllocReadResourceLC(
                    R_QTN_SERVTAB_GALLERY_TITLE_AVATAR);
            // This is to clear the NaviPane(Pane where Service Tabs are displayed) 
            // before launching the avatar selection dialogue from photos fetcher.
            // since photos fetcher does't take care of clearing the Navi pane.
            CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane(); // Get the StatusPane    
            CAknNavigationControlContainer* naviPane = static_cast<CAknNavigationControlContainer*>(statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );            
            CAknNavigationDecorator* naviDecoration = naviPane->Top();  // Get the current NaviPane
            naviPane->PushDefaultL( EFalse ); // Clear the NaviPane
            // Launching Avatar Selection 
            if (MGFetch::RunL( *files, EImageFile, EFalse, *softkey,
                    *title, mimeTypes , NULL) )
                {
                if(VerifySelectedFilesL(files))
                    {
                    selectedFile =  (files->MdcaPoint(0)).AllocL(); // alloclate for first filename
                    StoreMimeTypeL(*selectedFile); // get the mime type and store
                    }
                }
            CleanupStack::PopAndDestroy( 4); //  title, softkey ,mimeTypes ,files
            naviPane->PushL(*naviDecoration); // Restoring the previous Navipane 
            break;
            }
        case ECmdAvatarNewPhoto:
            {
            CDesCArray* fileNames = new( ELeave )CDesCArrayFlat(1);
            CleanupStack::PushL(fileNames);
            CNewFileServiceClient* newFile = NewFileServiceFactory::NewClientL();
            CleanupStack::PushL(newFile);

            CAiwGenericParamList* params = CAiwGenericParamList::NewLC();


            TBool okToSend = EFalse;
            TRAPD( err, okToSend =
            newFile->NewFileL( *fileNames, params, ENewFileServiceImage, EFalse ) );

            if (err)
                {
                // restore state
                User::LeaveIfError(err);
                }

            if (okToSend)
                {
                selectedFile = ( fileNames->MdcaPoint(0)).AllocL(); // alloclate for first filename
                StoreMimeTypeL(*selectedFile); // get the mime type and store
                }

            CleanupStack::PopAndDestroy( 3, fileNames); // params, newFile, fileNames

            break;
            }
        }
    return selectedFile;
    }
// ---------------------------------------------------------
// CAvatarSelectionHandler::ShowErrorL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CAvatarSelectionHandler::ShowErrorL(TErrorType aErrorType)
    {
	TRACER_AUTO;
    switch( aErrorType )
        {  
        case EProtected:
            {
            HBufC* prompt = StringLoader::LoadLC( R_QTN_SERVTAB_GALLERY_ERROR_DRMFILE );
            VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) ;
            CleanupStack::PopAndDestroy(prompt);
            break;
            }
        case ECorrupted:
            {
            HBufC* prompt = StringLoader::LoadLC( R_QTN_SERVTAB_GALLERY_ERROR_BADFILE );
            VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) ;
            CleanupStack::PopAndDestroy(prompt);
            break;
            }    
        }

    }

// ---------------------------------------------------------
// CAvatarSelectionHandler::MimeTypeL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CAvatarSelectionHandler::StoreMimeTypeL(const TDesC& aFilename)
    {
	TRACER_AUTO;
    TUid dummyUid( KNullUid );
    TDataType dataType;
    User::LeaveIfError( iApaSession.AppForDocument(
            aFilename,
            dummyUid, dataType ) );

    iMimeType = dataType.Des8().AllocL();
    }

// ---------------------------------------------------------
// CAvatarSelectionHandler::MimeTypeL
// (other items were commented in a header).
// ---------------------------------------------------------
//
const TDesC8& CAvatarSelectionHandler::MimeTypeL()
    {
    return *iMimeType;
    }
//  End of File
