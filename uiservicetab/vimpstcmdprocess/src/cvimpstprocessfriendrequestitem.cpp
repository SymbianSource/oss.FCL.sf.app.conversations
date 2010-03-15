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
* Description:  implementation for CVIMPSTProcessFriendRequestItem
*
*/

#include "cvimpstprocessfriendrequestitem.h"
#include "vimpstutils.h"
#include <vimpstuires.rsg>
#include <StringLoader.h>
#include "tvimpstconsts.h"



// -----------------------------------------------------------------------------
// CVIMPSTProcessFriendRequestItem::default constructor
// -----------------------------------------------------------------------------
//
CVIMPSTProcessFriendRequestItem::CVIMPSTProcessFriendRequestItem(CVIMPSTProcessArray& aParent):
    iParent(aParent)
    {
    
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessFriendRequestItem::NewL
// -----------------------------------------------------------------------------
//
CVIMPSTProcessFriendRequestItem* CVIMPSTProcessFriendRequestItem::NewL(CVIMPSTProcessArray& aParent, const TDesC& aRequesterId,
                                                                        const TDesC& aRequestorDisplayName)
    {
    CVIMPSTProcessFriendRequestItem* self = new (ELeave) CVIMPSTProcessFriendRequestItem(aParent);
    CleanupStack::PushL(self);
    self->ConstructL(aRequesterId,aRequestorDisplayName);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessFriendRequestItem::ConstructL
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessFriendRequestItem::ConstructL(const TDesC& aRequesterId, const TDesC& aRequestorDisplayName)
    {
     iUesrId = aRequesterId.AllocL();
     iDisplayName = aRequestorDisplayName.AllocL();
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
// CVIMPSTProcessFriendRequestItem::destructor
// -----------------------------------------------------------------------------
//
CVIMPSTProcessFriendRequestItem::~CVIMPSTProcessFriendRequestItem()
    {
    delete iUesrId;
    delete iDisplayName;
    
    if(iFormattedString)
        delete iFormattedString;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessFriendRequestItem::GetItemNameText
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTProcessFriendRequestItem::GetItemNameText()
    {
    if((*iDisplayName).Length())
        return iDisplayName->Des();
    else 
        return iUesrId->Des();
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessFriendRequestItem::Type
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TItem CVIMPSTProcessFriendRequestItem::Type()
    {
    return TVIMPSTEnums::EFriendRequestItem;
    }

/**
* Returns the user id of the listbox item 
* @return TPtrC: user id of the listbox item.
*/
TPtrC CVIMPSTProcessFriendRequestItem::GetItemUserId()
    {
    return iUesrId->Des();
    }

// -----------------------------------------------------------------------------
//CVIMPSTProcessFriendRequestItem: SetMsgPending
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessFriendRequestItem::SetMsgPending(TBool aPendingMessageStatus)
    {
    iSMsgPending = aPendingMessageStatus;
    
    TRAP_IGNORE(DoFormatStringL());
    }
// -----------------------------------------------------------------------------
//CVIMPSTProcessFriendRequestItem: GetMsgPending
// -----------------------------------------------------------------------------
//
TBool CVIMPSTProcessFriendRequestItem::IsMsgPending()
    {
    return iSMsgPending;
    }
// -----------------------------------------------------------------------------
//CVIMPSTProcessFriendRequestItem: SetConversationOpen
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessFriendRequestItem::SetConversationOpen(TBool aConversationOpen)
    {
    iIsConversationOpen = aConversationOpen;
    
    TRAP_IGNORE(DoFormatStringL());
    }
// -----------------------------------------------------------------------------
//CVIMPSTProcessFriendRequestItem: IsConversationOpen
// -----------------------------------------------------------------------------
//
TBool CVIMPSTProcessFriendRequestItem::IsConversationOpen()
    {
    return iIsConversationOpen;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessFriendRequestItem::FormattedItemString
// -----------------------------------------------------------------------------
//
TPtrC16 CVIMPSTProcessFriendRequestItem::FormattedItemString()
    {
    if(iFormattedString)
        return iFormattedString->Des();
    else
        return KNullDesC();
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessFriendRequestItem::DoFormatString
// -----------------------------------------------------------------------------
//
void CVIMPSTProcessFriendRequestItem::DoFormatStringL()
    {
    
    TPtr dataPtr( iFormattedString->Des() );
        dataPtr.Zero();
    
    
    if ( TVIMPSTEnums::ESVCERegistered == iParent.GetLoginState())
            {
            // append friend request icon online for column A 
            dataPtr.Append( KLocalPendingRequest );
            }
        else
            {// append friend request icon offline for column A 
            dataPtr.Append( KLocalPendingRequestOffline );
            }
        
        // B-column  append friend request text
        HBufC* friendRequestText = StringLoader::LoadLC(R_SERVTAB_FRIEND_REQUEST);
        dataPtr.Append( friendRequestText->Des() );
        CleanupStack::PopAndDestroy(friendRequestText);
        
        // append  \t before second line text
        dataPtr.Append( KTab );
        // append the  user id 
        dataPtr.Append(GetItemNameText().Left( KFriendsListMaxIdentificationLength ));    
    
    }
