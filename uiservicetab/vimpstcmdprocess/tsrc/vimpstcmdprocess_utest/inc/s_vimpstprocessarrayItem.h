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
#include "cvimpstprocessarrayitem.h"

class vimpstprocessarrayitem_stub: public MVIMPSTProcessArrayItem
{
	private:
	
	vimpstprocessarrayitem_stub(TDesC & aItemName);
     
    public:
    
    static MVIMPSTProcessArrayItem* NewL(TDesC & aItemName);
    
    
    
    TPtrC GetItemNameText()
        {
        return KNullDesC();
        }
      
      
      /**
       * Sets the name of the listbox item 
       * @param aName: Name of the listbox item to be set
       * @return void
       */

      void SetConversationOpen(TBool aConvOpen )
          {
          
          }

      /**
       * gets the messages pending status of the contact item 
       * @return iSMsgPending: true if the contact has pending messages 
       * else false
       */
       TBool IsConversationOpen()
          {
          return EFalse;
          }
    /**
    * Returns the default type of the listbox item 
    * @return TEnumsPC::TItem: type of the listbox item as either EContactitem, EContactListItem etc
    */
     TVIMPSTEnums::TItem Type() ;
    
    /**
    * Returns the name of the listbox item 
    * @return TPtrC: name of the listbox item to be displayed
    */
   //  TPtrC GetItemNameText()  ;
     
     TBool IsMsgPending();
     void SetMsgPending(TBool aMsgPending); 
    
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
     TInt GetItemIndex() ;
    
    /**
    * return the VPBK Contact Link
    */
     MVPbkContactLink* ContactLink() ;  
    
    
    /**
    * Sets the on engine/storage side index of the listbox item 
    * @param aIndex: index of the listbox item on engine/storage side
    * @return void
    */
    void SetItemIndex(TInt aIndex) ;
    
    /**
    * Returns the user id of the listbox item 
    * @return TPtrC: user id of the listbox item.
    */
    TPtrC GetItemUserId() ;
    /**
     * Returns the avatar index of the listbox item 
     * @return TInt: avatar index of the listbox item.
     */
    TInt  AvatarIndex() ;
    
    /**
     * sets the avatar index of the listbox item 
     * @param aAvatarIndex: avatar index to be set 
     */
    void  SetAvatarIndex(TInt aAvatarIndex ) ; 
    
    /**
     * returns status text of the contact
     */
    const TDesC& StatusText()
        {
        
        }
    
    /**
     * returns online status of the contact
     */
   TVIMPSTEnums::TOnlineStatus OnlineStatus()
       {
       
       }
    
    
    /**
     * return formatted string needed for listbox for this item
     */
    TPtrC16 FormattedItemString()
        {
        
        }
        
   /**
     * return formatted string needed for listbox for this item
     */
    void  DoFormatStringL()
        {
        
        }
    
    private:
    
    TDesC& iItemName;
   
    };

// End of File
