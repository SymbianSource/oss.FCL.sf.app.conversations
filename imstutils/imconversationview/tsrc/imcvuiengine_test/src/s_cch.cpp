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
* Description: s_cch.cpp
*
*/

//  EXTERNAL INCLUDES

#include <cch.h>
#include <cchtypes.h>
#include <spentry.h>
#include <spsettings.h>
#include <spproperty.h>
#include "s_cch.h"

CCchService* myService = NULL;



TInt CreateSpEntryL( TInt aServiceId )
    {    
    CSPSettings* myCSPSettings = CSPSettings::NewL();
    CSPEntry* entry = CSPEntry::NewLC();
    entry->SetServiceId( aServiceId );
    User::LeaveIfError( entry->SetServiceName( KTestServiceName ) );
    CSPProperty* property = NULL;
    // set voip enabled property
    property = CSPProperty::NewLC();
    property->SetName( ESubPropertyVoIPEnabled );
    property->SetValue( EOn );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );

    // set presence settings id property
    property = CSPProperty::NewLC();
    property->SetName( ESubPropertyPresenceSettingsId );
    property->SetValue( 1 );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );

    // set im settings id property
    property = CSPProperty::NewLC();
    property->SetName( ESubPropertyIMSettingsId );
    property->SetValue( 1 );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );

    // set Presence FW PSC plugin UID
    property = CSPProperty::NewLC();
    property->SetName( EPropertyPCSPluginId );
    property->SetValue( 0x20007B6D );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );

    property = CSPProperty::NewLC();
    property->SetName( EPropertyContactViewPluginId );        
    property->SetValue( KMECOIMPLEMENTATIONUID );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );


    myCSPSettings->AddEntryL( *entry ); 
    TInt createdServiceId = entry->GetServiceId();
    CleanupStack::PopAndDestroy( entry );

    return createdServiceId;
    } 


/*************** Stubs ****************/
CCchService* CCch::GetService( TInt /*aServiceId*/ )
    {
    if( !myService )
        {
        myService = new ( ELeave ) MyCchService();
        return myService;
        }
    return myService;
    }
/*************** Stubs ****************/

CCch::CCch()
    {

    }

CCch::~CCch()
    {
    if(myService)
        {
        myService = NULL;
        }

    }
// ---------------------------------------------------------------------------
// CCch::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCch* CCch::NewL( )
    {
    CCch* self = CCch::NewLC( );
    CleanupStack::Pop( self );
    return self;
    }
// ---------------------------------------------------------------------------
// CCch::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCch::ConstructL()
    {
    //iImplementation = CCchImpl::NewL( this );
    
    }
// ---------------------------------------------------------------------------
// CCch::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCch* CCch::NewLC( )
    {
    CCch* self = new (ELeave) CCch( );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CCchUIHandler::GetServiceIds
// ---------------------------------------------------------------------------
//
TInt CCch::GetServiceIds( RArray<TInt>& aIds ) const
{

}

// ---------------------------------------------------------------------------
// CCchUIHandler::GetServiceIds
// ---------------------------------------------------------------------------
//
TInt CCch::GetServices( TCCHSubserviceType aType, 
        RPointerArray<CCchService>& aServices )
    {

    }

// ---------------------------------------------------------------------------
// CCchUIHandler::ConnectivityDialogsAllowed
// ---------------------------------------------------------------------------
//
TBool CCch::ConnectivityDialogsAllowed() const
{

}

// ---------------------------------------------------------------------------
// CCchUIHandler::SetConnectivityDialogsAllowed
// ---------------------------------------------------------------------------
//
void CCch::SetConnectivityDialogsAllowed( 
        TBool aConnectivityDialogsAllowed )
    {
     }
// ========================== OTHER EXPORTED FUNCTIONS =========================
// --------------------------------------------------------------------------
// TCchServiceStatus::TCchServiceStatus
// --------------------------------------------------------------------------
//
TCchServiceStatus::TCchServiceStatus()
: iState( ECCHUninitialized )
, iError( KErrNone )
        {
        }

// --------------------------------------------------------------------------
// TCchServiceStatus::State
// --------------------------------------------------------------------------
//
TCCHSubserviceState TCchServiceStatus::State() const
{ 
return iState; 
}

// --------------------------------------------------------------------------
// TCchServiceStatus::Error
// --------------------------------------------------------------------------
//  
TInt TCchServiceStatus::Error() const
{ 
return iError; 
}

// --------------------------------------------------------------------------
// TCchServiceStatus::SetState
// --------------------------------------------------------------------------
//
void TCchServiceStatus::SetState( TCCHSubserviceState aState )
    { 
    iState = aState; 
    }

// --------------------------------------------------------------------------
// TCchServiceStatus::SetError
// --------------------------------------------------------------------------
//
void TCchServiceStatus::SetError( TInt aError )
    { 
    iError = aError; 
    }
//  End of File

/************* End of Stubs ************/



TInt MyCchService::Enable( TCCHSubserviceType aType )
    { 
    return KErrNone; 
    }

/**
 * Launches service disable. See MCchServiceObserver for status changes.
 *
 * @param aType Type of service
 * @return Symbian error
 */
TInt MyCchService::Disable( TCCHSubserviceType /*aType*/ ){ return KErrNone; }

/**
 * Returns the current service state
 *
 * @param aType Type of service
 * @param aStatus Status of the service, return value
 * @return Symbian error code
 */
TInt MyCchService::GetStatus( TCCHSubserviceType aType, 
        TCchServiceStatus& aStatus ) const 

        { 
        if ( (ECCHIMSub == aType)  )
            {		
            aStatus.SetState(ECCHEnabled);	
            aStatus.SetError(KErrNone);			
            return KErrNone; 
            }	

        return KErrNotFound	; 

        }

/**
 * Returns the connection parameters
 *
 * @param aType Type of service
 * @param aParameter Connection parameter of the service
 * @param aValue Value of the parameter
 * @return Symbian error code
 */
TInt MyCchService::GetConnectionParameter( TCCHSubserviceType /*aType*/,
        TCchConnectionParameter /*aParameter*/, TInt& /*aValue*/ ) const{ return KErrNone;}

/**
 * Returns the connection parameters
 *
 * @param aType Type of service
 * @param aParameter Connection parameter of the service
 * @param aValue Value of the parameter
 * @return Symbian error code
 */
TInt MyCchService::GetConnectionParameter( TCCHSubserviceType /*aType*/, 
        TCchConnectionParameter aParameter, RBuf& aValue ) const

        {
        if (aParameter == ECchUsername)
            {
            TBuf<512> username;
            username = KTestUserName;            
            aValue.Copy(username);  
            return KErrNone;
            }

        return KErrNotFound;
        }

/**
 * Sets the connection parameters
 *
 * @param aType The type of service
 * @param aParameter Connection parameter of the service
 * @param aValue Value of the parameter
 * @return Symbian error code
 */
TInt MyCchService::SetConnectionParameter( TCCHSubserviceType /*aType*/, 
        TCchConnectionParameter /*aParameter*/, TInt /*aValue*/ )

    {
    return KErrNone;
    }

/**
 * Sets the connection parameters
 *
 * @param aType The type of service
 * @param aParameter Connection parameter of the service
 * @param aValue Value of the parameter
 * @return Symbian error code
 */
TInt MyCchService::SetConnectionParameter( TCCHSubserviceType /*aType*/, 
        TCchConnectionParameter /*aParameter*/, const TDesC& /*aValue*/ )
    {
    return KErrNone;	
    }

/**
 * Reserves the service for exclusive usage
 *
 * @param aType Type of service
 * @return Symbian error code
 */
TInt MyCchService::Reserve( TCCHSubserviceType /*aType*/ ){ return KErrNone; }

/**
 * Frees the service of exclusive usage
 *
 * @param aType Type of service
 * @return Symbian error code
 */
TInt MyCchService::Free( TCCHSubserviceType /*aType*/ ){ return KErrNone; }

/**
 * Is the service reserved
 *
 * @param aType Type of service
 * @param aReserved True if the service is reserved
 * @return Symbian error code
 */
TInt MyCchService::IsReserved( TCCHSubserviceType /*aType*/, TBool& /*aReserved*/ ) const{ return KErrNone; }

/**
 * Returns the service id
 *
 * @return The id of the service
 */
TInt MyCchService::ServiceId() const{ return KErrNone; }

/**
 * @deprecated Do not use this anymore, change to AddObserver!
 *
 * Adds observer for listening service events
 *
 * @param aObserver Event observing class
 */
void MyCchService::SetObserver( MCchServiceStatusObserver& aObserver )
    {
    iObserver = &aObserver;
    }

/**
 * @deprecated Do not use this anymore, change to 
 * RemoveObserver method with paramater
 * 
 * Removes the observer of service events
 */
void MyCchService::RemoveObserver( )
    {
    iObserver = NULL;
    }


/**
 * Adds observer for listening service events
 *
 * @param aObserver Event observing class
 * @return KErrAlreadyExist Observer already added
 *         KErrNone Observer was added
 */
TInt MyCchService::AddObserver( MCchServiceStatusObserver& /*aObserver*/ ){ return KErrNone; }

/**
 * Removes the observer of service events
 *
 * @param aObserver Event observing class
 * @return KErrNotFound Observer was not found
 *         KErrNone Observer was removed
 */
TInt MyCchService::RemoveObserver( MCchServiceStatusObserver& /*aObserver*/ ){ return KErrNone; }

/**
 * Checks if the type is supported
 *
 * @param aType Type of service
 * @param aSupported True if the service type is supported
 * @return Symbian error code
 */
TInt MyCchService::IsSupported( TCCHSubserviceType /*aType*/, TBool& /*aSupported*/ ) const{ return KErrNone; }


//end of file



