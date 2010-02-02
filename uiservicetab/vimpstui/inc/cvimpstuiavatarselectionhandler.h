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



#ifndef CVIMPSTUIAVATARSELECTIONHANDLER_H
#define CVIMPSTUIAVATARSELECTIONHANDLER_H

//  INCLUDES
#include    <e32base.h>
#include    <MMGFetchVerifier.h>
#include    <apgcli.h>             // RApaLsSession
#include    <AknServerApp.h>       // MAknServerAppExitObserver
#include    <NewFileService.hrh>   // TNewServiceFileType

#include "tvimpstenums.h"
// FORWARD DECLARATIONS

class CDocumentHandler;
class CImageDecoder;


// CLASS DECLARATION

/**
 *  avatar selection handler
 *
 */
class CAvatarSelectionHandler : public CActive,
                                public MAknServerAppExitObserver
    {
public:  // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CAvatarSelectionHandler* NewLC();

    /**
     * Destructor.
     */
    virtual ~CAvatarSelectionHandler();
    
  



public: 
    /**
    *  funtion to select the avatar image file , and allocates memory for 
    *  selected filename if  any file gets selected lese returns  NULL
    * 
    *  @param aCommand , command value , which says
    *  image should be selected from gallery or camera.
    * 
    *  @return allocated value for filename if image is selected else NULL
    *  ownership is transfered to the caller
    */
    HBufC* HandleAvatarSelectionL(TInt aCommand );

    /**
     *  Gives the mimetype for the selected file 
     *  The return value is valid if its called aftter
     *  calling  HandleAvatarSelectionL 
     *  else  the returns NULL
     *  
     * 
     *  @return allocated value for mimetype if image is selected else NULL
     *  
     */
    const TDesC8&  MimeTypeL();

protected:  // Functions from base classes

    /**
     * @see MAknServerAppExitObserver
     * 
     */
    void HandleServerAppExit( TInt aReason );


protected: // From CActive

    /**
     * @see CActive
     */
    void RunL();

    /**
     * @see CActive
     */
    void DoCancel();

    /**
     * @see CActive.
     */
    TInt RunError( TInt aError );	 

private:
    /**
     *  funtion to show the errors based on the param passed
     * 
     *  @param aErrorType :  TErrorType enum value
     */
    void ShowErrorL(TErrorType aErrorType);
    /**
     * C++ default constructor.
     */
    CAvatarSelectionHandler();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Retrieves the optimal decode size (as small as possible)
     * for validating the image
     */
    TSize DecodeSize( const TSize& aSize );

    /**
     * Retrieves the mime type from the file
     * 
     * @param filename for which the mimetype to be find
     */    
    void StoreMimeTypeL(const TDesC& aFilename);
    
    /**
     * Verifies selected files
     * 
     * @param aSelectedFiles Array of files to be verified
     */
    TBool VerifySelectedFilesL( const MDesCArray* aSelectedFiles );
    
    
private:    // Data

    // Exit reason for server apps
    TInt iServerAppExitReason;

    // Activescheduler wait
    CActiveSchedulerWait iWait;    

    // apa session owns
    RApaLsSession iApaSession;

    // owns, document handler
    CDocumentHandler* iDocHandler;

    // owns
    CImageDecoder* iDecoder;

    //owns
    HBufC8* iMimeType;
    };

#endif      // CVIMPSTUIAVATARSELECTIONHANDLER_H

// End of File
