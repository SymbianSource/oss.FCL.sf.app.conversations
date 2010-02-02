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
* Description:  Handles statuspane, context pane
*
*/

#ifndef CIMCVAPPSTATUSPANEHANDLER_H
#define CIMCVAPPSTATUSPANEHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <akntitle.h> // CAknTitlePane
#include <akncontext.h> // CAknContextPane
#include <AknIndicatorContainer.h> //CFbsBitmap
#include <akntabgrp.h>
#include <aknnavide.h> 
#include <AknTabObserver.h>
#include <aknnavi.h> 

#include "mimcvengineopenchats.h"
#include "cimcvnewmsgfadecontrol.h"


// CLASS DECLARATION
class MIMCVTabObserver;
class MIMCVEngineFactory;
/**
 *  This class handles status, title and contextpane operations
 *
 *  @lib imcvuiapp.exe
 *  @since 5.0
 */
class CIMCVAppStatusPaneHandler : public CBase,
								public MAknTabObserver,
								public MIMCVEngineOpenChatsObserver
								
    {
    public:  // Constructors and destructor

        /**
         * C++ default constructor.
         */
        CIMCVAppStatusPaneHandler(MIMCVEngineFactory& aEngineFactory );

        /**
         * Two-phased constructor.
         */
        static CIMCVAppStatusPaneHandler* NewL(MIMCVEngineFactory& aEngineFactory );

        /**
         * Destructor.
         */
        virtual ~CIMCVAppStatusPaneHandler();

    public: // New functions

        /**
         * Functione to insert titlepane
         */

		void ClearTitlePaneL( );
        
        /**
         * Hides navi-pane decorators.
         * @param aAllowDuplicates If this is true, duplicate default navi pane
         * contents are allowed. Duplicates need to be removed with PopNaviPane
         * when not needed anymore.         
         */		
        void ClearNaviPaneL( TBool aAllowDuplicates = EFalse );
        
        /**
         * SetPicture decorators
         * @since 5.0
         * @param aPicture Bitmap file
         * @param aMask Bitmap mask file
         */		
        void SetPicture( const CFbsBitmap* aPicture, const CFbsBitmap* aMask );

	
        /**
         * Sets title-pane text.
         * @since 5.0
         * @param aTitle Title text
         */		
        void SetTitleL( const TDesC& aTitle );
		
		/**
         * Shows navi-pane's tabgroup-decorator according to aResourceId 
         * and activates a tab which has aType.                  
         * @param aId - user id
         * @param noCreate - ETrue - a New decorator is created else EFalse
         */
        TInt ShowTabGroupL( const TDesC& aId,
        					const TBool noCreate = EFalse ); 
        
		
        /**
         * RefreshTab, refreshes the title pane and tab text 
         * if the user has edited.      
         * @param aUserId - user id of the edited contact     
         */ 
        void RefreshTabL(const TDesC& aUserId,const TDesC& aDisplayName);
    
    public:
		
		/** 
         * API used to regiter for MIMCVTabObserver Events
         */
		void AddObserver(MIMCVTabObserver* aObserver) ;
		
		/** 
         * API used to unregiter from MIMCVTabObserver events
         */
		void RemoveObserver(MIMCVTabObserver* aObserver );	
		
		
		/** 
         * SetActiveTabByIndexL - Sets the Tab Active indicated by Index
         * @param aIndex - Index of the Conversation Item in Tab
         */
		void SetActiveTabByIndexL( const TInt aIndex );				
		
									
		/**
         * Refresh title pane text, needed in skin change events.        
         */
        void RefreshTitlePaneTextL();
        
        
        /**
         * Returns the pointer to CAknTabGroup.        
         */
        CAknTabGroup* TabGroup() const;
        
        void PopNaviPane();
   
	public:
		
		
		/**
		 * Returns IMessage indicator
		 * @return handle to imessage indicator
		 */
		CIMCVNewMsgFadeControl* IMessageIndicator() const;
		
		/**
		 * Initialises instant messaging navipane indicator. 
		 */
		void InitMessageIndicatorL();
		
		/**
		 * Hides fade text
		 */
		void HideFadeText();
		
		/**
		 * Destroys instant messaging indicators. This method is called when
		 * view is deactivated.
		 */
		void DestroyMessageIndicator();
		
		/**
		 * Pushes fade text control to navistack
		 */
		void PushFadeTextL();
		
		/**
		 * Shows fade text
		 * @param aTextToFade Descriptor that contains message to fade
		 */
		void ShowFadeText( TDesC& aTextToFade );
		
		/**
		 * Shows navipane indicators
		 */
		void ShowNavipaneIndicators();
		
		/**
		 * Hides navi pane message info
		 */
		void HideNavipaneIndicators();

		/**
		 * Setting previous title that was present before clear
		 */
		
		void SetPreviousTitleL();		
		/**
		 * Sending the Conversation View into BackGround
		 */
		void SendBackGroundL();

		/**
		 * Bringing the Conversation View into Fore Ground
		 */
		void BringToForegroundL();

	protected:  // New functions
		
		/**
		 * creates new message navipane indicator
		 * @return CAknNavigationDecorator
		 */
		CAknNavigationDecorator* CreateIMessageIndicatorL();
		
		/**
		 * Starts fade operation
		 */
		void RunFadeText();		
		
		/**
		 * Resets the text to fade
		 */
		void ResetFadeText();		
		
	
	private:
		
		/** 
         * @see MIMCVEngineOpenChatsObserver
         */
		void HandleChangeL(TInt aServiceId, TInt aIndex, TOpenChatsEventType aChangeType) ;
		
	    /**
         * @see MIMCVEngineOpenChatsObserver
         */
 	    void SetStatusMsgL(TInt aServiceId, TInt aIndex,const TDesC & aMsg);
 	    
 	    /*
 	     * play message tone, when receiving a new message.
 	     */
 	    void PlayMsgBeepL();

	
	public:	
		/** 
         * API to show the tabs
         */
		void ShowTabL();
		
		/** 
         * API to hide the tabs
         */
		void HideTab();
	
    private:

        /**
         * Creates tabs based on the Open Conversation List         
         */
        void CreateTabsL(const TBool noCreate); 
    
    
    private:     
		/** 
         * @see MAknTabObserver
         */        
        void TabChangedL(TInt aIndex);
    	
    private:

        /**
         * By default Symbian OS constructor is private.
         */
        void ConstructL();

    private:    // Data

        // Doesn't own. Handle to title-pane
        CAknTitlePane*					iTitlePane;

          // Doesn't own. Handle to context-pane
        CAknContextPane*	            iContextPane;
        
        //not Owns the factory for OpenChats 
        MIMCVEngineFactory& 	iEngineFactory;
        
		//Doesn't own
    	RPointerArray< MIMCVTabObserver > iObserverList; 
    	
    	//previous tabID
    	TInt iPrevTabId;
    	
    	//Current TabID
        TInt iCurrentTabId;
        
        //owns the new message icon
        HBufC* iNewMsgIcon;        
        
        //Stores the OpenConversations Tab Count
        TInt iOpenChatArrayCount;   
        
		//Owns. New decorated tab-group
        CAknNavigationDecorator*		iNewDecoratedTabGroup;
        
        // Doesn't own. Handle to navi-pane
        CAknNavigationControlContainer*	iNaviPane;
        
        //Owns.
		CAknNavigationDecorator* iDecoratedFadeText;	
		
		HBufC* iLastTitle;
		
		//iStatusPaneText is used to show truncated received msg (when user is conversation with one user,
		//and receive msg from some other user.)
		HBufC* iStatusPaneText;
    
       //service id
       TInt iServiceId;


    };

#endif      // CIMCVAPPSTATUSPANEHANDLER_H

// End of File
