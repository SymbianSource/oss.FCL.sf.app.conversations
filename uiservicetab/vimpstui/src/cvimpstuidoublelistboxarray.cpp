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
* Description:  CVIMPSTUiDoubleListboxArray-array decorator
*
*/


// INCLUDE FILES
#include	"cvimpstuidoublelistboxarray.h"
#include    "tvimpstconsts.h"
#include    "vimpstutils.h"
#include    "mvimpstprocessarray.h"
#include    "cvimpstuidoublelistboxtabviewcontrol.h" // tab control
#include 	<vimpstuires.rsg>
#include    "tvimpstenums.h"
#include    "vimpstdebugprint.h" 
// platform includes

#include    <AknUtils.h>
#include    <StringLoader.h>
#include    <AknIconUtils.h>
#include    <AknIconArray.h>
#include    <eikclbd.h>
#include    <AknsConstants.h>

#include    <vimpstui.mbg>

const TInt KAvtarLength = 3;
 _LIT( KAvatarString ,   "%d\t" );
// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CVIMPSTUiDoubleListboxArray* CVIMPSTUiDoubleListboxArray::NewL(MVIMPSTProcessArray& aItemModel,
							            CFormattedCellListBoxData* aListboxData,
							            CVIMPSTUiDoubleStyleListBox& aListBox,
							            CVIMPSTUiDoubleListBoxTabViewControl& aTabbedviewControl
							            )
    {
    CVIMPSTUiDoubleListboxArray* self = new (ELeave) CVIMPSTUiDoubleListboxArray( aItemModel, 
    								aListboxData, aListBox,aTabbedviewControl);
    								
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CVIMPSTUiDoubleListboxArray::~CVIMPSTUiDoubleListboxArray()
    {
    delete iData;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CVIMPSTUiDoubleListboxArray::CVIMPSTUiDoubleListboxArray( MVIMPSTProcessArray& aItemModel,
        CFormattedCellListBoxData* aListboxData, CVIMPSTUiDoubleStyleListBox& aListBox /*= NULL*/,
        CVIMPSTUiDoubleListBoxTabViewControl& aTabbedviewControl)
    :
     iItemArray(aItemModel), 
     iListboxData( aListboxData ),
     iListBox(aListBox),
     iTabbedviewControl(aTabbedviewControl)
    {
    }

// Symbian OS default constructor can leave.
void CVIMPSTUiDoubleListboxArray::ConstructL()
	{
	
	 // maximum length of icons appended to identification
    // There can be four icons (A+B and two on D-column)
    iMaxLengthOfIcons =
        KDefaultAvatar().Length()+  // longest A-Column icon
        (KTab().Length() * 2)+ // length of tabs between items 
        KContactOnlineIconA().Length(); // longest D-Column icon
        
    // Reserve enough memory to handle maximum size item
	iData = HBufC::NewL( KFriendsListMaxIdentificationLength +
	                     KStatusMsgMaxLength +
					     iMaxLengthOfIcons );
	
	iPresenceSupported = iItemArray.IsSubServiceSupportedL(TVIMPSTEnums::EPresence);
	iIMSupported = iItemArray.IsSubServiceSupportedL(TVIMPSTEnums::EIM );
    }


// ---------------------------------------------------------
// CVIMPSTUiDoubleListboxArray::MdcaCount
// Returns the number of descriptor elements in a descriptor array.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CVIMPSTUiDoubleListboxArray::MdcaCount() const
	{
	return iItemArray.Count();
	}

// ---------------------------------------------------------
// CVIMPSTUiDoubleListboxArray::MdcaCount
// Return pointer to descriptor data in given index of an array
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPtrC16 CVIMPSTUiDoubleListboxArray::MdcaPoint( TInt aIndex ) const
	{
	CHAT_DP( D_CHAT_LIT("CVIMPSTUiArray::MdcaPoint: aIndex: %d"), aIndex );	    
	// Although this is a const method, we do change the member data.
	// This is done because of performance
	// -> no need to create new buffer every time.
    TPtr dataPtr( iData->Des() );
    dataPtr.Zero();

  
    // fetch the item and process correct type    
    TVIMPSTEnums::TItem type = iItemArray.GetType(aIndex);
    CHAT_DP( D_CHAT_LIT(" -> item type: %d"), type );	    
 
    switch( type )
        {
        case TVIMPSTEnums::EOwnStatusItem:
	        {
	        CHAT_DP( D_CHAT_LIT(" -> own data") );	    
	        TRAP_IGNORE( AppendOwnDataL(dataPtr,aIndex ) );
	        break;	
	        }	
        case TVIMPSTEnums::EContactItem: 		
        case TVIMPSTEnums::EUnknonContactItem:
        case TVIMPSTEnums::EFriendRequestItem:
            {
            return iItemArray.GetFormattedString(aIndex);
            }
        default:
            {
            // shouldn't be here
            CHAT_DP( D_CHAT_LIT(" -> default") );	    
            break;
            }
        } //end outer switch
    
    CHAT_DP( D_CHAT_LIT("CVIMPSTUiArray::MdcaPoint out") );	    
    return dataPtr;
	}
	


// ---------------------------------------------------------
// CVIMPSTUiDoubleListboxArray::AppendContact
// Append formatted contact identification to buffer
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListboxArray::AppendOwnDataL( TPtr& aBuffer,
                                      TInt aIndex
                                      ) const
    {    
    CHAT_DP( D_CHAT_LIT("CVIMPSTUiArray::AppendOwnDataL" ) );		
    TVIMPSTEnums::TVIMPSTRegistrationState loginstate = iItemArray.GetLoginState();
    CHAT_DP( D_CHAT_LIT("CVIMPSTUiArray::AppendOwnDataL, login state: %d" ), loginstate );		
    TInt avtarIndex = iItemArray.OwnAvatarIndexL();

    if( avtarIndex )
        {
        TBuf<KAvtarLength> buf;
        buf.Format( KAvatarString, avtarIndex);
        aBuffer.Append( buf);
        }
    else
        {
        // append default avatar  for column A 
        aBuffer.Append( KDefaultAvatar );
        }
    // B-column  append dispplay id 
     
  
    // append only max length of the user id allowed : extra check
    
    TPtrC identification(VIMPSTUtils::DisplayId(iItemArray.GetItemUserId(aIndex)));
    aBuffer.Append( identification.Left( KFriendsListMaxIdentificationLength ) );

   

    // append  \t before second line text
    aBuffer.Append( KTab );

    // append status text for second line and Icon for column D  based on conditions 
    if (  TVIMPSTEnums::ESVCERegistered == loginstate ) 
        {
        TVIMPSTEnums::TOnlineStatus status = iItemArray.GetOnlineStatusL(aIndex);
        CHAT_DP( D_CHAT_LIT("CVIMPSTUiArray::AppendOwnDataL, status: %d" ), status );		
        TPtrC statusText = iItemArray.StatusTextL( aIndex );
        if( statusText.Length()  > 0) // if status text show it
            {
            // append only KStatusMsgMaxLenght of status masg
            aBuffer.Append( statusText.Left( KStatusMsgMaxLength ) );
            }
        else
            { // show default if no status text based on status
            switch(status) // no need to handle the serviceout case as its not applicable here in case of own data
                {
                case TVIMPSTEnums::EOnline:
                    {
                    HBufC* onlineText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_ONLINE );
                    aBuffer.Append( onlineText->Des());
                    CleanupStack::PopAndDestroy(); //onlineText
                    break;
                    }
                case TVIMPSTEnums::EBusy:
                    {
                    HBufC* busyText = StringLoader::LoadLC( R_SERVDETAIL_STATUSTEXT_BUSY ) ;
                    aBuffer.Append( busyText->Des());
                    CleanupStack::PopAndDestroy(); //busyText
                    break;
                    }
                case TVIMPSTEnums::EAway:   
                    {
                    HBufC* awayText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_AWAY );
                    aBuffer.Append( awayText->Des());
                    CleanupStack::PopAndDestroy(); //awayText
                    break;
                    }
                case TVIMPSTEnums::EOnPhone:   
                    {
                    HBufC* onphoneText = StringLoader::LoadLC( R_SERVDETAIL_STATUSTEXT_ONTHEPHONE );
                    aBuffer.Append( onphoneText->Des());
                    CleanupStack::PopAndDestroy(); //onphone text
                    break;
                    }
                case TVIMPSTEnums::EDoNotDisturb:   
                    {
                    HBufC* dndText = StringLoader::LoadLC( R_SERVDETAIL_STATUSTEXT_DND );
                    aBuffer.Append( dndText->Des());
                    CleanupStack::PopAndDestroy(); //dnd Text
                    break;
                    }
               case TVIMPSTEnums::EInvisible:
                   {
                   HBufC* invisibleText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_INVISIBLE );
                   aBuffer.Append( invisibleText->Des());
                   CleanupStack::PopAndDestroy(); //invisible text
                   break;
                   }  
               case TVIMPSTEnums::EOffline:  
               default:
                    {
                    HBufC* offlineText = StringLoader::LoadLC( R_QTN_SERVTAB_DEFAULT_STATUSTEXT_OFFLINE );
                    aBuffer.Append( offlineText->Des());
                    CleanupStack::PopAndDestroy(); //offlineText
                    break;
                    }
                }// end case for status text
            }
        // append tab before appending icon
        aBuffer.Append( KTab );

        CHAT_DP( D_CHAT_LIT("CVIMPSTUiArray::AppendOwnDataL, handle status enum: %d" ), status );		
        switch(status)
            {
            case TVIMPSTEnums::EOnline:
                {
                aBuffer.Append( KContactOnlineIconA );
                break;
                }
            case TVIMPSTEnums::EInvisible:
                {
                aBuffer.Append( KListInvisibleIcon );
                break;
                }
            case TVIMPSTEnums::EBusy:
            case TVIMPSTEnums::EDoNotDisturb:
                {
                aBuffer.Append( KListBusyIcon );
                break;
                }
            case TVIMPSTEnums::EAway:   
                {
                aBuffer.Append( KListAwayIcon );
                break;
                }
			case TVIMPSTEnums::EOnPhone:   
			    {
			    aBuffer.Append( KFrndOnMobileIcon );
			    break;
			    }
            case TVIMPSTEnums::EOffline: //Flowthrough 
                {
                aBuffer.Append( KContactOfflineIconA );
                break;
                }
            case TVIMPSTEnums::EBlocked:  
            case TVIMPSTEnums::EPending:
            default:
                {
                aBuffer.Append( KEmptyIconA );
                break;
                }
            }//end switch 

        }         
    else 
        {
        // except in  disconnecting state show username
        if(TVIMPSTEnums::ESVCENetworkDisConnecting != loginstate )                               
            {
            CHAT_DP( D_CHAT_LIT("CVIMPSTUiArray::AppendOwnDataL, offline state" ) );		
            TPtrC userId = iItemArray.LoginUserIdFromStoreL(); 
            aBuffer.Append( VIMPSTUtils::DisplayId( userId ));
            }
        aBuffer.Append( KTab ); // append tab 
        aBuffer.Append( KEmptyIconC );  // no icon
        }

}





//  End of File
