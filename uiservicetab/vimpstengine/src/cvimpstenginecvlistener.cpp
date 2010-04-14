/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  active object class for interacting with conversation view
*
*/

// INCLUDE FILES

#include "cvimpstenginecvlistener.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"
#include "mvimpststoragecontact.h"
#include "cvimpstengineservicetablefetcher.h"
#include "uiservicetabtracer.h"

//system includes
#include <e32base.h>
#include <MVPbkContactLink.h>

// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
// CVIMPSTEngineCVListener::CVIMPSTEngineCVListener
// --------------------------------------------------------------------------
//
CVIMPSTEngineCVListener::CVIMPSTEngineCVListener(
    CVIMPSTEngineServiceTableFetcher& aServiceTableFetcher ): 
    CActive( CActive::EPriorityStandard ),
    iServiceTableFetcher( aServiceTableFetcher )
    {
    CActiveScheduler::Add( this );
    }


// --------------------------------------------------------------------------
// CVIMPSTEngineCVListener::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTEngineCVListener::ConstructL()
    {
	TRACER_AUTO;
    
    // subscribing for the property published by conversation view.
    // attach the properties.
    User::LeaveIfError( iProperty.Attach(KConvViewUID,KXspIdServiceIDKey  ) );
    //start listening property published by CV (to get Contact link and Display name.).
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// --------------------------------------------------------------------------
// CVIMPSTEngineCVListener::NewL
// --------------------------------------------------------------------------
//
CVIMPSTEngineCVListener* CVIMPSTEngineCVListener::NewL(
        CVIMPSTEngineServiceTableFetcher& aServiceTableFetcher )
    {
    CVIMPSTEngineCVListener* self = new(ELeave) CVIMPSTEngineCVListener( aServiceTableFetcher );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// ---------------------------------------------------------
// CVIMPSTEngineCVListener::~CVIMPSTEngineCVListener()
// C++ Destructor 
// ---------------------------------------------------------
//
CVIMPSTEngineCVListener::~CVIMPSTEngineCVListener()
    {
    Cancel();
    iProperty.Close();
    }
// ---------------------------------------------------------
// CVIMPSTEngineCVListener::RunL()
// ---------------------------------------------------------
//
void CVIMPSTEngineCVListener::RunL()
    {
	TRACER_AUTO;
    // resubscribe before processing new value to prevent missing updates
    //TBuf <KMaxSerIdXspIdLen> serId_usrId;
    HBufC16* serId_usrId = HBufC16::NewLC(KMaxSerIdXspIdLen);
    if ( serId_usrId )
        {
        TPtr serId_usrIdPtr(serId_usrId->Des());
        iProperty.Get(KConvViewUID,KXspIdServiceIDKey,serId_usrIdPtr);
        }
    // parse the service id and userid form the buf and get the 
    // contactlink and first name and publish it.
    TInt posOfDelimiter  = serId_usrId->Find(KDelimiter);
    if(KErrNotFound != posOfDelimiter )
        {
        TInt serviceId = KErrNotFound;
        TBuf<KMaxServiceIdLength> servIdBuf;
        TPtrC ptr(serId_usrId->Left(posOfDelimiter));
        servIdBuf = ptr;
        TLex16 lex(servIdBuf);
        lex.Val(serviceId);
       
        // Check if this service id belongs to us. If there is customized 
        // service with own service tab, we MUST not handle this callback.
        RArray<TUint32> supportedServices;
        CleanupClosePushL( supportedServices );
        iServiceTableFetcher.GetMasterServiceIdsL( supportedServices );
        TInt ourService = supportedServices.Find( serviceId );
        if( KErrNotFound != ourService )
            {
            MVIMPSTStorageServiceView* storage =  CVIMPSTStorageManagerFactory::ContactListInterfaceL((TUint32)serviceId);
            __ASSERT_ALWAYS( storage, User::Leave( KErrGeneral ));
            TPtrC useridPtr = serId_usrId->Right(serId_usrId->Length() - (posOfDelimiter+ 1) );
            MVIMPSTStorageContact* contact = storage->FindContactByUserId(useridPtr);
            if(contact)
                {
                TBuf8<KMaxPackedContactLinkLength> link;
                link = contact->ContactLink()->PackLC()->Des();
     
                CleanupStack::PopAndDestroy(); //pop and destroy the hbufc8*
                               
                TInt length = KMaxServiceIdLength + useridPtr.Length() + link.Length()
                               + contact->Name().Length() + (KDelimiter().Length() * 3);
                TPtrC fullname = contact->Name();
                HBufC* dispName = NULL;
                posOfDelimiter = fullname.Find(KDelimiter);
                TInt displayIndex = fullname.Length() - (posOfDelimiter+ 1);
                if(KErrNotFound != posOfDelimiter && (displayIndex > 0))
                    {
                    dispName = fullname.Right( displayIndex  ).AllocLC() ;
                    }
                else
                    {
                    dispName= fullname.AllocLC();
                    }
                HBufC16* contactLink = HBufC16::NewLC(link.Length());
                TPtr16 ptr = contactLink->Des();
                ptr.Copy(link);
                HBufC* userDetails = HBufC::NewLC(length);
                TPtr userDetailsPtr = userDetails->Des();
         
                userDetailsPtr.Zero();
                userDetailsPtr.AppendNum(serviceId);
                userDetailsPtr.Append(KDelimiter());
                userDetailsPtr.Append(useridPtr);
                userDetailsPtr.Append(KDelimiter());
                userDetailsPtr.Append(*dispName);
                userDetailsPtr.Append(KDelimiter());
                userDetailsPtr.Append( *contactLink );
                
                TRACE( " publishing = %S",&(*userDetails)); 
                
                iProperty.Set(KConvViewUID,KContactLinkDisplayNameKey,*userDetails);
                CleanupStack::PopAndDestroy(3);//userDetails,contactLink,dispName
              
                }
            // contact not found
            else
                {
            TRACE( "publishing null display name"); 
                iProperty.Set(KConvViewUID,KContactLinkDisplayNameKey,KNullDesC());
                }
            }
        CleanupStack::PopAndDestroy( &supportedServices );
        }
        
    if(serId_usrId)
    		{
    		CleanupStack::PopAndDestroy( serId_usrId );
    		}  
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// ---------------------------------------------------------
// CVIMPSTEngineCVListener::RunL()
// ---------------------------------------------------------
//
void CVIMPSTEngineCVListener::DoCancel()
    {    
	TRACER_AUTO;
    iProperty.Cancel();
    }
  		     
// ---------------------------------------------------------
// CIMCVEngineStorageListener::StopListening()
// ---------------------------------------------------------
//
TInt CVIMPSTEngineCVListener::RunError( TInt aError )
    {
    TRACER_AUTO;
    if ( KErrCancel != aError )
        {
        iProperty.Subscribe( iStatus );
        SetActive();
        }
    return KErrNone;
    }
//  End of File  

