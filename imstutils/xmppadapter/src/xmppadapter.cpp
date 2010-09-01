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
* Description:  Handles XMPPAdapter settings in OMA provisioning.
*
*/


// INCLUDE FILES
#include    <f32file.h>
#include    <CWPCharacteristic.h>
#include    <CWPParameter.h>
#include    <pdpcontextmanagerinternalcrkeys.h>
#include    <centralrepository.h>

#include    "imconnectionprovideruids.hrh"

#include    "WPAdapterUtil.h"
#include    "xmppadapter.h"

#include "xmppservicesettingsapi.h"
#include "xmppparams.h"

#include <spentry.h>
#include <spsettings.h>
#include <spproperty.h>
#include <spdefinitions.h>
#include <cvimpstsettingsstore.h> // application specific settings writer

#include "xmppadapterlogger.h"
#include <e32base.h>
#include <StringLoader.h>
#include <wpgeneralxmppsettingsadapter.rsg>
#include <bautils.h>
#include <f32file.h>
#include <WPAdapterUtil.h> // Adapter utils
#include <barsc.h>
// CONSTANTS
_LIT( KMecoImAppID,       "wXMPP" );  
//_LIT( KXMPPProtocol,       "APROTOCOL" );   
//_LIT( KXMPPServerAddr,       "AADDR" );  
_LIT(KSsl,"SSL");
_LIT(KPortNbr, "APORTNBR"); 
_LIT(KBrandId, "BRANDID");
const TInt KGranularity = 4;

_LIT( KResorcePathFolder, "\\resource\\");
_LIT( KResorcePathFile, "WPGeneralXMPPSettingsAdapter.rsc");


// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CXMPPAdapter::CXMPPAdapter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CXMPPAdapter::CXMPPAdapter() : iIsAppIdOk(EFalse)
    {
    LOGGER ( TXT ( "CXMPPAdapter::CXMPPAdapter() Start" ) );
    LOGGER ( TXT ( "CXMPPAdapter::CXMPPAdapter() Start" ) );

    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CXMPPAdapter::ConstructL()
    {
    LOGGER ( TXT ( "CXMPPAdapter::ConstructL() Start" ) );
    iSnapId = 11;
    iAppref = NULL;
    iAppName = NULL;
    RFs fs;
    
    TFileName resFilePath;
    resFilePath.Zero();
    resFilePath.Append( KResorcePathFolder() );
    resFilePath.Append( KResorcePathFile() );

    LOGGER ( TXT ( "CXMPPAdapter::ConstructL() End1" ) );
    TFileName path;
    Dll::FileName( path );
    
    User::LeaveIfError( fs.Connect() );
   
    TDriveName drive( TParsePtrC( path ).Drive() );
    
    HBufC* resourceFilePath = HBufC::NewLC( KMaxFileName );
    TPtr ptr( resourceFilePath->Des() );
    
    
    ptr.Append( resFilePath );
    ptr.Insert( 0, drive );
    
    path.Zero();
    path.Append( *resourceFilePath );

    
    BaflUtils::NearestLanguageFile( fs, path );
    iResFile.OpenL( fs, path );
    //To decide on whether default name is needed or not
    iDefaultName = ReadResourceL( R_QTN_IPCOMM_MSG_TEXT );
    iTitle = ReadResourceL( R_QTN_IPCOMM_MSG_TITLE );
    
    CleanupStack::PopAndDestroy(); //resourceFilePath
    iResFile.Close();
    
    LOGGER ( TXT ( "CXMPPAdapter::ConstructL() End" ) );

    
    fs.Close();
    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CXMPPAdapter* CXMPPAdapter::NewL()
    {
    LOGGER ( TXT ( "CXMPPAdapter::NewL() Start" ) );
    CXMPPAdapter* self = new( ELeave ) CXMPPAdapter;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    LOGGER ( TXT ( "CXMPPAdapter::NewL() End" ) );
    return self;
    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::~CXMPPAdapter
// Destructor
// -----------------------------------------------------------------------------
//
CXMPPAdapter::~CXMPPAdapter()
    { 
    LOGGER ( TXT ( "CXMPPAdapter::~CXMPPAdapter Start" ) );   
    delete iUserName;
    delete iPassword;
    delete iProviderId;
    delete iXmppServerAddress;
    delete iProtocol;
    delete iAppId;
    delete iDomainName;
    delete iBrandId;
    delete iAppref;
    
    if ( iAppName )
    delete iAppName;
    
    delete iDefaultName;
    delete iTitle;
    LOGGER ( TXT ( "CXMPPAdapter::~CXMPPAdapter End" ) );      
    }
// -----------------------------------------------------------------------------
// CXMPPAdapter::ReadResourceLC Reads the resource without having to use a CCoeEnv
// -----------------------------------------------------------------------------
//
HBufC* CXMPPAdapter::ReadResourceL( TInt aId )
    {
    LOGGER ( TXT ( "CXMPPAdapter::ReadResourceL() " ) );
    iResFile.ConfirmSignatureL( aId );
    HBufC8* readBuffer = iResFile.AllocReadLC( aId );
    TResourceReader theReader;
    theReader.SetBuffer( readBuffer );
    TPtrC textData = theReader.ReadTPtrC();
    
    HBufC* textBuffer = HBufC::NewL( textData.Length() );
    *textBuffer = textData;
    
    CleanupStack::PopAndDestroy( readBuffer );
    LOGGER ( TXT ( "CXMPPAdapter::ReadResourceL() " ) );
    return textBuffer;

    }
// -----------------------------------------------------------------------------
// CXMPPAdapter::ItemCount
// -----------------------------------------------------------------------------
//
TInt CXMPPAdapter::ItemCount() const
    {
    LOGGER ( TXT ( "CXMPPAdapter::ItemCount() " ) );
    // SummaryTitle(), SummaryText(), SaveL(), SetAsDefaultL() and
    // CanSetAsDefault() are called ItemCount() times
    return (iUserName && iPassword && iProviderId && iXmppServerAddress && iDomainName && iProtocol) ? 1 : 0;
    }


// -----------------------------------------------------------------------------
// CXMPPAdapter::DetailsL
// -----------------------------------------------------------------------------
//
TInt CXMPPAdapter::DetailsL( TInt /*aItem*/, MWPPairVisitor& /*aVisitor*/ )
    {
    LOGGER ( TXT ( "CXMPPAdapter::DetailsL() " ) );
    // Detail view is a feature for later release.
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::SaveL
// -----------------------------------------------------------------------------
//
void CXMPPAdapter::SaveL( TInt /*aItem*/ )
    {
    LOGGER ( TXT ( "CXMPPAdapter::CanSetAsDefault() Start" ) );
    
    __ASSERT_DEBUG( (iUserName && iPassword && iProviderId  && iXmppServerAddress && iDomainName && iProtocol), User::Leave( KErrCorrupt ) );

	CreateSettingsRecordL();   
    LOGGER ( TXT ( "CXMPPAdapter::CanSetAsDefault() End" ) );
    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::CanSetAsDefault
// -----------------------------------------------------------------------------
//
TBool CXMPPAdapter::CanSetAsDefault( TInt /*aItem*/ ) const
    {
    LOGGER ( TXT ( "CXMPPAdapter::CanSetAsDefault() t" ) );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::SetAsDefault
// -----------------------------------------------------------------------------
//
void CXMPPAdapter::SetAsDefaultL( TInt /*aItem*/ )
    {
    LOGGER ( TXT ( "CXMPPAdapter::SetAsDefaultL () Start" ) );
    // This shouldn't be called because CanSetAsDefault
    // always returns EFalse.
    LOGGER ( TXT ( "CXMPPAdapter::SetAsDefaultL() End" ) );
    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::VisitL
// -----------------------------------------------------------------------------
//
void CXMPPAdapter::VisitL( CWPCharacteristic& aCharacteristic )
    {
    LOGGER ( TXT ( "CXMPPAdapter::VisitL( CWPCharacteristic& aCharacteristic ) Start" ) );
    switch( aCharacteristic.Type() )
        {
        case KWPApplication:
            {
            
            aCharacteristic.AcceptL( *this );
            iIsAppIdOk = ETrue;
            break;            
            }
        case KWPResource:
        	{
        	if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
	        	{	       
	        	aCharacteristic.AcceptL( *this );
	        	}            
	        break;            	
        	}
        case KWPAppAddr:
            {
            
            if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
	        	{
	        	aCharacteristic.AcceptL( *this );
	        	}
            break;            
            }
        default:
        	{
            break;
        	}
        }
    LOGGER ( TXT ( "CXMPPAdapter::VisitL( CWPCharacteristic& aCharacteristic ) End" ) );
    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::VisitL
//
//   Used to parse, the following always-on related provisioning settings
//
//   <characteristic type="VENDORCONFIG">
//          <parm name="NAME" value="MRBLOBBY"/>
//          <parm name="AWON-PDPC" value="11"/>
//          <parm name="T-RETRY" value="100"/>
//      </characteristic>
//   </characteristic>
// -----------------------------------------------------------------------------
//
void CXMPPAdapter::VisitL( CWPParameter& aParameter )
    {
    LOGGER ( TXT ( "CXMPPAdapter::VisitL( CWPParameter& aParameter ) Start" ) );
    switch( aParameter.ID() )
        {
        case EWPNamedParameter:
        	{
        	if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
	        	{
	        	if(aParameter.Name().Compare(KSsl) == 0)
	        		{
	        	    ParseIntegerL(aParameter.Value(),iSSL);
	        		}
	        	if(aParameter.Name().Compare(KPortNbr) == 0)
	        		{
	        		ParseIntegerL(aParameter.Value(),iPortNbr);
	        		}
	        	if(aParameter.Name().Compare(KBrandId) == 0)
	        	    {
	        	    if (iBrandId)
	                    {
	                    delete iBrandId;
	                    iBrandId = NULL;  
	                    }
	        	    iBrandId = aParameter.Value().AllocL();
	        	    }
	        	}
            break;
        	}
        case EWPParameterName:  
        	{
        	if ( iAppName )
        	    {
        	    delete iAppName;
        	    iAppName = NULL;
        	    }
        	iAppName = aParameter.Value().AllocL();
	         break; 
        	}          
           
        case EWPParameterAppID:
            {
        	if (iAppId)
	        	{
	        	delete iAppId;
	        	iAppId = NULL;	
	        	}
	        iAppId = aParameter.Value().AllocL();
            iAppIdPtr.Set(iAppId->Des());
            break;
            }    
        
        case EWPParameterAProtocol:
        	{
        	if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
	        	{
         		if (iProtocol)
		        	{
		        	delete iProtocol;
		        	iProtocol = NULL;	
		        	}
        		iProtocol = aParameter.Value().AllocL();
	        	}
        	break;	
        	}
        case EWPParameterAddr:
        	{
        	if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
	        	{
        		if (iXmppServerAddress)
		        	{
		        	delete iXmppServerAddress;
		        	iXmppServerAddress = NULL;	
		        	}
        		iXmppServerAddress = aParameter.Value().AllocL();
	        	}
        	break;	
        	}
        case EWPParameterAAuthName:
        	{
        	if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
	        	{     		
	        	
	        	if (iUserName)
		        	{
		        	delete iUserName;
		        	iUserName = NULL;	
		        	}
	        	iUserName = aParameter.Value().AllocL();
	        	}
        	break;	
        	}
        case EWPParameterAAuthSecret:
        	{
        	if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
	        	{       	
				if (iPassword)
					{
					delete iPassword;
					iPassword = NULL;	
					}

	        	iPassword = aParameter.Value().AllocL();
	        	}
        	break;	
        	}
        
        case EWPParameterProviderID:
        	{
			if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
				{
	        	if (iProviderId)
					{
					delete iProviderId;
					iProviderId = NULL;	
					}

	        	iProviderId = aParameter.Value().AllocL();
				}
        	break;	
        	}
        case EWPParameterDomain :
            {
            if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
            {           
            if (iDomainName)
                {
                delete iDomainName;
                iDomainName = NULL;   
                }
        
            iDomainName = aParameter.Value().AllocL();
            }
            break;  
            }
        case EWPParameterAppRef:
        		{
				if ( 0 == iAppIdPtr.Compare( KMecoImAppID() ) )
					{
		        	if (iAppref)
						{
						delete iAppref;
						iAppref = NULL;	
						}
		        	iAppref= aParameter.Value().AllocL();
					}
				break;	
			      }
        default:
            break;
        }
    LOGGER ( TXT ( "CXMPPAdapter::VisitL( CWPParameter& aParameter ) End" ) );
    }


// ---------------------------------------------------------------------------
// From class CWPAdapter.
// CWPGeneralVoIPSettingsAdapter::SummaryTitle
// Returns the summary title of general XMPP settings item.
// ---------------------------------------------------------------------------
//
const TDesC16& CXMPPAdapter::SummaryTitle( 
    TInt /*aIndex*/ ) const
    {
    LOGGER ( TXT ( "CXMPPAdapter::SummaryText() " ) );
    if ( iIsAppIdOk )
        {
        LOGGER ( TXT ( "returning CXMPPAdapter::SummaryText() " ) );
        //LOGGER ( TXT ( "CXMPPAdapter::ConstructL() Title in SummaryText is %s" ), *iTitle );
        return *iTitle;
        }
    return KNullDesC16;
    }

// ---------------------------------------------------------------------------
// CWPGeneralVoIPSettingsAdapter::SummaryText
// From class CWPAdapter.
// Returns the summary text of general XMPP settings.
// ---------------------------------------------------------------------------
//
const TDesC16& CXMPPAdapter::SummaryText( 
    TInt /*aIndex*/ ) const
    {
    LOGGER ( TXT ( "CXMPPAdapter::SummaryText() " ) );
    if ( iIsAppIdOk )
        {
        LOGGER ( TXT ( " returnign CXMPPAdapter::SummaryText() " ) );
        if ( NULL == iAppName )
            {
            return *iDefaultName;
            }
        return *iAppName;
        }
    LOGGER ( TXT ( "CXMPPAdapter::SummaryText() " ) );
    return KNullDesC16;
    }


// -----------------------------------------------------------------------------
// CXMPPAdapter::VisitLinkL
// -----------------------------------------------------------------------------
//
void CXMPPAdapter::VisitLinkL( CWPCharacteristic& /*aCharacteristic*/ )
    {
    LOGGER ( TXT ( "CXMPPAdapter::VisitLinkL() " ) );
    }

// -----------------------------------------------------------------------------
// CXMPPAdapter::ParseIntegerL
// -----------------------------------------------------------------------------
//
void CXMPPAdapter::ParseIntegerL( const TDesC& aPtr, TInt& aInt )
    {
    LOGGER ( TXT ( "CXMPPAdapter::ParseIntegerL " ) );
    TLex lex( aPtr );
    User::LeaveIfError( lex.Val( aInt ) );
    }
// -----------------------------------------------------------------------------
// CXMPPAdapter::CheckforXmppserviceL
// This function will check for the IM Subservice in sp settings table
// if it exists in any of the service it will return ETrue else it will return EFalse
// -----------------------------------------------------------------------------
//
TBool CXMPPAdapter::CheckforXmppserviceL()
    {
    // get the existing services from sp settings table
    CSPSettings* serviceSettings = CSPSettings::NewLC(); 
    RIdArray serviceIds;
    User::LeaveIfError( serviceSettings->FindServiceIdsL( serviceIds ) );
    TBool xmppexists = EFalse;
    TInt servicecount = serviceIds.Count();
    for ( TInt i( 0 ); i < servicecount; i++ )
        {
        CSPProperty* property = CSPProperty::NewLC();
        TInt err = serviceSettings->FindPropertyL( serviceIds[ i ],EPropertyIMSubServicePluginId,
                *property );
        CleanupStack::PopAndDestroy( property ); 
        if(!err)
            {
            xmppexists = ETrue;
            break; // just to break the loop
            }
        } 
    CleanupStack::PopAndDestroy( serviceSettings ); 
    return xmppexists;

    }
//------------------------------------------------------------------------
//CXMPPAdapter::::CreateSettingsRecordL()
//------------------------------------------------------------------------
void CXMPPAdapter::CreateSettingsRecordL()
    {
    LOGGER ( TXT ( "CXMPPAdapter::CreateSettingsRecordL() Start" ) );
    CXmppSettingsApi* sapi = CXmppSettingsApi::NewLC();
    iSettingsRecordId=0;
    TRAPD(err,sapi->DefaultSettingsRecordL(iSettingsRecordId));
    if( err == KErrNone && iSettingsRecordId > 0)
        {
        if(CheckforXmppserviceL())
            {
            CleanupStack::PopAndDestroy( sapi );
            User::Leave(KErrNotSupported);
            }
        } 
    sapi->CreateSettingsRecordL( iSettingsRecordId );
    
    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamServerPort(),/*5223 443*/ iPortNbr); 
    
    sapi->SetParamL( iSettingsRecordId, XmppParams::KXmppParamHttpsProxyServerPort(),8080 );

    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamSnapId(),5 );
    
    sapi->SetParamL( iSettingsRecordId, XmppParams:: KXmppParamXmppServerAddress(),
                                              *iXmppServerAddress );
                                                                                        
    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamUsername(),*iUserName );
    
    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamPassword(),*iPassword );
    
    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamResource(),
                                              _L("gtalk_resource") );

    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamHttpsProxyServerAddress(),
                                              _L("172.16.42.135") );

    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamAlais(), _L("") );

    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamOldSSLRequired(),/*ETrue  EFalse */ iSSL );
    sapi->SetParamL( iSettingsRecordId, XmppParams::KXmppParamRegisterRequired(),EFalse );
                                              
    sapi->SetParamL( iSettingsRecordId,
                                  XmppParams::KXmppParamConnMgrBus(),
                    _L("org.freedesktop.Telepathy.ConnectionManager.gabble" ) );
                          		  
    sapi->SetParamL( iSettingsRecordId,
								  XmppParams::KXmppParamConnMgrPath(),
					_L("/org/freedesktop/Telepathy/ConnectionManager/gabble" ) );
								  
    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamProtocol(),*iProtocol );
    
    sapi->SetParamL( iSettingsRecordId, XmppParams::KXmppParamPresenceStatusFreeTextOnline(),
                                              _L("") );

    sapi->SetParamL( iSettingsRecordId, XmppParams::KXmppParamPresenceStatusFreeTextAway(),
                                              _L(""));

    sapi->SetParamL( iSettingsRecordId, XmppParams::KXmppParamPresenceStatusFreeTextBusy(),
                                              _L(""));                                              

    sapi->SetParamL( iSettingsRecordId, XmppParams::KXmppParamPresenceStatusFreeTextOffline(),
                                              _L(""));  
    
    sapi->SetParamL( iSettingsRecordId, XmppParams::KXmppParamPresenceStatusFreeTextInvisible(),
                                              _L(""));  
    
    sapi->SetParamL( iSettingsRecordId, XmppParams::KXmppParamLastLoggedInUserName(),
                                                  _L(""));    

    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamPresenceStatus(),0 );
                                              
    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamAutoAcceptBuddyReq(),EFalse );
    
    sapi->SetParamL( iSettingsRecordId,XmppParams::KXmppParamDomain(),*iDomainName );
    UpdateServiceSettingsL();                                          

    WriteAppSettingsL();
    CleanupStack::PopAndDestroy( sapi );
    LOGGER ( TXT ( "CXMPPAdapter::CreateSettingsRecordL() End" ) );
    }
    
// ---------------------------------------------------------------------------
//CXMPPAdapter::UpdateServiceSettingsL
// ---------------------------------------------------------------------------
//
void CXMPPAdapter::UpdateServiceSettingsL()
    {
    LOGGER ( TXT ( "CXMPPAdapter::UpdateServiceSettingsL() Start" ) );
    CSPSettings* serviceSettings = CSPSettings::NewLC();   
    RIdArray serviceIds;

    User::LeaveIfError( serviceSettings->FindServiceIdsL( serviceIds ) );
    
    

    if( serviceIds.Count() )
        {
        CDesCArrayFlat* serviceNames = new( ELeave ) CDesCArrayFlat( KGranularity );
        CleanupStack::PushL( serviceNames );

        User::LeaveIfError( serviceSettings->FindServiceNamesL( serviceIds,
                                                                *serviceNames) );

        for ( TInt i=0; i < serviceNames->MdcaCount(); i++ )
            {
            if ( ( serviceNames->MdcaPoint(i)).Compare( *iProviderId )  == 0 )
                {
                
                iServiceProviderId = serviceIds[i];
                break;
                }
            }
        CleanupStack::PopAndDestroy( serviceNames );
        }
    
    if ( 0 >= iServiceProviderId )
        {
        
        CSPEntry* entry = CSPEntry::NewLC();
        entry->SetServiceName( *iProviderId );
        TInt entryRetvalue = serviceSettings->AddEntryL( *entry );
        User::LeaveIfError( entryRetvalue );
        iServiceProviderId = entry->GetServiceId();
        CleanupStack::PopAndDestroy( entry );
        }
	      

    // Add/Update properties
    CSPProperty* property = CSPProperty::NewLC(); 
    
    
    property->SetName( ESubPropertyIMLaunchUid );
    property->SetValue( 0x200255D0 );

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
	
		
    property->SetName( EPropertyContactViewPluginId );
    property->SetValue( 0x20012423 ); // plugin view id  

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
	
    
    property->SetName( ESubPropertyIMSettingsId );
    property->SetValue( iSettingsRecordId );

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
    
    
    
    property->SetName( EPropertyIMSubServicePluginId );
    property->SetValue( KIMConnectivityPluginImplUid );

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
    
    
    property->SetName( ESubPropertyIMPreferredIAPId );
    property->SetValue( iSnapId );

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
    
    
    
    property->SetName( ESubPropertyIMPreferredSNAPId );
    property->SetValue( iSnapId );

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
    
    
    property->SetName( ESubPropertyIMEnabled );
    property->SetValue( EOff );

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
    
    RBuf brandId;
    if( iBrandId )
        {
        brandId.CreateL(*iBrandId );
        }
    else
        {
        brandId.CreateL(KNullDesC);
        }
    CleanupClosePushL( brandId );
    property->SetName( EPropertyBrandId );
    property->SetValue( brandId );

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
    CleanupStack::PopAndDestroy( &brandId );
    
    
    property->SetName( EPropertyBrandLanguage );
    
    // ELangInternationalEnglish is selected as Default Brand Language Id. 
    property->SetValue( ELangInternationalEnglish );       

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
    
    
	RBuf contactDB;
	TBuf <50> string;
	// The store name should be service specific, and not hardcoded, hence we use iServiceProviderId to form uniqueness.
	string.Format(_L("cntdb://c:vimpststore%d.cdb"), iServiceProviderId);
	contactDB.CreateL (string); 
    CleanupClosePushL( contactDB );
    property->SetName( EPropertyContactStoreId );
    property->SetValue( contactDB );

    User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
                                                               *property ) );
    CleanupStack::PopAndDestroy( &contactDB );
    
	/*PresenceSubservice*/
	property->SetName( ESubPropertyPresenceSettingsId );
	property->SetValue( iSettingsRecordId );

	User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
	                                               *property ) );



	property->SetName( EPropertyPresenceSubServicePluginId );
	property->SetValue( KIMConnectivityPluginImplUid );

	User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
	                                               *property ) );


	property->SetName( ESubPropertyPresencePreferredIAPId );
	property->SetValue( iSnapId );

	User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
	                                               *property ) );



	property->SetName( ESubPropertyPresencePreferredSNAPId );
	property->SetValue( iSnapId );

	User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
	                                               *property ) );


	property->SetName( ESubPropertyPresenceEnabled );
	property->SetValue( EOff );

	User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
	                                               *property ) );



	property->SetName( EPropertyPCSPluginId );
	property->SetValue( 0x101FB3E7 ); 

	User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
	                                                               *property ) ); 

	property->SetName( ESubPropertyPresenceAddrScheme );
	property->SetValue( *iDomainName ); 
	   
	User::LeaveIfError( serviceSettings->AddOrUpdatePropertyL( iServiceProviderId,
	                                                                   *property ) ); 

	//End of Presence Subservice

	CleanupStack::PopAndDestroy( property );
    CleanupStack::PopAndDestroy( serviceSettings );
    LOGGER ( TXT ( "CXMPPAdapter::UpdateServiceSettingsL() End" ) );
    }

// ---------------------------------------------------------------------------
//CXMPPAdapter::WriteAppSettingsL
// ---------------------------------------------------------------------------
//
void CXMPPAdapter::WriteAppSettingsL()
    {
    MVIMPSTSettingsStore*  settingsStore = CVIMPSTSettingsStore::NewLC();
    // write cancel login not supported : since xmpp doesnot have the support
    settingsStore->SetL( iServiceProviderId , EServiceCancelLoginNotSupported , 1 );
    // write change connection not supported : since xmpp doesnot have the support
    settingsStore->SetL( iServiceProviderId , EServiceChangeConnectionNotSupported , 1 );
    CleanupStack::PopAndDestroy();//settingStore
    }
//  End of File
