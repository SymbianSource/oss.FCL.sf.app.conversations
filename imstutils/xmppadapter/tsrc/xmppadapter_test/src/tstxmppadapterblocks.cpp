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
* Description:  tstxmppadapterblocks.cpp
*
*/




// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "tstxmppadapter.h"
#include "xmppadapter.h"
#include  <CWPCharacteristic.h>
#include  <CWPParameter.h>
#include "WPElementFactory.h"
#include  <CWPAdapter.h>
#include  <ecom/ecom.h>


// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;


//CONSTANTS
// Constants for setter methods.
_LIT8( KNameOK,       "Test general IM settings" );
_LIT8( KNameNotOK,    "Test other settings" );
_LIT( KMecoImAppID,       "wXMPP" );  
_LIT( KMecoNotImAppID,       "XMPP" );
_LIT( KXMPPProtocol,       "APROTOCOL" );   
_LIT( KXMPPServerAddr,       "AADDR" );  
_LIT( KXMPPAuthName,       "testui123@gmail.com" );   
_LIT( KXMPPAuthSecret,       "testui_123" ); 
_LIT( KXMPPDomain,       "DOMAIN" ); 
_LIT(KProviderID,"GTalk"); 
_LIT(KProtocolvalue,"jabber"); 
_LIT(KServer,"talk.google.com"); 
_LIT(KDomain,"gmail.com");
_LIT(KInteger,"121");
_LIT(KSslvalue,"1");
_LIT(KPortNbrvalue,"443");
_LIT(KBrandIdvalue,"Gizmo");

_LIT(KSsl,"SSL");
_LIT(KPortNbr, "APORTNBR");
_LIT(KBrandId, "BRANDID");

 




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CtstXmppAdapter::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CtstXmppAdapter::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CtstXmppAdapter::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CtstXmppAdapter::RunMethodL( 
    CStifItemParser& aItem ) 
    {
	iLog->Log ( _L ( "In RunMethodL method" ) );
	

    //TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksRequests ); 
    //TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles );  

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.    
       
        ENTRY( "xmppadapter_new", CtstXmppAdapter::xmppadapter_newL ),
        ENTRY( "xmppadapter_visit_kwpapplication", CtstXmppAdapter::xmppadapter_visit_kwpapplicationL ),
        ENTRY( "xmppadapter_visit_EWPParameterAppID", CtstXmppAdapter::xmppadapter_visit_EWPParameterAppIDL ),
        ENTRY( "xmppadapter_visit_KWPResource", CtstXmppAdapter::xmppadapter_visit_KWPResourceL ),
        //ENTRY( "xmppadapter_visit_withoutAppId_KWPResource", CtstXmppAdapter::xmppadapter_visit_withoutAppId_KWPResourceL ),
        
        ENTRY( "xmppadapter_visit_EWPParameterNonAppID", CtstXmppAdapter::xmppadapter_visit_EWPParameterNonAppIDL ),

        ENTRY( "xmppadapter_visit_EWPParameterAAuthName", CtstXmppAdapter::xmppadapter_visit_EWPParameterAAuthNameL ),
        ENTRY( "xmppadapter_visit_EWPParameterAAuthSecret", CtstXmppAdapter::xmppadapter_visit_EWPParameterAAuthSecretL ),
        ENTRY( "xmppadapter_visit_EWPParameterName_Protocol", CtstXmppAdapter::xmppadapter_visit_EWPParameterName_ProtocolL ),
        ENTRY( "xmppadapter_visit_EWPParameterProviderID", CtstXmppAdapter::xmppadapter_visit_EWPParameterProviderIDL ),
        ENTRY( "xmppadapter_visit_EWPParameterName_Server", CtstXmppAdapter::xmppadapter_visit_EWPParameterName_ServerL ),
        ENTRY( "xmppadapter_visitLink", CtstXmppAdapter::xmppadapter_visitLinkL ),
        ENTRY( "xmppadapter_SummaryText", CtstXmppAdapter::xmppadapter_SummaryTextL ),
        ENTRY( "xmppadapter_SummaryTitle", CtstXmppAdapter::xmppadapter_SummaryTitleL ),
        ENTRY( "xmppadapter_Save", CtstXmppAdapter::xmppadapter_SaveL ),
        ENTRY( "xmppadapter_ItemCount", CtstXmppAdapter::xmppadapter_ItemCountL ),
        ENTRY( "xmppadapter_ItemZeroCount", CtstXmppAdapter::xmppadapter_ItemZeroCountL ),
        ENTRY( "xmppadapter_Details", CtstXmppAdapter::xmppadapter_DetailsL ),
        ENTRY( "xmppadapter_visit_KWPAppAddr", CtstXmppAdapter::xmppadapter_visit_KWPAppAddrL ),
        ENTRY( "xmppadapter_visit_EWPNamedParameter_KSsl", CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KSslL ),
        ENTRY( "xmppadapter_visit_EWPNamedParameter_KPortNbr", CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KPortNbrL ),
        ENTRY( "xmppadapter_visit_EWPParameterDomainL", CtstXmppAdapter::xmppadapter_visit_EWPParameterDomainL ),
        ENTRY( "xmppadapter_visit_EWPNamedParameter_KBrandIdL", CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KBrandIdL ),
                
        
       
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_newL
//----------------------------------------------------------------------------
//
TInt CtstXmppAdapter::xmppadapter_newL( CStifItemParser& /*aItem */)
	{
  	iLog->Log ( _L("In xmppadapter_newL" ) );
  	iEcomDtorID = TUid::Null();
  	CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
	if ( NULL == xmppadapter ) 
	    {
	    iLog->Log ( _L( "xmppadapter_newL failed to allocate memory" ) );		
		return KErrGeneral;	
	    }
	else
		{
		REComSession::DestroyedImplementation( iEcomDtorID );
 
        iLog->Log ( _L( "xmppadapter_newL test case passed" ) );
		}
	
	return KErrNone;			
	}
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPParameterAppIDL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_kwpapplicationL(CStifItemParser& /*aItem*/ )
    {
    
   	iLog->Log ( _L("xmppadapter_visit_kwpapplicationL" ) );
   	iEcomDtorID = TUid::Null();
   	CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
   	if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_kwpapplicationL test case Failed" ) );
   	    return KErrGeneral;	
	    }
	else
		{
		CWPCharacteristic* klf = 
        WPElementFactory::CreateCharacteristicLC( KWPApplication);
	    xmppadapter->VisitL(*klf);
	    iLog->Log ( _L( "xmppadapter_visit_kwpapplicationL  test case passed" ) );
	    CleanupStack::PopAndDestroy(klf);
	    REComSession::DestroyedImplementation( iEcomDtorID );
		}
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPParameterAppIDL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPParameterAppIDL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPParameterAppIDL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterAppIDL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
	    xmppadapter->VisitL(*klf);
        
	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterAppIDL  test case passed" ) );
	    CleanupStack::PopAndDestroy(klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
	
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_KWPResourceL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_KWPResourceL(CStifItemParser& /*aItem*/ )
    {
   	iLog->Log ( _L("xmppadapter_visit_KWPResource" ) );
   	iEcomDtorID = TUid::Null();
   	CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
   	if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_KWPResource test case Failed" ) );
   	    return KErrGeneral;	
	    }
	else
		{
		CWPParameter* klf1 = CWPParameter::NewLC();
   		klf1->SetID(EWPParameterAppID);
   		klf1->SetValueL(KMecoImAppID);
	    xmppadapter->VisitL(*klf1);
	    
		CWPCharacteristic* klf = 
        WPElementFactory::CreateCharacteristicLC( KWPResource);
	    xmppadapter->VisitL(*klf);
	    iLog->Log ( _L( "xmppadapter_visit_KWPResource  test case Passed" ) );
	    CleanupStack::PopAndDestroy(2,klf1);
	    REComSession::DestroyedImplementation( iEcomDtorID );
		}
    return KErrNone;
    }
/* //----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_withoutAppId_KWPResourceL
//----------------------------------------------------------------------------
//
TInt CtstXmppAdapter::xmppadapter_visit_withoutAppId_KWPResourceL(CStifItemParser& /*aItem )
    {
   	iLog->Log ( _L("xmppadapter_visit_withoutAppId_KWPResourceL" ) );
   	iEcomDtorID = TUid::Null();
   	CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
   	if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_withoutAppId_KWPResourceL test case Failed" ) );
   	    return KErrGeneral;	
	    }
	else
		{
		CWPCharacteristic* klf = 
        WPElementFactory::CreateCharacteristicLC( KWPResource);
	    xmppadapter->VisitL(*klf);
	    iLog->Log ( _L( "xmppadapter_visit_withoutAppId_KWPResourceL  test case Passed" ) );
	    CleanupStack::PopAndDestroy(klf);
	    REComSession::DestroyedImplementation( iEcomDtorID );
		}
    return KErrNone;
    }
*/
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPParameterNonAppIDL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPParameterNonAppIDL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPParameterNonAppIDL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterNonAppIDL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoNotImAppID);
	    xmppadapter->VisitL(*klf);
        
	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterNonAppIDL  test case passed" ) );
	    CleanupStack::PopAndDestroy(klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
	
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPParameterAAuthNameL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPParameterAAuthNameL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPParameterAAuthName" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterAAuthName test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
   		xmppadapter->VisitL(*klf);
   		
   		CWPParameter* klf1 = CWPParameter::NewLC();
        klf1->SetID(EWPParameterAAuthName);
   		klf1->SetValueL(KXMPPAuthName);
   		xmppadapter->VisitL(*klf1);
	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterAAuthName  test case passed" ) );
	    CleanupStack::PopAndDestroy(2,klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPParameterAAuthSecretL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPParameterAAuthSecretL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPParameterAAuthSecret" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterAAuthSecret test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
   		xmppadapter->VisitL(*klf);
   		
   		CWPParameter* klf1 = CWPParameter::NewLC();
        klf1->SetID(EWPParameterAAuthSecret);
   		klf1->SetValueL(KXMPPAuthSecret);
   		xmppadapter->VisitL(*klf1);
	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterAAuthSecret  test case passed" ) );
	    CleanupStack::PopAndDestroy(2,klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPParameterProviderIDL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPParameterProviderIDL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPParameterProviderID" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterProviderID test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
   		xmppadapter->VisitL(*klf);
   		
   		CWPParameter* klf1 = CWPParameter::NewLC();
        klf1->SetID(EWPParameterProviderID);
   		klf1->SetValueL(KProviderID);
   		xmppadapter->VisitL(*klf1);
	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterProviderID  test case passed" ) );
	    CleanupStack::PopAndDestroy(2,klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }

//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPParameterName_ProtocolL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPParameterName_ProtocolL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPParameterProviderID" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterName_ProtocolL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
   		xmppadapter->VisitL(*klf);
   		
   		CWPParameter* klf1 = CWPParameter::NewLC();
        klf1->SetID(EWPParameterName);
        klf1->SetNameL(KXMPPProtocol);
   		klf1->SetValueL(KProtocolvalue);
   		xmppadapter->VisitL(*klf1);
	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterName_ProtocolL  test case passed" ) );
	    CleanupStack::PopAndDestroy(2,klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
		    
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPParameterName_ServerL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPParameterName_ServerL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPParameterName_ServerL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterName_ServerL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
   		xmppadapter->VisitL(*klf);
   		
   		CWPParameter* klf1 = CWPParameter::NewLC();
        klf1->SetID(EWPParameterAddr);
        klf1->SetNameL(KXMPPServerAddr);
   		klf1->SetValueL(KServer);
   		xmppadapter->VisitL(*klf1);
	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterName_ServerL  test case passed" ) );
	    CleanupStack::PopAndDestroy(2,klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visitLinkL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visitLinkL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visitLinkL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visitLinkL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   	    CWPCharacteristic* klf = 
        WPElementFactory::CreateCharacteristicLC( KWPResource);
   		xmppadapter->VisitLinkL(*klf);
	    iLog->Log ( _L( "xmppadapter_visitLinkL  test case passed" ) );
	    CleanupStack::PopAndDestroy(klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
		    
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_SummaryTextL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_SummaryTextL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_SummaryText" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                    iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_SummaryText test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		if(xmppadapter->SummaryText(0).Compare(KNullDesC()) == 0)
   			{
   		    iLog->Log ( _L( "xmppadapter_SummaryText  test case passed" ) );
   			}
	    else
	    	{
	    	REComSession::DestroyedImplementation( iEcomDtorID );
		    iLog->Log ( _L( "xmppadapter_SummaryText test case Failed" ) );
	   	    return KErrGeneral;
	    	}
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_SummaryTitleL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_SummaryTitleL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_SummaryText" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                    iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_SummaryTitle test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		if(xmppadapter->SummaryTitle(0).Compare(KNullDesC()) == 0)
   			{
   		    iLog->Log ( _L( "xmppadapter_SummaryTitle  test case passed" ) );
   			}
	    else
	    	{
	    	REComSession::DestroyedImplementation( iEcomDtorID );
		    iLog->Log ( _L( "xmppadapter_SummaryTitle test case Failed" ) );
	   	    return KErrGeneral;
	    	}
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_SaveL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_SaveL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_SaveL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_SaveL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		//1 appid
   		CWPParameter* klf1 = CWPParameter::NewLC();
   		klf1->SetID(EWPParameterAppID);
   		klf1->SetValueL(KMecoImAppID);
	    xmppadapter->VisitL(*klf1);
   		
   		//2 user id
   	   	klf1->SetID(EWPParameterAAuthName);
   		klf1->SetValueL(KXMPPAuthName);
   		xmppadapter->VisitL(*klf1);
	    
	    //3 providerid
	    klf1->SetID(EWPParameterProviderID);
   		klf1->SetValueL(KProviderID);
   		xmppadapter->VisitL(*klf1);
	    
	    //4 password
	   	klf1->SetID(EWPParameterAAuthSecret);
   		klf1->SetValueL(KXMPPAuthSecret);
   		xmppadapter->VisitL(*klf1);
	    
	    //5 parameter- protocol
	   	klf1->SetID(EWPParameterAProtocol);
        klf1->SetNameL(KXMPPProtocol);
   		klf1->SetValueL(KProtocolvalue);
	    xmppadapter->VisitL(*klf1);
	    
	    //6 parameter- serveraddress
	    klf1->SetID(EWPParameterAddr);
        klf1->SetNameL(KXMPPServerAddr);
   		klf1->SetValueL(KServer);
	    xmppadapter->VisitL(*klf1);
	    
	     //7 parameter- ssl
	    klf1->SetID(EWPNamedParameter);
        klf1->SetNameL(KSsl);
   		klf1->SetValueL(KSslvalue);
	    xmppadapter->VisitL(*klf1);
	    
         //8 parameter- portnumber
	    klf1->SetID(EWPNamedParameter);
        klf1->SetNameL(KPortNbr);
   		klf1->SetValueL(KPortNbrvalue);
	    xmppadapter->VisitL(*klf1);

       klf1->SetID(EWPParameterDomain);
       klf1->SetNameL(KXMPPDomain);
       klf1->SetValueL(KDomain);
       xmppadapter->VisitL(*klf1);
	    
	    xmppadapter->SaveL(0);
	    iLog->Log ( _L( "xmppadapter_SaveL  test case passed" ) );
	    CleanupStack::PopAndDestroy(klf1);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
		    
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_SaveL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_ItemCountL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_SaveL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_SaveL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		//1 appid
   		CWPParameter* klf1 = CWPParameter::NewLC();
   		klf1->SetID(EWPParameterAppID);
   		klf1->SetValueL(KMecoImAppID);
	    xmppadapter->VisitL(*klf1);
   		
   		//2 user id
   	   	klf1->SetID(EWPParameterAAuthName);
   		klf1->SetValueL(KXMPPAuthName);
   		xmppadapter->VisitL(*klf1);
	    
	    //3 providerid
	    klf1->SetID(EWPParameterProviderID);
   		klf1->SetValueL(KProviderID);
   		xmppadapter->VisitL(*klf1);
	    
	    //4 password
	   	klf1->SetID(EWPParameterAAuthSecret);
   		klf1->SetValueL(KXMPPAuthSecret);
   		xmppadapter->VisitL(*klf1);
	    
	    //5 parameter- protocol
	   	klf1->SetID(EWPParameterAProtocol);
   		klf1->SetValueL(KProtocolvalue);
	    xmppadapter->VisitL(*klf1);
	    
	    //6 parameter- serveraddress
	    klf1->SetID(EWPParameterAddr);
   		klf1->SetValueL(KServer);
	    xmppadapter->VisitL(*klf1);
	    
	     //7 parameter- ssl
	    klf1->SetID(EWPNamedParameter);
        klf1->SetNameL(KSsl);
   		klf1->SetValueL(KSslvalue);
	    xmppadapter->VisitL(*klf1);
	    
         //8 parameter- portnumber
	    klf1->SetID(EWPNamedParameter);
        klf1->SetNameL(KPortNbr);
   		klf1->SetValueL(KPortNbrvalue);
       xmppadapter->VisitL(*klf1);
	    klf1->SetID(EWPParameterDomain);
        klf1->SetNameL(KXMPPDomain);
   		klf1->SetValueL(KDomain);
	    xmppadapter->VisitL(*klf1);


	    if(xmppadapter->ItemCount())
	    	{
	    	iLog->Log ( _L( "xmppadapter_SaveL  test case passed" ) );
	    	}
	    else
	    	{
	    	CleanupStack::PopAndDestroy(klf1);
	   	    REComSession::DestroyedImplementation( iEcomDtorID );
	    	iLog->Log ( _L( "xmppadapter_SaveL test case Failed" ) );
   	        return KErrGeneral;	
	    	}
	    
	    CleanupStack::PopAndDestroy(klf1);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_SaveL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_ItemZeroCountL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_SaveL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_SaveL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
	    if(xmppadapter->ItemCount())
	    	{
	    	REComSession::DestroyedImplementation( iEcomDtorID );
	    	iLog->Log ( _L( "xmppadapter_SaveL  test case Failed" ) );
	        return KErrGeneral;	
	    	}
	    else
	    	{
	    	iLog->Log ( _L( "xmppadapter_SaveL test case Passed" ) );
	    	}
	    
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_DetailsL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_DetailsL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_SaveL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_SaveL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		// The DetailsL method is NOT supported by this adapter
    	MWPPairVisitor* tempPairVisitor = NULL;
    	TInt err = xmppadapter->DetailsL(0,*tempPairVisitor);
    	REComSession::DestroyedImplementation( iEcomDtorID );

	    if(KErrNotSupported != err )
	    	{
	    	iLog->Log ( _L( "xmppadapter_SaveL  test case Failed" ) );
	        return KErrGeneral;	
	    	}
	    else
	    	{
	    	iLog->Log ( _L( "xmppadapter_SaveL test case Passed" ) );
	    	}
		}
    return KErrNone;
    }
    
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_KWPAppAddrL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_KWPAppAddrL(CStifItemParser& /*aItem*/ )
    {
    
   	iLog->Log ( _L("xmppadapter_visit_KWPAppAddrL" ) );
   	iEcomDtorID = TUid::Null();
   	CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
   	if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_KWPAppAddrL test case Failed" ) );
   	    return KErrGeneral;	
	    }
	else
		{
		CWPCharacteristic* klf = 
        WPElementFactory::CreateCharacteristicLC( KWPAppAddr);
	    xmppadapter->VisitL(*klf);
	    iLog->Log ( _L( "xmppadapter_visit_KWPAppAddrL  test case passed" ) );
	    CleanupStack::PopAndDestroy(klf);
	    REComSession::DestroyedImplementation( iEcomDtorID );
		}
    return KErrNone;
    }
 
//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KSslL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KSslL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPNamedParameter_KSslL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPNamedParameter_KSslL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
   		xmppadapter->VisitL(*klf);
   		
   		CWPParameter* klf1 = CWPParameter::NewLC();
        klf1->SetID(EWPNamedParameter);
        klf1->SetNameL(KSsl);
   		klf1->SetValueL(KSslvalue);
   		xmppadapter->VisitL(*klf1);
	    iLog->Log ( _L( "xmppadapter_visit_EWPNamedParameter_KSslL  test case passed" ) );
	    CleanupStack::PopAndDestroy(2,klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }

//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KPortNbrL
//----------------------------------------------------------------------------
//	
TInt CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KPortNbrL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPNamedParameter_KPortNbrL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPNamedParameter_KPortNbrL test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
   		xmppadapter->VisitL(*klf);
   		
   		CWPParameter* klf1 = CWPParameter::NewLC();
        klf1->SetID(EWPNamedParameter);
        klf1->SetNameL(KPortNbr);
   		klf1->SetValueL(KPortNbrvalue);
   		xmppadapter->VisitL(*klf1);
	    iLog->Log ( _L( "xmppadapter_visit_EWPNamedParameter_KPortNbrL  test case passed" ) );
	    CleanupStack::PopAndDestroy(2,klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }

//----------------------------------------------------------------------------
// CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KBrandIdL
//----------------------------------------------------------------------------
//  
TInt CtstXmppAdapter::xmppadapter_visit_EWPNamedParameter_KBrandIdL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPNamedParameter_KBrandIdL" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
        {
        iLog->Log ( _L( "xmppadapter_visit_EWPNamedParameter_KBrandIdL test case Failed" ) );
        return KErrGeneral; 
        }                                                     
   else
        {
        CWPParameter* klf = CWPParameter::NewLC();
        klf->SetID(EWPParameterAppID);
        klf->SetValueL(KMecoImAppID);
        xmppadapter->VisitL(*klf);
        
        CWPParameter* klf1 = CWPParameter::NewLC();
        klf1->SetID(EWPNamedParameter);
        klf1->SetNameL(KBrandId);
        klf1->SetValueL(KBrandIdvalue);
        xmppadapter->VisitL(*klf1);
        iLog->Log ( _L( "xmppadapter_visit_EWPNamedParameter_KBrandIdL  test case passed" ) );
        CleanupStack::PopAndDestroy(2,klf);
        REComSession::DestroyedImplementation( iEcomDtorID );       
        }
    return KErrNone;
    }

TInt CtstXmppAdapter::xmppadapter_visit_EWPParameterDomainL(CStifItemParser& /*aItem*/ )
    {
    iLog->Log ( _L("xmppadapter_visit_EWPParameterDomain" ) );
    iEcomDtorID = TUid::Null();
    CXMPPAdapter* xmppadapter = static_cast<CXMPPAdapter*>(REComSession::CreateImplementationL( TUid::Uid( 0x0A6A4E93 ),
                                                     iEcomDtorID ));
                                                     
    if(!xmppadapter)
   		{
   	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterDomain  test case Failed" ) );
   	    return KErrGeneral;	
	    }                                                     
   else
   		{
   		CWPParameter* klf = CWPParameter::NewLC();
   		klf->SetID(EWPParameterAppID);
   		klf->SetValueL(KMecoImAppID);
   		xmppadapter->VisitL(*klf);
   		
   		CWPParameter* klf1 = CWPParameter::NewLC();
   		klf1->SetID(EWPParameterDomain);
   	    klf1->SetNameL(KXMPPDomain);
   	    klf1->SetValueL(KDomain);
   		xmppadapter->VisitL(*klf1);
	    iLog->Log ( _L( "xmppadapter_visit_EWPParameterDomain  test case passed" ) );
	    CleanupStack::PopAndDestroy(2,klf);
		REComSession::DestroyedImplementation( iEcomDtorID );	    
		}
    return KErrNone;
    }	    
	    
//  End of File

