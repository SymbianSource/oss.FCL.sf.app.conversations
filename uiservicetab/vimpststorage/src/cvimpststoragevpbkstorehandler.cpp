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
* Description:  Class that provides handling of vpbk store
*
*/


// INCLUDE FILES
#include "cvimpststoragevpbkstorehandler.h"

#include "mvimpststoragevpbkstorehandler.h"

//vpbk interfaces
#include <MVPbkContactStoreObserver.h>
#include <CVPbkContactManager.h>
#include <MVPbkContactLink.h>
#include <CVPbkContactStoreUriArray.h>
#include <MVPbkContactOperationBase.h>
#include <MVPbkContactStore.h>
#include <MVPbkContactStoreProperties.h>
#include <MVPbkContactView.h>
#include <MVPbkContactStoreList.h>
#include <MVPbkContactFieldTextData.h>
#include <MVPbkContactFieldUriData.h>
#include <MVPbkContactFieldBinaryData.h>
#include <MVPbkContactFieldData.h>
#include <CVPbkContactViewDefinition.h>

#include <MVPbkViewContact.h>
#include <MVPbkStoreContact.h>
#include <TVPbkContactStoreUriPtr.h>
#include <MVPbkStoreContactFieldCollection.h>
#include <MVPbkStoreContactField.h>
#include <MVPbkFieldType.h>
#include <VPbkEng.rsg>
#include <CVPbkContactLinkArray.h>
#include <CVPbkFieldTypeSelector.h>
#include <CPbk2SortOrderManager.h>
#include <MVPbkContactFieldBinaryData.h>

#include "vimpstdebugtrace.h" 
#include "tvimpstconsts.h"
#include <cntdb.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::
//      CVIMPSTStorageVPbkStoreHandler
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkStoreHandler:: CVIMPSTStorageVPbkStoreHandler( MVIMPSTStorageVPbkStoreHandler& aStoreHandler,TBool aLocalStore  ):
	CActive( CActive::EPriorityIdle ),
	iStoreHandler(aStoreHandler ),
	iStoreEventType( EVPbkUnknownEvent ),
	iFetcingIndex( 0 ),
	iLocalStore( aLocalStore )
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CVIMPSTStorageVPbkStoreHandler()") );
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ConstructL(const TDesC& aStoreName,const TDesC& aServiceName )
    { 
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ConstructL() begin") );
    iAddIdInProgress = NULL;
    iStoreName = aStoreName.AllocL();
    iServiceName = aServiceName.AllocL();
    CVPbkContactStoreUriArray* uriArray = CVPbkContactStoreUriArray::NewLC();
    uriArray->AppendL( TVPbkContactStoreUriPtr( aStoreName ) );    
    if( !uriArray->IsIncluded( TVPbkContactStoreUriPtr( KPhoneBookDbURI ) ) )
	    {
	    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ConstructL() phonebook store apended") );
	    uriArray->AppendL( TVPbkContactStoreUriPtr( KPhoneBookDbURI )   );	
	    }
    iContactManager = CVPbkContactManager::NewL( *uriArray, NULL );
    CleanupStack::PopAndDestroy( uriArray );
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ConstructL() iContactManager created") );
    iStores = &iContactManager->ContactStoresL();
    iStores->OpenAllL( *this );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ConstructL() OpenAllL called") );
    iSortOrderManager = CPbk2SortOrderManager::NewL( iContactManager->FieldTypes(), NULL ); 
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ConstructL() end") );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::NewL
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkStoreHandler* CVIMPSTStorageVPbkStoreHandler::NewL( const TDesC& aContactDb,
																	const TDesC& aServiceName,
													                MVIMPSTStorageVPbkStoreHandler& aStoreHandler,
													                TBool aLocalStore  )
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::NewL begin") );
	CVIMPSTStorageVPbkStoreHandler* self = NewLC( aContactDb ,aServiceName, aStoreHandler,aLocalStore );
	CleanupStack::Pop( self );
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::NewL end") );
	return self;
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::NewLC
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkStoreHandler* CVIMPSTStorageVPbkStoreHandler::NewLC( const TDesC& aContactDb,
																	const TDesC& aServiceName,
																	MVIMPSTStorageVPbkStoreHandler& aStoreHandler,
																	TBool aLocalStore   )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::NewLC begin") );
    CVIMPSTStorageVPbkStoreHandler* self =
        new (ELeave) CVIMPSTStorageVPbkStoreHandler( aStoreHandler ,aLocalStore );
    CleanupStack::PushL( self );
    self->ConstructL( aContactDb,aServiceName );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::NewLC end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::
//      ~CVIMPSTStorageVPbkStoreHandler
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkStoreHandler:: ~CVIMPSTStorageVPbkStoreHandler()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::~CVIMPSTStorageVPbkStoreHandler begin") );
    if( iWait.IsStarted() )
	    {
	    iWait.AsyncStop();	
	    }
   	Cancel();
   	ResetAndDestroyRArray();
   	iUnCommittedVPbkContacts.ResetAndDestroy();
   	delete iAddIdInProgress;
   	delete iContactOperation;
    delete iContactLink;
    delete iStoreContact;
    delete iContactView;
    if ( iContactManager )
        {
        TRAP_IGNORE( iContactManager->ContactStoresL().CloseAll( *this ) );
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::~CVIMPSTStorageVPbkStoreHandler CloseAll called") );
        delete iContactManager;
        }
    delete iStoreName;
    delete iSortOrderManager;
    delete iServiceName;
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::~CVIMPSTStorageVPbkStoreHandler end") );
    }
 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactViewReady
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactViewReady( 
    MVPbkContactViewBase& aView )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactViewReady() begin") );
    if ( iContactView == &aView && ( iStoreEventType == EVPbkUnknownEvent ) )
        { 
        iStoreEventType = EVPbkContactReading ;          
        TRAP_IGNORE( HandleContactViewReadyL( *iContactView ) );
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactViewReady() end") );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactViewUnavailable
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactViewUnavailable( 
    MVPbkContactViewBase& /*aView*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactViewUnavailable begin") );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactViewUnavailable end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactAddedToView
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactAddedToView( 
    MVPbkContactViewBase& /*aView*/, 
    TInt /*aIndex*/, const MVPbkContactLink& /*aContactLink*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactAddedToView begin"));
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactAddedToView End "));
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactRemovedFromView
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactRemovedFromView( 
    MVPbkContactViewBase& /*aView*/, 
    TInt /*aIndex*/, const MVPbkContactLink& /*aContactLink*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactRemovedFromView begin") );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactRemovedFromView End"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactViewError
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactViewError( 
    MVPbkContactViewBase& /*aView*/, 
    TInt /*aError*/, TBool /*aErrorNotified*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactViewError() begin") );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactViewError() end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::StoreReady
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::StoreReady( 
    MVPbkContactStore& /*aContactStore*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StoreReady() begin"));
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StoreReady() end") );
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::StoreUnavailable
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::StoreUnavailable( 
    MVPbkContactStore& /*aContactStore*/, 
    TInt /*aReason*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StoreUnavailable() begin") );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StoreUnavailable() end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL
// ---------------------------------------------------------------------------
//
MVPbkContactStore* CVIMPSTStorageVPbkStoreHandler::GetDefaultStoreL( const TDesC& aUri )
	 {
	 TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetDefaultStoreL() "));
	 return iStores->Find( TVPbkContactStoreUriPtr( aUri ) );	
	 }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL( 
    MVPbkContactStore& aContactStore , 
    TVPbkContactStoreEvent aStoreEvent )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() begin") );
    if( iStore == &aContactStore )
	    {
	    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() store event") );
	  	switch( aStoreEvent.iEventType )
		    {
		    case TVPbkContactStoreEvent::EContactAdded:
			    {
			    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() EContactAdded event") );
			    delete iContactLink;
			    iContactLink = NULL;
				MVPbkContactLink* contactLink = aStoreEvent.iContactLink->CloneLC();
			 	iContactLink = 	contactLink;
				CleanupStack::Pop(); // contactLink
				IssueRequest(); // complete the request	
				TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() EContactAdded done") );
			    break;	
			    }
			case TVPbkContactStoreEvent::EContactDeleted:
			    {
			    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() EContactDeleted event") );
			   	MVPbkContactLink* contactLink = aStoreEvent.iContactLink->CloneLC();
			   	delete iContactLink;
		    	iContactLink = NULL;
		 		iContactLink = 	contactLink;
		 		CleanupStack::Pop(); // contactLink
				TVIMPSTVPbkStoreEvent storeEvent(EVPbkContactDeleted, iContactLink );
		    	iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;	
		    	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() EContactDeleted done") );
			    break;	
			    }
			case TVPbkContactStoreEvent::EContactChanged:
				{
				TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() EContactChanged event") );
				delete iContactLink;
				iContactLink = NULL;
				MVPbkContactLink* contactLink = aStoreEvent.iContactLink->CloneLC();
				iContactLink = 	contactLink;
				CleanupStack::Pop(); // contactLink
				iStoreEventType = EVPbkContactChanged;
				IssueRequest(); // complete the request which will begin retriving the iContactLink
				TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() EContactChanged done") );
				break;	
				}
			case TVPbkContactStoreEvent::EUnknownChanges:
			    {
			    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() EUnknownChanges event") );
			    iStoreEventType = EVPbkUnknownChanges;
			    TVIMPSTVPbkStoreEvent storeEvent(iStoreEventType);
			    iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;
			    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() EUnknownChanges done") );
			    break;	
			    }
		    default:
			 	break;
		    }
	    }
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL() end") ); 
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::OpenComplete
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::OpenComplete()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::OpenComplete() begin") );
    // stores OpenAll completed
    TRAP_IGNORE( DoHnadleOpenCompleteL() );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::OpenComplete() end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::DoHnadleOpenCompleteL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::DoHnadleOpenCompleteL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleOpenCompleteL() begin") );
    iStore = GetDefaultStoreL( *iStoreName ) ;
    CVPbkContactViewDefinition* viewDefinition = 
        				CVPbkContactViewDefinition::NewL();
    CleanupStack::PushL( viewDefinition );    
    viewDefinition->SetType( EVPbkContactsView );
    viewDefinition->SetUriL( *iStoreName );
    if ( iStore )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleOpenCompleteL() store view creating"));
        MVPbkContactView* tmpView = iStore->CreateViewLC(
						            *viewDefinition,
						            *this,
						            iSortOrderManager->SortOrder() );        
        CleanupStack::Pop(); // tmpView            
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleOpenCompleteL() store view created"));        
        iContactView = tmpView;
        }
    CleanupStack::PopAndDestroy();  //viewDefinition, can not pop ny name
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleOpenCompleteL() end"));
    }
	
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactOperationCompleted
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactOperationCompleted(
    TContactOpResult /*aResult*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactOperationCompleted() begin"));
    // called when contact commit is successful
    iOperationError = KErrNone;
    if( iWait.IsStarted() )
	    {
	    iWait.AsyncStop();	
	    }
    TRAP_IGNORE( CommitNextPendingContactsL() );
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactOperationCompleted() end"));
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactOperationFailed
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactOperationFailed( 
    TContactOp /*aOpCode*/, TInt aErrorCode, TBool /*aErrorNotified*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactOperationFailed() begin"));
    // called when contact commit is unsuccessful
    iOperationError = aErrorCode;
    if( iWait.IsStarted() )
	    {
	    iWait.AsyncStop();	
	    }
    TRAP_IGNORE( CommitNextPendingContactsL() );
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ContactOperationFailed() end"));
    }    
 
 
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::StepComplete
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::StepComplete(
        MVPbkContactOperationBase& /*aOperation*/,
        TInt /*aStepSize*/ )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StepComplete() begin"));
    // called when batch operation call
    iOperationError = KErrNone;
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StepComplete() end"));
    }


// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::StepFailed
// --------------------------------------------------------------------------
//
TBool CVIMPSTStorageVPbkStoreHandler::StepFailed(
        MVPbkContactOperationBase& /*aOperation*/,
        TInt /*aStepSize*/,
        TInt aError )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StepFailed() begin"));
    // called when batch operation call
    iOperationError = aError;
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StepFailed() end"));
   	return EFalse;
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::OperationComplete
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::OperationComplete(
        MVPbkContactOperationBase& aOperation )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::OperationComplete() begin"));
    // called when batch operation call for opening the multiple store
    if ( &aOperation == iContactOperation )
        {
        delete iContactOperation;
        iContactOperation = NULL;
        }
    iOperationError = KErrNone;
    if( iWait.IsStarted() )
	    {
	    iWait.AsyncStop();	
	    }
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::OperationComplete() end"));
    }   

//----------------------------------------------------------------------
//CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationComplete()
//----------------------------------------------------------------------
void CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationComplete( MVPbkContactOperationBase& aOperation,
                                            				MVPbkStoreContact* aContact )
    {  
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationComplete() begin")); 
    // called when contact details is retrived
    TRAP_IGNORE( DoHnadleSingleContactOperationCompleteL( aOperation, aContact ) );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationComplete() end"));
    }

//----------------------------------------------------------------------
//CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL()
//----------------------------------------------------------------------
void CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL( MVPbkContactOperationBase& aOperation,
                                            								 MVPbkStoreContact* aContact )
    {  
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() begin"));
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() StoreEventType: %d"),iStoreEventType);
    // called when contact details is retrived
    if( iStoreContact )
        {
        delete 	iStoreContact;
        iStoreContact = NULL;
        }
    iStoreContact = aContact ; // iStoreContact  owns thea Contact
    if( ( &aOperation == iContactOperation) && iStoreContact )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() inside if (operation) "));
        if( iWait.IsStarted() )
		    {
		    iWait.AsyncStop();
		    }
        delete iContactOperation;
        iContactOperation = NULL;
        TRACE( T_LIT("DoHnadleSingleContactOperationCompleteL() before iCommitOperationInProgress: %d"),iCommitOperationInProgress);
        iCommitOperationInProgress = EFalse;
        TRACE( T_LIT("DoHnadleSingleContactOperationCompleteL()After iCommitOperationInProgress: %d"),iCommitOperationInProgress);

        if( iStoreEventType != EVPbkUpdatingAvatar )
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() inside if (iStoreEventType != EVPbkUpdatingAvatar ) "));
            if( !iLocalStore && iStoreEventType == EVPbkContactRetriving )
                {
                TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() EVPbkContactRetriving"));
		    	RetriveFieldDataAndSendEventL( *iStoreContact );
			   }
            else 
                {
                TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() inside else start"));
                HBufC* userId = GetContactFieldDataLC( *iStoreContact, R_VPBK_FIELD_TYPE_IMPP );
                HBufC* firstname = GetContactFieldDataLC( *iStoreContact,R_VPBK_FIELD_TYPE_FIRSTNAME );
                HBufC* lastname = GetContactFieldDataLC( *iStoreContact,R_VPBK_FIELD_TYPE_LASTNAME );

                HBufC* disname = NULL;
                /*
                 *	There are three scenarios where I can form the Display name.
                 *	1. First and last name are present. 
                 *	2. First name is present but no last name is present.
                 *	3. Lastname is there and no first name.
                 *	4. if none, then assigning NULL 
                 */
                if ( firstname && lastname )
                    {
                    disname = HBufC::NewLC( firstname->Des().Length() + lastname->Des().Length()+ 1 );
                    TPtr disnamePtr( disname->Des() );
                    disnamePtr.Append(*firstname);
                    if(firstname->Length() && lastname->Length() )
                        {
                        disnamePtr.Append(KSpace);
                        }
                    disnamePtr.Append(*lastname);
                    }
                else
                    {
                    disname = KNullDesC().AllocLC();
                    }
                HBufC8* avatarContent = GetContactBinaryFieldDataLC( *iStoreContact,R_VPBK_FIELD_TYPE_THUMBNAILPIC );
                TRACE( T_LIT("DoHnadleSingleContactOperationCompleteL()  iStoreHandler.HandleVPbkStoreEventL: %d"), iStoreEventType);
                TPtr useridPtr = userId->Des();
                TRACE( T_LIT("DoHnadleSingleContactOperationCompleteL()  inside else UserId : %S"), &useridPtr);
                TVIMPSTVPbkStoreEvent storeEvent(iStoreEventType,
                        iContactLink,
                        NULL,
                        *userId,
                        *disname,
                        *avatarContent);
                iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;
                CleanupStack::PopAndDestroy(5); // disname,lastname,firstname,userId,avatarContent 
                TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() inside else end"));
                }
            delete iAddIdInProgress;
            iAddIdInProgress = NULL;
            }

        iOperationError = KErrNone;
        if( iStoreEventType == EVPbkContactReading && iContactView )
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() if after iOperationError"));
            HandleContactViewReadyL( *iContactView );   
            }
        
        else if(!iCommitOperationInProgress )
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() else if after if after iOperationError"));
            CommitNextPendingContactsL();// commit only if any pending contact
            }
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL() end"));
        }
    }
//----------------------------------------------------------------------
//CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationComplete()
//----------------------------------------------------------------------
void CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationFailed( MVPbkContactOperationBase& aOperation,
                                                                   TInt aError)
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationFailed() begin"));
	if( &aOperation == iContactOperation )
        {
        delete iContactOperation;
        iContactOperation = NULL;
        }
    iOperationError = aError;
    iCommitOperationInProgress = EFalse;
    if( iWait.IsStarted() )
	    {
	    iWait.AsyncStop();	
	    }
    TRAP_IGNORE( CommitNextPendingContactsL() );// commit only if any pending contact
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationFailed() begin"));
    }
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL( MVPbkContactViewBase& aView  )
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL() begin"));
	TInt contactCount = aView.ContactCountL();
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL() begin"));
	if( contactCount == iFetcingIndex  && iStoreEventType == EVPbkContactReading )
		{
		TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL() reading completed") );
		iStoreEventType = EVPbkContactReadCompleted;
		TVIMPSTVPbkStoreEvent storeEvent(iStoreEventType);
		iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;
		}
	else if( iFetcingIndex < contactCount && iFetcingIndex>=0 )
		{
		TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL() reading"));
		const MVPbkViewContact& contact =  aView.ContactAtL( iFetcingIndex );
		MVPbkContactLink* contactLink = contact.CreateLinkLC();
		delete iContactLink;
		iContactLink = NULL;
		iContactLink = 	contactLink;
		CleanupStack::Pop(); // contactLink
		IssueRequest();
		iFetcingIndex++;
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL() end"));
	}
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::IssueRequest
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::IssueRequest()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::IssueRequest() begin"));
    if( !IsActive() )
	    {
	    TRequestStatus* status = &iStatus;
	    User::RequestComplete( status, KErrNone );
	    SetActive();
	    }
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::IssueRequest() end"));
    }
	
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RunL
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::RunL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RunL() begin") );
    if( iContactLink )
	    {
	    delete iContactOperation;
	    iContactOperation = NULL;
	    iContactOperation = iContactManager->RetrieveContactL( *iContactLink, *this );	
	    }
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RunL() end"));
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::DoCancel
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::DoCancel()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoCancel() begin") );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::DoCancel() end") );
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RunError
// --------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkStoreHandler::RunError( TInt aError )
    {  
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RunError()") );  
    return aError;
    }

//////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL(const TDesC& aUserId, 
        						 						const TDesC& aDisplayName )   
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() begin") );
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() aUserId = %S"), &aUserId );
	
	if( iAddIdInProgress && iAddIdInProgress->Compare( aUserId ) == 0 )
		{
		TPtr iAddIdInProgressPtr = iAddIdInProgress->Des();
		TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() iAddIdInProgressPtr = %S"), &iAddIdInProgressPtr );
		TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() already Adding") );
		return KErrAlreadyExists;	
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() After If") );
	delete iAddIdInProgress;
	iAddIdInProgress = NULL;
	iAddIdInProgress = aUserId.AllocL();
	
    if(iStore)
		{
		MVPbkStoreContact* storedContact = iStore->CreateNewContactLC(); //1
		if( !storedContact )   
			{
			return NULL;    
			}
		if(aUserId.Length()!= 0)
			{
			TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() valid xsp Id") );
			TInt prefixLocation = aUserId.Locate( ':' );
			const MVPbkContactStoreProperties& storeProperties =  storedContact->ParentStore().StoreProperties();
			const MVPbkFieldTypeList& supportedFieldTypes = storeProperties.SupportedFields();
			TInt fieldCount = supportedFieldTypes.FieldTypeCount();

			for( TInt index = 0 ; index < fieldCount ; index++ )
				{        
				const MVPbkFieldType* fieldType = &(supportedFieldTypes.FieldTypeAt(index));
				if ( fieldType && ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_IMPP ))
					{
					TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() XSP Id-IMMP field creating") );
					MVPbkStoreContactField* fielduri = storedContact->CreateFieldLC(*fieldType);  //2
					// Check for prefix and remove if found
					//append the internettelephoney field.
					HBufC* fieldData = NULL;
					TInt serFieldLength = aUserId.Length();
					if( prefixLocation >=0 && ( prefixLocation < serFieldLength ) )
						{
						fieldData = aUserId.Right(serFieldLength - prefixLocation - 1).AllocLC(); //3
						}
					else
						{
						fieldData = aUserId.AllocLC(); // 3
						}
					if (fieldData)
						{
						TPtr fieldDataPtr( fieldData->Des() );
						MVPbkContactFieldUriData& address = MVPbkContactFieldUriData::Cast( fielduri->FieldData() );
						// TODO check here about servicename:userId
						address.SetUriL(*iServiceName,*fieldData);
						// fielduri ownership is transfered to storedContact
						storedContact->AddFieldL(fielduri);   
						CleanupStack::PopAndDestroy(fieldData); //3 
						CleanupStack::Pop(); //fielduri 2         
						}//End if (fieldData)
					else
						{
						CleanupStack::PopAndDestroy(); //2 fielduri 	
						}
					TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() XSP Id-IMMP field created") );
					}//end of if
				else if ( fieldType &&  
					( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_FIRSTNAME ))
					{
					TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() first name field creating") );
					HBufC* firstName = NULL;
					
					// if there is no display name then allocate hte user id as displayname.
					if(!(aDisplayName.Length()))
                       {
                        // if aServiceField is sip:user1@ouped111147.nmp.nokia.com, then 
                        // rippedaServiceField will be user1@ouped111147.nmp.nokia.com
                        TPtrC rippedaServiceField = aUserId.Mid(prefixLocation+1);
                        //append first name.
                        TInt atLocation = rippedaServiceField.Locate( '@' ); 
                        if( atLocation > 0 && ( atLocation < rippedaServiceField.Length() ) )
                            {
                            // atLocation must be greater that 0
                            firstName = rippedaServiceField.Left( atLocation ).AllocLC() ;  //2 
                            }
                        else
                            {
                            firstName = rippedaServiceField.AllocLC(); //2
                            }
                        }
					// there is a display name hence allocate it.
					else
					    {
					    firstName =  aDisplayName.AllocLC();//2
					    }
					
					if (firstName)
						{
						MVPbkStoreContactField* firstNamefield = storedContact->CreateFieldLC(*fieldType); //2
						MVPbkContactFieldTextData& firstNameText = MVPbkContactFieldTextData::Cast( firstNamefield->FieldData() );
						firstNameText.SetTextL(*firstName);
						storedContact->AddFieldL(firstNamefield); // firstNamefield ownership transfered to storedContact
						CleanupStack::Pop();  //firstNamefield  2 
						CleanupStack::PopAndDestroy(firstName); // 3
						}//End if (firstName)

					TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() first name field created") );                    
					}//end of if.
				}//end of for.
			TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() iCommitOperationInProgress = %d"),iCommitOperationInProgress );
			if( iCommitOperationInProgress )
			    {
			    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() inside if") );
			    iUnCommittedVPbkContacts.AppendL( storedContact ); //takes ownership
			    CleanupStack::Pop(); //storedContact
			    }
			else
			    {
			    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() inside else") );
			    iCommitOperationInProgress = ETrue;
			    if(iStoreContact)
			        {
			        delete iStoreContact;
			        iStoreContact = NULL;
			        }
			    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() contact fields added") );
			    iStoreContact = storedContact;
			    CleanupStack::Pop(); //storedContact
			    iStoreContact->CommitL(*this);
			    iStoreEventType = EVPbkContactAdded;
			    }
			TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() contact commited") );
			//Start the wait untill adding of new contact is complete
			}
		}
    //return the added contact 
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() iOperationError =%d"),iOperationError );
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CreateVPbkContactL() end") );        
	return iOperationError; 
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL
// ---------------------------------------------------------------------------
//  
void CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() 
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() Start") );
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() iStoreEventType = %d"),iStoreEventType );
    if( !iCommitOperationInProgress && iUnCommittedVPbkContacts.Count() && iStoreEventType != EVPbkUpdatingAvatar)
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() Commit next") );
        iCommitOperationInProgress = ETrue;
        if(iStoreContact)
            {
            delete iStoreContact;
            iStoreContact = NULL;
            }
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() comming...") );
        iStoreContact = iUnCommittedVPbkContacts[0];
        iUnCommittedVPbkContacts.Remove(0);
        iUnCommittedVPbkContacts.Compress();
        iStoreContact->CommitL(*this);
        iStoreEventType = EVPbkContactSynchronizing;
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() Commit called") );
        }
    else if(!iCommitOperationInProgress && iPendingAvatars.Count()  && iStoreEventType != EVPbkUpdatingAvatar)
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() Commit next pending avatar") );
        TVIMPSTAvatarData avatarData = iPendingAvatars[0];
        UpdateAvatarFieldDataL( *avatarData.iContactLink, *avatarData.iAvatarContent );
        iPendingAvatars.Remove( 0 );
        delete avatarData.iContactLink;
        avatarData.iContactLink = NULL;
        delete avatarData.iAvatarContent;
        avatarData.iAvatarContent = NULL;
        iPendingAvatars.Compress();
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() End") );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RemoveVPbkContactL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkStoreHandler::RemoveVPbkContactL(const CVPbkContactLinkArray& aContactLinkArray ) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveVPbkContactL() begin") );
	// text now holds the name (or first field of the contact) 
    delete iContactOperation;
    iContactOperation = NULL;
    iContactOperation = iContactManager->DeleteContactsL(aContactLinkArray, *this  );
    if( !iWait.IsStarted() )
	    {
	    iWait.Start();	
	    }
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveVPbkContactL() iOperationError=%d"),iOperationError ); 
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveVPbkContactL() end") );     
    return iOperationError;
  	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RetrieveVPbkContactL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkStoreHandler::RetrieveVPbkContactL(const TDesC8& aPackedLinks) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RetrieveVPbkContactL() begin") );   
	iStoreEventType = EVPbkContactRetriving;
	CVPbkContactLinkArray* linkArray = CVPbkContactLinkArray::NewLC(aPackedLinks, *iStores );
    // text now holds the name (or first field of the contact) 
    TInt count = linkArray->Count();
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::StoreReady() count=%d"),count );   
    for ( TInt i=0; i<count; i++ )
	    {
	    delete iContactLink;
	    iContactLink = NULL;
	    // local variable caz cant push member variable to cleanup stack
	    MVPbkContactLink* cloneLink = linkArray->At( i ).CloneLC();
	    iContactLink = cloneLink;
	    CleanupStack::Pop(); // cloneLink
	    
	    delete iContactOperation;
    	iContactOperation = NULL;
    	
	    iContactOperation = iContactManager->RetrieveContactL( *iContactLink, *this  );
	    if( !iWait.IsStarted() )
		    {
		    iWait.Start();	
		    }
	    }
    CleanupStack::PopAndDestroy();//linkArray
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RetrieveVPbkContactL() end") );   
    return iOperationError;
    }
 
                
                
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
														    const TDesC8& aAvatartData )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL() begin") ); 
  
    // local variable caz cant push member variable to cleanup stack
    MVPbkContactLink* cloneLink = aContactLink.CloneLC();
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL() iCommitOperationInProgress: %d"),iCommitOperationInProgress );  
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL() iUnCommittedVPbkContacts: %d"),iUnCommittedVPbkContacts.Count() );
    if(iCommitOperationInProgress || iUnCommittedVPbkContacts.Count() || iStoreEventType == EVPbkUpdatingAvatar )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL()Inside if") ); 
        TVIMPSTAvatarData avatarData = {
                                       cloneLink,
                                       aAvatartData.AllocL()
                                      };
        iPendingAvatars.Append( avatarData );
        CleanupStack::Pop(); // cloneLink
        return KErrInUse;
        }
    iCommitOperationInProgress = ETrue;
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL()Inside after if") ); 
    delete iContactLink;
    iContactLink = NULL;
    iContactLink = cloneLink;
    CleanupStack::Pop(); // cloneLink
    
    //before updating the avatar, check if there are any pending operation already, if there are any,
    // then delete them and create the next one.
    if(iContactOperation)
        {
        delete iContactOperation;
        iContactOperation = NULL;
        }
    iStoreEventType = EVPbkUpdatingAvatar;
    iContactOperation = iContactManager->RetrieveContactL( *iContactLink, *this  );
    if( !iWait.IsStarted() )
        {
        iWait.Start();  
        }    
     // exception KErrInUse If another asynchronous operation is 
    // already in progress.
    // exception KErrAccessDenied if the contact can not be modified.
    iStoreContact->LockL(*this);
  

    //wait till lock operation is succeded
    if(!iWait.IsStarted() ) 
	    {
	    iWait.Start();	
	    }
	TInt fieldcount = iStoreContact->Fields().FieldCount();
	for( TInt j = 0 ; j < fieldcount ; j++ )
		{
		const MVPbkFieldType* fieldType = iStoreContact->Fields().FieldAt(j).BestMatchingFieldType();
		if( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_THUMBNAILPIC )
			{
			iStoreContact->RemoveField(j);
			break;
			}
		}
	if( aAvatartData.Length() )
		{
		const MVPbkContactStoreProperties& storeProperties =  iStoreContact->ParentStore().StoreProperties();
	    const MVPbkFieldTypeList& supportedFieldTypes = storeProperties.SupportedFields();
	    TInt fieldCount = supportedFieldTypes.FieldTypeCount();
	   	for( TInt index = 0 ; index < fieldCount ; index++ )
	        {        
	        const MVPbkFieldType* fieldType = &( supportedFieldTypes.FieldTypeAt(index) );
		   //find the relevant voip fields/email id fileds.
	        if ( fieldType && ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_THUMBNAILPIC ) )
	            {
				//updating avatar field in the virtual phonebook
				//found the required fieldtype
				MVPbkStoreContactField* storeField = iStoreContact->CreateFieldLC(*fieldType);
				// The aField instance can not be used after AddFieldL
				//storeField ownership is transferred 
				TInt fieldIndex = iStoreContact->AddFieldL(storeField);
				MVPbkContactFieldData& dataField = iStoreContact->Fields().FieldAt( fieldIndex ).FieldData();
				MVPbkContactFieldBinaryData& data = MVPbkContactFieldBinaryData::Cast(dataField);
				data.SetBinaryDataL(aAvatartData);
				CleanupStack::Pop(); // storeField  
				break;                   
				}
	        }
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL()Inside after if(avatardata.length)") ); 
	iStoreEventType = EVPbkContactChanged;
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL()Inside after if(avatardata.length ): %d"),iStoreEventType ); 
	iStoreContact->CommitL(*this);
	//wait for delete Avatar operation to complete.
	if(!iWait.IsStarted() ) 
		{
		iWait.Start();	
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL() end") );   
	return iOperationError;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RemoveAvatarFieldL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkStoreHandler::RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact)
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAvatarFieldL() begin") );   
	aStoreContact.LockL(*this);
    //wait till lock operation is succeded
    if(!iWait.IsStarted() ) 
	    {
	    iWait.Start();	
	    }
	TInt fieldcount = aStoreContact.Fields().FieldCount();
	for( TInt j = 0 ; j < fieldcount ; j++ )
		{
		const MVPbkFieldType* fieldType = aStoreContact.Fields().FieldAt(j).BestMatchingFieldType();
		if( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_THUMBNAILPIC )
			{
			aStoreContact.RemoveField(j);
			break;
			}
		}
	aStoreContact.CommitL(*this);
	//wait for delete Avatar operation to complete.
	if(!iWait.IsStarted() ) 
		{
		iWait.Start();	
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAvatarFieldL() iOperationError =%d"),iOperationError );   
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAvatarFieldL() end") );   
	return iOperationError;	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC()
// ---------------------------------------------------------------------------
// 
HBufC* CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC(const MVPbkStoreContact& aContact,
												    		 TInt aFieldResId ) 
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC() begin") );   
    HBufC* retValue = NULL;
    const MVPbkStoreContactFieldCollection& fieldCollection = aContact.Fields();
	TInt fieldCount = fieldCollection.FieldCount();
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC() fieldCount%d"),fieldCount );   
	for( TInt index = 0 ; index < fieldCount ;  index++ )
		{
		const MVPbkStoreContactField& field = fieldCollection.FieldAt( index );
		const MVPbkFieldType* fieldType = field.BestMatchingFieldType();
		//find the relevant xSP field
		if ( fieldType && ( fieldType->FieldTypeResId() == aFieldResId ) )
			{
			const MVPbkContactFieldData& fieldData = field.FieldData();
			switch( fieldData.DataType() )
				{
				case EVPbkFieldStorageTypeText:
					{
					TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC() Text") );   
					retValue = MVPbkContactFieldTextData::Cast(fieldData).Text().AllocLC();
					break;					
					}
				case EVPbkFieldStorageTypeUri:				
					{
					TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC() Uri "));   
					retValue = MVPbkContactFieldUriData::Cast(fieldData).Text().AllocLC();
					break;	
					}
				
				}//End switch
			if( aFieldResId == R_VPBK_FIELD_TYPE_IMPP && retValue )
				{
				TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC() IMPP fild found"));   
				// TODO RAMC : Nikhil/Mukesh
				// in case of srver contacts it also find the first valid xsp id
				// not valid through NULL
				// which can be clarified 
				// earlier it show selection query in UI which one to use
				TPtrC scheme = MVPbkContactFieldUriData::Cast(fieldData).Scheme();
				if( scheme.Compare( *iServiceName ) == 0 )
					{
					TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC() actual xsp Id found"));   
					// actual xsp id found break the loop and return 
					break;	
					}
				else 
					{
					// actual xsp not yet found continue looking
					CleanupStack::PopAndDestroy(); // retValue;
					retValue = NULL;	
					}
				}
			else if( retValue )
				{
				// break this loop once the given aFieldResId is found
				// with valid data
				break;	
				}
			}
		}
	if( !retValue )
		{
		TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC() NULL value"));   
		// if NULL assign valid ptr
		retValue = KNullDesC().AllocLC();	
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC() end"));   
	return retValue;// ownership is return to caller
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC()
// ---------------------------------------------------------------------------
// 
HBufC8* CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC(const MVPbkStoreContact& aContact,
                                                             TInt aFieldResId ) 
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC() begin") );   
    HBufC8* retValue = NULL;
    const MVPbkStoreContactFieldCollection& fieldCollection = aContact.Fields();
    TInt fieldCount = fieldCollection.FieldCount();
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC() fieldCount%d"),fieldCount );   
    for( TInt index = 0 ; index < fieldCount ;  index++ )
        {
        const MVPbkStoreContactField& field = fieldCollection.FieldAt( index );
        const MVPbkFieldType* fieldType = field.BestMatchingFieldType();
        //find the relevant xSP field
        if ( fieldType && ( fieldType->FieldTypeResId() == aFieldResId ) )
            {
            const MVPbkContactFieldData& fieldData = field.FieldData();
            switch( fieldData.DataType() )
                {
                case EVPbkFieldStorageTypeBinary:
                    {
                    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC() Text") );
                    retValue = MVPbkContactFieldBinaryData::Cast(fieldData).BinaryData().AllocLC();
                    break;                  
                    }
                }//End switch
             if( retValue )
                {
                // break this loop once the given aFieldResId is found
                // with valid data
                break;  
                }
            }
        }
    if( !retValue )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC() NULL value"));   
        // if NULL assign valid ptr
        retValue = KNullDesC8().AllocLC();   
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC() end"));   
    return retValue;// ownership is return to caller
    }


// ---------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL
// 
// ---------------------------------------------------------		
void CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL(const MVPbkStoreContact& aContact ) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL() begin") );   
	HBufC* retValue = NULL;
    const MVPbkStoreContactFieldCollection& fieldCollection = aContact.Fields();
	TInt fieldCount = fieldCollection.FieldCount();
    for( TInt index = 0 ; index < fieldCount ;  index++ )
		{
		const MVPbkStoreContactField& field = fieldCollection.FieldAt( index );
		const MVPbkFieldType* fieldType = field.BestMatchingFieldType();
		//find the relevant xSP field
		//if ( fieldType && ( fieldType->FieldTypeResId() == aFieldResId ) )
		if ( fieldType && (( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_IMPP )
		                     || (fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_LANDPHONEHOME )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_MOBILEPHONEHOME )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_LANDPHONEWORK )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_MOBILEPHONEWORK )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_VOIPWORK )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_VOIPHOME )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_LANDPHONEGEN )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_MOBILEPHONEGEN )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_VOIPGEN )
		                     ||(fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_SIP ) ))
			{
			const MVPbkContactFieldData& fieldData = field.FieldData();
			switch( fieldData.DataType() )
				{
			    case EVPbkFieldStorageTypeText:
                    {
                    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL() Text") );   
                    retValue = MVPbkContactFieldTextData::Cast(fieldData).Text().AllocLC();
                    break;                  
                    }
				case EVPbkFieldStorageTypeUri:				
					{
					TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL() begin") ) ;   
					retValue = MVPbkContactFieldUriData::Cast(fieldData).Text().AllocLC();
					break;	
					}
				
				}//End switch
			if( retValue )
				{
				TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL() begin") );   
				TVIMPSTVPbkStoreEvent storeEvent(iStoreEventType,
									NULL,
									NULL,
									*retValue,
									KNullDesC );
				iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;	
				CleanupStack::PopAndDestroy(); // retValue;
				retValue = NULL;
				}
			}
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL() begin") );   
	}

// ---------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL
// 
// ---------------------------------------------------------		
TInt CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL()
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL() begin") );   
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL() before if") ); 
	if(iAddIdInProgress)
	    {
	    TPtr iAddIdInProgressPtr = iAddIdInProgress->Des();
	    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL() iAddIdInProgressPtr  = %S"), &iAddIdInProgressPtr ); 
	    delete iAddIdInProgress;
	    iAddIdInProgress = NULL;
	    }
	// intialize it to efalse as, somewhere it would have missed.
	iCommitOperationInProgress = EFalse;
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL() After if") ); 
	TInt count = 0;
	if( iContactView )
		{
		count = iContactView->ContactCountL();	
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL() count = %d"),count) ;   
	TPtr iStoreNamePtr(iStoreName->Des() );
	TInt index = iStoreNamePtr.Find(_L(":") );
	if( index >= 0 && count )
		{
		TPtrC store = iStoreNamePtr.Right( iStoreNamePtr.Length() - index -3 );	
		CContactIdArray* contactIds = CContactIdArray::NewLC();
		CContactDatabase* contactDb = NULL;
		//Open the database
		TRAPD(err, contactDb = CContactDatabase::OpenL( store ) );
		if(err == KErrNone )
			{	
			CleanupStack::PushL( contactDb );
			// rest if already old ids exist
			contactIds->Reset();
			TContactIter iterator(*contactDb);
			TContactItemId contactId = iterator.FirstL();	
			while(contactId != KNullContactId)
				{
				TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL() in side while") );   
				// store into iContactIds for later deletion of contacts
				contactIds->AddL(contactId );
				// get the next contactids
				contactId = iterator.NextL();	
				}
			contactDb->DeleteContactsL(*contactIds);
			// reset old ids
			contactIds->Reset();
			CleanupStack::PopAndDestroy( contactDb );
			}
		CleanupStack::PopAndDestroy( contactIds );
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL() end") );   
	return count;	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ResetAndDestroyRArray
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ResetAndDestroyRArray()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ResetAndDestroyRArrayL() begin") );
    TInt count = iPendingAvatars.Count();
    while( count )
        {
        TVIMPSTAvatarData contactInf = iPendingAvatars[ 0 ];
        iPendingAvatars.Remove(0 );
        delete contactInf.iAvatarContent;
        contactInf.iAvatarContent = NULL;  
        delete contactInf.iContactLink;
        contactInf.iContactLink = NULL;                      
         // not exist need to add 
        iPendingAvatars.Compress(); 
        count = iPendingAvatars.Count();
        }
    iPendingAvatars.Reset();
    TRACE( T_LIT("CVIMPSTStorageVPbkStoreHandler::ResetAndDestroyRArrayL() end") );
    }
// End of file
