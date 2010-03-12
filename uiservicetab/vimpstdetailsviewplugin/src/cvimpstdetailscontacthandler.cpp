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
* Description:  contact handler class implementaion to virtual phonebook observers
 *
*/

 
#include "cvimpstdetailscontacthandler.h"

#include "mvimpstdetailsviewcontactobserver.h"
#include "cvimpstdetailsviewplugin.h"

#include "tvimpstconsts.h"
#include "vimpstdetailsviewpluginuids.hrh"
#include <vimpstdetailsviewpluginrsc.rsg>

// virtual phonebook
#include <CVPbkContactManager.h>
#include <MVPbkContactOperationBase.h>
#include <MVPbkContactLinkArray.h>
#include <MVPbkFieldType.h>
#include <MVPbkStoreContact.h>
#include <MVPbkContactStore.h>
#include <MVPbkContactStoreProperties.h>
#include <VPbkEng.rsg>
#include <CVPbkContactStoreUriArray.h>
#include <MVPbkContactStoreList.h>
#include <CVPbkContactLinkArray.h>
#include <TVPbkContactStoreUriPtr.h>
#include <MVPbkContactFieldData.h>
#include <MVPbkContactFieldUriData.h>
#include <MVPbkContactLink.h> 
#include <VPbkContactStoreUris.h>
// cca 
#include <mccapppluginparameter.h>
#include <mccaparameter.h>
#include <imconnectionproviderconsts.h>
#include <MVPbkContactFieldTextData.h>
// system include
#include <e32property.h>
#include <s32mem.h>
#include "vimpstdebugtrace.h"
#include "vimpstutils.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::NewL
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsContactHandler* CVIMPSTDetailsContactHandler::NewL( MCCAParameter& aParameter ,
																 MVIMPSTDetailsViewContactObserver& aObserver,
																 const TDesC& aServiceStoreUri,
																 const TDesC& aServiceName,
																 TUint aserviceId
																 )
	{
	CVIMPSTDetailsContactHandler* self =
	    new(ELeave) CVIMPSTDetailsContactHandler( aObserver, aserviceId );
	CleanupStack::PushL(self);
	self->ConstructL( aParameter, aServiceStoreUri, aServiceName );
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::~CVIMPSTDetailsContactHandler
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsContactHandler::~CVIMPSTDetailsContactHandler()
	{
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: ~CVIMPSTDetailsContactHandler() start") );
	Cancel();
	if (iWait.IsStarted())
	    {
	    iWait.AsyncStop();
	    }
	delete iLinks;
	delete iRetrieveOperation;
	delete iXSPUserId;
	delete iStoreContact;
	if (iStoreList)
		{
		TRACED( T_LIT("CVIMPSTDetailsContactHandler:: ~CVIMPSTDetailsContactHandler() calling store close") );
		iStoreList->CloseAll(*this);
		iStoreList = NULL;
		}
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: ~CVIMPSTDetailsContactHandler() iStoreList deleted") );
	delete iServiceStoreUri;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: ~CVIMPSTDetailsContactHandler() iServiceStoreUri deleted") );
	delete iContactManager;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: ~CVIMPSTDetailsContactHandler() iContactManager deleted") );
	delete iServiceName;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: ~CVIMPSTDetailsContactHandler() iServiceName deleted") );
	delete iIdConverter;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: ~CVIMPSTDetailsContactHandler() end") );
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::CVIMPSTDetailsContactHandler
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsContactHandler::CVIMPSTDetailsContactHandler(
    MVIMPSTDetailsViewContactObserver& aObserver,TUint aServiceId  )
	: CActive(EPriorityStandard),
    iObserver( aObserver ),
    iStoreType( EStoreUnkwon ),
    iServiceId (aServiceId )
	{
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::ConstructL( MCCAParameter& aParameter ,const TDesC& aServiceStoreUri,const TDesC& aServiceName )
	{
	TRACED( T_LIT("CVIMPSTDetailsContactHandler::ConstructL start") );
	CVPbkContactStoreUriArray* uriArray = CVPbkContactStoreUriArray::NewLC();
	TUid launchUid = TUid::Null() ;	
	iServiceStoreUri = aServiceStoreUri.AllocL();
	iServiceName = aServiceName.AllocL();
	 //find whether its a local store or xsp store
    if ( iServiceStoreUri && VPbkContactStoreUris::DefaultCntDbUri().Compare( *iServiceStoreUri )
                            != 0 )
        {
        iStoreType = EStoreServer; // store has xsp store
        }
    else
        {        
        iStoreType = EStoreLocal; // same as phonebook2 store name
        }       
	launchUid = aParameter.LaunchedViewUid();
	//always load the default store plus the corresponding service store
	uriArray->AppendL(TVPbkContactStoreUriPtr(VPbkContactStoreUris::DefaultCntDbUri()));
	if( launchUid.iUid != (KVIMPSTDetailsViewPluginImplmentationUid + iServiceId) )
		{
		// launched from other than service tab	
		// need to open service store
		 if ( iServiceStoreUri )
            {
            TPtr serviceStoreNamePtr = iServiceStoreUri->Des(); 
            uriArray->AppendL( TVPbkContactStoreUriPtr( serviceStoreNamePtr ) );
            TRACED( T_LIT("ServiceStoreUriL() - storename3: %S"), &serviceStoreNamePtr );
            }
		}

	HBufC& contactData = aParameter.ContactDataL();

	HBufC8* buf = HBufC8::NewLC( contactData.Length() );
	buf->Des().Copy(contactData);
	TPtrC8 bufDes( buf->Des() );

	RDesReadStream readStream( bufDes );
	readStream.PushL();

	const TInt versionNumber = readStream.ReadUint8L();

	TInt uriCount = readStream.ReadUint16L();
	TRACED( T_LIT("ServiceStoreUriL() - uriCount = %d"),uriCount );
	// Read URIs
	for ( TInt i = 0; i < uriCount; ++i )
	    {
	    const TInt uriLength = readStream.ReadUint16L();
	    HBufC* uriBuffer = HBufC::NewLC(uriLength);
	    TPtr uriPtr = uriBuffer->Des();
	    readStream.ReadL( uriPtr, uriLength );
	    if( uriPtr.Length() > 0 )
	        {
	        uriArray->AppendL( TVPbkContactStoreUriPtr( uriPtr ) );
	        TRACED( T_LIT("ServiceStoreUriL() - storename4: %S"), &uriPtr );
	        }
	    CleanupStack::PopAndDestroy( uriBuffer );
	    }
	CleanupStack::PopAndDestroy(); // readStream
	CleanupStack::PopAndDestroy(); // buf


	iContactManager = CVPbkContactManager::NewL(*uriArray);
	CleanupStack::PopAndDestroy(); // uriArray

	iStoreList = &iContactManager->ContactStoresL();
	iStoreList->OpenAllL(*this);

    if( !iWait.IsStarted() )
        {
        iWait.Start();  
        }
    if(iStoreType == EStoreLocal)
        {
        const TDesC& contactdb = VPbkContactStoreUris::DefaultCntDbUri();
        MVPbkContactStore* contactStore =  iStoreList->Find(TVPbkContactStoreUriPtr(contactdb));
        iIdConverter = CVPbkContactIdConverter::NewL(*contactStore);
        }
    TRACED( T_LIT("CVIMPSTDetailsContactHandler::ConstructL end") );
	}


// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::GetDisplayNameLC
// ---------------------------------------------------------------------------
//
HBufC* CVIMPSTDetailsContactHandler::GetDisplayNameLC()
    {
    HBufC* firstName = KNullDesC().AllocL();
    HBufC* lastName = KNullDesC().AllocL();
    HBufC* retValue = NULL;   
    if( iStoreContact )
        {		
        MVPbkStoreContactFieldCollection& fieldCollection = iStoreContact->Fields();
        TInt fieldCount = fieldCollection.FieldCount();	    
        for( TInt i=0; i< fieldCount; i++ )
            {
            MVPbkStoreContactField& field = fieldCollection.FieldAt( i );   
            const MVPbkFieldType* type = field.BestMatchingFieldType() ;
            if( R_VPBK_FIELD_TYPE_FIRSTNAME == type->FieldTypeResId() )
                {
                const MVPbkContactFieldData& fieldData = field.FieldData();
                if( fieldData.DataType() == EVPbkFieldStorageTypeText  )
                    {
                    delete firstName;
                    firstName =  MVPbkContactFieldTextData::Cast(fieldData).Text().AllocL();                    
                    break;
                    }
                }
            }
        for( TInt i=0; i< fieldCount; i++ )
            {
            MVPbkStoreContactField& field = fieldCollection.FieldAt( i );   
            const MVPbkFieldType* type = field.BestMatchingFieldType() ;
            if( R_VPBK_FIELD_TYPE_LASTNAME == type->FieldTypeResId() )
                {
                const MVPbkContactFieldData& fieldData = field.FieldData();
                if( fieldData.DataType() == EVPbkFieldStorageTypeText  )
                    {
                    delete lastName;
                    lastName = MVPbkContactFieldTextData::Cast(fieldData).Text().AllocL();                       
                    break;
                    }
                }
            }        
        TPtr ptrFirstName = firstName->Des();
        TPtr ptrLastName = lastName->Des();
        if(ptrFirstName.Length() || ptrLastName.Length())
            {
            retValue = HBufC::NewLC(ptrFirstName.Length()+ ptrLastName.Length() + 1);
            TPtr ptrRet = retValue->Des();        
            ptrRet.Append(ptrFirstName);
            if(ptrFirstName.Length())
                {
                //append one space before appending lastname
                ptrRet.Append(' ');
                }        
            ptrRet.Append(ptrLastName);
            }
        else
            {
            retValue = VIMPSTUtils::LoadResourceL( R_SERVDETAIL_UNNAMED );
            CleanupStack::PushL(retValue);
            }
        }
    delete firstName;
    delete lastName;        
    return retValue;
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::SetLinks(
    		MVPbkContactLinkArray* aLinks, TBool aReadXspId )
	{
    TRACED( T_LIT("CVIMPSTDetailsContactHandler::SetLinks start") );
    delete iLinks;
	iLinks = NULL; 
	iLinks = aLinks;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler::SetLinks iLinks deleted ") );
	iCurrentLink = NULL;
	
	iReadXspId = aReadXspId;	
	if( iRetrieveOperation )
    	{
        delete iRetrieveOperation;
        iRetrieveOperation = NULL;
        TRACED( T_LIT("CVIMPSTDetailsContactHandler::SetLinks iRetrieveOperation deleted ") );
    	}
	TRACED( T_LIT("CVIMPSTDetailsContactHandler::SetLinks calling issue request") );
	IssueRequest();
	TRACED( T_LIT("CVIMPSTDetailsContactHandler::SetLinks end") );
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::Links
// ---------------------------------------------------------------------------
//
MVPbkContactLinkArray* CVIMPSTDetailsContactHandler::Links() const
    {
    return iLinks;
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::GetXSPUserId
// ---------------------------------------------------------------------------
//
const TDesC& CVIMPSTDetailsContactHandler::GetXSPUserId()
	{
	if( iXSPUserId )
		{
		return *iXSPUserId;	
		}
	return KNullDesC;
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::ContactStoresL
// --------------------------------------------------------------------------
//
MVPbkContactStoreList& CVIMPSTDetailsContactHandler::ContactStoresL() 
	{
	return *iStoreList;
	}
// --------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::StoreType
// --------------------------------------------------------------------------
//
TStoreType CVIMPSTDetailsContactHandler::StoreType() 
    {
    return iStoreType;
    }
 
 // -------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::CancelOngoingRequest
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::CancelOngoingRequest()
	{
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: CancelOngoingRequest() start ") );
	delete iRetrieveOperation;
	iRetrieveOperation = NULL;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: CancelOngoingRequest() iRetrieveOperation deleted ") );
	delete iXSPUserId;
	iXSPUserId = NULL;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: CancelOngoingRequest() iXSPUserId deleted ") );
	delete iStoreContact;
	iStoreContact = NULL;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: CancelOngoingRequest() iStoreContact deleted ") );
	delete iLinks;
	iLinks = NULL;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: CancelOngoingRequest() end ") );
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::RunL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::RunL()
    {
    TRACED( T_LIT("CVIMPSTDetailsContactHandler:: RunL() start ") );
    TInt index = 0;
    if (iLinks && iCurrentLink )
        {
        TRACED( T_LIT("CVIMPSTDetailsContactHandler:: RunL() next link ") );
        index = iLinks->Find(*iCurrentLink) + 1;
        }
    if ( iLinks && index < iLinks->Count() )
        {
        TRACED( T_LIT("CVIMPSTDetailsContactHandler:: RunL() calling retrive ") );
        // will get called until the index become equal to count
        iCurrentLink = &iLinks->At(index);            
        TRAPD( err, iRetrieveOperation = iContactManager->RetrieveContactL(*iCurrentLink, *this) );       
        TRACED( T_LIT("CVIMPSTDetailsContactHandler:: RunL() retrive called  err = %d"),err );
        }
    else
        {
        TRACED( T_LIT("CVIMPSTDetailsContactHandler:: RunL() job finished ") );
        iCurrentLink = NULL;
        }
    TRACED( T_LIT("CVIMPSTDetailsContactHandler:: RunL() end ") );
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::RunError
// ---------------------------------------------------------------------------
//
TInt CVIMPSTDetailsContactHandler::RunError(TInt aError)
	{
	return aError;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::DoCancel
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::DoCancel()
	{
	}


// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::VPbkSingleContactOperationComplete
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::VPbkSingleContactOperationComplete(
    MVPbkContactOperationBase& /*aOperation*/,
    MVPbkStoreContact* aContact)
    {
    TRACED( T_LIT("CVIMPSTDetailsContactHandler:: VPbkSingleContactOperationComplete start ") );
    TBool isIssueReguest = ETrue;
    
    delete iRetrieveOperation;
    iRetrieveOperation = NULL;

    delete iStoreContact;
    iStoreContact = NULL;
    iStoreContact =  aContact;

    if( iStoreContact && iReadXspId )
        {
        MVPbkStoreContactFieldCollection& fieldCollection = iStoreContact->Fields();

        TInt fieldCount = fieldCollection.FieldCount();

        for( TInt i=0; i< fieldCount; i++ )
            {
            MVPbkStoreContactField& field = fieldCollection.FieldAt( i );	
            const MVPbkFieldType* type = field.BestMatchingFieldType() ;
            if( R_VPBK_FIELD_TYPE_IMPP == type->FieldTypeResId() )
                {
                const MVPbkContactFieldData& fieldData = field.FieldData();
                if( fieldData.DataType() == EVPbkFieldStorageTypeUri  )
                    {
                    TPtrC scheme = MVPbkContactFieldUriData::Cast(fieldData).Scheme();
                    if(iServiceName->Compare(scheme) == 0)
                        {
                        TRACED( T_LIT("CVIMPSTDetailsContactHandler:: EVPbkFieldStorageTypeUri called ") );
                        const MVPbkContactFieldUriData& uri = MVPbkContactFieldUriData::Cast(fieldData);
                        HBufC* user = NULL;
                        delete iXSPUserId;
                        iXSPUserId = NULL;
                        // call back api is not leaving function
                        TRAP_IGNORE( user = uri.Uri().AllocL() ;
                        if( user )
                            {
                            CleanupStack::PushL( user );
                            TPtr userPtr( user->Des() );
                            TInt index = userPtr.Find( KColon );
                            if( index != KErrNotFound )
                                {
                                TInt length = userPtr.Length();
                                iXSPUserId = userPtr.Right( length - index - 1 ).AllocL() ;
                                }
                            CleanupStack::PopAndDestroy();// user
                            }
                        TRACED( T_LIT("CVIMPSTDetailsContactHandler:: EVPbkFieldStorageTypeUri end  ") );
                        );
                        break;
                        }
                    }
                }
            }
        if( iXSPUserId && iContactManager  )
            {
            TRACED( T_LIT("CVIMPSTDetailsContactHandler:: search is called ") );
            // call back api is not leaving function
            TRAP_IGNORE( iRetrieveOperation = iContactManager->FindL(*iXSPUserId, iContactManager->FieldTypes(),*this) );	
            isIssueReguest = EFalse;
            }
        else
            {
            // call back api is not leaving function
            TRAP_IGNORE( iObserver.HandleContactUnavailableL() );             
            }		
        }	
    else if( iStoreContact  )
        {
        TRACED( T_LIT("CVIMPSTDetailsContactHandler:: VPbkSingleContactOperationComplete iStoreContact Valid ") );
        TBool validLink = ETrue;
        if( iStoreType == EStoreLocal && iCurrentLink )
            {
            //Local Store
            // to check whether this contact has been added to the ServiceTab. with the help of db file          
            validLink = CheckWhetherValidLocalLinkL( *iCurrentLink);            
            }
        if( validLink )
            {
            TRACED( T_LIT("CVIMPSTDetailsContactHandler::HandleContactReadyL ") );
            TRAP_IGNORE( iObserver.HandleContactReadyL( *iStoreContact ) ); 
            }
        else
            {
            TRACED( T_LIT("CVIMPSTDetailsContactHandler:: VPbkSingleContactOperationComplete No data") );
            TRAP_IGNORE( iObserver.HandleContactUnavailableL() ); 
            }
        }
    if( isIssueReguest )
        {
        IssueRequest();
        }
    TRACED( T_LIT("CVIMPSTDetailsContactHandler:: VPbkSingleContactOperationComplete end ") );
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::VPbkSingleContactOperationFailed
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::VPbkSingleContactOperationFailed(
    MVPbkContactOperationBase& /*aOperation*/,
    TInt /*aError*/)
	{
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: VPbkSingleContactOperationFailed satrt ") );
	delete iRetrieveOperation;
	iRetrieveOperation = NULL;
	
	TInt index = 0;
	if (iLinks && iCurrentLink)
		{
		index = iLinks->Find(*iCurrentLink) + 1;
		
		if ( index >= iLinks->Count() )
			{
			TRACED( T_LIT("CVIMPSTDetailsContactHandler:: VPbkSingleContactOperationFailed calling HandleContactUnavailableL start") );
			TRAP_IGNORE( iObserver.HandleContactUnavailableL() );
			TRACED( T_LIT("CVIMPSTDetailsContactHandler:: VPbkSingleContactOperationFailed calling HandleContactUnavailableL end ") );
			return;
			}
	  }
	IssueRequest();
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: VPbkSingleContactOperationFailed end ") );
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::FindCompleteL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::FindCompleteL( MVPbkContactLinkArray* aResults )
	{
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: FindCompleteL satrt ") );
	CVPbkContactLinkArray* linkArray = NULL;	
	delete iRetrieveOperation;
	iRetrieveOperation = NULL;	
	if( aResults && iServiceStoreUri )
		{
		TInt linkCount = aResults->Count();	
		for( TInt a=0; a< linkCount; a++ )
			{
			const MVPbkContactLink& link = aResults->At( a ) ;
			MVPbkContactStore& store = link.ContactStore();	
			const MVPbkContactStoreProperties& storeProperty = store.StoreProperties();
			if( storeProperty.Uri().UriDes().Compare( *iServiceStoreUri  ) == 0 )
				{
				TRACED( T_LIT("CVIMPSTDetailsContactHandler:: FindCompleteL one result found ") );
			    linkArray = CVPbkContactLinkArray::NewLC();
			    MVPbkContactLink* clone = link.CloneLC();
			    // take ownership clone
			    linkArray->AppendL(clone);
			    CleanupStack::Pop(2); // clone, linkArray
			    TRACED( T_LIT("CVIMPSTDetailsContactHandler:: FindCompleteL come out of loop ") );
			    break;
				}
			}		
		}	
	delete aResults;
	aResults = NULL;
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: FindCompleteL aResults deleted ") );
	if( linkArray )
		{
		// take ownership linkArray
		SetLinks( linkArray, EFalse );	
		}
	else
		{
		iObserver.HandleContactUnavailableL();
		}
	TRACED( T_LIT("CVIMPSTDetailsContactHandler:: FindCompleteL end ") );
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::FindFailed( TInt /*aError*/ )
	{
	TRAP_IGNORE( iObserver.HandleContactUnavailableL() ); // takes ownership of contacts.	
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::IssueRequest(TInt aError)
	{
	if( !IsActive() )
		{
		SetActive();
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, aError);	
		}
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::OpenComplete
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::OpenComplete()
    {
    TRACED( T_LIT("CVIMPSTDetailsContactHandler::OpenComplete start") );
    if (iWait.IsStarted())
        {
        iWait.AsyncStop();
        }
    TRACED( T_LIT("CVIMPSTDetailsContactHandler::OpenComplete end") );
    }

// --------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::StoreReady
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::StoreReady(MVPbkContactStore& /*aContactStore*/ )
	{
	TRACED( T_LIT("CVIMPSTDetailsContactHandler::StoreReady") );
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::StoreUnavailable
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::StoreUnavailable(
	MVPbkContactStore& /*aContactStore*/,
	TInt /*aReason*/)
	{
	TRACED( T_LIT("CVIMPSTDetailsContactHandler::StoreUnavailable start") );
	if (iWait.IsStarted())
		{
		iWait.AsyncStop();
		}
	TRACED( T_LIT("CVIMPSTDetailsContactHandler::StoreUnavailable end") );
	}

// -------------------------------------------------------------------------
// CVIMPSTDetailsContactHandler::HandleStoreEventL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::HandleStoreEventL(
	MVPbkContactStore& /*aContactStore*/,
	TVPbkContactStoreEvent /*aStoreEvent*/)
	{

	}

// -----------------------------------------------------------
// CVIMPSTDetailsContactHandler::CheckWhetherValidLocalLink
// -----------------------------------------------------------
//

TBool CVIMPSTDetailsContactHandler::CheckWhetherValidLocalLinkL(
        const MVPbkContactLink& aCurrentLink )
    {   
    TBool validLocalLink = EFalse;
    User::LeaveIfError( iFs.Connect() );
    // Open existing or create new database.
    if ( DbExists() )
        {
        OpenDbL();
        OpenTableL();
        // Get Column number for contact data size
        iColset = iDb.ColSetL( KContactTable ); 
        TInt32 linkId = iIdConverter->LinkToIdentifier(aCurrentLink);       
        TDbColNo colNo = iColset->ColNo( KContactId );
        validLocalLink = SeekRowL( colNo, linkId );        
        CloseDb();
        }    
    return validLocalLink;
    }

//******************* Database Operations *********************************//
// -----------------------------------------------------------
// CVIMPSTDetailsContactHandler::DbExists
// -----------------------------------------------------------
//
TBool CVIMPSTDetailsContactHandler::DbExists()
    {
    TBuf<512> dbName;    
    RFile temp;
    TBuf< KMaxPath > storagePath;
    
    dbName.Append( iServiceName->Des() );
    dbName.Append( KStorageExtn );
    
    storagePath.Append( KDbPath );
    storagePath.Append( dbName );
    
    TInt err( temp.Open( iFs, *(&storagePath), EFileShareReadersOrWriters|EFileRead ) );

    TInt size( 0 );
    if ( err == KErrNone )
        {
        temp.Size( size );
        }

    temp.Close();

    if ( size == 0 )
        {
        return EFalse;
        }

    return ( err == KErrNone );
    }



// -----------------------------------------------------------
// CVIMPSTDetailsContactHandler::OpenDbL
// -----------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::OpenDbL()
    {    
    TBuf< KMaxPath > storagePath;
    TBuf<512> dbName;    
    dbName.Append( iServiceName->Des() );
    dbName.Append( KStorageExtn );
    
    storagePath.Append( KDbPath );
    storagePath.Append( dbName );
    
    iFileStore = CPermanentFileStore::OpenL( iFs, storagePath, EFileShareReadersOrWriters|EFileWrite );
    iFileStore->SetTypeL( iFileStore->Layout() );
    iDb.OpenL( iFileStore, iFileStore->Root() );
    }

// -----------------------------------------------------------
// CVIMPSTDetailsContactHandler::CloseDb
// -----------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::CloseDb()
    { 
    iTable.Close();
    delete iColset;    
    delete iFileStore;    
    iFileStore = NULL;
    iDb.Close();
    iFs.Close();
    }

// ----------------------------------------------------------
// CVIMPSTDetailsContactHandler::SeekRowL
// ----------------------------------------------------------
//
TBool CVIMPSTDetailsContactHandler::SeekRowL( TDbColNo aColNo, TInt32& aIdentifier )
    {    
    TBool ret = EFalse;
    iTable.BeginningL();
    while ( iTable.NextL() )
        {
        iTable.GetL();
        if ( iTable.ColInt32( aColNo ) == aIdentifier )
            {
            ret = ETrue;
            break; 
            }
        }   
    return ret;
    }

// ----------------------------------------------------------
// CVIMPSTDetailsContactHandler::OpenTableL
// ----------------------------------------------------------
//
void CVIMPSTDetailsContactHandler::OpenTableL()
    {    
    TInt err( iTable.Open( iDb, KContactTable ) );
    if ( err != KErrNone )
        {
        iTable.Close();        
        User::Leave( err );
        return;
        }    
    return;
    }
   
// End of File
