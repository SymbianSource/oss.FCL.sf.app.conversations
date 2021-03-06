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
* Description: Search manager extension for CVIMPSTEngine
*
*/

// INCLUDE FILES
#include "cvimpstenginesearchmgrextention.h"

#include <ximpcontext.h>
#include <ximpcontextobserver.h>
#include <presenceobjectfactory.h>
#include <ximpidentity.h>
#include <ximpobjectfactory.h>
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>


#include "cvimpstenginerequestmapper.h"
#include "cvimpstenginerequest.h"
#include "vimpstdebugprint.h"

#include "vimpstallerrors.h"
#include "tvimpstconsts.h"

#include <search.h>
#include <searchfeature.h>
#include <searchobjectfactory.h>
#include <searchelement.h>
#include <searchkeysevent.h>
#include <searchkeyinfo.h>
#include <searchinfo.h>
#include <searchevent.h>

#include "mvimpstenginesearchextentioneventobserver.h"
#include "vimpstdebugtrace.h"



// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::NewL
// Two-phased constructor.
// ---------------------------------------------------------
CVIMPSTEngineSearchMgrExtention* CVIMPSTEngineSearchMgrExtention::NewL(	
							MXIMPContext& aPresenceCtx,						
							CVIMPSTEngineRequestMapper& aRequestMapper)
    {
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::NewL start"));
    CVIMPSTEngineSearchMgrExtention* self = 
    			CVIMPSTEngineSearchMgrExtention::NewLC(
    											aPresenceCtx, 
    											aRequestMapper
    										    );
    CleanupStack::Pop( self );
   	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::NewL end"));
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
CVIMPSTEngineSearchMgrExtention* CVIMPSTEngineSearchMgrExtention::NewLC(
							MXIMPContext& aPresenceCtx,
							CVIMPSTEngineRequestMapper& aRequestMapper)
	{
	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::NewLC start"));
	CVIMPSTEngineSearchMgrExtention* self = new (ELeave) 
					CVIMPSTEngineSearchMgrExtention(aPresenceCtx, aRequestMapper);
	CleanupStack::PushL( self );
	self->ConstructL();
	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::NewLC end"));
	return self;
	}
    
// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::ConstructL
// ---------------------------------------------------------
void CVIMPSTEngineSearchMgrExtention::ConstructL()
	{
	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::ConstructL start"));

	// ximp search plugin
    iSearchFeature = MSearchFeature::NewL(&iPresenceCtx); 
    
	iSearch = &iSearchFeature->Search();
	
	iAlreadySubscibed = EFalse;
	
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::ConstructL end") );    	
		
    }
	
// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::CVIMPSTEngineSearchMgrExtention
// ---------------------------------------------------------	
CVIMPSTEngineSearchMgrExtention::CVIMPSTEngineSearchMgrExtention( MXIMPContext& aPresenceCtx,											
											CVIMPSTEngineRequestMapper& aRequestMapper)
	:iPresenceCtx(aPresenceCtx),
	iRequestMapper(aRequestMapper),
	iSearchSupported(TVIMPSTEnums::ESupportUnKnown)
	{
		
	}

// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::~CVIMPSTEngineSearchMgrExtention
// ---------------------------------------------------------
CVIMPSTEngineSearchMgrExtention::~CVIMPSTEngineSearchMgrExtention()
    {
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::~CVIMPSTEngineSearchMgrExtention start"));    
    
	iSearchObservers.Reset();
	delete iSearchFeature;	
	iSearchFeature = NULL;
    
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::~CVIMPSTEngineSearchMgrExtention end"));
    }


// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::SearchContactsL
// ---------------------------------------------------------
TInt CVIMPSTEngineSearchMgrExtention::SearchContactsL( RArray<TVIMPSTSearchKeyData>& aKeyDataArray )
	{
	TRACE( T_LIT( "CVIMPSTEngineSearchMgrExtention::SearchContactsL Start" ) );
	
	TInt err = KErrNotSupported;
	if (iSearchSupported == TVIMPSTEnums::ESupported)
		{
			
		TInt keydataCount = aKeyDataArray.Count();
		
		MXIMPObjectFactory& objFactory = iPresenceCtx.ObjectFactory();
		MSearchObjectFactory& searchObjFact= iSearchFeature->SearchObjectFactory();
		
		MXIMPIdentity* searchId = objFactory.NewIdentityLC();
		searchId->SetIdentityL(_L("SEARCH"));
		
		RPointerArray< MSearchElement > searchList;
		CleanupClosePushL( searchList );
		
		for( TInt i = 0; i<keydataCount; i++  )
			{
			MXIMPIdentity* searchKeyId = objFactory.NewIdentityLC();
			TVIMPSTSearchKeyData data = (aKeyDataArray)[i];
			searchKeyId->SetIdentityL( data.iSearchKeyData );	
			
			MSearchElement* srchElement = searchObjFact.NewSearchElementLC();
			if( data.iSearchKey != TVIMPSTEnums::EVIMPSTSearchKeyUknown )
				{
				srchElement->SetRequestL( *searchKeyId, ConverttoTSearchKey( data.iSearchKey ) );	
				}
	    	else
		    	{
		    	MXIMPIdentity* searchLabel = objFactory.NewIdentityLC();	
		    	searchLabel->SetIdentityL( data.iSearchLabel );
		    	srchElement->SetRequestL( *searchKeyId, *searchLabel );
		    	CleanupStack::PopAndDestroy ();//searchLabel
		    	}
			searchList.Append(srchElement);
			
			CleanupStack::Pop();//srchElement
			CleanupStack::PopAndDestroy();//searchKeyId
				
			}
		
		
		TXIMPRequestId reqId = iSearch->SearchL(*searchId,searchList,KMaxSearchLimit );
		
		iRequestMapper.CreateRequestL(reqId, ETrue,EVIMPSTXimpOperationSearchContact);// waite here
		
		CleanupStack::Pop()	; // searchList
		searchList.ResetAndDestroy();//	searchList
		CleanupStack::PopAndDestroy ();//searchId 
		
		TRACE( T_LIT( "CVIMPSTEngineSearchMgrExtention::SearchContactsL End " ) );
		err = iReqResult;
		}

	return err;
	}
	

// -----------------------------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::SubscribeForSearchKeysL()
// -----------------------------------------------------------------------------
//
TInt CVIMPSTEngineSearchMgrExtention::SubscribeForSearchKeysL()
	{
	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::SubscribeForSearchKeysL Start"));
    TRACE( T_LIT( "--> SubscribeForSearchKeysL:iSearchSupported: %d" ), iSearchSupported );
    TRACE( T_LIT( "--> SubscribeForSearchKeysL: iAlreadySubscibed: %d" ), iAlreadySubscibed );    
		
	if ( (iSearchSupported == TVIMPSTEnums::ENotSupported) || iAlreadySubscibed )
		{
		TRACE( T_LIT( "--> SubscribeForSearchKeysL: inside if " ));    
		return KErrNone;	
		}

	if( iSearch )
		{
	    TRACE( T_LIT( "--> SubscribeForSearchKeysL: inside if(iSearch) " ));    

		iAlreadySubscibed = ETrue;
		
		TXIMPRequestId reqId = iSearch->GetSearchKeysL();			
		TRACE( T_LIT( "--> SubscribeForSearchKeysL: iSearch->GetSearchKeysL() " ));    

		iRequestMapper.CreateRequestL(reqId, ETrue ,EVIMPSTXimpOperationSubscribeSearchKeys);// waite here
		}
	
	if( ( iReqResult != KPREQUESTERRSUCCESSFUL ) && ( iReqResult != KErrNone ) )
		{
		iSearchSupported = TVIMPSTEnums::ENotSupported;
		}
	else
	    {
	    iSearchSupported = TVIMPSTEnums::ESupported;
	    }

	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::SubscribeForSearchKeysL end "));
	return iReqResult;;
	}	

// -----------------------------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::IsSupported()
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::FeatureSupport CVIMPSTEngineSearchMgrExtention::IsSupported() const 
	{
	return iSearchSupported;
	}

// ------------------------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::ConverttoTSearchKey
// 
// ------------------------------------------------------------------------
TSearchKey CVIMPSTEngineSearchMgrExtention::ConverttoTSearchKey(TVIMPSTEnums::TVIMPSTSearchKey aKey)
    {
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::ConverttoTSearchKey start"));
    
    TSearchKey ret = EUserFirstName;
     switch( aKey )
		{
		case TVIMPSTEnums::EVIMPSTFirstName :
			{
			ret = EUserFirstName;	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTMiddleName :
			{
			ret = EUserMiddleName ;	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTLastName :
			{
			ret = EUserLastName;	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTFullName :
			{
			ret = EUserFullName;	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTCountry :
			{
			ret = EUserCountry;	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTCity :
			{
			ret = EUserCity;	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTEmailAddress :
			{
			ret = EUserEmailAddress;	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTUser :
			{
			ret = EUser;	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTOrganisation :
			{
			ret = EOrganisation;	        		
			break;		
			}
			
		case  TVIMPSTEnums::EVIMPSTNickName  :
			{
		    ret =  EUserNickName; 		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTState  :
			{
			ret =  EState; 	 	        		
			break;		
			}
		case  TVIMPSTEnums::EVIMPSTOrganisationUnit  :
			{
		    ret = EOrganisationUnit;    		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTBirthday :
			{
		    ret = EBirthday;     		
			break;		
			}
			
		default:
			{
			ret = EUserFirstName ;	
			}

		} 
	
	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::ConverttoTSearchKey end"));
	return ret; 
    }       	


// -----------------------------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::Type()
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::ExtentionType CVIMPSTEngineSearchMgrExtention::Type() const
	{
	return TVIMPSTEnums::ESearch;
	}
	

// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::RegisterObserver
// 
// ---------------------------------------------------------
void CVIMPSTEngineSearchMgrExtention::RegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* aObserver) 
	{
	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::RegisterObserver start"));
	
	TInt index = iSearchObservers.Find(aObserver);
	if( index == KErrNotFound )
		{
		iSearchObservers.Append( aObserver );	
		}

	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::RegisterObserver end"));
    }
	
// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::UnRegisterObserver
// 
// ---------------------------------------------------------
void  CVIMPSTEngineSearchMgrExtention::UnRegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* aObserver) 
	{
	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::UnRegisterObserver start"));
	
	TInt index = iSearchObservers.Find(aObserver);
	
	if( index >=0 )
		{
		iSearchObservers.Remove( index );
		iSearchObservers.Compress();
		}
		
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::UnRegisterObserver end"));
    }
    


// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::HandleSessionContextEventL
// ---------------------------------------------------------
void CVIMPSTEngineSearchMgrExtention::HandleSessionContextEventL(const MXIMPContext& /*aContext*/,
                                                             const MXIMPBase& aEvent,
                                                             TXimpOperation aXimpOperation/* = EVIMPSTXimpOperationNoOperation*/ )
    {
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::HandleSessionContextEventL start"));
    
    switch( aEvent.GetInterfaceId() )
        {
        
        case MXIMPRequestCompleteEvent::KInterfaceId:
        	{
            TRACE( T_LIT("InsideCallbackswitch::MXIMPRequestCompleteEvent"));
            if ( (EVIMPSTXimpOperationSearchContact == aXimpOperation) 
             		|| (EVIMPSTXimpOperationSubscribeSearchKeys == aXimpOperation) )
	            {            
	            const MXIMPRequestCompleteEvent* event =
	                TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( 
	                    aEvent, MXIMPBase::EPanicIfUnknown );
	            
	            iReqResult = event->CompletionResult().ResultCode();
	            const TXIMPRequestId& reqId = event->RequestId();
	            // Note:FindRequestId does not pass the ownership. hanece req
	            // should not be deleted.
	            CVIMPSTEngineRequest *req = iRequestMapper.FindRequestId( reqId );                
	            
	            if ( req )
	                {
		            req->StopWait() ;   
	                iRequestMapper.RemoveRequestId(reqId);
	                }
	            }
            break;
            }
        
        case MSearchEvent::KInterfaceId:
			{
			const MSearchEvent* srchEvent = 
			TXIMPGetInterface< const MSearchEvent >::From
									(aEvent, MXIMPBase::EPanicIfUnknown);	
			TInt count=	srchEvent->SearchResultCount();
			const MXIMPIdentity& identity=	srchEvent->SearchId();
			TPtrC id=srchEvent->SearchId().Identity();
			RArray<TVIMPSTSearchData> resultsArray;
			for(TInt i=0;i<count;++i)
				{
				const MSearchInfo& info=srchEvent->SearchResult(i);
				
				TVIMPSTSearchData data = 
								{
								info.GetFieldInfo(EFirstName),
								info.GetFieldInfo(ELastName),
								info.GetFieldInfo(EEmailAddress)	
								};
				TRAP_IGNORE( resultsArray.AppendL( data ) );
				}
	
			//should be done in reuqest complete event for search with erro handling
			//also first request complete will be received and then search result event
			
			for(TInt i=0; i< iSearchObservers.Count(); i++  )
				    {
				    TRAP_IGNORE( iSearchObservers[i]->HandleSearchResultL(resultsArray) );
				   	TRAP_IGNORE( iSearchObservers[i]->HandleSearchFinishedL( KErrNone ) );
				    }
			
			resultsArray.Close();	// close the array
			break;
			}
		case MSearchKeysEvent::KInterfaceId:
            {
            const MSearchKeysEvent* searchEvent =
            TXIMPGetInterface< const MSearchKeysEvent >::From ( aEvent,
                MXIMPBase::EPanicIfUnknown );
            TRAP_IGNORE( ProcessSearchKeysEventL( searchEvent ) );
            break;
            }
        default:
            {
            break;
            }
        }
   	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::HandleSessionContextEventL end"));	
    }    


// ---------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::ProcessSearchKeysEventL
// 
// ---------------------------------------------------------
void CVIMPSTEngineSearchMgrExtention::ProcessSearchKeysEventL(const MSearchKeysEvent* aSearchKeysEvent)
    {
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::ProcessSearchKeysEventL start"));
    
    TInt searchKeysCount = aSearchKeysEvent->InfoCount();
    
    RArray<TInt> enumKeyArray;
    RPointerArray<HBufC> labelKeyArray;
    
    CleanupClosePushL( enumKeyArray );
    CleanupClosePushL( labelKeyArray );
    
    HBufC* keyLabelText = NULL;
    for( TInt i = 0; i < searchKeysCount; i++ )
        {
        const MSearchKeyInfo& searchKeyInfo = aSearchKeysEvent->SearchKeyInfo ( i );
        switch( searchKeyInfo.Type() )
	        {
	        	case EKey:
	        		{
	        		//change to proper type once done at ximp side
	        		TSearchKey keyValue = searchKeyInfo.Key();
	        		TVIMPSTEnums::TVIMPSTSearchKey key = ConverttoTVIMPSTSearchKey(keyValue);
	        		enumKeyArray.Append(key);
	        		break;		
	        		}
				case ELabel:
	        		{
	        		keyLabelText = searchKeyInfo.Label().AllocL();
	        		
	        		labelKeyArray.Append(keyLabelText);
	        		break;		
	        		}	        		
	        	
	        }
        }
    
    for(TInt i=0; i< iSearchObservers.Count(); i++  )
	    {
	    iSearchObservers[i]->HandleSearchKeysEventL(enumKeyArray, labelKeyArray );	
	    }
	
	CleanupStack::PopAndDestroy(2); //enumKeyArray, labelKeyArray
    
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::ProcessSearchKeysEventL end"));
    }
        
// ------------------------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::ConverttoTVIMPSTSearchKey
// 
// ------------------------------------------------------------------------
TVIMPSTEnums::TVIMPSTSearchKey CVIMPSTEngineSearchMgrExtention::ConverttoTVIMPSTSearchKey( TSearchKey aKey)
    {
    TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::ConverttoTVIMPSTSearchKey start"));
    
     TVIMPSTEnums::TVIMPSTSearchKey retKey = TVIMPSTEnums::EVIMPSTFirstName ;
     switch( aKey )
		{
		case EUserFirstName:
			{
			retKey = TVIMPSTEnums::EVIMPSTFirstName;	        		
			break;		
			}
		case EUserMiddleName:
			{
			retKey = TVIMPSTEnums::EVIMPSTMiddleName;	        		
			break;		
			}
		case EUserLastName:
			{
			retKey = TVIMPSTEnums::EVIMPSTLastName;	        		
			break;		
			}
		case EUserFullName:
			{
			retKey = TVIMPSTEnums::EVIMPSTFullName;	        		
			break;		
			}
		case EUserCountry:
			{
			retKey = TVIMPSTEnums::EVIMPSTCountry;	        		
			break;		
			}
		case EUserCity:
			{
			retKey = TVIMPSTEnums::EVIMPSTCity;	        		
			break;		
			}
		case EUserEmailAddress:
			{
			retKey = TVIMPSTEnums::EVIMPSTEmailAddress;	        		
			break;		
			}
		case EUser:
			{
			retKey = TVIMPSTEnums::EVIMPSTUser;	        		
			break;		
			}
		case EOrganisation:
			{
			retKey = TVIMPSTEnums::EVIMPSTOrganisation;	        		
			break;		
			}
			
		case  EUserNickName :
			{
		    retKey = TVIMPSTEnums::EVIMPSTNickName; 		
			break;		
			}
		case EState :
			{
			retKey = TVIMPSTEnums::EVIMPSTState ; 	 	        		
			break;		
			}
		case  EOrganisationUnit :
			{
		    retKey = TVIMPSTEnums::EVIMPSTOrganisationUnit;    		
			break;		
			}
		case  EBirthday:
			{
		    retKey = TVIMPSTEnums::EVIMPSTBirthday;     		
			break;		
			}
			
		default:
			{
			retKey = TVIMPSTEnums::EVIMPSTFirstName;	
			}

		}
	
	TRACE( T_LIT("CVIMPSTEngineSearchMgrExtention::ConverttoTVIMPSTSearchKey end"));
	return retKey;  
    }       	
//----------------------------------------------------------------------------
// CVIMPSTEngineSearchMgrExtention::SetSupported()
// -----------------------------------------------------------------------------
//
void  CVIMPSTEngineSearchMgrExtention::SetSupported(TVIMPSTEnums::FeatureSupport aSupported)
    { 
    if ( aSupported == TVIMPSTEnums::ESupportUnKnown)
        {
        // at the logout time this flag  has to be set to EFalse
        iAlreadySubscibed = EFalse;
        }
    iSearchSupported = aSupported;
    }

// end of file
