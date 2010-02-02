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
* Description: Extension service for CVIMPSTUI
*
*/


// INCLUDE FILES
#include "cvimpstuiextensionservice.h"

#include <MPbk2AppUi.h>

// Edit view
#include <MPbk2ApplicationServices.h>
#include <MPbk2CommandHandler.h>
#include <Pbk2Commands.hrh>

#include "cvimpstuisearchfieldaray.h"
#include "mvimpstuisearchfield.h"
#include "cvimpstuisearchquerydialog.h"

#include "cvimpstuitabbedview.h"
#include "cvimpstuisearchview.h"
#include "cvimpstuiextensionviewinfo.h"
#include "cvimpstcmdhandler.h"

#include "mvimpstengine.h"
#include "vimpstutils.h"

#include "cvimpstuibranddata.h"
#include "tvimpstconsts.h"
#include "cvimpstuiblockview.h"
#include <vimpstui.mbg>
#include <vimpst_servicetab_default.mbg>
#include <vimpstuires.rsg>
#include <eikenv.h>
#include "cvimpstuimenuextension.h"

const TInt KDefaultIconId = EMbmVimpstuiExample_msn_tab;
const TInt KDefaultMaskId = EMbmVimpstuiExample_msn_tab_mask;

const TInt KBrandTabIconId = EMbmVimpst_servicetab_defaultQgn_prop_im_imsg ;
const TInt KBrandTabMaskId = EMbmVimpst_servicetab_defaultQgn_prop_im_imsg_mask;

_LIT(KDefaultFileLocation,"\\resource\\");
_LIT(KBrandFileLocation,"C:\\resource\\");
_LIT(KBrandFileNameDefault_C,"C:\\resource\\vimpst_servicetab_default.mif");
_LIT(KBrandFileNameDefault_Z,"Z:\\resource\\apps\\vimpst_servicetab_default.mif");
_LIT(KBrandFileNameHalf, "vimpst_servicetab_");
_LIT(KBrandFileNameExt, ".mif");
_LIT(KDefaultFileName, "vimpst_servicetab_default.mif");
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::CVIMPSTUIExtensionService
// ---------------------------------------------------------------------------
// 
CVIMPSTUIExtensionService::CVIMPSTUIExtensionService(
    TUint aServiceId,
    TInt aTabbedViewId,   
    TInt aSearchViewId, 
    TInt aBlockedViewId,  
    CVIMPSTUIMenuExtension& aMenuExtension,
    MVIMPSTEngine& aEngine ):
    iServiceId( aServiceId ),
    iTabbedViewId( aTabbedViewId ),        
    iSearchViewId( aSearchViewId ), 
    iBlockedViewId( aBlockedViewId ),
    iMenuExtension(aMenuExtension),
    iEngine( aEngine )
    {
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::ConstructL
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIExtensionService::ConstructL(const TDesC& aServiceName)
    {       
    TFileName path;    
    Dll::FileName(path); //get the location of the dll
    
    TDriveName drive( TParsePtrC( path ).Drive( ) ); // solve drive
    TBuf<KMaxResourcePathLength> serviceName(iEngine.ServiceName());
    HBufC* brandFileName = HBufC::NewLC( KMaxResourcePathLength );
    TPtr brandFileNamePtr( brandFileName->Des() );
    brandFileNamePtr.Append( KBrandFileNameHalf() );
    brandFileNamePtr.Append( serviceName );
    brandFileNamePtr.Append( KBrandFileNameExt() );

    RFs& session = CEikonEnv::Static()->FsSession();
    session.AddFileSystem( brandFileNamePtr );    
    TFindFile file_finder(session);
    CDir* file_list;
    path.Zero();
    TInt err = file_finder.FindWildByDir( brandFileNamePtr,KBrandFileLocation(),file_list);
    if( err == KErrNone )
        {
        brandFileNamePtr.Insert( 0, KBrandFileLocation());
        }
    else 
        {
        err = file_finder.FindWildByDir( KDefaultFileName(), KDefaultFileLocation(), file_list );
        brandFileNamePtr.Zero();
        if( err == KErrNone )
            {
            brandFileNamePtr.Insert( 0, KBrandFileNameDefault_C());
            }
        else
            {
            brandFileNamePtr.Insert( 0, KBrandFileNameDefault_Z());
            }
        }
    path.Append( brandFileNamePtr );
    iServiceName = aServiceName.AllocL();
    // View info's must be created in initial contruct phase
    iTabbedViewInfo = CVIMPSTUIExtensionViewInfo::NewL( 
        iTabbedViewId, ETrue, path, KBrandTabIconId, KBrandTabMaskId, *iServiceName );
    HBufC* resourceFilePath = HBufC::NewLC( KMaxResourcePathLength );
    TPtr ptr( resourceFilePath->Des() );
    ptr.Append( KDefaultIconFile() ); // without drive
    ptr.Insert( 0, drive ); // insert drive

    // NearestLanguageFile takes only TFileName
    path.Zero();
    path.Append( *resourceFilePath );


    
      
	iSearchViewInfo = CVIMPSTUIExtensionViewInfo::NewL(
        iSearchViewId, EFalse, path, KDefaultIconId, 
        KDefaultMaskId, *iServiceName  );    
        
    iBlockViewInfo = CVIMPSTUIExtensionViewInfo::NewL(
        iBlockedViewId, EFalse, path, KDefaultIconId, 
        KDefaultMaskId, *iServiceName  );    

	iCommandHandler = CVIMPSTCmdHandler::NewL(iEngine); 

	iUiFieldsArray = CVIMPSTUiSearchFieldArray::NewL();
    CleanupStack::PopAndDestroy(2,brandFileName); //resourceFilePath,brandTabIconFilePath
	delete file_list;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::NewL
// ---------------------------------------------------------------------------
// 
CVIMPSTUIExtensionService* CVIMPSTUIExtensionService::NewL(
    TUint aServiceId,
    TInt aTabbedViewId,
    TInt aSearchViewId,
    TInt aBlockedViewId, 
    const TDesC& aServiceName,    
    CVIMPSTUIMenuExtension& aMenuExtension,
    MVIMPSTEngine& aEngine )
    {
    CVIMPSTUIExtensionService* self = NewLC( 
        aServiceId, aTabbedViewId, aSearchViewId, aBlockedViewId, aServiceName, aMenuExtension, aEngine );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::NewLC
// ---------------------------------------------------------------------------
// 
CVIMPSTUIExtensionService* CVIMPSTUIExtensionService::NewLC(
    TUint aServiceId,
    TInt aTabbedViewId,    
    TInt aSearchViewId, 
    TInt aBlockedViewId,  
    const TDesC& aServiceName,    
    CVIMPSTUIMenuExtension& aMenuExtension,
    MVIMPSTEngine& aEngine )
    {
    CVIMPSTUIExtensionService* self =
        new (ELeave) CVIMPSTUIExtensionService( 
            aServiceId, aTabbedViewId, aSearchViewId, aBlockedViewId, aMenuExtension, aEngine );
    CleanupStack::PushL(self);
    self->ConstructL(aServiceName);
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::~CVIMPSTUIExtensionService
// ---------------------------------------------------------------------------
// 
CVIMPSTUIExtensionService::~CVIMPSTUIExtensionService()
    {
    delete iUiFieldsArray;

    delete iCommandHandler;
    iCommandHandler = NULL;
    
    delete iServiceName;
    iServiceName = NULL;
    
    delete iSearchString;
	iSearchString = NULL;

    //delete iBrandHandler;
    
    delete iTabbedViewInfo;
    iTabbedViewInfo = NULL;      
    delete iSearchViewInfo;
    iSearchViewInfo = NULL;       
    
    delete iBlockViewInfo;
    iBlockViewInfo = NULL;  
    }

  
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::TabbedViewL
// ---------------------------------------------------------------------------
// 
CVIMPSTUiTabbedView* 
    CVIMPSTUIExtensionService::TabbedViewL( 
        MxSPViewMapper& aMapper, 
        MxSPContactManager& aContactManager, 
        CPbk2UIExtensionView& aView,
        TInt aTabbedViewId,
        TUid aId,
        MVIMPSTEngine& aEngine ) 
    {
    if ( !iTabbedView )
        {
        iTabbedView = CVIMPSTUiTabbedView::NewL( 
            aMapper,
            aContactManager,
            aTabbedViewId,
            iServiceId,
            aView,
            aId,
            *this,
            aEngine,
            *iCommandHandler,
            iMenuExtension);        
        }
    return iTabbedView;
    }    
    
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::BlockViewL
// ---------------------------------------------------------------------------
// 
CVIMPSTUiBlockView* 
    CVIMPSTUIExtensionService::BlockViewL( 
        MxSPViewMapper& aMapper, 
        MxSPContactManager& aContactManager, 
        CPbk2UIExtensionView& aView,
        TInt aBlockedViewId,
        MVIMPSTEngine& aEngine ) 
    {
    if ( !iBlockedView )
        {
        iBlockedView = CVIMPSTUiBlockView::NewL( 
            aMapper,
            aContactManager,
            aBlockedViewId,
            aView,
            *this ,
            aEngine,
            *iCommandHandler ); 
        }
    return iBlockedView;
    }    
    
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SearchViewL
// ---------------------------------------------------------------------------
// 
CVIMPSTUiSearchView* 
    CVIMPSTUIExtensionService::SearchViewL( 
        MxSPViewMapper& aMapper, 
        MxSPContactManager& aContactManager, 
        CPbk2UIExtensionView& aView,
        TInt aSearchViewId,
        TUid aId,
        MVIMPSTEngine& aEngine )
    {
    if ( !iSearchView )
        {
        iSearchView = CVIMPSTUiSearchView::NewL( 
            aMapper,
            aContactManager,
            aSearchViewId,
            iServiceId,
            aView,        
            aId,
            *this,
            aEngine,
            *iCommandHandler ); 
        }
    return iSearchView;
    }      
  
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::TabbedViewId
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTUIExtensionService::TabbedViewId()
    {
    return iTabbedViewId;
    }
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SearchViewId
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTUIExtensionService::SearchViewId()
    {
    return iSearchViewId;
    }    
    
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::BlockViewId
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTUIExtensionService::BlockViewId()
    {
    return iBlockedViewId;
    }      

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::ServiceId
// ---------------------------------------------------------------------------
// 
TUint32 CVIMPSTUIExtensionService::ServiceId()
    {
    return iServiceId;
    }
    
    
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::ServiceName
// ---------------------------------------------------------------------------
// 
const TDesC& CVIMPSTUIExtensionService::ServiceName()
    {
    return *iServiceName;
    }             

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SearchString
// ---------------------------------------------------------------------------
// 
const TDesC& CVIMPSTUIExtensionService::SearchString()
    {
    return *iSearchString;
    } 
    

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SetSearchStringL
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIExtensionService::SetSearchStringL( const TDesC& aSearchString )
    {
    if( iSearchString )
	    {
	    delete iSearchString;
	    iSearchString = NULL;
	    }
    iSearchString = aSearchString.AllocL();
    }                       

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::TabbedViewInfoL
// ---------------------------------------------------------------------------
//    
CVIMPSTUIExtensionViewInfo* 
    CVIMPSTUIExtensionService::TabbedViewInfoL()
    {
    return iTabbedViewInfo;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SearchViewInfo
// ---------------------------------------------------------------------------
//    
CVIMPSTUIExtensionViewInfo* 
    CVIMPSTUIExtensionService::SearchViewInfo()
    {
    return iSearchViewInfo;
    } 
    
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::BlockedViewInfo
// ---------------------------------------------------------------------------
//    
CVIMPSTUIExtensionViewInfo* 
    CVIMPSTUIExtensionService::BlockedViewInfo()
    {
    return iBlockViewInfo;
    }     
        
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::HasViewId()
// ---------------------------------------------------------------------------
// 
TBool CVIMPSTUIExtensionService::HasViewId( TInt aViewId )
    {
    return ( iTabbedViewId == aViewId || iSearchViewId == aViewId ||iBlockedViewId == aViewId  );
    }     


// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::ServiceEngine()
// ---------------------------------------------------------------------------
// 

MVIMPSTEngine& CVIMPSTUIExtensionService::ServiceEngine ()
	{
	return iEngine;
	}
   
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SetSearchFieldsDataL
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIExtensionService::SetSearchFieldsDataL( RArray<TInt>& aEnumKeysArray, RPointerArray<HBufC>& aLebelKeysArray )
    {
    // reset all old fields and create new fields
    if( iUiFieldsArray )
	    {
	    delete iUiFieldsArray;
	   	iUiFieldsArray = NULL;	
	    }
	iUiFieldsArray = CVIMPSTUiSearchFieldArray::NewL();
	 	    
    TInt enumCount = aEnumKeysArray.Count();
    TInt labelCount = aLebelKeysArray.Count();
    
    TBool isBasicField = EFalse ;
    TInt basicFieldIndex = 0;   
      
    if( !enumCount && !labelCount )
	    {
	    return;	
	    }
    for( TInt i =0; i< enumCount; i++ ) 
	    {
	    TVIMPSTEnums::TVIMPSTSearchKey enumKey = static_cast<TVIMPSTEnums::TVIMPSTSearchKey>( aEnumKeysArray[i] );
	    // get the data associaed with key ,
	    // also check if basic field
	    HBufC* akeyValue = GetSearchKeyLabelL( enumKey, isBasicField );
	    if( akeyValue )
			{
			if( isBasicField )
				{
				if( enumKey == TVIMPSTEnums::EVIMPSTFirstName )
					{
					basicFieldIndex = 0 ; // always on top 1st element	
					}
			
				iUiFieldsArray->InsertFieldsL(basicFieldIndex , *akeyValue, EVIMPSTEdwinText,EIMBasicSearch ,enumKey);
			
				basicFieldIndex++ ; 
				
				}
			else
				{
				iUiFieldsArray->CreateFieldsL( *akeyValue, EVIMPSTEdwinText ,EIMAdvancedSearch, enumKey);	
				}
			delete akeyValue;
			akeyValue = NULL;
			}
	    	
	    }
    
     for( TInt i =0; i< labelCount; i++ ) 
	    {
	    // not owns akeyValue 
	    HBufC* akeyValue = aLebelKeysArray[i];
	    if( akeyValue )
			{
			iUiFieldsArray->CreateFieldsL( *akeyValue, EVIMPSTEdwinText );
			}
	    	
	    }
	// this will sort only basic fields
	SortBasicFields();
    }                       
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SetSearchFieldsDataL
// ---------------------------------------------------------------------------
// 
CVIMPSTUiSearchFieldArray& CVIMPSTUIExtensionService::GetSearchFieldArray()  
    {
   	return *iUiFieldsArray;
    } 
        
// ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::GetSearchKeyLabelL
// ---------------------------------------------------------------------------
// 
HBufC* CVIMPSTUIExtensionService::GetSearchKeyLabelL(TVIMPSTEnums::TVIMPSTSearchKey aSearchKey ,TBool& aBasicField )  
    {

    HBufC* keyText = NULL;
    aBasicField = EFalse;
    
    switch( aSearchKey )
	    {
	    case TVIMPSTEnums::EVIMPSTMiddleName :
			{
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_MIDDLENAME ); 		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTLastName :
			{
			keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_LASTNAME ); 
			aBasicField = ETrue;	 	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTFullName :
			{
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_FULLNAME );    		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTCountry :
			{
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_COUNTRY );     		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTCity :
			{
			keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_CITY ); 	        		
			break;		
			}
	
		case TVIMPSTEnums::EVIMPSTNickName :
			{
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_NICKNAME ); 		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTState :
			{
			keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_STATE ); 	 	        		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTOrganisationUnit :
			{
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_ORGUNIT );    		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTBirthday :
			{
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_BIRTHDAY );     		
			break;		
			}
			
		case TVIMPSTEnums::EVIMPSTEmailAddress :
			{
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_EMAIL );     		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTUser :
			{
			aBasicField = ETrue;	
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_USER ); 		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTOrganisation :
			{
		    keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_ORG );    		
			break;		
			}
		case TVIMPSTEnums::EVIMPSTFirstName:
		default:
			{
			aBasicField = ETrue;
		   	keyText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_FIRSTNAME );
	     	break; 
			}	
	    }
	
	return 	keyText;
	
    } 
 
 // ---------------------------------------------------------------------------
// CVIMPSTUIExtensionService::SortBasicFields
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIExtensionService::SortBasicFields()  
	{
	TInt count = iUiFieldsArray->Count();
	TInt index = 0;
	if( count >2 ) // atleast there has to be 3 item for sort , first name is already on top
		{
		MVIMPSTUiSearchField& field = iUiFieldsArray->At(1);
		if( field.GetSearchKey() != TVIMPSTEnums::EVIMPSTLastName )
			{
			for(index =2; index < count; index++ )
				{
				field = iUiFieldsArray->At(index);
				if( field.GetSearchKey() == TVIMPSTEnums::EVIMPSTLastName )	
					{
					iUiFieldsArray->RemoveField(index);
					iUiFieldsArray->InsertField(&field, 1); // insert at 1st number;	
					return;
					}
				}
				
			}
		}
	
	}
	
// End of file
