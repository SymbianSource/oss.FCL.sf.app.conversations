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

#include "uiservicetabtracer.h"
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
	TRACER_AUTO;
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ConstructL(const TDesC& aStoreName,const TDesC& aServiceName )
    { 
	TRACER_AUTO;
    iAddIdInProgress = NULL;
    iStoreName = aStoreName.AllocL();
    iServiceName = aServiceName.AllocL();
    CVPbkContactStoreUriArray* uriArray = CVPbkContactStoreUriArray::NewLC();
    uriArray->AppendL( TVPbkContactStoreUriPtr( aStoreName ) );    
    if( !uriArray->IsIncluded( TVPbkContactStoreUriPtr( KPhoneBookDbURI ) ) )
	    {
	    TRACE( "phonebook store apended" );
	    uriArray->AppendL( TVPbkContactStoreUriPtr( KPhoneBookDbURI )   );	
	    }
    iContactManager = CVPbkContactManager::NewL( *uriArray, NULL );
    CleanupStack::PopAndDestroy( uriArray );
	TRACE( "iContactManager created" );
    iStores = &iContactManager->ContactStoresL();
    iStores->OpenAllL( *this );
    TRACE( " OpenAllL called" );
    iSortOrderManager = CPbk2SortOrderManager::NewL( iContactManager->FieldTypes(), NULL ); 
   
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
	TRACER_AUTO;
	CVIMPSTStorageVPbkStoreHandler* self = NewLC( aContactDb ,aServiceName, aStoreHandler,aLocalStore );
	CleanupStack::Pop( self );
	
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
	TRACER_AUTO;
    CVIMPSTStorageVPbkStoreHandler* self =
        new (ELeave) CVIMPSTStorageVPbkStoreHandler( aStoreHandler ,aLocalStore );
    CleanupStack::PushL( self );
    self->ConstructL( aContactDb,aServiceName );
   
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::
//      ~CVIMPSTStorageVPbkStoreHandler
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkStoreHandler:: ~CVIMPSTStorageVPbkStoreHandler()
    {
	TRACER_AUTO;
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
        TRACE( "CloseAll called" );
        delete iContactManager;
        }
    delete iStoreName;
    delete iSortOrderManager;
    delete iServiceName;
   
    }
 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactViewReady
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactViewReady( 
    MVPbkContactViewBase& aView )
    {
	TRACER_AUTO;
    if ( iContactView == &aView && ( iStoreEventType == EVPbkUnknownEvent ) )
        { 
        iStoreEventType = EVPbkContactReading ;          
        TRAP_IGNORE( HandleContactViewReadyL( *iContactView ) );
        }
   
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactViewUnavailable
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactViewUnavailable( 
    MVPbkContactViewBase& /*aView*/ )
    {
	TRACER_AUTO;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactAddedToView
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactAddedToView( 
    MVPbkContactViewBase& /*aView*/, 
    TInt /*aIndex*/, const MVPbkContactLink& /*aContactLink*/ )
    {
	TRACER_AUTO;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactRemovedFromView
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactRemovedFromView( 
    MVPbkContactViewBase& /*aView*/, 
    TInt /*aIndex*/, const MVPbkContactLink& /*aContactLink*/ )
    {
	TRACER_AUTO;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactViewError
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactViewError( 
    MVPbkContactViewBase& /*aView*/, 
    TInt /*aError*/, TBool /*aErrorNotified*/ )
    {
	TRACER_AUTO;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::StoreReady
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::StoreReady( 
    MVPbkContactStore& /*aContactStore*/ )
    {
	TRACER_AUTO;
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::StoreUnavailable
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::StoreUnavailable( 
    MVPbkContactStore& /*aContactStore*/, 
    TInt /*aReason*/ )
    {
	TRACER_AUTO;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::HandleStoreEventL
// ---------------------------------------------------------------------------
//
MVPbkContactStore* CVIMPSTStorageVPbkStoreHandler::GetDefaultStoreL( const TDesC& aUri )
	 {
	TRACER_AUTO;
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
	TRACER_AUTO;
    if( iStore == &aContactStore )
	    {
	    TRACE( "store event" );
	  	switch( aStoreEvent.iEventType )
		    {
		    case TVPbkContactStoreEvent::EContactAdded:
			    {
			    TRACE( "EContactAdded event");
			    delete iContactLink;
			    iContactLink = NULL;
				MVPbkContactLink* contactLink = aStoreEvent.iContactLink->CloneLC();
			 	iContactLink = 	contactLink;
				CleanupStack::Pop(); // contactLink
				IssueRequest(); // complete the request	
				TRACE( " EContactAdded done" );
			    break;	
			    }
			case TVPbkContactStoreEvent::EContactDeleted:
			    {
			    TRACE( "EContactDeleted event" );
			   	MVPbkContactLink* contactLink = aStoreEvent.iContactLink->CloneLC();
			   	delete iContactLink;
		    	iContactLink = NULL;
		 		iContactLink = 	contactLink;
		 		CleanupStack::Pop(); // contactLink
				TVIMPSTVPbkStoreEvent storeEvent(EVPbkContactDeleted, iContactLink );
		    	iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;	
		    	TRACE( "EContactDeleted done" );
			    break;	
			    }
			case TVPbkContactStoreEvent::EContactChanged:
				{
				TRACE( " EContactChanged event" );
				delete iContactLink;
				iContactLink = NULL;
				MVPbkContactLink* contactLink = aStoreEvent.iContactLink->CloneLC();
				iContactLink = 	contactLink;
				CleanupStack::Pop(); // contactLink
				iStoreEventType = EVPbkContactChanged;
				IssueRequest(); // complete the request which will begin retriving the iContactLink
				TRACE("EContactChanged done");
				break;	
				}
			case TVPbkContactStoreEvent::EUnknownChanges:
			    {
			    TRACE( " EUnknownChanges event" );
			    iStoreEventType = EVPbkUnknownChanges;
			    TVIMPSTVPbkStoreEvent storeEvent(iStoreEventType);
			    iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;
			    TRACE( "EUnknownChanges done" );
			    break;	
			    }
		    default:
			 	break;
		    }
	    }
   
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::OpenComplete
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::OpenComplete()
    {
	TRACER_AUTO;
    // stores OpenAll completed
    TRAP_IGNORE( DoHnadleOpenCompleteL() );
   
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::DoHnadleOpenCompleteL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::DoHnadleOpenCompleteL()
    {
	TRACER_AUTO;
    iStore = GetDefaultStoreL( *iStoreName ) ;
    CVPbkContactViewDefinition* viewDefinition = 
        				CVPbkContactViewDefinition::NewL();
    CleanupStack::PushL( viewDefinition );    
    viewDefinition->SetType( EVPbkContactsView );
    viewDefinition->SetUriL( *iStoreName );
    if ( iStore )
        {
        TRACE( "store view creating");
        MVPbkContactView* tmpView = iStore->CreateViewLC(
						            *viewDefinition,
						            *this,
						            iSortOrderManager->SortOrder() );        
        CleanupStack::Pop(); // tmpView            
        TRACE( "store view created");        
        iContactView = tmpView;
        }
    CleanupStack::PopAndDestroy();  //viewDefinition, can not pop ny name
    
    }
	
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactOperationCompleted
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactOperationCompleted(
    TContactOpResult /*aResult*/ )
    {
	TRACER_AUTO;
    // called when contact commit is successful
    iOperationError = KErrNone;
    if( iWait.IsStarted() )
	    {
	    iWait.AsyncStop();	
	    }
    TRAP_IGNORE( CommitNextPendingContactsL() );
	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ContactOperationFailed
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ContactOperationFailed( 
    TContactOp /*aOpCode*/, TInt aErrorCode, TBool /*aErrorNotified*/ )
    {
	TRACER_AUTO;
    // called when contact commit is unsuccessful
    iOperationError = aErrorCode;
    if( iWait.IsStarted() )
	    {
	    iWait.AsyncStop();	
	    }
    TRAP_IGNORE( CommitNextPendingContactsL() );
	
    }    
 
 
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::StepComplete
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::StepComplete(
        MVPbkContactOperationBase& /*aOperation*/,
        TInt /*aStepSize*/ )
    {
	TRACER_AUTO;
    // called when batch operation call
    iOperationError = KErrNone;
    
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
	TRACER_AUTO;
    // called when batch operation call
    iOperationError = aError;
   
   	return EFalse;
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::OperationComplete
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::OperationComplete(
        MVPbkContactOperationBase& aOperation )
    {
	TRACER_AUTO;
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
	
    }   

//----------------------------------------------------------------------
//CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationComplete()
//----------------------------------------------------------------------
void CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationComplete( MVPbkContactOperationBase& aOperation,
                                            				MVPbkStoreContact* aContact )
    {  
	TRACER_AUTO;
    // called when contact details is retrived
    TRAP_IGNORE( DoHnadleSingleContactOperationCompleteL( aOperation, aContact ) );
    
    }

//----------------------------------------------------------------------
//CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL()
//----------------------------------------------------------------------
void CVIMPSTStorageVPbkStoreHandler::DoHnadleSingleContactOperationCompleteL( MVPbkContactOperationBase& aOperation,
                                            								 MVPbkStoreContact* aContact )
    {  
	TRACER_AUTO;
    TRACE( "StoreEventType: %d",iStoreEventType);
    // called when contact details is retrived
    if( iStoreContact )
        {
        delete 	iStoreContact;
        iStoreContact = NULL;
        }
    iStoreContact = aContact ; // iStoreContact  owns thea Contact
    if( ( &aOperation == iContactOperation) && iStoreContact )
        {
        TRACE( "inside if (operation) ");
        if( iWait.IsStarted() )
		    {
		    iWait.AsyncStop();
		    }
        delete iContactOperation;
        iContactOperation = NULL;
        TRACE( " before iCommitOperationInProgress: %d",iCommitOperationInProgress);
        iCommitOperationInProgress = EFalse;
        TRACE( "After iCommitOperationInProgress: %d",iCommitOperationInProgress);

        if( iStoreEventType != EVPbkUpdatingAvatar )
            {
            TRACE( " inside if (iStoreEventType != EVPbkUpdatingAvatar ) ");
            if( !iLocalStore && iStoreEventType == EVPbkContactRetriving )
                {
                TRACE( " EVPbkContactRetriving");
		    	RetriveFieldDataAndSendEventL( *iStoreContact );
			   }
            else 
                {
                TRACE( "inside else start");
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
                TRACE( "iStoreHandler.HandleVPbkStoreEventL: %d", iStoreEventType);
                TPtr useridPtr = userId->Des();
                TRACE( "inside else UserId : %S", &useridPtr);
                TVIMPSTVPbkStoreEvent storeEvent(iStoreEventType,
                        iContactLink,
                        NULL,
                        *userId,
                        *disname,
                        *avatarContent);
                iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;
                CleanupStack::PopAndDestroy(5); // disname,lastname,firstname,userId,avatarContent 
                TRACE( " inside else end");
                }
            delete iAddIdInProgress;
            iAddIdInProgress = NULL;
            }

        iOperationError = KErrNone;
        if( iStoreEventType == EVPbkContactReading && iContactView )
            {
            TRACE( " if after iOperationError");
            HandleContactViewReadyL( *iContactView );   
            }
        
        else if(!iCommitOperationInProgress )
            {
            TRACE( "else if after if after iOperationError");
            CommitNextPendingContactsL();// commit only if any pending contact
            }
       
        }
    }
//----------------------------------------------------------------------
//CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationComplete()
//----------------------------------------------------------------------
void CVIMPSTStorageVPbkStoreHandler::VPbkSingleContactOperationFailed( MVPbkContactOperationBase& aOperation,
                                                                   TInt aError)
    {
	TRACER_AUTO;
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
	
    }
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::HandleContactViewReadyL( MVPbkContactViewBase& aView  )
	{
	TRACER_AUTO;
	TInt contactCount = aView.ContactCountL();
	TRACE( "begin");
	if( contactCount == iFetcingIndex  && iStoreEventType == EVPbkContactReading )
		{
		TRACE( "reading completed" );
		iStoreEventType = EVPbkContactReadCompleted;
		TVIMPSTVPbkStoreEvent storeEvent(iStoreEventType);
		iStoreHandler.HandleVPbkStoreEventL( storeEvent ) ;
		}
	else if( iFetcingIndex < contactCount && iFetcingIndex>=0 )
		{
		TRACE( " reading");
		const MVPbkViewContact& contact =  aView.ContactAtL( iFetcingIndex );
		MVPbkContactLink* contactLink = contact.CreateLinkLC();
		delete iContactLink;
		iContactLink = NULL;
		iContactLink = 	contactLink;
		CleanupStack::Pop(); // contactLink
		IssueRequest();
		iFetcingIndex++;
		}
	
	}
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::IssueRequest
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::IssueRequest()
    {
	TRACER_AUTO;
    if( !IsActive() )
	    {
	    TRequestStatus* status = &iStatus;
	    User::RequestComplete( status, KErrNone );
	    SetActive();
	    }
	
    }
	
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RunL
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::RunL()
    {
	TRACER_AUTO;
    if( iContactLink )
	    {
	    delete iContactOperation;
	    iContactOperation = NULL;
	    iContactOperation = iContactManager->RetrieveContactL( *iContactLink, *this );	
	    }
	
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::DoCancel
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::DoCancel()
    {
	TRACER_AUTO;
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RunError
// --------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkStoreHandler::RunError( TInt aError )
    {  
	TRACER_AUTO;
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
	TRACER_AUTO;
	TRACE( "aUserId = %S", &aUserId );
	
	if( iAddIdInProgress && iAddIdInProgress->Compare( aUserId ) == 0 )
		{
		TPtr iAddIdInProgressPtr = iAddIdInProgress->Des();
		TRACE("iAddIdInProgressPtr = %S", &iAddIdInProgressPtr );
		TRACE("already Adding" );
		return KErrAlreadyExists;	
		}
	TRACE( "After If" );
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
			TRACE( " valid xsp Id" );
			TInt prefixLocation = aUserId.Locate( ':' );
			const MVPbkContactStoreProperties& storeProperties =  storedContact->ParentStore().StoreProperties();
			const MVPbkFieldTypeList& supportedFieldTypes = storeProperties.SupportedFields();
			TInt fieldCount = supportedFieldTypes.FieldTypeCount();

			for( TInt index = 0 ; index < fieldCount ; index++ )
				{        
				const MVPbkFieldType* fieldType = &(supportedFieldTypes.FieldTypeAt(index));
				if ( fieldType && ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_IMPP ))
					{
					TRACE(" XSP Id-IMMP field creating" );
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
					TRACE( " XSP Id-IMMP field created" );
					}//end of if
				else if ( fieldType &&  
					( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_FIRSTNAME ))
					{
					TRACE( " first name field creating" );
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

					TRACE( "first name field created" );                    
					}//end of if.
				}//end of for.
			TRACE( "iCommitOperationInProgress = %d",iCommitOperationInProgress );
			if( iCommitOperationInProgress )
			    {
			    TRACE("inside if" );
			    iUnCommittedVPbkContacts.AppendL( storedContact ); //takes ownership
			    CleanupStack::Pop(); //storedContact
			    }
			else
			    {
			    TRACE( "inside else");
			    iCommitOperationInProgress = ETrue;
			    if(iStoreContact)
			        {
			        delete iStoreContact;
			        iStoreContact = NULL;
			        }
			    TRACE( "contact fields added" );
			    iStoreContact = storedContact;
			    CleanupStack::Pop(); //storedContact
			    iStoreContact->CommitL(*this);
			    iStoreEventType = EVPbkContactAdded;
			    }
			TRACE( "contact commited");
			//Start the wait untill adding of new contact is complete
			}
		}
    //return the added contact 
    TRACE( "iOperationError =%d",iOperationError );
	      
	return iOperationError; 
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL
// ---------------------------------------------------------------------------
//  
void CVIMPSTStorageVPbkStoreHandler::CommitNextPendingContactsL() 
    {
	
	TRACER_AUTO;
    TRACE("iStoreEventType = %d",iStoreEventType );
    if( !iCommitOperationInProgress && iUnCommittedVPbkContacts.Count() && iStoreEventType != EVPbkUpdatingAvatar)
        {
        TRACE( " Commit next" );
        iCommitOperationInProgress = ETrue;
        if(iStoreContact)
            {
            delete iStoreContact;
            iStoreContact = NULL;
            }
        TRACE( "comming..." );
        iStoreContact = iUnCommittedVPbkContacts[0];
        iUnCommittedVPbkContacts.Remove(0);
        iUnCommittedVPbkContacts.Compress();
        iStoreContact->CommitL(*this);
        iStoreEventType = EVPbkContactSynchronizing;
        TRACE( " Commit called" );
        }
    else if(!iCommitOperationInProgress && iPendingAvatars.Count()  && iStoreEventType != EVPbkUpdatingAvatar)
        {
        TRACE( "Commit next pending avatar" );
        TVIMPSTAvatarData avatarData = iPendingAvatars[0];
        UpdateAvatarFieldDataL( *avatarData.iContactLink, *avatarData.iAvatarContent );
        iPendingAvatars.Remove( 0 );
        delete avatarData.iContactLink;
        avatarData.iContactLink = NULL;
        delete avatarData.iAvatarContent;
        avatarData.iAvatarContent = NULL;
        iPendingAvatars.Compress();
        }
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RemoveVPbkContactL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkStoreHandler::RemoveVPbkContactL(const CVPbkContactLinkArray& aContactLinkArray ) 
	{
	TRACER_AUTO;
	// text now holds the name (or first field of the contact) 
    delete iContactOperation;
    iContactOperation = NULL;
    iContactOperation = iContactManager->DeleteContactsL(aContactLinkArray, *this  );
    if( !iWait.IsStarted() )
	    {
	    iWait.Start();	
	    }
	TRACE( "iOperationError=%d",iOperationError ); 
	 
    return iOperationError;
  	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RetrieveVPbkContactL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkStoreHandler::RetrieveVPbkContactL(const TDesC8& aPackedLinks) 
	{
	TRACER_AUTO;  
	iStoreEventType = EVPbkContactRetriving;
	CVPbkContactLinkArray* linkArray = CVPbkContactLinkArray::NewLC(aPackedLinks, *iStores );
    // text now holds the name (or first field of the contact) 
    TInt count = linkArray->Count();
    TRACE( "count=%d",count );   
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
   
    return iOperationError;
    }
 
                
                
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkStoreHandler::UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
														    const TDesC8& aAvatartData )
    {
	TRACER_AUTO;
  
    // local variable caz cant push member variable to cleanup stack
    MVPbkContactLink* cloneLink = aContactLink.CloneLC();
    TRACE( "iCommitOperationInProgress: %d",iCommitOperationInProgress );  
    TRACE( "iUnCommittedVPbkContacts: %d",iUnCommittedVPbkContacts.Count() );
    if(iCommitOperationInProgress || iUnCommittedVPbkContacts.Count() || iStoreEventType == EVPbkUpdatingAvatar )
        {
        TRACE( "Inside if" ); 
        TVIMPSTAvatarData avatarData = {
                                       cloneLink,
                                       aAvatartData.AllocL()
                                      };
        iPendingAvatars.Append( avatarData );
        CleanupStack::Pop(); // cloneLink
        return KErrInUse;
        }
    iCommitOperationInProgress = ETrue;
    TRACE( "Inside after if" ); 
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
	TRACE( "Inside after if(avatardata.length)" ); 
	iStoreEventType = EVPbkContactChanged;
	TRACE( "Inside after if(avatardata.length ): %d",iStoreEventType ); 
	iStoreContact->CommitL(*this);
	//wait for delete Avatar operation to complete.
	if(!iWait.IsStarted() ) 
		{
		iWait.Start();	
		}
	
	return iOperationError;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RemoveAvatarFieldL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkStoreHandler::RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact)
	{
	TRACER_AUTO;
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
	TRACE( "iOperationError =%d",iOperationError );   
	
	return iOperationError;	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC()
// ---------------------------------------------------------------------------
// 
HBufC* CVIMPSTStorageVPbkStoreHandler::GetContactFieldDataLC(const MVPbkStoreContact& aContact,
												    		 TInt aFieldResId ) 
    {
	TRACER_AUTO;
    HBufC* retValue = NULL;
    const MVPbkStoreContactFieldCollection& fieldCollection = aContact.Fields();
	TInt fieldCount = fieldCollection.FieldCount();
    TRACE( " fieldCount%d",fieldCount );   
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
					TRACE(" Text" );   
					retValue = MVPbkContactFieldTextData::Cast(fieldData).Text().AllocLC();
					break;					
					}
				case EVPbkFieldStorageTypeUri:				
					{
					TRACE(" Uri ");   
					retValue = MVPbkContactFieldUriData::Cast(fieldData).Text().AllocLC();
					break;	
					}
				
				}//End switch
			if( aFieldResId == R_VPBK_FIELD_TYPE_IMPP && retValue )
				{
				TRACE( " IMPP fild found");   
				// TODO RAMC : Nikhil/Mukesh
				// in case of srver contacts it also find the first valid xsp id
				// not valid through NULL
				// which can be clarified 
				// earlier it show selection query in UI which one to use
				TPtrC scheme = MVPbkContactFieldUriData::Cast(fieldData).Scheme();
				if( scheme.Compare( *iServiceName ) == 0 )
					{
					TRACE( " actual xsp Id found");   
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
		TRACE( " NULL value");   
		// if NULL assign valid ptr
		retValue = KNullDesC().AllocLC();	
		}
	
	return retValue;// ownership is return to caller
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC()
// ---------------------------------------------------------------------------
// 
HBufC8* CVIMPSTStorageVPbkStoreHandler::GetContactBinaryFieldDataLC(const MVPbkStoreContact& aContact,
                                                             TInt aFieldResId ) 
    {
	TRACER_AUTO;  
    HBufC8* retValue = NULL;
    const MVPbkStoreContactFieldCollection& fieldCollection = aContact.Fields();
    TInt fieldCount = fieldCollection.FieldCount();
    TRACE( "fieldCount%d",fieldCount );   
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
                    TRACE( " Text" );
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
        TRACE( " NULL value");   
        // if NULL assign valid ptr
        retValue = KNullDesC8().AllocLC();   
        }
   
    return retValue;// ownership is return to caller
    }


// ---------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL
// 
// ---------------------------------------------------------		
void CVIMPSTStorageVPbkStoreHandler::RetriveFieldDataAndSendEventL(const MVPbkStoreContact& aContact ) 
	{
	TRACER_AUTO;  
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
                    TRACE( "Text" );   
                    retValue = MVPbkContactFieldTextData::Cast(fieldData).Text().AllocLC();
                    break;                  
                    }
				case EVPbkFieldStorageTypeUri:				
					{
					TRACE( "begin" ) ;   
					retValue = MVPbkContactFieldUriData::Cast(fieldData).Text().AllocLC();
					break;	
					}
				
				}//End switch
			if( retValue )
				{
				TRACE( " begin" );   
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
	 
	}

// ---------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL
// 
// ---------------------------------------------------------		
TInt CVIMPSTStorageVPbkStoreHandler::RemoveAllVPbkContactsL()
	{
	TRACER_AUTO; 
	TRACE( " before if" ); 
	if(iAddIdInProgress)
	    {
	    TPtr iAddIdInProgressPtr = iAddIdInProgress->Des();
	    TRACE( "iAddIdInProgressPtr  = %S", &iAddIdInProgressPtr ); 
	    delete iAddIdInProgress;
	    iAddIdInProgress = NULL;
	    }
	// intialize it to efalse as, somewhere it would have missed.
	iCommitOperationInProgress = EFalse;
	TRACE( " After if"); 
	TInt count = 0;
	if( iContactView )
		{
		count = iContactView->ContactCountL();	
		}
	TRACE( "count = %d",count) ;   
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
				TRACE( "in side while" );   
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
	
	return count;	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkStoreHandler::ResetAndDestroyRArray
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkStoreHandler::ResetAndDestroyRArray()
    {
	TRACER_AUTO;
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
   
    }
// End of file
