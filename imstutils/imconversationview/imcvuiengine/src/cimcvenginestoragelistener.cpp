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
* Description:  active object class for interacting with servicetab for link and Display Name
*
*/

// INCLUDE FILES

#include "cimcvenginestoragelistener.h"
#include "cimcvenginemessagehandler.h"
#include "imcvlogger.h"

//system includes

// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
// CIMCVEngineStorageListener::CIMCVEngineStorageListener
// --------------------------------------------------------------------------
//
CIMCVEngineStorageListener::CIMCVEngineStorageListener(MIMCVEngineStorageObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
    iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// --------------------------------------------------------------------------
// CIMCVEngineStorageListener::ConstructL
// --------------------------------------------------------------------------
//
void CIMCVEngineStorageListener::ConstructL()
    {
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::ConstructL : start"));
    TInt err = RProperty::Define(KConvViewUID,KXspIdServiceIDKey,RProperty::EText);
    if(KErrAlreadyExists != err && KErrNone != err )
        {
        User::LeaveIfError(err);
        }
    //define properties that engine will publish.
    err = RProperty::Define(KConvViewUID,KContactLinkDisplayNameKey,RProperty::ELargeText);
    if(KErrAlreadyExists != err && KErrNone != err )
        {
        User::LeaveIfError(err);
        }
   //Attaching iProperty with the property published by engine to get contact link and display name.
    err = iProperty.Attach(KConvViewUID,KContactLinkDisplayNameKey);
    User::LeaveIfError(err);
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::ConstructL : end"));
    }
// --------------------------------------------------------------------------
// CIMCVEngineStorageListener::NewL
// --------------------------------------------------------------------------
//
CIMCVEngineStorageListener* CIMCVEngineStorageListener::NewL(MIMCVEngineStorageObserver& aObserver)
    {
    CIMCVEngineStorageListener* self = new(ELeave) CIMCVEngineStorageListener( aObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// --------------------------------------------------------------------------
// CIMCVEngineStorageListener::GetContactLinkInfoL
// --------------------------------------------------------------------------
//
void CIMCVEngineStorageListener::GetContactLinkInfoL(TInt aServiceId, const TDesC& aXspID)
    {
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::GetContactLinkInfoL : start"));
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::GetContactLinkInfoL aServiceId= %d"),aServiceId );
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::GetContactLinkInfoL aXspID= %S"),&aXspID );
    StartListening();
    
    TInt length = KMaxServiceIdLength + aXspID.Length() + KDelimiter().Length();
    HBufC* userDetails = HBufC::NewLC(length);
    
    TPtr userDetailsPtr = userDetails->Des();
    userDetailsPtr.Zero();
    userDetailsPtr.AppendNum(aServiceId);
    userDetailsPtr.Append(KDelimiter());
    userDetailsPtr.Append(aXspID);
    
    RProperty::Set(KConvViewUID,KXspIdServiceIDKey,userDetails->Des());
    if( !iWait.IsStarted() )
      {
      // codescanner warning can be ignored, we are not starting an active object but 
      // CActiveSchedulerWait.
      iWait.Start();
      }
    CleanupStack::PopAndDestroy(userDetails);
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::GetContactLinkInfoL : end"));
    }

// ---------------------------------------------------------
// CIMCVEngineStorageListener::~CIMCVEngineStorageListener()
// C++ Destructor 
// ---------------------------------------------------------
//
CIMCVEngineStorageListener::~CIMCVEngineStorageListener()
    {
    if( iWait.IsStarted() )
         {
         // codescanner warning can be ignored, we are not starting an active object but 
         // this is CActiveSchedulerWait.
         iWait.AsyncStop();  
         }
    StopListening(); // Cancel any request, if outstanding
    // delete the earlier defined property
    RProperty::Delete(KConvViewUID,KXspIdServiceIDKey);
    RProperty::Delete(KConvViewUID,KContactLinkDisplayNameKey);
    iProperty.Close(); // Destroy the property object
    }

// ---------------------------------------------------------
// CIMCVEngineStorageListener::RunL()
// ---------------------------------------------------------
//
void CIMCVEngineStorageListener::RunL()
    {
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::RunL : start"));
    if( iStatus.Int() == KErrNone )
        {
        // TBuf <KMaxDisplayNameContactLinkLength> servId_usrId_name_link;
        HBufC16* servId_usrId_name_link = HBufC16::New(KMaxDisplayNameContactLinkLength);
        TPtr servId_usrId_name_linkPtr( servId_usrId_name_link->Des() );
        //Get the property in "ServiceId:UserId:Name:ContactLink" formate
        iProperty.Get(KConvViewUID,KContactLinkDisplayNameKey,servId_usrId_name_linkPtr );

        IM_CV_LOGS(TXT("CIMCVEngineStorageListener::RunL received property = %S"),&servId_usrId_name_linkPtr );

        TInt posOfDelimiter  = servId_usrId_name_link->Find(KDelimiter);
        if(KErrNotFound != posOfDelimiter )
            {
            TInt serviceId;
            HBufC16* userID = NULL;
            HBufC16* contactLink = NULL;
            HBufC16* displayName = NULL;

            TBuf<KMaxServiceIdLength> serviceIdBuf;
            TPtrC ptr(servId_usrId_name_link->Left(posOfDelimiter));
            serviceIdBuf = ptr;
            TLex16 lex(serviceIdBuf);
            lex.Val(serviceId);

            TPtrC usrId_name_link = servId_usrId_name_link->Right(servId_usrId_name_link->Length() - (posOfDelimiter+ 1) );
            posOfDelimiter = usrId_name_link.Find(KDelimiter);
            //extracting user id
            if(KErrNotFound != posOfDelimiter )
                {
                userID = (usrId_name_link.Left(posOfDelimiter)).AllocLC();
                }

            //extracting display name.
            TPtrC name_link = usrId_name_link.Right(usrId_name_link.Length() - (posOfDelimiter+ 1) );
            posOfDelimiter = name_link.Find(KDelimiter);
            if(KErrNotFound != posOfDelimiter   )
                {
                displayName = (name_link.Left(posOfDelimiter)).AllocLC();
                }

            //extracting contact link
            TPtrC link = name_link.Right(name_link.Length() - (posOfDelimiter+ 1) );
            contactLink = link.AllocLC();

            HBufC8* convertedLink = NULL;

            if(contactLink)
                {
                convertedLink = HBufC8::NewLC(contactLink->Length());
                TPtr8 convertedLinkPtr = convertedLink->Des();
                convertedLinkPtr.Copy(*contactLink);
                }
            if( !convertedLink )
                {
                convertedLink = KNullDesC8().AllocLC();
                }

            if(convertedLink && userID && displayName)
                {
                iObserver.HandleServiceBuddyDetailsFetchedL( serviceId,*userID,*convertedLink,*displayName);

                IM_CV_LOGS(TXT("storage listener RunL userID = %S"),&(*userID) );
                IM_CV_LOGS(TXT("storage listener RunL convertedLink = %S"),&(*convertedLink) );
                IM_CV_LOGS(TXT("storage listener RunL displayName = %S"),&(*displayName) );

                }
            delete (servId_usrId_name_link);
            //delete all alocated memory
            if(convertedLink)
                {
                CleanupStack::PopAndDestroy(convertedLink);
                }
            if(contactLink)
                {
                CleanupStack::PopAndDestroy(contactLink);
                }
            if(displayName)
                {
                CleanupStack::PopAndDestroy(displayName);
                }
            if(userID)
                {
                CleanupStack::PopAndDestroy(userID);
                }
            }
        }
    if( iWait.IsStarted() )
          {
          // codescanner warning can be ignored, we are not starting an active object but 
          // this is CActiveSchedulerWait.
          iWait.AsyncStop();  
          }
    
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::RunL : end"));
    }

// ---------------------------------------------------------
// CIMCVEngineStorageListener::RunL()
// ---------------------------------------------------------
//
void CIMCVEngineStorageListener::DoCancel()
    {    
    iProperty.Cancel();
    }

// ---------------------------------------------------------
// CIMCVEngineStorageListener::StartListening()
// ---------------------------------------------------------
//
void CIMCVEngineStorageListener::StartListening()
    {
    if( !IsActive() )
        {
        iProperty.Subscribe(iStatus);
        SetActive(); // Tell scheduler a request is active  
        }
    }

// ---------------------------------------------------------
// CIMCVEngineStorageListener::StopListening()
// ---------------------------------------------------------
//
void CIMCVEngineStorageListener::StopListening()
    {
    if( IsActive() )
        {
        Cancel(); // Cancel any request, if outstanding
        }
    }

// ---------------------------------------------------------
// CIMCVEngineStorageListener::StopListening()
// ---------------------------------------------------------
//
TInt CIMCVEngineStorageListener::RunError( TInt aError )
    {
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::RunError : start"));
    IM_CV_LOGS(TXT("CIMCVEngineStorageListener::RunError : end"));
    if( iWait.IsStarted() )
         {
         // codescanner warning can be ignored, we are not starting an active object but 
         // this is CActiveSchedulerWait.
         iWait.AsyncStop();  
         }
    return aError;
    }

    		         
//  End of File  

