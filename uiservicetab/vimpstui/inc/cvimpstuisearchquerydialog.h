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
* Description:  search fields editor class declaration 
*
*/


#ifndef CVIMPSTUISEARCHQUERYDIALOG_H
#define CVIMPSTUISEARCHQUERYDIALOG_H

//  INCLUDES
#include <AknForm.h>
#include <babitflags.h>

#include "mvimpstsearchuibuilder.h"
#include "cvimpstuisearchfieldaray.h"

// FORWARD DECLARATIONS

class CAknTitlePane;
class CVIMPSTUiSearchFieldArray;
class CAknNavigationControlContainer;
class MVIMPSTSearchUiBuilder;
class CAknTitlePane;

// CLASS DECLARATION
/**
 * search dialog implementation.
 *  @lib vimpstui.lib
 *  @since S60 v5.0
 */
class CVIMPSTUiSearchQueryDialog : public CAknForm,
								   public MVIMPSTSearchUiBuilder
    {
    public: // Constructors and destructor

        /**
         * Creates a new instance of this class.
         * @param aUiFieldArray, reference to field array class 
         * @return  A new instance of this class.
         */
        static CVIMPSTUiSearchQueryDialog* NewL( CVIMPSTUiSearchFieldArray& aUiFieldArray );

        /**
         * Destructor.
         */
        virtual ~CVIMPSTUiSearchQueryDialog();

    public: // Interface

        /**
         * Executes the dialog.
         */
        void ExecuteLD();

    public: // From CAknForm
    
    	/**
         * see @ base class CAknForm
         */
        void PreLayoutDynInitL();
        
        /**
         * see @ base class CAknForm
         */
        void PostLayoutDynInitL();
        
        /**
         * see @ base class CAknForm
         */
        void SetInitialCurrentLineL();
        
        /**
         * see @ base class CAknForm
         */           
        TBool OkToExitL(TInt aKeycode );
        
        /**
         * see @ base class CAknForm
         */  
        TKeyResponse OfferKeyEventL ( const TKeyEvent& aKeyEvent, TEventCode aType );
      
	
	public: // From MVIMPSTSearchUiBuilder
	
		/**
         * see @ base class MVIMPSTSearchUiBuilder
         * @param aCaption, caption of field line 
		 * @param aControlId, control id  
	  	 * @param aControlType, control type 
         * @return  A new instance of CCoeControl.
         */
        CCoeControl* CreateLineL(
                		const TDesC& aCaption,
                		TInt aControlId,
                		TInt aControlType );
                		
        /**
         * see @ base class MVIMPSTSearchUiBuilder
         * @param aControlId, control id  
         * @return  A new instance of CEikCaptionedControl.
         */             
        CEikCaptionedControl* LineControl(
               			 TInt aControlId ) const;
       
        /**
         * see @ base class MVIMPSTSearchUiBuilder
         * @param aControlId, control id  
         */
        void TryChangeFocusL(TInt aControlId );
          
         /**
          * see @ base class MVIMPSTSearchUiBuilder
          * @param aText, caption text  
          */      
        void SetCurrentLineCaptionL( const TDesC& aText );
        
        /**
         * see @ base class MVIMPSTSearchUiBuilder
         * @param aControlId, control id  
         */
        CCoeControl* Control( TInt aControlId ) const;
        
        /**
         * see @ base class MVIMPSTSearchUiBuilder
         * @param aState, ETrue or EFalse  
         */
        void SetEditableL( TBool aState );
        
        /**
         * see @ base class MVIMPSTSearchUiBuilder
         * @param aControlId, control id
         * @param aDimmed, ETrue if hide or EFalse   
         */
        void SetLineDimmed( TInt aControlId , TBool aDimmed ) ;
                

    public: // new implementation 
        
        /**
         * AreAllControlsEmptyL check the all control text are empty 
         * @return ETrue if empty else EFalse
         */ 
        TBool AreAllControlsEmptyL() const;
        
        /**
         *IsEndKeyPressed check to find if End Key was pressed
         * @return ETrue if End Key Pressed
         */
        TBool IsEndKeyPressed();
      

    private: // new Implementation
    	
    	/**
         * constructor 
         * @param aUiFieldArray, reference to ui fields array
         */ 
        CVIMPSTUiSearchQueryDialog(CVIMPSTUiSearchFieldArray& aUiFieldArray);
        
        /**
         * ConstructL 
         */ 
        void ConstructL();
        
        /**
         * Construct the  context menu 
         */    
        void ConstructContextMenuL();
        
        /**
         * Construct the navi pane for dialog
         */ 
        void ConstructNaviPaneL();
        
        /**
         * call to update the title pane
         */ 
        void UpdateTitleL();
        
        /**
         * ok or done has been press
         * perform the saving operation
         */            
        void CmdDoneL( );
        
        /**
         * StoreTitlePaneTextL, 
         * store the previous title text
         */   
        inline void StoreTitlePaneTextL();
        
        /**
         * UpdateCbasL, 
         * to update the rsk, lsk
         */   
        void UpdateCbasL();
        
         /**
          * show ui fields into form, 
          */  
        void ShowSearchUiFieldsL();
        
        /**
         * check if basic field are supported or not
         * @retrun ETrue if Supported else EFalse
         */
        TBool IsBasicFieldAvailableL();
        
        /**
         * set the cba with resource
         * @param aResourceId , resource to use
         */
        void SetCbaCommandSetL( TInt aResourceId ) ;
      
    private: // Data
        
        // not Own: Manages dialog fields
        CVIMPSTUiSearchFieldArray& iUiFieldArray;
       
        // own : last cba resource set
        TInt iCbaCommandSet;
        
        // Own: Title pane text to restore on exit
        HBufC* iStoredTitlePaneText;
      	
      	// owns : search field type
      	TIMPSTSearchFieldTypes iSearchType;
               
        // Own: Context menu bar
        CEikMenuBar* iContextMenuBar;
       
        // Ref: Status pane handle
        CAknNavigationControlContainer* iNaviContainer;
               
        // Ref: Application's title pane
        CAknTitlePane* iTitlePane;
        
        //own: Is End Key Pressed
        TBool isEndKeyPressed;
        
    };

#endif // CVIMPSTUISEARCHQUERYDIALOG_H

// End of File

