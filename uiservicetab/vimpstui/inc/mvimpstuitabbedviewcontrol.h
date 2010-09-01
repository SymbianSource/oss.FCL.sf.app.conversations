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
* Description:  parent class for tabbed view control
*
*/


#ifndef _MVIMPSTUITABBEDVIEWCONTROL_H
#define _MVIMPSTUITABBEDVIEWCONTROL_H

// INCLUDES
#include <coecntrl.h>
#include "mvimpstprocessArrayobserver.h"

#include <eiklbo.h> // MEikListBoxObserver
#include <AiwServiceHandler.h>
#include <AiwContactSelectionDataTypes.h>

#include <MVPbkContactStoreListObserver.h>
#include <AiwContactSelectionDataTypes.h>
#include <MVPbkSingleContactOperationObserver.h> // for phonebook2
#include <MVPbkContactStoreObserver.h>


// FORWARD DECLARATIONS

class MPbk2KeyEventHandler;
class MVIMPSTCmdHandler;
class MVIMPSTProcessArray;
class CVIMPSTUiBrandData;
class CAknSearchField;
class MVIMPSTEngine;
class CVIMPSTUiTabbedView;
class CVPbkFieldTypeSelector;
class CVPbkContactManager;
class CAiwServiceHandler;
class CPbk2SortOrderManager;
class MPbk2ContactNameFormatter;
class MVPbkContactStore;
class TVPbkContactStoreEvent;
class CCAStatusPaneHandler;

//CLASS DECLARATION
/**
 *  parent class for tabbed view control
 *
 *  there are two implementations of this class for  single style/line and double style/line  listboxes
 *  container holds the corresponding type of listbox and array.
 *  The common methods accessed from view( tabbed view ) are here which has to be 
 *  implemented by the concrete class containers.
 *  
 *  @lib vimpstui.dll
 *  @since S60 v5.0
 */
class MVIMPSTUiTabbedViewControl
    {
public:
    /**
     *  Public destructor 
     */  
    virtual ~MVIMPSTUiTabbedViewControl() {}; 
public:
    /*
     * Returns the coecontrol pointer from the  aControl
     * 
     * @return  the pointer to  CCoeControl 
     */
    virtual CCoeControl*  CoeControl() = 0;
    /**
     * Returns the currently focused item's index.
     * @return TInt, current item index.
     */
    virtual TInt CurrentItemIndex() const = 0;
    
    /**
     * send the message using selected item
     * this will launch the conversation view if im is enabled.
     */
    virtual  void SendMessageL()= 0;

      
    /**
     * Activates find-pane
     */
    virtual void ActivateFindPaneL() = 0 ;
    
    /**
     * De-activates find-pane
     */
    virtual void DeactivateFindPaneL() = 0;

   /**
     * Checks if the last contact is focused and Sets the iLastItemFlag
     */            
    virtual void CheckAndSetLastItemFlag()= 0 ; 
    /**
     * Sets the flag to clear the find pane
     */ 
    virtual void SetClrFindPaneFlag(TBool aFlag) = 0;
    
    /**
     * Updates view after edit.
     * @param aIndex is the index of item that has changed.
     * @param aType is list item type. 
     */
    virtual void UpdateViewL( TInt aIndex, TVIMPSTEnums::TItem aType)= 0;
    
    /**
     * Handles item addition for this control.
     */
    virtual void HandleItemAdditionL()= 0;
    
    /**
    *Handle the avatar deletion,updation in UI
    */
	virtual void HandleAvatarRefreshL( const TDesC& aUserId,TBool aBlock)= 0;
	
	/**
    *Handle the avatar deletion in UI
    */
	virtual void SetFocussedId(const TDesC& aContact) = 0 ;
	
	/**
    *Handle the avatar deletion in UI
    */
	virtual TPtrC GetFocussedId() = 0 ;
	
    
    };


#endif // _MVIMPSTUITABBEDVIEWCONTROL_H
