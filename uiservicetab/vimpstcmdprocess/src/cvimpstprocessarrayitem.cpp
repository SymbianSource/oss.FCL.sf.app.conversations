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
* Description:  CVIMPSTProcessContactItem, CVIMPSTProcessContactListItem, CVIMPSTProcessOwnDataItem,
				 CVIMPSTProcessConversationItem:  Method implementation
*
*/


#include "cvimpstprocessarrayitem.h"
#include "tvimpstconsts.h"
#include "vimpstutils.h"
#include <vimpstuires.rsg>
#include <StringLoader.h>


const TInt KAvtarLength = 3;
 _LIT( KAvatarString ,   "%d\t" );

// -----------------------------------------------------------------------------
// CVIMPSTProcessContactItem::NewL
// -----------------------------------------------------------------------------
//
CVIMPSTProcessContactItem* CVIMPSTProcessContactItem::NewL(CVIMPSTProcessArray& aParent,const TDesC& aContactName,
														const TDesC& aContactId ,
													MVPbkContactLink* aContactLink,
	                                                const TDesC& aStatusText,
	                                                TVIMPSTEnums::TOnlineStatus aOnlineStatus)
    {
    CVIMPSTProcessContactItem* self = new( ELeave ) CVIMPSTProcessContactItem
    							( aParent, aContactLink);
    CleanupStack::PushL( self );
    self->ConstructL(aContactId,aContactName,aStatusText,aOnlineStatus );
    CleanupStack::Pop(); //Codescanner warning ignored
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessContactItem::ConstructL
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessContactItem::ConstructL(const TDesC& aContactId,
										  const TDesC& aContactName,
                                          const TDesC& aStatusText,
                                          TVIMPSTEnums::TOnlineStatus aOnlineStatus )
    {
    iContactUserId = aContactId.AllocL();
    iItemNameTextContact = aContactName.AllocL();
    iSMsgPending = EFalse;
    iStatusText = aStatusText.AllocL();
    iOnlineStatus = aOnlineStatus;
    
    TInt maxLengthOfIcons = KDefaultAvatar().Length()+  // longest A-Column icon
            (KTab().Length() * 2)+ // length of tabs between items 
            KContactOnlineIconA().Length(); // longest D-Column icon

    iFormattedString = HBufC::NewL( KFriendsListMaxIdentificationLength +
            KStatusMsgMaxLength +
            maxLengthOfIcons );
    
    
    DoFormatStringL();
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessContactItem::CVIMPSTProcessContactItem
// -----------------------------------------------------------------------------
//   
CVIMPSTProcessContactItem::CVIMPSTProcessContactItem(CVIMPSTProcessArray& aParent, MVPbkContactLink* aContactLink)
: iParent(aParent),
  iContactLink(aContactLink)
    {
    } 
    
// -----------------------------------------------------------------------------
// CVIMPSTProcessContactItem::~CVIMPSTProcessContactItem
// -----------------------------------------------------------------------------
//  
CVIMPSTProcessContactItem::~CVIMPSTProcessContactItem()
	{
	delete iContactUserId;
	delete iItemNameTextContact;
	if(iStatusText)
	    {
        delete iStatusText;
	    }
	if(iFormattedString)
	    delete iFormattedString;
	}
      
// -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem: Type
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TItem CVIMPSTProcessContactItem::Type()
	{
	return TVIMPSTEnums::EContactItem;
	}

// -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem: GetItemNameText
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTProcessContactItem::GetItemNameText()
    {
     if( iItemNameTextContact )
	    {
	    return *iItemNameTextContact;	
	    }
	return KNullDesC();
    }
 
 // -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem: GetItemUserId
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTProcessContactItem::GetItemUserId()
    {
    if( iContactUserId )
	    {
	    return *iContactUserId;	
	    }
	return KNullDesC();
    }


// -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem::StatusText
// -----------------------------------------------------------------------------
//
const TDesC& CVIMPSTProcessContactItem::StatusText()
    {
    if( iStatusText )
        {
        return *iStatusText; 
        }
    return KNullDesC;
    }
// -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem::OnlineStatus
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TOnlineStatus CVIMPSTProcessContactItem::OnlineStatus()
    {
    return iOnlineStatus;
    }


// -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem: SetMsgPending
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessContactItem::SetMsgPending(TBool aPendingMessageStatus)
    {
    iSMsgPending = aPendingMessageStatus;
    
    TRAP_IGNORE(DoFormatStringL());
    }
// -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem: GetMsgPending
// -----------------------------------------------------------------------------
//
TBool CVIMPSTProcessContactItem::IsMsgPending()
    {
    return iSMsgPending;
    }

// -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem: SetConversationOpen
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessContactItem::SetConversationOpen(TBool aConversationOpen)
    {
    iIsConversationOpen = aConversationOpen;
    
    TRAP_IGNORE(DoFormatStringL());
    }
// -----------------------------------------------------------------------------
//CVIMPSTProcessContactItem: IsConversationOpen
// -----------------------------------------------------------------------------
//
TBool CVIMPSTProcessContactItem::IsConversationOpen()
    {
    return iIsConversationOpen;
    }

void CVIMPSTProcessContactItem::DoFormatStringL()
    {
    TPtr dataPtr( iFormattedString->Des() );
    dataPtr.Zero();
               
        TVIMPSTEnums::TVIMPSTRegistrationState loginstate = iParent.GetLoginState();
        
        TInt avtarIndex = AvatarIndex();
        
        TVIMPSTEnums::TOnlineStatus status = OnlineStatus();

        if( avtarIndex && (status != TVIMPSTEnums::EBlocked))
            {
            // if there is any avatar index then append that icon
            TBuf<KAvtarLength> buf;
            buf.Format( KAvatarString, avtarIndex);
            dataPtr.Append( buf);
            }
        else if( status == TVIMPSTEnums::EPending && TVIMPSTEnums::ESVCERegistered == loginstate)//if contact is remote pending
            {
            // append friend request icon online for column A 
            dataPtr.Append( KRemotePendingRequest );
            }
        else if( status == TVIMPSTEnums::EBlocked && TVIMPSTEnums::ESVCERegistered == loginstate)//if contact is remote pending
            {
            // append blocked icon for column A 
            dataPtr.Append( KFrndBlockedIcon );
            }
        else 
            {
            // append default avatar  for column A 
            dataPtr.Append( KDefaultAvatar );
            }

        // B-column  append dispplay id 
        TPtrC identification(GetItemNameText());
        // append only max length of the user id allowed : extra check 
        dataPtr.Append( identification.Left( KFriendsListMaxIdentificationLength ) );
        
        // append  \t before second line text
        dataPtr.Append( KTab );
        
        // append status text for second line and Icon for column D  based on conditions 
        if ( ( TVIMPSTEnums::ESVCERegistered == loginstate ) && ( iParent.IsPresenceSupported() ) )
            {
            
                TPtrC statusText = StatusText();
                if( statusText.Length()  > 0) // if status text show it
                    {
                    // append only KStatusMsgMaxLenght of status masg
                    dataPtr.Append( statusText.Left( KStatusMsgMaxLength ) );
                    }
                else
                    { // show default if no status text based on status
                    switch(status)
                        {
                        case TVIMPSTEnums::EOnline:
                            {
                        
                            HBufC* onlineText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_ONLINE );
                            dataPtr.Append( onlineText->Des());
                            CleanupStack::PopAndDestroy(); //onlineText
                            break;
                            }
                        case TVIMPSTEnums::EBusy:
                            {
                            HBufC* busyText = StringLoader::LoadLC( R_SERVDETAIL_STATUSTEXT_BUSY ) ;
                            dataPtr.Append( busyText->Des());
                            CleanupStack::PopAndDestroy(); //busyText
                            break;
                            }
                        case TVIMPSTEnums::EAway:   
                            {
                            HBufC* awayText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_AWAY );
                            dataPtr.Append( awayText->Des());
                            CleanupStack::PopAndDestroy(); //Away text
                        break;
                        }
                    case TVIMPSTEnums::EOnPhone:   
                        {
                        HBufC* onphoneText = StringLoader::LoadLC( R_SERVDETAIL_STATUSTEXT_ONTHEPHONE );
                        dataPtr.Append( onphoneText->Des());
                        CleanupStack::PopAndDestroy(); // on phone text
                        break;
                        }
                    case TVIMPSTEnums::EDoNotDisturb:   
                        {
                        HBufC* dndText = StringLoader::LoadLC( R_SERVDETAIL_STATUSTEXT_DND );
                        dataPtr.Append( dndText->Des());
                            CleanupStack::PopAndDestroy(); //dnd Text
                            break;
                            }
                    case TVIMPSTEnums::EPending:
                        {
                        HBufC* pndText = StringLoader::LoadLC( R_REMOTEPENDING_STATUSTEXT );
                        dataPtr.Append( pndText->Des());
                        CleanupStack::PopAndDestroy(); //pnd Text
                        break;
                        }
                    case TVIMPSTEnums::EBlocked:   
                        {
                        HBufC* blockedText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_BLOCKED );
                        dataPtr.Append( blockedText->Des());
                        CleanupStack::PopAndDestroy(); //blockedText 
                        break;
                        }
                    case TVIMPSTEnums::EOffline: //Flowthrough  
                        {
                        HBufC* offlineText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_OFFLINE );
                        dataPtr.Append( offlineText->Des());
                        CleanupStack::PopAndDestroy(); //offlineText
                        break;
                        }
                    case TVIMPSTEnums::ECallForward: //Flowthrough  
                        {
                        HBufC* callforwardText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_CALLFORWARD );
                        dataPtr.Append( callforwardText->Des());
                        CleanupStack::PopAndDestroy(); //callforwardText
                        break;
                        }
                    case TVIMPSTEnums::EServiceOut:
                    default:
                        {
                        break;
                        }
                    }// end of switch
                }// end of else 
            
                // append tab before appending icon
                dataPtr.Append( KTab );

                switch(status)
                    {
                    case TVIMPSTEnums::EOnline:
                        {
                        dataPtr.Append( KContactOnlineIconA );
                        break;
                        }
                    case TVIMPSTEnums::EInvisible:
                        {
                        dataPtr.Append( KListInvisibleIcon );
                        break;
                        }
                    case TVIMPSTEnums::EBusy:
                    case TVIMPSTEnums::EDoNotDisturb:
                        {
                        dataPtr.Append( KListBusyIcon );
                        break;
                        }
                    case TVIMPSTEnums::EAway:   
                        {
                        dataPtr.Append( KListAwayIcon );
                        break;
                        }
                     case TVIMPSTEnums::EOnPhone:   
                        {
                        dataPtr.Append( KFrndOnMobileIcon );
                        break;
                        }
                    case TVIMPSTEnums::EOffline: //Flowthrough  
                        {
                        dataPtr.Append( KContactOfflineIconA );
                        break;
                        }
                    case TVIMPSTEnums::ECallForward: //Flowthrough  
                        {
                        dataPtr.Append( KCallForward );
                        break;
                        }
                    case TVIMPSTEnums::EBlocked: 
                    case TVIMPSTEnums::EPending:
                    case TVIMPSTEnums::EServiceOut:
                    default:
                        {
                        dataPtr.Append( KEmptyIconA );
                        break;
                        }
                    }//end switch 
               
                if( iParent.IsIMSupported() )  
                    {
                    
                    if( IsMsgPending() )
                        {
                        // unread message
                        dataPtr.Append( KContactNewMsgIcon );   
                        }
                    else if( IsConversationOpen()  )
                        {
                        // open conversation
                        dataPtr.Append( KContactOpenConvIcon ); 
                        }
                    else
                        {
                        //empty
                        dataPtr.Append( KEmptyIconC );  
                        }
                    }
              }
            
    }


TPtrC16 CVIMPSTProcessContactItem::FormattedItemString()
    {
    if(iFormattedString)
        return iFormattedString->Des();
    else
        return KNullDesC();
    }


// -----------------------------------------------------------------------------
// CVIMPSTProcessContactListItem::NewL
// -----------------------------------------------------------------------------
//
CVIMPSTProcessContactListItem* CVIMPSTProcessContactListItem::NewL(CVIMPSTProcessArray& aParent)
    {
    CVIMPSTProcessContactListItem* self = new( ELeave ) CVIMPSTProcessContactListItem(aParent);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();//Codescanner warning ignored
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessContactListItem::ConstructL
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessContactListItem::ConstructL()
    {
    iItemNameTextList = HBufC::NewL(KFriendsListMaxIdentificationLength);
    }
// -----------------------------------------------------------------------------
// CVIMPSTProcessContactListItem::c++ default constructor
// -----------------------------------------------------------------------------
//   
CVIMPSTProcessContactListItem::CVIMPSTProcessContactListItem(CVIMPSTProcessArray& aParent)
: iParent(aParent)
    {
    	
    }    
// -----------------------------------------------------------------------------
// CVIMPSTProcessContactListItemPC::~CVIMPSTProcessContactListItemPC destructor
// -----------------------------------------------------------------------------
//
CVIMPSTProcessContactListItem::~CVIMPSTProcessContactListItem()
    {
    delete iItemNameTextList;
    iItemNameTextList = NULL;
    }    
        
// -----------------------------------------------------------------------------
//CVIMPSTProcessContactListItem: Type
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TItem CVIMPSTProcessContactListItem::Type()
	{
	return TVIMPSTEnums::EContactListItem;
	}

// -----------------------------------------------------------------------------
//CVIMPSTProcessContactListItem: IsCollapsed
// -----------------------------------------------------------------------------
//
TBool CVIMPSTProcessContactListItem::IsCollapsed()
	{
	return iCollapsedStatus;
	}

// -----------------------------------------------------------------------------
//CVIMPSTProcessContactListItem: SetCollapsed
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessContactListItem::SetCollapsed(TBool aCollapsedStatus)
	{
	iCollapsedStatus = aCollapsedStatus;
	}

// -----------------------------------------------------------------------------
//CVIMPSTProcessContactListItem: GetItemNameText
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTProcessContactListItem::GetItemNameText()
	{
	return *iItemNameTextList;
	}

// -----------------------------------------------------------------------------
//CVIMPSTProcessContactListItem: SetItemNameText
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessContactListItem::SetItemNameText(const TDesC& aItemName)
	{
	*iItemNameTextList = aItemName;
	}


void CVIMPSTProcessContactListItem::DoFormatStringL()
    {
    //not implemented
    }

TPtrC16 CVIMPSTProcessContactListItem::FormattedItemString()
    {
    //Not Implemented
    return KNullDesC();
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessOwnDataItem::NewL
// -----------------------------------------------------------------------------
//
CVIMPSTProcessOwnDataItem* CVIMPSTProcessOwnDataItem::NewL(CVIMPSTProcessArray& aParent, TDesC& aOwnId)
    {
    CVIMPSTProcessOwnDataItem* self = new( ELeave ) CVIMPSTProcessOwnDataItem(aParent, aOwnId);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //Codescanner warning ignored
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessOwnDataItem::ConstructL
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessOwnDataItem::ConstructL()
    {    

    }
    
// -----------------------------------------------------------------------------
// CVIMPSTProcessOwnDataItem::CVIMPSTProcessOwnDataItem
// -----------------------------------------------------------------------------
//   
CVIMPSTProcessOwnDataItem::CVIMPSTProcessOwnDataItem(CVIMPSTProcessArray& aParent, TDesC& aOwnId)
:iParent(aParent),
 iOwnDataId(aOwnId) 
    {
    	
    }
// -----------------------------------------------------------------------------
// CVIMPSTProcessOwnDataItem::~CVIMPSTProcessOwnDataItem
// -----------------------------------------------------------------------------
//   
CVIMPSTProcessOwnDataItem::~CVIMPSTProcessOwnDataItem()
    {
    	
    }

// -----------------------------------------------------------------------------
//CVIMPSTProcessOwnDataItem: Type
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TItem CVIMPSTProcessOwnDataItem::Type()
	{
	return TVIMPSTEnums::EOwnStatusItem;
	}

// -----------------------------------------------------------------------------
//CVIMPSTProcessOwnDataItemPC: GetItemUserId
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTProcessOwnDataItem::GetItemUserId() 
	{
	return iOwnDataId;
	}

void CVIMPSTProcessOwnDataItem::DoFormatStringL()
    {
    //not implemented
    }

TPtrC16 CVIMPSTProcessOwnDataItem::FormattedItemString()
    {
    //Not Implemented
    return KNullDesC();
    }

/////////////////////////////////////////////////////////////////////////
///////////////////////////////// Unknowncontact item ////////////////////


// -----------------------------------------------------------------------------
// CVIMPSTProcessUnknownContactItem::default constructor
// -----------------------------------------------------------------------------
//
CVIMPSTProcessUnknownContactItem::CVIMPSTProcessUnknownContactItem(CVIMPSTProcessArray& aParent)
: iParent(aParent)
    {
    
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessUnknownContactItem::NewL
// -----------------------------------------------------------------------------
//
CVIMPSTProcessUnknownContactItem* CVIMPSTProcessUnknownContactItem::NewL(CVIMPSTProcessArray& aParent, const TDesC& aBuddyId )
    {
    CVIMPSTProcessUnknownContactItem* self = new (ELeave) CVIMPSTProcessUnknownContactItem(aParent);
    CleanupStack::PushL(self);
    self->ConstructL(aBuddyId);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessUnknownContactItem::ConstructL
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessUnknownContactItem::ConstructL(const TDesC& aBuddyId)
    {
     iUesrId = aBuddyId.AllocL();
     //load formatted string from resource.
     iItemNameText = VIMPSTUtils::LoadResourceL(R_SERVTAB_UNKNOWN_BUDDY);
     iIndex = -1;//default index if no index is set.
     
     TInt maxLengthOfIcons = KDefaultAvatar().Length()+  // longest A-Column icon
             (KTab().Length() * 2)+ // length of tabs between items 
             KContactOnlineIconA().Length(); // longest D-Column icon

     iFormattedString = HBufC::NewL( KFriendsListMaxIdentificationLength +
             KStatusMsgMaxLength +
             maxLengthOfIcons );
     
     DoFormatStringL();
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessUnknownContactItem::destructor
// -----------------------------------------------------------------------------
//
CVIMPSTProcessUnknownContactItem::~CVIMPSTProcessUnknownContactItem()
    {
    delete iItemNameText;
    delete iUesrId;
    
    if(iFormattedString)
        delete iFormattedString;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessUnknownContactItem::GetItemNameText
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTProcessUnknownContactItem::GetItemNameText()
    {
    return iItemNameText->Des();
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessUnknownContactItem::Type
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TItem CVIMPSTProcessUnknownContactItem::Type()
    {
    return TVIMPSTEnums::EUnknonContactItem;
    }
/**
* Returns the user id of the listbox item 
* @return TPtrC: user id of the listbox item.
*/
TPtrC CVIMPSTProcessUnknownContactItem::GetItemUserId()
    {
    return iUesrId->Des();
    }

// -----------------------------------------------------------------------------
//CVIMPSTProcessUnknownContactItem: SetMsgPending
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessUnknownContactItem::SetMsgPending(TBool aPendingMessageStatus)
    {
    iSMsgPending = aPendingMessageStatus;
    
    TRAP_IGNORE(DoFormatStringL());
    }
// -----------------------------------------------------------------------------
//CVIMPSTProcessUnknownContactItem: GetMsgPending
// -----------------------------------------------------------------------------
//
TBool CVIMPSTProcessUnknownContactItem::IsMsgPending()
    {
    return iSMsgPending;
    }

// -----------------------------------------------------------------------------
//CVIMPSTProcessUnknownContactItem: SetConversationOpen
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessUnknownContactItem::SetConversationOpen(TBool aConversationOpen)
    {
    iIsConversationOpen = aConversationOpen;
    
    TRAP_IGNORE(DoFormatStringL());
    }
// -----------------------------------------------------------------------------
//CVIMPSTProcessUnknownContactItem: IsConversationOpen
// -----------------------------------------------------------------------------
//
TBool CVIMPSTProcessUnknownContactItem::IsConversationOpen()
    {
    return iIsConversationOpen;
    }

void CVIMPSTProcessUnknownContactItem::DoFormatStringL()
    {
    TPtr dataPtr( iFormattedString->Des() );
    dataPtr.Zero();
    
    if ( TVIMPSTEnums::ESVCERegistered == iParent.GetLoginState())
            {
            // append friend request icon online for column A 
            dataPtr.Append( KDefaultAvatar );
            }
        else
            {// append friend request icon offline for column A 
            dataPtr.Append( KLocalPendingRequestOffline );
            }
        
        // B-column  append friend request text
        dataPtr.Append(GetItemUserId().Left( KFriendsListMaxIdentificationLength ));
        
        // append  \t before second line text
        dataPtr.Append( KTab );
        
        TPtrC identification(GetItemNameText() ); 
           
        dataPtr.Append( identification );
            

        
        
        // append tab before appending icon
        dataPtr.Append( KTab );

        dataPtr.Append( KEmptyIconA ); 
        
        if( iParent.IsIMSupported() )  
            {
            if( this->IsMsgPending() )
                {
                // unread message
                dataPtr.Append( KContactNewMsgIcon );   
                }
            else if( this->IsConversationOpen())
                {
                // open conversation
                dataPtr.Append( KContactOpenConvIcon ); 
                }
            else
                {
                //empty
                dataPtr.Append( KEmptyIconC );  
                }
            }
    
    }


TPtrC16 CVIMPSTProcessUnknownContactItem::FormattedItemString()
    {
    if(iFormattedString)
        return iFormattedString->Des();
    else
        return KNullDesC();
    }

