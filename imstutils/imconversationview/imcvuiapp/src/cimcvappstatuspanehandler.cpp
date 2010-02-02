/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Handles statuspane, context pane, navi pane
<<<<<<< C:\DOCUME~1\mbn\LOCALS~1\Temp\cimcvappstatuspanehandler-3_GAA.cpp
*
*/


// INCLUDE FILES
#include	"cimcvappstatuspanehandler.h"
#include 	"mimcvtabobserver.h"


// for logs
#include 	"imcvlogger.h"
//#include <aknindicatorcontainer.h> 
#include <eikenv.h>
#include <aknenv.h>
#include <aknnavi.h>
#include <aknnavide.h>
#include <aknEditStateIndicator.h>
#include <AknIndicatorContainer.h>
#include <featmgr.h>
#include <hlplch.h>
#include <e32property.h>

// system includes
#include   <layoutmetadata.cdl.h>
#include   <aknlayoutscalable_avkon.cdl.h>
#include <imconnectionproviderconsts.h>
#include "cimcvenginefactory.h"
#include "mimcvenginefactory.h"
const TUint16 KNewMessageIcon( 0xF818 ); 

// ================= MEMBER FUNCTIONS =======================
// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::ConstructL
// (other items were commented in a header).
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppStatusPaneHandler::CIMCVAppStatusPaneHandler(MIMCVEngineFactory& aEngineFactory):
    iEngineFactory( aEngineFactory )
    {
    IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::CIMCVAppStatusPaneHandler()" ));
    }
// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::ConstructL
// (other items were commented in a header).
// Symbian OS default constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::ConstructL()
    {
    IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::ConstructL() start" ));
    // Get pointer to status-pane
    CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->
		    StatusPane();
	
	iNaviPane = static_cast<CAknNavigationControlContainer*>(
		    statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
		    
     // Get pointer to title-pane
    iTitlePane = static_cast<CAknTitlePane*>( statusPane->ControlL(
			TUid::Uid( EEikStatusPaneUidTitle ) ) );

	iContextPane = static_cast<CAknContextPane*>( statusPane->ControlL(
		    TUid::Uid( EEikStatusPaneUidContext ) ) );
	
	iNewMsgIcon = HBufC::NewL( 1 );  
	
	TPtr iconPtr( iNewMsgIcon->Des() );
	iconPtr.Append( TChar( KNewMessageIcon ) );
	
	InitMessageIndicatorL();	
	
    //Get the service id  - this property is published by imcvlauncher
    TInt error = RProperty::Get( KMeCoPropertyUid, KMeCoConvViewServiceIdKey, iServiceId );
    User::LeaveIfError(error);
	
	iEngineFactory.RegisterOpenChatObserver(this);
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::ConstructL() end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::NewL
// (other items were commented in a header).
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppStatusPaneHandler* CIMCVAppStatusPaneHandler::NewL(MIMCVEngineFactory& aEngineFactory)
    {
    IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::NewL() start" ));
    CIMCVAppStatusPaneHandler* self = new ( ELeave ) CIMCVAppStatusPaneHandler( aEngineFactory );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::NewL() end" ));
    return self;
    }


// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::~CIMCVAppStatusPaneHandler
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CIMCVAppStatusPaneHandler::~CIMCVAppStatusPaneHandler()
    {
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::~CIMCVAppStatusPaneHandler() start" ));
	
	if (iLastTitle)
		{
		delete iLastTitle;
		iLastTitle = NULL;	
		}
	DestroyMessageIndicator();
	
	iObserverList.Reset();
    
    delete iNewMsgIcon;    
	delete iNewDecoratedTabGroup;	
	
	iEngineFactory.UnRegisterOpenChatObserver();
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::~CIMCVAppStatusPaneHandler() end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::SetPicture
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::SetPicture( const CFbsBitmap* aPicture ,
									  const CFbsBitmap* aMask )
	{
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::SetPicture() start" ));
    TBool landscape( Layout_Meta_Data::IsLandscapeOrientation() );
    
    TBool cpVisible( iContextPane ? iContextPane->IsVisible() : EFalse );
    TBool useTitle( !cpVisible || landscape );
	if( useTitle )
		{
		iTitlePane->SetSmallPicture(aPicture, aMask ,useTitle );
		}
	 else
		 {
		 iContextPane->SetPicture( aPicture, aMask );	
		 }
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::SetPicture() end" ));
	}

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::SetTitleL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::SetTitleL( const TDesC& aTitle )
	{
	
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::SetTitleL() start %S" ), &aTitle);
	iTitlePane->SetTextL( aTitle );
	if (iLastTitle)
		{
		delete iLastTitle;
		iLastTitle = NULL;	
		}
	iLastTitle = aTitle.AllocL();
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::SetTitleL() end" ));
	}
	
	

// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::ShowTabGroupL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMCVAppStatusPaneHandler::ShowTabGroupL( const TDesC& aId, 
										const TBool noCreate /* = EFalse */ )
    {	
	
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::ShowTabGroupL() start %S" ), &aId);
	iPrevTabId = 0;
    iCurrentTabId =0;
	
	iCurrentTabId = iEngineFactory.FindIndexL(aId);
	
    CreateTabsL(noCreate); 
        
   
   return iCurrentTabId;        
   }
    



// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::CreateTabsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::CreateTabsL(const TBool noCreate)
	{	
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::CreateTabs() start " ));
	iOpenChatArrayCount = iEngineFactory.Count(); 
	

	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::CreateTabs() OPEN CHAT ARRAY COUNT %d" ), iOpenChatArrayCount);	
	if ( (noCreate) && (iNewDecoratedTabGroup) )
		{
		
		CAknTabGroup* origtabGroup = 
	    	static_cast<CAknTabGroup*>( iNewDecoratedTabGroup->DecoratedControl() );   
	    	
		if ( origtabGroup->TabCount() == (iOpenChatArrayCount-1) )
			{	
		
			for (TInt index=0; index<iOpenChatArrayCount; index++)
				{
				if ( (iEngineFactory.IsMsgPendingL(index)) 
						&& ( index != iCurrentTabId ) )
					{
					origtabGroup->ReplaceTabTextL(index, iNewMsgIcon->Des());				
					}
				else
					{						
					origtabGroup->ReplaceTabTextL( index, iEngineFactory.GetDisplayNameL(index) );
					}						
				}
			
			return;
				
			}		

		} 
		
		
	CAknNavigationDecorator* tmpDecoration = iNaviPane->CreateTabGroupL();
	
	CleanupStack::PushL( tmpDecoration );
	
	CAknTabGroup* tabGroup = 
    	static_cast<CAknTabGroup*>( tmpDecoration->DecoratedControl() );   
    
    if( iOpenChatArrayCount > 2 )
            {
            tabGroup->SetTabFixedWidthL( EAknTabWidthWithThreeTabs );
            }
    else if ( iOpenChatArrayCount == 2 )
            {
            tabGroup->SetTabFixedWidthL( EAknTabWidthWithTwoTabs );
            }
    else
            {
            tabGroup->SetTabFixedWidthL( EAknTabWidthWithOneTab );
            }
            
    
	tabGroup->SetObserver( this );
	
	
	for (TInt index=0; index<iOpenChatArrayCount; index++)
		{
	
		if ( (iEngineFactory.IsMsgPendingL(index)) 
				&& ( index != iCurrentTabId ) )
			{
			tabGroup->AddTabL(index, iNewMsgIcon->Des());	
			}
		else
			{
            if (  1 == iOpenChatArrayCount )
                {
                // When a single conversation exists , no need to display name on tab
                tabGroup->AddTabL( index, KNullDesC );
                }
            else
                {
                // When more than one conversation exists , display names are displayed on the tabs
                tabGroup->AddTabL( index, iEngineFactory.GetDisplayNameL(index) );
                }
			}
        }
				
    CleanupStack::Pop( tmpDecoration );
    
    delete iNewDecoratedTabGroup; 
	iNewDecoratedTabGroup = tmpDecoration;

	ShowTabL();
	

	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::CreateTabs() end" ));

	}
	
	


// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::TabChangedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::TabChangedL(TInt aIndex)
    {
    TInt activechatitemindex; 
    CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>( 
            iNewDecoratedTabGroup->DecoratedControl()); 
    if ( tabGroup )
        {
        tabGroup->SetActiveTabByIndex( aIndex );
        iEngineFactory.SetActiveItemL(tabGroup->ActiveTabId());
        iPrevTabId = iCurrentTabId;
        iCurrentTabId = tabGroup->ActiveTabId();
        // Get the corrent position in open chat array using tab id
        if( iOpenChatArrayCount > iEngineFactory.Count() )
            {
            activechatitemindex =  (( iPrevTabId<iCurrentTabId )?iPrevTabId:iCurrentTabId);
            }
        else
            {
            activechatitemindex =  iCurrentTabId ;   
            }
        tabGroup->ReplaceTabTextL( iCurrentTabId, iEngineFactory.GetDisplayNameL(activechatitemindex) ); // replace of iCurrentTabId using activechatitemindex	
        for (TInt index=0; index<iObserverList.Count(); index++)
            {
            iObserverList[index]->SwitchTabL( iEngineFactory.GetActiveServiceId(), iEngineFactory.GetTargetIdL(iCurrentTabId), iCurrentTabId );	
            }

        }	
    }

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler: AddObserver
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::AddObserver(
									MIMCVTabObserver* aObserver) 
	{	    
	
	ASSERT( aObserver);	
	TInt status( iObserverList.Find( aObserver ) );
    if ( status == KErrNotFound )
	    {
		status = iObserverList.Append( aObserver ) ;
		if( status != KErrNone)
			{
			CActiveScheduler::Current()->Error( status );	
			}
	    }
	    
	}


// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler: RemoveObserver
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::RemoveObserver(
									MIMCVTabObserver* aObserver )
	{
	
	ASSERT( aObserver);
	const TInt status( iObserverList.Find( aObserver ) );
    if ( status != KErrNotFound )
        {
        iObserverList.Remove( status );
        iObserverList.Compress();
        }

	
	}


// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::ShowTabL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::ShowTabL()
    {   
    CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>( 
                        iNewDecoratedTabGroup->DecoratedControl());        
    
    tabGroup->SetActiveTabById( iCurrentTabId );
    iEngineFactory.SetActiveItemL(iCurrentTabId);
    
    /* Pushes new decorator object to top of the stack and draws it*/
    
    iNaviPane->PushL( *iNewDecoratedTabGroup );        
    
    }


// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::HideTab
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::HideTab()
    {   		
	
    /* Pops*/        
    iNaviPane->Pop( iNewDecoratedTabGroup );
    
    }




// ---------------------------------------------------------	
// CIMCVAppStatusPaneHandler::SetActiveTabByIndexL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::SetActiveTabByIndexL( const TInt aIndex )
	{

	CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>( 
                        iNewDecoratedTabGroup->DecoratedControl());        
    
    if (!tabGroup)
	    {
		return ;
	    }
	
	iCurrentTabId = aIndex;
	
	TRAP_IGNORE( tabGroup->ReplaceTabL( iCurrentTabId, 
	        iEngineFactory.GetDisplayNameL(iCurrentTabId) ) );
			
	tabGroup->SetActiveTabById( iCurrentTabId );   
	
	iEngineFactory.SetActiveItemL(iCurrentTabId);
	    	
	}

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::RefreshTitlePaneTextL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::RefreshTitlePaneTextL()
	{
	// seems that this is the only way that the title pane text color updates..
    if( iTitlePane->Text() )
        {
        HBufC* origText = iTitlePane->Text()->AllocLC();
	    HBufC* empty = HBufC::NewLC( 1 );
	    iTitlePane->SetText( empty );
	    iTitlePane->SetText( origText);
	    CleanupStack::Pop( 2, origText ); // empty, origText
        }
	}


// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::TabGroup
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknTabGroup* CIMCVAppStatusPaneHandler::TabGroup() const
	{
	// Returns pointer to topmost decorator's tabgroup
	return static_cast<CAknTabGroup*>( 
	                        iNewDecoratedTabGroup->DecoratedControl() );
	}



// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::PopNaviPane
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::PopNaviPane()
	{
	// Pops the topmost decorator out of decorator stack.
    iNaviPane->Pop();
    
	}
		

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::ClearNaviPaneL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::ClearNaviPaneL( TBool aAllowDuplicates )
	{
	// Pushes default decorator object (empty one)
	// to top of the stack and draws it. if aAllowDuplicates is ETrue
	// the new empty decorator is created on top of the stack, rather than
	// raising the existing one.
    iNaviPane->PushDefaultL( aAllowDuplicates );   
    }


// ---------------------------------------------------------	
// CIMCVAppStatusPaneHandler::HandleChangeL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::HandleChangeL(TInt /*aServiceId*/, TInt aIndex, TOpenChatsEventType aChangeType) 
	{
    switch( aChangeType )
		{
		case ENewChat:
			{
			if( iCurrentTabId >= aIndex )
			    {
			    iCurrentTabId = iCurrentTabId+1;
			    iEngineFactory.SetActiveItemL( iCurrentTabId );
			    }
			const TDesC& chatId ( iEngineFactory.GetActiveItemL() );
			TRAP_IGNORE(  
            	{
            	if ( (iCurrentTabId == -1) || (!chatId.Length()) )
					{	
					ShowTabGroupL(iEngineFactory.GetTargetIdL(aIndex));								
					}
				else
					{
					ShowTabGroupL(chatId) ;		
					}
            	}
         	);
			break;
			}
		case EChatClosed: 
			{
			if( aIndex == 0 ) 
				{
				return;	
				}
			iEngineFactory.SetActiveItemL( aIndex -1 );
			
			const TDesC& chatId (iEngineFactory.GetActiveItemL());
			
			TRAP_IGNORE(  
            	{
            	if ( (iCurrentTabId == -1) || (!chatId.Length()) )
					{	
					ShowTabGroupL(iEngineFactory.GetTargetIdL(aIndex));								
					}
				else
					{
					ShowTabGroupL(chatId) ;		
					}
            	}
         	);
         
			break;		
			}
    
		
		case EUnReadCountChanged:
			{
			    					
			const TDesC& chatId (iEngineFactory.GetActiveItemL());
			
			if (!chatId.Length())
				{
				break;
				}

			TRAP_IGNORE(
				{				
				ShowTabGroupL(chatId, ETrue); 					
				PushFadeTextL();			
				ShowFadeText( *iStatusPaneText );
				delete iStatusPaneText;
				iStatusPaneText = NULL;
				}
			);
        	
			break;
			}
	
		default:
			{
			//should never be here.
			break;
			}
		}
	    	
	
	}	    
	
// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::SetStatusMsgL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::SetStatusMsgL(TInt /*aServiceId*/, TInt aIndex,const TDesC & aMsg)
	{
	if(aIndex>=0)
		{
		_LIT(KColun, ": ");
		iStatusPaneText = HBufC::NewL(iEngineFactory.GetTargetIdL(aIndex).Length() +
							aMsg.Length() + KColun().Length());
		TPtr statusPanePtr(iStatusPaneText->Des());
		
		statusPanePtr.Copy(iEngineFactory.GetTargetIdL(aIndex));
		statusPanePtr.Append(KColun);
		statusPanePtr.Append(aMsg);
		}
	}

	
	
// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::RunFadeText()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::RunFadeText()
	{
	static_cast<CIMCVNewMsgFadeControl*>(iDecoratedFadeText->DecoratedControl())->Start();
	}

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::HideFadeText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::HideFadeText()
	{
	if ( iDecoratedFadeText ) 
		{
		iNaviPane->Pop( iDecoratedFadeText );
		}
	}

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::ShowFadeText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::ShowFadeText( TDesC& aTextToFade )
	{
	IMessageIndicator()->SetMessage( aTextToFade );
	RunFadeText();
	}

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::ShowNavipaneIndicators
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::ShowNavipaneIndicators()
	{
    MAknEditingStateIndicator* editStateIndicator = 
                                    CAknEnv::Static()->EditingStateIndicator();                                   
    
    if ( editStateIndicator )
        {
        CAknIndicatorContainer* indicatorContainer = editStateIndicator->IndicatorContainer();
              
        if( indicatorContainer )
            {
            indicatorContainer->SetIndicatorState( 
    		    TUid::Uid( EAknNaviPaneEditorIndicatorMessageInfo ), 
    		    EAknIndicatorStateOn );
            }        
        }
	}

// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::HideNavipaneIndicators()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::HideNavipaneIndicators()
	{
    MAknEditingStateIndicator* editStateIndicator = 
                                    CAknEnv::Static()->EditingStateIndicator();                                  
    
    if ( editStateIndicator )
        {
        CAknIndicatorContainer* indicatorContainer = editStateIndicator->IndicatorContainer(); 
        
        if( indicatorContainer )
            {
            indicatorContainer->SetIndicatorState( 
    			TUid::Uid( EAknNaviPaneEditorIndicatorMessageInfo ), 
    			EAknIndicatorStateOff ); 
            }               
        }
	}

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::IMessageIndicator
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CIMCVNewMsgFadeControl* CIMCVAppStatusPaneHandler::IMessageIndicator() const
	{
	if( iDecoratedFadeText )
		{
		return static_cast<CIMCVNewMsgFadeControl*>(iDecoratedFadeText->DecoratedControl());
		}
	else
		{
		return NULL;
		}

	}

// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::PushFadeTextL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::PushFadeTextL()
	{
	if ( iDecoratedFadeText ) 
		{
		iNaviPane->PushL( *iDecoratedFadeText );
		}
	}

// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::ResetFadeText()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::ResetFadeText()
	{
	IMessageIndicator()->Reset();
	}


// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::InitMessageIndicatorL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::InitMessageIndicatorL()
	{
	CAknNavigationDecorator* fade = CreateIMessageIndicatorL();
	delete iDecoratedFadeText;
	iDecoratedFadeText = fade;
	}

// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::DestroyMessageIndicator()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::DestroyMessageIndicator()
	{
	delete iDecoratedFadeText;
	iDecoratedFadeText = NULL;
	}
// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::CreateIMessageIndicatorL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknNavigationDecorator* CIMCVAppStatusPaneHandler::CreateIMessageIndicatorL()
	{
	
	CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->
		    StatusPane();		    
	
	CIMCVNewMsgFadeControl* fadetext = CIMCVNewMsgFadeControl::NewLC( 
		statusPane->PaneRectL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
	fadetext->SetContainerWindowL( *iNaviPane );
	
	CAknNavigationDecorator* decoratedFadetext = 
		CAknNavigationDecorator::NewL( iNaviPane, fadetext, 
		CAknNavigationDecorator::EHintText );

	CleanupStack::Pop( fadetext );
	CleanupStack::PushL( decoratedFadetext );
	decoratedFadetext->SetContainerWindowL( *iNaviPane );
	decoratedFadetext->SetControlContext( iNaviPane );
	decoratedFadetext->MakeScrollButtonVisible( EFalse );
	
	CleanupStack::Pop( decoratedFadetext );

	return decoratedFadetext;	
	}	

// ---------------------------------------------------------
// CIMCVAppStatusPaneHandler::ClearTitlePaneL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::ClearTitlePaneL( )
    {
    iTitlePane->SetTextL( _L(" ") );
    iTitlePane->SetSmallPicture( NULL, NULL, EFalse );
    }

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::SetPreviousTitleL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::SetPreviousTitleL()
	{
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::SetPreviousTitleL() start" ));	
	
	HBufC* setTitle = NULL; 
	if (iLastTitle)
		{
		setTitle = iLastTitle->AllocLC();
		}
	else
		{
		setTitle = KNullDesC().AllocLC();	
		}			
	SetTitleL(*setTitle);	
	CleanupStack::PopAndDestroy();		
	IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::SetPreviousTitleL() end" ));
	}


// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::SendBackGroundL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::SendBackGroundL()
	{
	ClearTitlePaneL();
	ClearNaviPaneL(ETrue);		    	
	}

// -----------------------------------------------------------------------------
// CIMCVAppStatusPaneHandler::BringToForegroundL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::BringToForegroundL()
	{
	SetPreviousTitleL();	
	ShowTabL();
	}		    

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::RefreshTabL()
// -----------------------------------------------------------------------------
//
void CIMCVAppStatusPaneHandler::RefreshTabL(const TDesC& aUserId, const TDesC& aDisplayName)
    {
    IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::RefreshTab() Start" ));
    IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::RefreshTab()aUserId: %S " ), &aUserId);
    IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::RefreshTab() aDisplayName : %S " ), &aDisplayName);
    // find if itsthe active conversation, only then the
    // the statuspane title and tab text should be updated.
    CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>( 
                        iNewDecoratedTabGroup->DecoratedControl());        
    if (tabGroup)
        {
        TInt index = iEngineFactory.FindIndexL(aUserId);
        IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::RefreshTab() Get the index  %d" ), index);
        if(KErrNotFound != index)
            {
            const TDesC& activeContversationName = iEngineFactory.GetTargetIdL(index);
            IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::RefreshTab(): %S " ), &activeContversationName);

            if(activeContversationName.Compare(aUserId) == 0 )
                {
                IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::RefreshTab(): Inside if" ));
                SetTitleL(aDisplayName);
                }
            // only the tab text needs to be updated.
            tabGroup->ReplaceTabTextL( index, aDisplayName); 
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppStatusPaneHandler::RefreshTab() End" ));
    }
//End of file
