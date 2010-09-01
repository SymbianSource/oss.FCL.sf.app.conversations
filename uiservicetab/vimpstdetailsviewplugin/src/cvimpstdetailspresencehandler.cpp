/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the presence handler
*
*/


#include "cvimpstdetailspresencehandler.h"

#include "tvimpstconsts.h"

//presence cache header
#include <presencecachereader2.h> // cache reader
#include <avabilitytext.h>

// ximpfw
#include <ximpclient.h>
#include <ximpcontext.h>
#include <StringLoader.h>
#include <vimpstdetailsviewpluginrsc.rsg>
#include "uiservicetabtracer.h"

// ---------------------------------------------------------------------------
// NewL, two-phase construction
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsPresenceHandler* CVIMPSTDetailsPresenceHandler::NewL(const TDesC& aServiceName ,
																   MVIMPSTDetailsPresenceObserver& aObserver,
																   TBool aPresenceSupported )
    {
    CVIMPSTDetailsPresenceHandler* self= new (ELeave) CVIMPSTDetailsPresenceHandler( aObserver, aPresenceSupported );
    CleanupStack::PushL(self);
    self->ConstructL( aServiceName );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// C++ (first phase) constructor
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsPresenceHandler::CVIMPSTDetailsPresenceHandler(MVIMPSTDetailsPresenceObserver& aObserver, 
                                                             TBool aPresenceSupported)
   : iPresenceObserver( aObserver ),
   iStatus( TVIMPSTEnums::EOffline ),
   iPresenceSupported( aPresenceSupported )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// ConstructL, second phase constructor
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsPresenceHandler::ConstructL(const TDesC& aServiceName )
	{
	TRACER_AUTO;
 	iServiceName = aServiceName.AllocL();
 	
 		
    iXIMClient = MXIMPClient::NewClientL();        
    //Create new sink to receive ximp context events
    iPresenceContext = iXIMClient->NewPresenceContextLC();
 	CleanupStack::Pop(); //  iPresenceContext
 	
    //initialize the presence cache.
    iPresenceCacheReader = MPresenceCacheReader2::CreateReaderL(); 														 
    iPresenceCacheReader->SetObserverForSubscribedNotifications(this);
    iUserId = NULL;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsPresenceHandler::~CVIMPSTDetailsPresenceHandler()
    {
    delete iStatusMessage;
    delete iUserId;
    delete iStatusText;
    delete iServiceName;
    delete iPresenceCacheReader;
    delete iPresenceContext;
    delete iXIMClient;
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::SubscribePresenceL
// ---------------------------------------------------------------------------
//  
void CVIMPSTDetailsPresenceHandler::SubscribePresenceL(const TDesC& aUserId )
	{
	TRACER_AUTO;
	if( iUserId )
		{
		delete iUserId;
		iUserId = NULL;	
		}
	iUserId = aUserId.AllocL();
	iStatus = TVIMPSTEnums::EOffline;
	if( !iPresenceSupported )
	    {
	    return;
	    }
	HBufC* name = HBufC::NewLC( aUserId.Length() + iServiceName->Length() + KColon().Length() );
	TPtr namePtr( name->Des() );
	namePtr.Zero();
	
	// append the service name followed by user id ,seperated by colon
	namePtr.Append(*iServiceName);
	namePtr.Append(KColon);
    namePtr.Append(aUserId);

    iPresenceCacheReader->SubscribePresenceBuddyChangeL(namePtr);
    MPresenceBuddyInfo2* presenceBuddyInfo = iPresenceCacheReader->PresenceInfoLC(namePtr);
   
    if ( presenceBuddyInfo )
        {
        TPtrC buddyXSPId = presenceBuddyInfo->BuddyId(); // read the buddyID : returns in XSP format
        TPtrC buddyId = buddyXSPId.Right( buddyXSPId.Length() - iServiceName->Length() - KColon().Length());
        // read the availability /presence state enum value 
        MPresenceBuddyInfo2::TAvailabilityValues availabilityEnum = presenceBuddyInfo->Availability();
        TPtrC avablityText = presenceBuddyInfo->AvailabilityText();
        ConvertPresenceCacheEnums( availabilityEnum ,avablityText ); // convert the presence cache enum value to service tab enum 
		if(TVIMPSTEnums::EUnknown ==  iStatus)
			{
			//get the blocked and pending state
			GetKeyFieldsAndValuesL(*presenceBuddyInfo);
			}
        TPtrC statusMsg = presenceBuddyInfo->StatusMessage(); // read the  status message     
        ProcessStatusMesssageL( statusMsg  );
        CleanupStack::PopAndDestroy();  // presenceBuddyInfo
        }
    CleanupStack::PopAndDestroy(); //  name
   	}



// ---------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::HandlePresenceReadL
// ---------------------------------------------------------------------------
//  
void CVIMPSTDetailsPresenceHandler::HandlePresenceReadL(TInt /*aErrorCode*/,
        RPointerArray<MPresenceBuddyInfo2>& /*aPresenceBuddyInfoList*/)
        {
        //Not required as we are not subcribing to all buddies presence

        }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::HandlePresenceNotificationL
// ---------------------------------------------------------------------------
//  
void CVIMPSTDetailsPresenceHandler::HandlePresenceNotificationL(TInt /*aErrorCode*/,
        MPresenceBuddyInfo2* aPresenceBuddyInfo)
    {
	TRACER_AUTO;
    if ( aPresenceBuddyInfo )
        {
        CleanupDeletePushL( aPresenceBuddyInfo );  
        TPtrC buddyXSPId = aPresenceBuddyInfo->BuddyId(); // read the buddyID : returns in XSP format
        TPtrC buddyId = buddyXSPId.Right( buddyXSPId.Length() - iServiceName->Length() - KColon().Length());
        // read the availability /presence state enum value 
       MPresenceBuddyInfo2::TAvailabilityValues availabilityEnum = aPresenceBuddyInfo->Availability();
       TPtrC avablityText = aPresenceBuddyInfo->AvailabilityText();
       // convert the presence cache enum value to service tab enum 
       ConvertPresenceCacheEnums( availabilityEnum ,avablityText ); 
       
       // its either  blocked or pending stats, hence get the corrrect state
        if(TVIMPSTEnums::EUnknown ==  iStatus)
            {
            //get the blocked and pending state
            GetKeyFieldsAndValuesL(*aPresenceBuddyInfo);
            }
       
        TPtrC statusMsg = aPresenceBuddyInfo->StatusMessage(); // read the  status message     
        ProcessStatusMesssageL( statusMsg  );
        CleanupStack::PopAndDestroy();  // aPresenceBuddyInfo
        }
    }    

// ---------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::ConvertPresenceCacheEnums
// ---------------------------------------------------------------------------
//  
void CVIMPSTDetailsPresenceHandler::ConvertPresenceCacheEnums(MPresenceBuddyInfo2::TAvailabilityValues aAvailabilityEnum, TPtrC aAvabilityText)
    {
    // convert the presence cache enums to UI enumvalues
    // by default if the enum doesnot match then its  TVIMPSTEnums::UnKnown
    switch( aAvailabilityEnum )
        {
        case MPresenceBuddyInfo2::EBusy:
            {
           if(0==aAvabilityText.Compare(KAwayState))
                {
                iStatus  = TVIMPSTEnums::EAway;
                }
            else if(0==aAvabilityText.Compare(KOnPhoneState))
                {
                iStatus  = TVIMPSTEnums::EOnPhone;
                }
            else if(0==aAvabilityText.Compare(KDndState))
                {
                iStatus  = TVIMPSTEnums::EDoNotDisturb;
                }
            else
                {
                iStatus  = TVIMPSTEnums::EBusy;
			    }
            break;
			}
        case MPresenceBuddyInfo2::EAvailable:
            {
            iStatus  = TVIMPSTEnums::EOnline;
            break;
            }
        case MPresenceBuddyInfo2::ENotAvailable:
            {
            iStatus  = TVIMPSTEnums::EOffline;
            break;
            }
        case MPresenceBuddyInfo2::EUnknownAvailability:
            {
            iStatus  = TVIMPSTEnums::EUnknown;
			if(0==aAvabilityText.Compare(KInvisibleState))
			    {
			    iStatus  = TVIMPSTEnums::EInvisible;
			    }
			
            break;
            }    
        default:
            {
            iStatus  = TVIMPSTEnums::EOffline;
            break;
            }    
        }
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::ProcessStatusMesssageL
// ---------------------------------------------------------------------------
//  
void CVIMPSTDetailsPresenceHandler::ProcessStatusMesssageL(const TDesC& aStatusMessage  )
	{
	TRACER_AUTO;
	if( iStatusMessage )
		{
		delete iStatusMessage ;
		iStatusMessage = NULL;	
		}
	if( aStatusMessage.Length() > 0 )
	    {
	    // MAx length of KStatusMsgMaxLength is allocated 
	    // if status message is longer than KStatusMsgMaxLength
	    iStatusMessage = aStatusMessage.Left( KStatusMsgMaxLength ).AllocL(); 
	    }
	iPresenceObserver.HandlePresenceChangedL();	
    }

// --------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::GetPresenceStatus
// --------------------------------------------------------------------------
//
TVIMPSTEnums::TOnlineStatus CVIMPSTDetailsPresenceHandler::GetPresenceStatus()
	{
	if(!iUserId)
	    {
	    return TVIMPSTEnums::EUnknown;
	    }
	return iStatus;
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::GetHeaderLabelDataL
// --------------------------------------------------------------------------
//
const TDesC& CVIMPSTDetailsPresenceHandler::GetHeaderLabelDataL( TInt aIndex )
	{
	TRACER_AUTO;
	if( aIndex == 0 && iServiceName ) // first label data
		{
		return *iServiceName;
		}
	else if( aIndex == 2 && iPresenceSupported ) // second label data
		{
		return GetStatusTextL();
		}
	return KNullDesC; // return empty string for index 1 (label 2) for line in between the service name and presence
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::GetStatusMessageL
// --------------------------------------------------------------------------
//
const TDesC& CVIMPSTDetailsPresenceHandler::GetStatusMessageL()
	{
	TRACER_AUTO;
	if( iStatusMessage && iStatusMessage->Length() )
		{
		return *iStatusMessage;	
		}
	return GetStatusTextL();
	}


// --------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::IsSupported
// --------------------------------------------------------------------------
//
TBool CVIMPSTDetailsPresenceHandler::IsSupported() const
    {
    return iPresenceSupported;
    }
 // --------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::GetStatusTextL
// --------------------------------------------------------------------------
//
const TDesC& CVIMPSTDetailsPresenceHandler::GetStatusTextL()
	{
	TRACER_AUTO;
	TInt resourceId =  R_SERVDETAIL_STATUSTEXT_OFFLINE;
  
	switch( iStatus )
		{
		case TVIMPSTEnums::EOffline:
		    {
		  	resourceId = R_SERVDETAIL_STATUSTEXT_OFFLINE;
		    break;
		    }
		case TVIMPSTEnums::EBusy:
		    {
			resourceId = R_SERVDETAIL_STATUSTEXT_BUSY;
		    break;
		    }
		case TVIMPSTEnums::EOnline:
		    {
		  	resourceId = R_SERVDETAIL_STATUSTEXT_ONLINE;
		    break;
		    }
		case TVIMPSTEnums::EInvisible:
		    {
		  	resourceId = R_SERVDETAIL_STATUSTEXT_DND;
		    break;
		    }
		case TVIMPSTEnums::EOnPhone:
		    {
		  	resourceId = R_SERVDETAIL_STATUSTEXT_ONTHEPHONE;
		    break;
		    }
		case TVIMPSTEnums::EDoNotDisturb:
		    {
		  	resourceId = R_SERVDETAIL_STATUSTEXT_DND;
		    break;
		    }
		case TVIMPSTEnums::EAway:
		    {
		   	resourceId = R_SERVDETAIL_STATUSTEXT_AWAY;
		    break;
		    }
		case TVIMPSTEnums::EBlocked:
		    {
		    resourceId = R_SERVDETAIL_STATUSTEXT_BLOCKED;
		    break;
		    }
	    case TVIMPSTEnums::EPending:
	        {
	        resourceId = R_SERVDETAIL_STATUSTEXT_PENDING;
	        break;
	        }
	    case TVIMPSTEnums::ECallForward:
	        {
	        resourceId = R_SERVDETAIL_STATUSTEXT_CALLFORWARD;
	        break;
	        }	                
		case TVIMPSTEnums::EUnknown:
		case TVIMPSTEnums::EServiceOut:
		default:
		    {
		    break;
		    }
		  }	
	if( iStatusText )
		{
		delete iStatusText;
		iStatusText = NULL;	
		}
	
	if( CCoeEnv::Static() && TVIMPSTEnums::EServiceOut != iStatus 
                            && TVIMPSTEnums::EUnknown != iStatus )
		{
		iStatusText = StringLoader::LoadL( resourceId );	
		}
	if( iStatusText )
		{
		return *iStatusText;	
		}
	return KNullDesC;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsPresenceHandler::GetKeyFieldsAndValuesL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsPresenceHandler::GetKeyFieldsAndValuesL(MPresenceBuddyInfo2& aPresenceBuddyInfo)
    {
	TRACER_AUTO;
    // At any point of time fro remote and blocked contact only one of the keys
    // KPendingRequestExtensionValue/KBlockedExtensionValue will be assigned, and not both the keys.
    TPtrC8 value = aPresenceBuddyInfo.GetAnyField( KExtensionKey() ) ;
    if(value.Compare( KPendingRequestExtensionValue ) == 0)
        {
        iStatus = TVIMPSTEnums::EPending;
        }
    else if(value.Compare( KBlockedExtensionValue ) == 0)
        {
        iStatus  = TVIMPSTEnums::EBlocked;
        }
    else if(value.Compare( KServiceExtensionValue ) == 0)
       {
       iStatus  = TVIMPSTEnums::EServiceOut;
       }
    else if(value.Compare( KCallForwardExtensionValue ) == 0)
       {
        iStatus  = TVIMPSTEnums::ECallForward;
        }
	else
		{
		iStatus  = TVIMPSTEnums::EOffline;
		}
    }
    
// end of file



