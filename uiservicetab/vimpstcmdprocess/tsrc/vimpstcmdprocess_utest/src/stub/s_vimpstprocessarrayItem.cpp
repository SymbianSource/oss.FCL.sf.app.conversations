/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

#include "s_vimpstprocessarrayItem.h"
#include "tvimpstenums.h"

    
    
    MVIMPSTProcessArrayItem* vimpstprocessarrayitem_stub::NewL(TDesC & aItemName)
	    {
	    MVIMPSTProcessArrayItem* item = new(ELeave) vimpstprocessarrayitem_stub(aItemName);
	    return item;
	    }
  
  vimpstprocessarrayitem_stub::vimpstprocessarrayitem_stub(TDesC & aItemName)
  :iItemName(aItemName)
  {
  	
  }
  
  
  
     
     
     /**
      * Sets the name of the listbox item 
      * @param aName: Name of the listbox item to be set
      * @return void
      */

     

     /**
      * gets the messages pending status of the contact item 
      * @return iSMsgPending: true if the contact has pending messages 
      * else false
      */
     
         

   TBool vimpstprocessarrayitem_stub::IsMsgPending() 
      {
      return EFalse;
      }
  
   void vimpstprocessarrayitem_stub::SetMsgPending(TBool aMsgPending) 
      {
      
      }
    /**
    * Returns the default type of the listbox item 
    * @return TEnumsPC::TItem: type of the listbox item as either EContactitem, EContactListItem etc
    */
     TVIMPSTEnums::TItem vimpstprocessarrayitem_stub::Type() 
	     {
	     return TVIMPSTEnums::EContactItem;
	     }
    
    /**
    * Returns the name of the listbox item 
    * @return TPtrC: name of the listbox item to be displayed
    */
     
    /**
    * Sets the name of the listbox item 
    * @param aName: Name of the listbox item to be set
    * @return void
    */
    //virtual void SetItemNameText(const TDesC& aName) = 0;
        
    /**
    * Returns the array index of the listbox item on engine/storage side
    * @return TInt: index of the listbox item on engine/storage side
    */
     TInt vimpstprocessarrayitem_stub::GetItemIndex() 
     {
     	return 0;
     }
    
    /**
    * return the VPBK Contact Link
    */
     MVPbkContactLink* vimpstprocessarrayitem_stub::ContactLink() 
     {
     	return NULL;
     }
    
    
    /**
    * Sets the on engine/storage side index of the listbox item 
    * @param aIndex: index of the listbox item on engine/storage side
    * @return void
    */
    void vimpstprocessarrayitem_stub::SetItemIndex(TInt aIndex) 
    {
    	
    }
    
    /**
    * Returns the user id of the listbox item 
    * @return TPtrC: user id of the listbox item.
    */
    TPtrC vimpstprocessarrayitem_stub::GetItemUserId() 
    {
    TPtrC ptr;
    return ptr;	
    }
    /**
     * Returns the avatar index of the listbox item 
     * @return TInt: avatar index of the listbox item.
     */
    TInt  vimpstprocessarrayitem_stub::AvatarIndex() 
    {
    	return 0;
    }
    
    /**
     * sets the avatar index of the listbox item 
     * @param aAvatarIndex: avatar index to be set 
     */
    void  vimpstprocessarrayitem_stub::SetAvatarIndex(TInt aAvatarIndex )
    {
    	
    }
   
   
    
    
    

// End of File
