/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
:                
*
*/




// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <cntdb.h>
#include <cntitem.h>
//#include <cntmodel.h>
#include <cntfldst.h>
#include <cntfield.h>
//#include <cvpbkcontactmanager.h> 

#include "vimpststorage_test.h"

#include "cvimpststoragemanager.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststoragecontacts.h"
#include "mvimpststorageitemmodel.h"
#include "cvimpststoragecontact.h"
#include "cvimpststoragecontactlist.h"
#include "bamdesca.h"
#include "cvimpststoragevpbkstorehandler.h"
#include "CVIMPSTStorageViewId.h"
#include "CVIMPSTStorageContactSorter.h"
#include "MVIMPSTStorageContact.h"

#include "s_MVPbkContactLinkstub.h"
#include <CVPbkContactStoreUriArray.h>
#include <CVPbkContactManager.h>
#include "CVPbkStoreContactStub.h"
#include "CVPbkContactOperationBaseStub.h"

_LIT( KIMContactListId,"IMContacts" );
//_LIT( KVPContactListId,"VPContacts" );
_LIT( KStoreName, "TestStore.cdb" );
_LIT( KServiceName, "TestService" )	;
_LIT( KDisplayname, "my contact list" );
//_LIT( KVPDisplayname, "Voip list" );
_LIT( KUserId, "id3@dummydomain.com" );
const TUint32 serviceId(3);

const TInt KNumberGranularity( 12 );

const TInt ProcessInterval = 10000000; //10 seconds

//extern
extern TInt gGiveFieldCount;
extern TBool gBestMatchingFieldType;

extern CVPbkContactOperationBaseStub* gOperationStub;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cvimpststorage_test::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cvimpststorage_test::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cvimpststorage_test::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cvimpststorage_test::RunMethodL( 
    CStifItemParser& aItem ) 
    {

	//first of all create database.
    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "InitialiseLibraryTest", Cvimpststorage_test::InitialiseLibraryTestL ),
        ENTRY( "InitialiseViewTest", Cvimpststorage_test::InitialiseViewTestL ),
        ENTRY( "CVIMPSTStorageContactTestCase1L", Cvimpststorage_test::CVIMPSTStorageContactTestCase1L ),
        ENTRY( "CVIMPSTStorageContactTestCase2L", Cvimpststorage_test::CVIMPSTStorageContactTestCase2L ),
        ENTRY( "CVIMPSTStorageContactTestCase3L", Cvimpststorage_test::CVIMPSTStorageContactTestCase3L ),
        ENTRY( "CVIMPSTStorageContactTestCase4L", Cvimpststorage_test::CVIMPSTStorageContactTestCase4L ),        
        ENTRY( "CVIMPSTStorageContactTestCase6L", Cvimpststorage_test::CVIMPSTStorageContactTestCase6L ),
        ENTRY( "CVIMPSTStorageContactTestCase7L", Cvimpststorage_test::CVIMPSTStorageContactTestCase7L ),
        ENTRY( "CVIMPSTStorageContactTestCase8L", Cvimpststorage_test::CVIMPSTStorageContactTestCase8L ),
        ENTRY( "CVIMPSTStorageContactTestCase9L", Cvimpststorage_test::CVIMPSTStorageContactTestCase9L ),
        ENTRY( "CVIMPSTStorageContactTestCase10L", Cvimpststorage_test::CVIMPSTStorageContactTestCase10L ),
        
        ENTRY( "CVIMPSTStorageContactTestCase11L", Cvimpststorage_test::CVIMPSTStorageContactTestCase11L ),
        ENTRY( "CVIMPSTStorageContactTestCase12L", Cvimpststorage_test::CVIMPSTStorageContactTestCase12L ),
        ENTRY( "CVIMPSTStorageContactTestCase13L", Cvimpststorage_test::CVIMPSTStorageContactTestCase13L ),
        ENTRY( "CVIMPSTStorageContactTestCase14L", Cvimpststorage_test::CVIMPSTStorageContactTestCase14L ),
        ENTRY( "CVIMPSTStorageContactTestCase15L", Cvimpststorage_test::CVIMPSTStorageContactTestCase15L ),
        
        ENTRY( "CVIMPSTStorageContactListTestCase16L", Cvimpststorage_test::CVIMPSTStorageContactListTestCase16L ),
        ENTRY( "CVIMPSTStorageContactListTestCase17L", Cvimpststorage_test::CVIMPSTStorageContactListTestCase17L ),
        ENTRY( "CVIMPSTStorageContactListTestCase18L", Cvimpststorage_test::CVIMPSTStorageContactListTestCase18L ),
        ENTRY( "CVIMPSTStorageContactListTestCase19L", Cvimpststorage_test::CVIMPSTStorageContactListTestCase19L ),
        ENTRY( "CVIMPSTStorageContactListTestCase20L", Cvimpststorage_test::CVIMPSTStorageContactListTestCase20L ),
        ENTRY( "CVIMPSTStorageContactListTestCase21L", Cvimpststorage_test::CVIMPSTStorageContactListTestCase21L ),
        
        ENTRY( "CVIMPSTStorageXSPStoreHandlerTestCase01L", Cvimpststorage_test::CVIMPSTStorageXSPStoreHandlerTestCase01L ),
        
        
        
                       
         
                
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }
    
/**
 * Called when a contact is deleted.
 * @param aId The ID of the deleted contact
 * @param aFromPEC, ETrue, if event is coming from PEC
 */
 void Cvimpststorage_test::HandleContactDeleteL( const TDesC& /*aContactId*/ )
	 {
	 iObserverReceivedNotification = ETrue;	
	 }
 

/**
 * Called when a contact is added.
 * @param aList. List for contact that was added.
 * @param aContact The contact which was added
 * @param aFromPEC, ETrue, if event is coming from PEC
 */
 void Cvimpststorage_test::HandleAdditionL( MVIMPSTStorageContactList& /*aList*/, MVIMPSTStorageContact& /*aContact*/ )
 	 {
	 iObserverReceivedNotification = ETrue;	
	 }


/**
 * Called when a contact or list is about to change, or has been changed.
 * @param aList List that is changed. Can be NULL if contact is changed.
 * @param aContact Contact that is changed. Can be NULL if list is changed
 * @param aEventType The type of event: EStorageEventPreChange before
 * the change (contact still has the old information), or
 * EStorageEventPostChange after the change took place 
 * (contact has new information)
 */
 void Cvimpststorage_test::HandleChangeL( MVIMPSTStorageContactList* /*aList*/, MVIMPSTStorageContact* /*aContact*/,
        TVIMPSTEnums::TCAObserverEventType /*aEventType*/,
        TBool /*aUserIdChanged*/ )
	 {
	 iObserverReceivedNotification = ETrue;	
	 }

// -----------------------------------------------------------------------------
// Cvimpststorage_test::InitialiseLibraryTestL
// -----------------------------------------------------------------------------
//

TInt Cvimpststorage_test::InitialiseLibraryTestL( CStifItemParser& /*aItem*/  )
	{
	_LIT( KState, "State" );
    _LIT( KResult, "Initialisation successful" );   
/*
	CVIMPSTStorageManagerFactory::InitialiseLibraryL();	
	CVIMPSTStorageManagerFactory::Release();
*/
    // Test case passed    
    TestModuleIf().Printf( 1, KState, KResult );  
	    
    // Case was executed
    return KErrNone;
	}




// -----------------------------------------------------------------------------
// Cvimpststorage_test::InitialiseViewTestL
// -----------------------------------------------------------------------------
//
TInt  Cvimpststorage_test::InitialiseViewTestL( CStifItemParser& /*aItem*/  )
	{
	/*CVIMPSTStorageManagerFactory::InitialiseLibraryL();
		
	CVIMPSTStorageManagerFactory::InitialiseViewL(serviceId,KStoreName, KServiceName);	

	CVIMPSTStorageManagerFactory::Release();	
	  */  
    // Case was executed
    return KErrNone;		
	}
	
	
TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase1L()
	{
	
	
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
                                 NULL,
                                 NULL,                                 
                                 NULL,                                                                  
                                 NULL);

	if (contact)
		{		
		delete contact;
		return KErrNone ;
		}
	
	return KErrGeneral;	
	}


TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase2L()
	{
	//Ownership of contactLink is transferred to CVIMPSTStorageContact
	//hence dont delete
	MVPbkContactLink* contactLink = new (ELeave) CVPbkContactLinkStub();
	CVPbkContactStoreUriArray* uriArray = CVPbkContactStoreUriArray::NewL();
	CVPbkContactManager* contactMgr = CVPbkContactManager::NewL(*uriArray);
							
								
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
                                 contactLink,
                                 NULL,                                 
                                 contactMgr,                                                                  
                                 NULL);

	if (contact)
		{
		if (contactLink == contact->ContactLink())
			{
			delete contact;
			delete contactMgr;
			delete uriArray;		
			return KErrNone ;
			}
		}
	
	delete contactMgr;
	delete uriArray;
	delete contactLink;
	return KErrGeneral;	
	}
	
TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase3L()
	{
	//Ownership of contactLink is transferred to CVIMPSTStorageContact
	//hence dont delete
	MVPbkContactLink* contactLink = new (ELeave) CVPbkContactLinkStub();
							
								
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
                                 contactLink,
                                 NULL,                                 
                                 NULL,                                                                  
                                 NULL);

	if (contact)
		{	
		delete contact;
		return KErrNone ;
		}
	
	delete contactLink;
	return KErrGeneral;	
	}	
		
		
TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase4L()
	{	
	//TestCase for when there is no service field for a contact
	gGiveFieldCount = 0;							
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
                                 NULL,
                                 NULL,                                 
                                 NULL,                                                                  
                                 NULL);

	if (contact)
		{
		CVPbkStoreContactStub* storedContact = new (ELeave) CVPbkStoreContactStub();
		CleanupStack::PushL( storedContact );
		
		// Read all necessary data from contact and store to 
	    // own type
	    CDesCArray* arrayForService = new (ELeave) CDesCArrayFlat( 
	        KNumberGranularity );
	    CleanupStack::PushL( arrayForService );

		TInt err = contact->GetServiceFieldsL(storedContact, *arrayForService);
		
		contact->SetServiceFieldsL(*arrayForService);
		
		RPointerArray<HBufC> serviceDetails = contact->ServiceDetails();
			
		CleanupStack::PopAndDestroy(2); //storedContact, arrayForService	
		
		if ((err == KErrNotFound) && (0 == serviceDetails.Count()))
			{
			delete contact;
			return KErrNone ;
			}
		
		}	
	delete contact;
	return KErrGeneral;	
	}			

		
TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase6L()
	{	
	//TestCase for when there are some service field for a contact
	//but the BestMatchingFieldType() function returns NULL
	//Our code should not crash as this returns a Ptr value
	gGiveFieldCount = 4;
	gBestMatchingFieldType = EFalse;							
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
                                 NULL,
                                 NULL,                                 
                                 NULL,                                                                  
                                 NULL);

	if (contact)
		{
		CVPbkStoreContactStub* storedContact = new (ELeave) CVPbkStoreContactStub();
		CleanupStack::PushL( storedContact );
		
		// Read all necessary data from contact and store to 
	    // own type
	    CDesCArray* arrayForService = new (ELeave) CDesCArrayFlat( 
	        KNumberGranularity );
	    CleanupStack::PushL( arrayForService );

		TInt err = contact->GetServiceFieldsL(storedContact, *arrayForService);		
		contact->SetServiceFieldsL(*arrayForService);
		
		RPointerArray<HBufC> serviceDetails = contact->ServiceDetails();
		
		
		if ((0 == arrayForService->MdcaCount()) && (arrayForService->MdcaCount() == serviceDetails.Count()))
			{
			delete contact;
			CleanupStack::PopAndDestroy(2); //storedContact, arrayForService
			return KErrNone ;	
			}
		
		CleanupStack::PopAndDestroy(2); //storedContact, arrayForService
		}	
	delete contact;
	return KErrGeneral;	
	}
	
	
TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase7L()
	{
	_LIT(KTestCaseUserId, "testUser");									
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
                                 NULL,
                                 NULL,                                 
                                 NULL,                                                                  
                                 NULL);

	if (contact)
		{
		contact->SetUserIdL(KTestCaseUserId());
		
		if (0 == KTestCaseUserId().Compare(contact->UserId()))
			{
			delete contact;
			return KErrNone;	
			}
		}	
	
	delete contact;
	return KErrGeneral;	
	}			
	

TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase8L()
	{
	_LIT(KTestCaseUserId, "testUser");									
	
	iObserverReceivedNotification = EFalse;
	
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( this,
                                 NULL,
                                 NULL,                                 
                                 NULL,                                                                  
                                 NULL);

	if (contact)
		{
		//Initial Presence
		if (TVIMPSTEnums::EOffline != contact->OnlineStatus())
			{
			delete contact;
			return KErrGeneral;	
			}
		
		contact->SignalChanges();
		//Set the presence
		contact->SetOnlineStatus(TVIMPSTEnums::EOnline);
		if (TVIMPSTEnums::EOnline != contact->OnlineStatus())
			{
			delete contact;								
			return KErrGeneral;	
			}	
		contact->SignalChanges();
		contact->SetOnlineStatus(TVIMPSTEnums::EOnline);
		if (TVIMPSTEnums::EOnline != contact->OnlineStatus())
			{
			delete contact;			
			return KErrGeneral;	
			}	
		
		contact->SignalChanges();
		delete contact;
		
		if (!iObserverReceivedNotification)
			{
			return KErrGeneral;		
			}

		return KErrNone;
		}	
	
	delete contact;
	return KErrGeneral;	
	}		

TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase9L()
	{
	_LIT(KTestCaseStatusTestId, "status");									
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
                                 NULL,
                                 NULL,                                 
                                 NULL,                                                                  
                                 NULL);

	if (contact)
		{
		if (contact->StatusText().Length())
			{
			delete contact;
			return KErrGeneral;	
			}
		
		contact->SetStatusTextL(KTestCaseStatusTestId());
		
		if (0 == KTestCaseStatusTestId().Compare(contact->StatusText()))
			{
			delete contact;
			return KErrNone;	
			}
		}	
	
	delete contact;
	return KErrGeneral;	
	}			


TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase10L()
	{
	_LIT8(KTestCaseAvatarId, "avatar");	
	iObserverReceivedNotification = EFalse;								
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( this,
	                             NULL,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);

	if (contact)
		{
		if (!contact->AvatarContent().Length())
			{
			
			contact->SetAvatarIndex(3);
			
			if (3 == contact->AvatarIndex())
				{
				contact->iAvatarContent = KTestCaseAvatarId().AllocL();
				if (0 == KTestCaseAvatarId().Compare(contact->AvatarContent()))
					{
					contact->SignalAvatarChange();
					if (iObserverReceivedNotification)
						{
						delete contact;
						return KErrNone;					
						}
					}
				}
			}
		}
		
	delete contact;
	return KErrGeneral;		
	}

TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase11L()
	{
	iObserverReceivedNotification = EFalse;
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( this,
	                             NULL,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);
	CVPbkStoreContactStub* storedContact = new (ELeave) CVPbkStoreContactStub();
	CleanupStack::PushL( storedContact );
	CVPbkContactOperationBaseStub* operationBaseStub = NULL;
	if (contact)
		{
		TRAPD(err,contact->VPbkSingleContactOperationComplete(*operationBaseStub, storedContact));
		
		if ((KErrNone == err) && (storedContact==contact->VPbkStoreContact()))
			{
			CleanupStack::PopAndDestroy(); //storedContact
			delete contact;	
			return KErrNone;
			}
		}
	CleanupStack::PopAndDestroy(); //storedContact	
	delete contact;
	return KErrGeneral;		
	}


TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase12L()
	{
	iObserverReceivedNotification = EFalse;
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( this,
	                             NULL,
	                             NULL,                                 
	                             NULL,                                                                  
	                             this);
	
	CVPbkStoreContactStub* storedContact = new (ELeave) CVPbkStoreContactStub();
	CleanupStack::PushL( storedContact );
		
	if (contact)
		{
		TRAPD(err,contact->DoHandleVPbkSingleContactOperationCompleteL(storedContact));
		
		if (KErrNone == err)
			{
			CleanupStack::PopAndDestroy(); //storedContact
			delete contact;	
			return KErrNone;
			}
		}
	
	CleanupStack::PopAndDestroy(); //storedContact	
	delete contact;
	return KErrGeneral;		
	}


TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase13L()
	{
	iObserverReceivedNotification = EFalse;
	CVPbkStoreContactStub* storedContact = new (ELeave) CVPbkStoreContactStub();	
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( this,
	                             NULL,
	                             storedContact,                                 
	                             NULL,                                                                  
	                             this);
	if (contact)
		{
		contact->Commit();	
		
		TContactOpResult opResult;
		opResult.iOpCode = EContactCommit;
		opResult.iStoreContact = storedContact;
		opResult.iExtension = NULL;	
		
		contact->ContactOperationCompleted(opResult);
		
		opResult.iOpCode = EContactLock;
		contact->ContactOperationCompleted(opResult);		
		contact->ContactOperationFailed( 
    				opResult.iOpCode, KErrArgument, EFalse );
		
		contact->Commit(this, this);
		contact->ContactOperationCompleted(opResult);		
		contact->ContactOperationFailed( 
    				opResult.iOpCode, KErrArgument, EFalse );
		
		}
	
	
	delete contact;
	return KErrNone;		
	}



TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase14L()
	{
	iObserverReceivedNotification = EFalse;
	CVPbkStoreContactStub* storedContact = new (ELeave) CVPbkStoreContactStub();	
	MVPbkContactLink* contactLink = new (ELeave) CVPbkContactLinkStub();
	CVPbkContactStoreUriArray* uriArray = CVPbkContactStoreUriArray::NewL();
	CVPbkContactManager* contactMgr = CVPbkContactManager::NewL(*uriArray);
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( this,
	                             contactLink,
	                             storedContact,                                 
	                             contactMgr,                                                                  
	                             this);
	if (contact)
		{
		contact->Commit();	
		
		TContactOpResult opResult;
		opResult.iOpCode = EContactCommit;
		opResult.iStoreContact = storedContact;
		opResult.iExtension = NULL;	
		
		contact->ContactOperationCompleted(opResult);
		
		opResult.iOpCode = EContactLock;
		contact->ContactOperationCompleted(opResult);		
		contact->ContactOperationFailed( 
    				opResult.iOpCode, KErrArgument, EFalse );
		
		contact->Commit(this, this);
		contact->ContactOperationCompleted(opResult);		
		contact->ContactOperationFailed( 
    				opResult.iOpCode, KErrArgument, EFalse );
		
		}
	delete uriArray;
	delete contactMgr;	
	delete contact;
	return KErrNone;		
	}
	    								

TInt Cvimpststorage_test::CVIMPSTStorageContactTestCase15L()
	{
	
	CVPbkStoreContactStub* storedContact = new (ELeave) CVPbkStoreContactStub();	
	MVPbkContactLink* contactLink = new (ELeave) CVPbkContactLinkStub();
	CVPbkContactStoreUriArray* uriArray = CVPbkContactStoreUriArray::NewL();
	CVPbkContactManager* contactMgr = CVPbkContactManager::NewL(*uriArray);
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( this,
	                             contactLink,
	                             storedContact,                                 
	                             contactMgr,                                                                  
	                             this);	
	                             
	CVPbkContactOperationBaseStub* operationBaseStub = NULL;	                             
	if (contact)
		{
		
		contact->VPbkSingleContactOperationFailed(*operationBaseStub, KErrArgument);
		
		contact->iContactOperationObserver = this;		
		
		contact->VPbkSingleContactOperationFailed(*gOperationStub, KErrArgument);
		
		}
	delete uriArray;
	delete contactMgr;	
	delete contact;
	return KErrNone;		
	}
 	    								

TInt Cvimpststorage_test::CVIMPSTStorageContactListTestCase16L()
	{	
	_LIT(KContactListId, "listid");
	_LIT(KContactListName, "listName");
	_LIT(KContactName, "contact");	
	_LIT(KContactName1, "contact1");
	_LIT(KContactName2, "contact2");
	_LIT(KContactName3, "contact3");
	_LIT(KContactNotFound, "contactNotFound");	
		
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
	                             NULL,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact->SetUserIdL(KContactName());	                             	
	
	
	CVIMPSTStorageContact* contact1 = CVIMPSTStorageContact::NewL( NULL,
	                             NULL,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact1->SetUserIdL(KContactName1());	                             		
	
	CVIMPSTStorageContact* contact2 = CVIMPSTStorageContact::NewL( NULL,
	                             NULL,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact2->SetUserIdL(KContactName2());	                             		                             
	
	CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL() ;
	CleanupStack::PushL(sorter);
	
	sorter->SetSortAlgorithm(MVIMPSTStorageContact::ECompareAlphabetically);
	CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *this,
                                      *sorter,
                                      KContactListId(), 
                                      KContactListName() );	                                                     	                             
	if (list)
		{
		MVIMPSTStorageContact* cnt = NULL;		
		cnt = list->AddContactL(contact);
		if (cnt)
			{			
			//add same contact 
			cnt = list->AddContactL(contact)	;				
			if (cnt == contact)
				{
				cnt = list->AddContactL(contact1);					
				cnt = list->AddContactL(contact2);									
				
				//search for contacts based on Id
				cnt = list->FindContact(KContactName());
				MVIMPSTStorageContact* cnt1(list->FindContact(KContactName1()));
				MVIMPSTStorageContact* cnt2(list->FindContact(KContactName2()));
				MVIMPSTStorageContact* cnt3(list->FindContact(KContactNotFound()));
				
				sorter->SetSortAlgorithm(MVIMPSTStorageContact::ECompareByPresence);
				
				if ( (cnt == contact) && 
					(cnt1 == contact1) &&
					(cnt2 == contact2) &&
					(NULL == cnt3) )
					{
					TInt err = list->RemoveContact(KContactNotFound());
					TInt err1 = list->RemoveContact(KContactName2());
					TInt err2 = list->RemoveContact(KContactName1());
					TInt err3 = list->RemoveContact(KContactName());
					
					if ( (KErrNotFound == err) &&
						(KErrNone == err1) &&
						(KErrNone == err2) &&
						(KErrNone == err3)
						)
						{
						delete list;
						CleanupStack::PopAndDestroy(1);	
						return KErrNone;						
						}
											
					}
				}
			
			}
		}
	CleanupStack::PopAndDestroy(1);	
	return KErrGeneral;		
	}
	
	
TInt Cvimpststorage_test::CVIMPSTStorageContactListTestCase17L()
	{	
	_LIT(KContactListId, "listid");
	_LIT(KContactListName, "listName");
	_LIT(KContactName, "contact");	
	_LIT(KContactName1, "contact1");
	_LIT(KContactName2, "contact2");
	_LIT(KContactName3, "contact3");
	_LIT(KContactNotFound, "contactNotFound");
	
	MVPbkContactLink* contactLink = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact->SetUserIdL(KContactName());	                             	
	
	MVPbkContactLink* contactLink1 = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact1 = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink1,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact1->SetUserIdL(KContactName1());	                             	
	
	MVPbkContactLink* contactLink2 = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact2 = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink2,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact2->SetUserIdL(KContactName2());	                             		                             
	
	CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL() ;
	CleanupStack::PushL(sorter);
	
	MVPbkContactLink* contactLinkNotFound = new (ELeave) CVPbkContactLinkStub();
	
	CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *this,
                                      *sorter,
                                      KContactListId(), 
                                      KContactListName() );	                                                     	                             
	if (list)
		{
		MVIMPSTStorageContact* cnt = NULL;		
		cnt = list->AddContactL(contact);
		if (cnt)
			{			
			//add same contact 
			cnt = list->AddContactL(contact)	;				
			if (cnt == contact)
				{
				cnt = list->AddContactL(contact1);					
				cnt = list->AddContactL(contact2);									
				
				//search for contacts based on Id
				cnt = list->FindContact(*contactLink);
				MVIMPSTStorageContact* cnt1(list->FindContact(*contactLink1));
				MVIMPSTStorageContact* cnt2(list->FindContact(*contactLink2));
				MVIMPSTStorageContact* cnt3(list->FindContact(*contactLinkNotFound));
				
				if ( (cnt == contact) && 
					(cnt1 == contact1) &&
					(cnt2 == contact2) &&
					(NULL == cnt3) )
					{
					TInt err = list->RemoveContact(*contactLinkNotFound);
					TInt err1 = list->RemoveContact(*contactLink2);
					TInt err2 = list->RemoveContact(*contactLink1);
					TInt err3 = list->RemoveContact(*contactLink);
					
					if ( (KErrNotFound == err) &&
						(KErrNone == err1) &&
						(KErrNone == err2) &&
						(KErrNone == err3)
						)
						{
						delete list;
						CleanupStack::PopAndDestroy(1);	
						return KErrNone;						
						}
											
					}
				}
			
			}
		}
	CleanupStack::PopAndDestroy(1);	
	return KErrGeneral;		
	}	


TInt Cvimpststorage_test::CVIMPSTStorageContactListTestCase18L()
	{	
	_LIT(KContactListId, "listid");
	_LIT(KContactListName, "listName");
	_LIT(KContactName, "contact");	
	_LIT(KContactName1, "contact1");
	_LIT(KContactName2, "contact2");
	_LIT(KContactName3, "contact3");
	_LIT(KContactNotFound, "contactNotFound");
	
	MVPbkContactLink* contactLink = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);		
	
	CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL() ;
	CleanupStack::PushL(sorter);
	
	MVPbkContactLink* contactLinkNotFound = new (ELeave) CVPbkContactLinkStub();
	
	CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *this,
                                      *sorter,
                                      KContactListId(), 
                                      KContactListName() );	                                                     	                             
	if (list)
		{
		MVIMPSTStorageContact* cnt = NULL;		
		cnt = list->AddContactL(contact);
		if (cnt)
			{			
			//Pop the contact 
			cnt = list->PopContact(*contactLink)	;				
			if (cnt == contact)
				{								
				MVIMPSTStorageContact* cnt3(list->PopContact(*contactLinkNotFound));
				
				if ( (cnt == contact) && 					
					(NULL == cnt3) )
					{
					delete list;
					delete contact;
					CleanupStack::PopAndDestroy(1);
					return KErrNone;																	
					}
				}
			
			}
		}
	CleanupStack::PopAndDestroy(1);	
	return KErrGeneral;		
	}
	
	
TInt Cvimpststorage_test::CVIMPSTStorageContactListTestCase19L()
	{	
	_LIT(KContactListId, "listid");
	_LIT(KContactListName, "listName");
	_LIT(KContactName, "contact");	
	_LIT(KContactName1, "contact1");
	_LIT(KContactName2, "contact2");
	_LIT(KContactName3, "contact3");
	_LIT(KContactNotFound, "contactNotFound");
	
	MVPbkContactLink* contactLink = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact->SetUserIdL(KContactName());	                             	
	contact->SetOnlineStatus(TVIMPSTEnums::EOnline);
	
	MVPbkContactLink* contactLink1 = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact1 = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink1,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact1->SetUserIdL(KContactName1());	                             	
	contact1->SetOnlineStatus(TVIMPSTEnums::EOffline);
	
	MVPbkContactLink* contactLink2 = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact2 = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink2,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact2->SetUserIdL(KContactName2());	                             		                             
	contact2->SetOnlineStatus(TVIMPSTEnums::EBusy);
	
	MVPbkContactLink* contactLink3 = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact3 = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink3,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact3->SetUserIdL(KContactName3());	                             		                             
	contact3->SetOnlineStatus(TVIMPSTEnums::EAway);
	
	CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL() ;
	CleanupStack::PushL(sorter);
	
	MVPbkContactLink* contactLinkNotFound = new (ELeave) CVPbkContactLinkStub();
	
	CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *this,
                                      *sorter,
                                      KContactListId(), 
                                      KContactListName() );	                                                     	                             
	if (list)
		{
			
		list->AddContactL(contact);
		list->AddContactL(contact1);
		list->AddContactL(contact2);
		list->AddContactL(contact3);;
		
		MVIMPSTStorageContact* test = &list->operator[](0);
		if ( test && list->Count() == 4)
			{
			if (list->OnlineCount() == 3)					
				{
				delete list;
				CleanupStack::PopAndDestroy(1);
				return KErrNone;					
				}
			}
		}
	CleanupStack::PopAndDestroy(1);	
	return KErrGeneral;		
	}	


TInt Cvimpststorage_test::CVIMPSTStorageContactListTestCase20L()
	{	
	_LIT(KContactListId, "listid");
	_LIT(KContactListName, "listName");
	_LIT(KTestCase20ListId, "meco");
	_LIT(KTestCase20ListName, "Meco display");
	CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL() ;
	CleanupStack::PushL(sorter);
	
	CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *this,
                                      *sorter,
                                      KContactListId(), 
                                      KContactListName() );	                                                     	                             
	if (list)
		{
		
		if (0 == KContactListId().Compare(list->ListId()))
			{
			if (0 == KContactListName().Compare(list->DisplayName()))
				{
				
				list->SetDisplayNameL(KTestCase20ListName());
				list->SetListIdL(KTestCase20ListId());
				
				if ( (0 == KTestCase20ListId().Compare(list->ListId())) &&
					(0 == KTestCase20ListName().Compare(list->DisplayName())) )
					{
					CleanupStack::PopAndDestroy(1);	
					delete list;
					return KErrNone;						
					}
				}
			}
		}
	CleanupStack::PopAndDestroy(1);	
	return KErrGeneral;		
	}


TInt Cvimpststorage_test::CVIMPSTStorageContactListTestCase21L()
	{	
	_LIT(KContactListId, "listid");
	_LIT(KContactListName, "listName");
	_LIT(KContactName, "contact");	
	_LIT(KContactName1, "contact1");
	_LIT(KContactName2, "contact2");
	_LIT(KContactName3, "contact3");
	_LIT(KContactNotFound, "contactNotFound");
	
	MVPbkContactLink* contactLink = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact->SetUserIdL(KContactName());	                             	
	contact->SetOnlineStatus(TVIMPSTEnums::EOnline);
	
	MVPbkContactLink* contactLink1 = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact1 = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink1,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact1->SetUserIdL(KContactName1());	                             	
	contact1->SetOnlineStatus(TVIMPSTEnums::EOffline);
	
	MVPbkContactLink* contactLink2 = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact2 = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink2,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact2->SetUserIdL(KContactName2());	                             		                             
	contact2->SetOnlineStatus(TVIMPSTEnums::EBusy);
	
	MVPbkContactLink* contactLink3 = new (ELeave) CVPbkContactLinkStub();	
	CVIMPSTStorageContact* contact3 = CVIMPSTStorageContact::NewL( NULL,
	                             contactLink3,
	                             NULL,                                 
	                             NULL,                                                                  
	                             NULL);	
	contact3->SetUserIdL(KContactName3());	                             		                             
	contact3->SetOnlineStatus(TVIMPSTEnums::EAway);
	
	CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL() ;
	CleanupStack::PushL(sorter);
	
	MVPbkContactLink* contactLinkNotFound = new (ELeave) CVPbkContactLinkStub();
	
	CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *this,
                                      *sorter,
                                      KContactListId(), 
                                      KContactListName() );	                                                     	                             
	if (list)
		{
			
		list->AddContactL(contact);
		list->AddContactL(contact1);
		list->AddContactL(contact2);	
		
		list->Sort();
		contact1->SetOnlineStatus(TVIMPSTEnums::EOnline);
		list->ResortContact(contact1);	
		contact1->SetOnlineStatus(TVIMPSTEnums::EAway);	
		list->ResortUnKnownContact(contact1);
		
		MVIMPSTStorageContact* cnt = &(list->OnlineContact( 1 ));
		TInt index = list->FindIndexOfContact( cnt, TVIMPSTEnums::EFilterAll );
				
		if (index == 1)
			{
			delete list;		
			CleanupStack::PopAndDestroy(1);	
			return KErrNone;				
			}
		}
	delete list;		
	CleanupStack::PopAndDestroy(1);	
	return KErrGeneral;		
	}	



TInt Cvimpststorage_test::CVIMPSTStorageXSPStoreHandlerTestCase01L()
	{	
	_LIT(KStoreName, "MecoStore");
	_LIT(KServiceName, "MecoService");
	
	CVIMPSTStorageViewId* store = CVIMPSTStorageViewId::NewL(1,
										KStoreName(),
										KServiceName());
	//Create XSP Store Handler
    CVIMPSTStorageVPbkStoreHandler*	contactStoreHandler = 
    			CVIMPSTStorageVPbkStoreHandler::NewL(
    						KStoreName(), KIMContactListId, 1, KServiceName(),
    						*store);
                                               	                             
	if (contactStoreHandler)
		{
		
		delete contactStoreHandler;
		delete store;
		return KErrNone;
		}
	delete contactStoreHandler;		
	delete store;
	return KErrGeneral;		
	}	



	    								
/**
 * Called when all the contacts are fetched
 */
void Cvimpststorage_test::HandleContactAddedL(RPointerArray<HBufC>& /*aServiceDetails*/)
	{
		
	}

/**
 * Called when all the contacts are fetched
 */
void Cvimpststorage_test::HandleContactDeletedL(RPointerArray<HBufC>& /*aServiceDetails*/)
	{
		
	}



void Cvimpststorage_test::HandleContactOperationCompleteL( MVIMPSTStorageContact& /*aContact*/, TInt /*aError = KErrNone*/ )
	{
		
	}
				
/**
 * From  MVPbkContactObserver
 */
void Cvimpststorage_test::ContactOperationCompleted(TContactOpResult /*aResult*/)
	{
		
	}

/**
 * From MVPbkContactObserver
 */
void Cvimpststorage_test::ContactOperationFailed
    (TContactOp /*aOpCode*/, TInt /*aErrorCode*/, TBool /*aErrorNotified*/)
    {
    	
    }

		
		
//  [End of File] - Do not remove



