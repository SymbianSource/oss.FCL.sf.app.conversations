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
* Description:  implementation for command process array
*
*/


// INCLUDE FILES
#include <coemain.h>
#include    "vimpstdebugprint.h" 

#include "cvimpstprocessarray.h"

#include "mvimpstprocessArrayobserver.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"
#include "tvimpstenums.h"
#include "cvimpstprocessarrayitem.h"
#include "mvimpststorageitemmodel.h"
#include "tvimpstconsts.h"
#include "mvimpststoragecontactlist.h"
#include "mvimpststoragevpbkstorehandler.h"
#include "mvimpstengine.h"
#include "cvimpstprocessfriendrequestitem.h"
#include "mvimpstenginesubservice.h"
#include <MVPbkFieldType.h>
#include <MVPbkContactFieldUriData.h>
#include <MVPbkContactFieldTextData.h>
//rsg file for resource id
#include <vimpstuires.rsg>
#include "vimpstutils.h"
#include <MVPbkContactLink.h>
#include <VPbkEng.rsg>
#include "mvimpstengineimsubservice.h"
#include "mvimpstenginepresencesubservice.h"
#include "vimpstdebugtrace.h"
#include <apgtask.h> 
#include "imcvuiparams.h"

// --------------------------------------------------------------------------
// CVIMPSTProcessArray::CIMArrayProcess
// --------------------------------------------------------------------------
//
CVIMPSTProcessArray::CVIMPSTProcessArray( MVIMPSTEngine& aEngine)
:iOwnDataIndex(KOwnDataIndex),
iContactListIndex(KOwnDataIndex),
iEngine(aEngine)
        {
        iServiceId = iEngine.ServiceId();
        }

// --------------------------------------------------------------------------
// CVIMPSTProcessArray::~CVIMPSTProcessArray
// --------------------------------------------------------------------------
//
CVIMPSTProcessArray::~CVIMPSTProcessArray()
    {
    TRACE( T_LIT("CVIMPSTProcessArray::~CVIMPSTProcessArray Start") );
    if(iContactInterface)
        {
        iContactInterface->RemoveObserver( this );	
        }
    iItemArray.ResetAndDestroy();
    //iItemArray.Close();
    
    
    iUnKnownContactArray.Close();
    iAddRequestArray.Close();
    
    if(iData)
        {
        delete iData;
        iData = NULL;
        }
    //Get IM SubService     
    MVIMPSTEngineSubService* subService1 =    	   
    (iEngine.SubService(TVIMPSTEnums::EIM));

    if(subService1)
        {
        MVIMPSTEngineIMSubService& imSubService = 
        MVIMPSTEngineIMSubService::Cast (*subService1);
        imSubService.UnRegisterChatObserver(this);
        }

    //subscribe for ownpresencechangeevent    			
    MVIMPSTEngineSubService* subService =    	   
    (iEngine.SubService(TVIMPSTEnums::EPresence));

    if(subService)
        {
        MVIMPSTEnginePresenceSubService& presence = 
        MVIMPSTEnginePresenceSubService::Cast (*subService);
        presence.UnRegisterPresenceEventObserver(this);
        } 

    TRACE( T_LIT("CVIMPSTProcessArray::~CVIMPSTProcessArray End") );
    }

// --------------------------------------------------------------------------
// CVIMPSTProcessArray::NewL
// --------------------------------------------------------------------------
//
CVIMPSTProcessArray* CVIMPSTProcessArray::NewL(
        MVIMPSTEngine& aEngine)
    {
    TRACE( T_LIT("CVIMPSTProcessArray::NewL Start") );
    CVIMPSTProcessArray* self = new(ELeave) CVIMPSTProcessArray(aEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACE( T_LIT("CVIMPSTProcessArray::NewL End") );
    return self;
    
    }

// --------------------------------------------------------------------------
// CVIMPSTProcessArray::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTProcessArray::ConstructL()
    {
    TRACE( T_LIT("CVIMPSTProcessArray::ConstructL Start") );
    iContactInterface = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
    if(iContactInterface)
        {        
        iContactInterface->AddObserverL( this );// listen these events..        
        HBufC* unnamed = VIMPSTUtils::LoadResourceL( R_SERVTAB_UNNAMED );
        if( unnamed )
            {
            CleanupStack::PushL( unnamed );
            iContactInterface->SetUnnamedTextL(unnamed); // takes ownership
            CleanupStack::Pop( unnamed );	
            }
        }
    iContactListModel = CVIMPSTStorageManagerFactory::ItemModelInterfaceL(iServiceId);
    iLoginState = iEngine.ServiceState();
    
    iIMSupported = EFalse;
    iPresenceSupported = EFalse;
    //Get IM SubService     
    MVIMPSTEngineSubService* subService1 =(iEngine.SubService(TVIMPSTEnums::EIM));
    if(subService1)
        {
        MVIMPSTEngineIMSubService& imSubService = 
        MVIMPSTEngineIMSubService::Cast (*subService1);
        imSubService.RegisterChatObserver(this);
        iIMSupported = ETrue;
        }
    iData = HBufC::NewL(512);
    iAddRequestArray.Reset();
    iUnKnownContactArray.Reset();
    //subscribe for ownpresencechangeevent
    MVIMPSTEngineSubService* subService =    	   
    (iEngine.SubService(TVIMPSTEnums::EPresence));
    if(subService)
        {        
        MVIMPSTEnginePresenceSubService& presence = 
        MVIMPSTEnginePresenceSubService::Cast (*subService);
        presence.RegisterPresenceEventObserverL(this);     
        iPresenceSupported = ETrue;
        }
	ResetArray();
	TRACE( T_LIT("CVIMPSTProcessArray::ConstructL end") );
    }
// --------------------------------------------------------------------------
// CVIMPSTProcessArray::HandleStorageChangeL
// --------------------------------------------------------------------------
//
void CVIMPSTProcessArray::HandleStorageChangeL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType,
			         							MVIMPSTStorageContactList* /*aList*/, 
				        						MVIMPSTStorageContact* aContact,
				        						TInt aContactIndex )
    {
    TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL Function Start") );
    // 0th index OwnItem + unknow contacts + add request
    aContactIndex = aContactIndex + 1 + iUnKnownContactArray.Count() + iAddRequestArray.Count();
    
    switch( aEventType )
        {
        case TVIMPSTEnums::EStorageContactReadComplete:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageContactReadComplete Start") );
            if(iProcessObservers)
                {
                /* passing 0 so that the focus is on owndata item */
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EOwnStatusItem, 0 ); // focus own item
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageContactReadComplete End") );
            break;
            }
        case TVIMPSTEnums::EStorageContactFetchComplete:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageContactFetchComplete Start") );
            TRACE( T_LIT("HandleStorageChangeL EStorageContactFetchComplete iItemArray Count: %d"), iItemArray.Count() );

            if(iProcessObservers)
                {
                /* passing 0 so that the focus is on owndata item */
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EOwnStatusItem, 0 ); // focus own item
                }

            ResetArray();
            iFetchCompleted = ETrue;
            GetAndCreateOpenChatListL();
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageContactFetchComplete End") );
            break; 	
            }
        case TVIMPSTEnums::EStorageContactReading:
        case TVIMPSTEnums::EStorageContactFetching:
        case TVIMPSTEnums::EStorageContactSynchronizing:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageContactFetching/EStorageContactFetching Start") );
            /*
             *	This event occurs whenever we have contacts added to contactlist. This happens in the following scenarios
             *	1. At the time of login, when we fetch contacts, contact-by-contact is added to the storage. so we get this event.
             * 	2. When we add a contact manually (either from phonebook, or manu adding etc.. ) we get this callback.
             */
            TRACE( T_LIT("contactindex = %d"),aContactIndex );
            CVIMPSTProcessContactItem* contactItem = CVIMPSTProcessContactItem::NewL (*this, const_cast<TDesC&>(aContact->Name() ),
                    const_cast<TDesC&>(aContact->UserId() ),
                    aContact->ContactLink(),
                    const_cast<TDesC&>(aContact->StatusText()),
                    aContact->OnlineStatus());
            
            contactItem->SetAvatarIndex(aContact->AvatarIndex());

            /* All the time the index should be less than the item array's count, but during the fetch of contacts from cdb file to the view, 
             * if we havent got a contactviewready, and during that if I get contactaddition, before getting contactFetchComplete, we should keep 
             * appending the items to the array for display, once we get the entire list, its sorted anyway while insertion, and when presence occurs
             * the contacts are re-sorted. 
             */
            TRACE( T_LIT("itemarraycount = %d"),iItemArray.Count() );
            if (aContactIndex >= iItemArray.Count() )
                {
                TRACE( T_LIT("append contact item %d"), contactItem);
                iItemArray.Append(contactItem);
                }
            else
                {
                TRACE( T_LIT("Insert at index = %d"), aContactIndex);
                iItemArray.Insert(contactItem, aContactIndex );
                }               
            if(iProcessObservers)
                {
                /* passing 0 so that the focus is on owndata item */
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EOwnStatusItem, 0);
                }
            if( aContact->AvatarContent().Length() && iProcessObservers )
                {
                iProcessObservers->HandleAvatarChangeL( aContact->UserId() );
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageContactFetching/EStorageContactFetching End") );
            break;
            }
        case TVIMPSTEnums::EStorageEventContactAddition:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventContactAddition Start") );

            TBool removed = RemoveFromUnknonOrInvitationListL( aContact->UserId(), EFalse );
            if( removed )
                {
                aContactIndex = aContactIndex - 1; // one contact is removed from iUnknownContactArray 
                }
            /*
             *	This event occurs whenever we have contacts added to contactlist. This happens in the following scenarios
             *	1. At the time of login, when we fetch contacts, contact-by-contact is added to the storage. so we get this event.
             * 	2. When we add a contact manually (either from phonebook, or manu adding etc.. ) we get this callback.
                 */
            TPtrC userId = aContact->UserId();
                TRACE( T_LIT("contactindex = %d"),aContactIndex );
                CVIMPSTProcessContactItem* contactItem = CVIMPSTProcessContactItem::NewL ( *this, aContact->Name() ,
                                                                                            userId,
                                                                                            aContact->ContactLink(),
                                                                                            const_cast<TDesC&>(aContact->StatusText()),
                                                                                            aContact->OnlineStatus() );

                contactItem->SetAvatarIndex(aContact->AvatarIndex());
            /* All the time the index should be less than the item array's count, but during the fetch of contacts from cdb file to the view, 
             * if we havent got a contactviewready, and during that if I get contactaddition, before getting contactFetchComplete, we should keep 
             * appending the items to the array for display, once we get the entire list, its sorted anyway while insertion, and when presence occurs
             * the contacts are re-sorted. 
             */
            TRACE( T_LIT("itemarraycount = %d"),iItemArray.Count() );
            if (aContactIndex >= iItemArray.Count() )
                {
                TRACE( T_LIT("append contact item = %d"), contactItem);
                iItemArray.Append(contactItem);
                }                
            else
                {
                TRACE( T_LIT("Insert at index = %d"), aContactIndex);
                iItemArray.Insert(contactItem, aContactIndex );
                }                
             // check if  pending message exist
            if( TVIMPSTEnums::ESVCERegistered == iLoginState && userId.Length() )
                {
                if( IsConversationExistL( userId ) )
                    {
                    contactItem->SetConversationOpen(ETrue);
                    }
                if( IsUnreadMessageExistsL( userId ) )
                    {
                    contactItem->SetMsgPending(ETrue);
                    }
                }

            if(iProcessObservers)
                {
                /* passing aContactIndex so that the focus is on owndata item */
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, aContactIndex );
                }   
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventContactAddition End") );
            break;
            }
        case TVIMPSTEnums::EStorageEventContactDelete:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventContactDelete Start") );
            TRACE( T_LIT("contactindex = %d"),aContactIndex );
            TRACE( T_LIT("itemarraycount = %d"),iItemArray.Count() );
            if( aContactIndex < iItemArray.Count() )
                {
                MVIMPSTProcessArrayItem* deletedItem = iItemArray[ aContactIndex ]; 
                TRACE( T_LIT("contact removed in item array of index = %d"),aContactIndex );
                iItemArray.Remove (aContactIndex);
                delete deletedItem;
                iItemArray.Compress();
                TRACE( T_LIT("contactindex = %d"),aContactIndex );
                }
            if(iProcessObservers)
                {
                iProcessObservers->HandleDeletionL(TVIMPSTEnums::EContactItem, aContactIndex);
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventContactDelete End") );
            break;
            }
        case TVIMPSTEnums::EStorageAvatarChange:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAvatarChange Start") );
            if(iProcessObservers  && aContact )
                {
                TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAvatarChange inside if") );
                TPtrC aUserId = aContact->UserId();
                TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAvatarChange aUserId = %S"), &aUserId );
                iProcessObservers->HandleAvatarChangeL( aContact->UserId() );
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAvatarChange End") );
            break;            
            }
        case TVIMPSTEnums::EStorageOwnPresenceChange:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAvatarChange Start") );
            if(iProcessObservers)
                {
                /* passing index as 0, so thta focus remains at the owndata item */
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EOwnStatusItem, KErrNotFound );
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAvatarChange End") );
            break;
            }
        case TVIMPSTEnums::EStorageMultiplePresenceChange:
	        {
	        TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAvatarChange Start") );
	        // mostly this will get called after just login and fetch time
	        ResetArray();
            if(iProcessObservers )
                {
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, KErrNotFound );
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAvatarChange End") );
	        break;	
	        }
        case TVIMPSTEnums::EStoragePresenceChange:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStoragePresenceChange Start") );
            /* 
             *	After Sort we get the new index... So first we need to remove the contact from the old position, 
             *	and then re-insert it at the correct obtained position (newIndex in this case)
             */
            TRACE( T_LIT("contactindex = %d"),aContactIndex );
            TRACE( T_LIT("itemarraycount = %d"),iItemArray.Count() );
            if( aContactIndex < iItemArray.Count() )
                {                
                TInt newIndex = iContactListModel->IndexOfContact( aContact );
                TRACE( T_LIT("new index = %d"),newIndex );
                CVIMPSTProcessContactItem* newItem = CVIMPSTProcessContactItem::NewL(*this, const_cast<TDesC&>(aContact->Name() ),
                        const_cast<TDesC&>(aContact->UserId() ),
                        aContact->ContactLink(),
                        const_cast<TDesC&>(aContact->StatusText()),
                        aContact->OnlineStatus() );
                
                
                MVIMPSTProcessArrayItem* oldItem = iItemArray[ aContactIndex ]; 
                newItem->SetAvatarIndex(aContact->AvatarIndex()); // copy the avatar index too.
                
                TBool isOldConversationExists(EFalse);
                TBool isOldMsgPending(EFalse);
                // checking if previously chat/msgs were existing
                if( TVIMPSTEnums::ESVCERegistered == iLoginState && aContact->UserId().Length() )
                    {

                    if( IsConversationExistL( aContact->UserId() ) )
                        {
                        isOldConversationExists = ETrue;
                        }
                    if( IsUnreadMessageExistsL( aContact->UserId() ) )
                        {
                        isOldMsgPending = ETrue;
                        }
                   }
                TRACE( T_LIT("contact removed in item array of index = %d"),aContactIndex );
                iItemArray.Remove(aContactIndex );
                delete oldItem;
                iItemArray.Compress();
                
                // restoring if previously chat/msgs were existing
                if( TVIMPSTEnums::ESVCERegistered == iLoginState && aContact->UserId().Length() )
                    {
                    if( isOldConversationExists )
                        {
                        newItem->SetConversationOpen(ETrue);
                        }
                    if( isOldMsgPending )
                        {
                        newItem->SetMsgPending(ETrue);
                        }
                    }
                // Add it in the new index
                newIndex = newIndex + 1 + iUnKnownContactArray.Count()+ iAddRequestArray.Count();
                if (newIndex >= iItemArray.Count())
                    {
                    TRACE( T_LIT("append contact item = %d"), newIndex);
                    iItemArray.Append (newItem);
                    }
                else
                    {
                    TRACE( T_LIT("Insert at index = %d"), newItem);
                    iItemArray.Insert (newItem, newIndex);
                    }   
                }
            if(iProcessObservers )
                {
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, KErrNotFound );
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStoragePresenceChange End") );
            break;
            }
        case TVIMPSTEnums::EStorageEventOwnUserChanged:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventOwnUserChanged Start") );
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL start Event = %d"),aEventType );
            ResetArray();
            if(iProcessObservers )
                {
                iProcessObservers->HandleDeletionL(TVIMPSTEnums::EOwnStatusItem, 0 ); // focus own item

                if( aContact )
                    {
                    iProcessObservers->HandleAvatarChangeL( aContact->UserId() );
                    }
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventOwnUserChanged End") );

            break;
            }
        case TVIMPSTEnums::EStorageAllContactRemoved:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAllContactRemoved Start") );
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAllContactRemoved count = %d"), iItemArray.Count() );
            ResetArray();
            if(iProcessObservers )
                {
                iProcessObservers->HandleDeletionL(TVIMPSTEnums::EOwnStatusItem, 0 ); // focus own item
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageAllContactRemoved End") );
            break;
            }
        case TVIMPSTEnums::EStorageEventContactChange: 
	        {
	        TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventContactChange Start") );
	        // display name is changed ,might be contact is re-arranged
            TRACE( T_LIT("contactindex = %d"),aContactIndex );
            TRACE( T_LIT("itemarraycount = %d"),iItemArray.Count() );
            if( aContactIndex < iItemArray.Count() )
                {
                if ( aContact )
                     {
                    CVIMPSTProcessContactItem* newItem = CVIMPSTProcessContactItem::NewL(*this, const_cast<TDesC&>(aContact->Name() ),
                            const_cast<TDesC&>(aContact->UserId() ),
                            aContact->ContactLink(),
                            const_cast<TDesC&>(aContact->StatusText()),
                            aContact->OnlineStatus() );
                    
                    
                    MVIMPSTProcessArrayItem* oldItem = iItemArray[ aContactIndex ];
                    newItem->SetAvatarIndex(aContact->AvatarIndex()); // copy the avatar index too.
                    TRACE( T_LIT("contact removed in item array of index = %d"),aContactIndex );
                    // set the conversation open flag from old contact, as only the display name would have changed.
                    newItem->SetConversationOpen(oldItem->IsConversationOpen());
                    newItem->SetMsgPending( oldItem->IsMsgPending() );
                    iItemArray.Remove(aContactIndex );
                    delete oldItem;
                    iItemArray.Compress();
                    TRACE( T_LIT("Insert at index = %d"), aContactIndex);
                    TInt newIndex = iContactListModel->IndexOfContact( aContact );
                     // Add it in the new index
                    newIndex = newIndex + 1 + iUnKnownContactArray.Count() + iAddRequestArray.Count();
                    if (newIndex >= iItemArray.Count())
                        {
                        TRACE( T_LIT("append contact item = %d"), newIndex);
                        iItemArray.Append (newItem);
                        }
                    else
                        {
                        TRACE( T_LIT("Insert at index = %d"), newItem);
                        iItemArray.Insert(newItem, newIndex);
                        } 
                    // inform the cv about the display name changes
                    if(aContact && aContact->UserId().Length() && newItem->IsConversationOpen())
                        {
                        TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventContactChange "));
                        TApaTaskList taskList( CCoeEnv::Static()->WsSession() );
                        TApaTask task( taskList.FindApp( KConversationViewAppUid ) );
                    
                        if ( task.Exists() )
                            {
                            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventContactChange:task exists"));
                            // packing of data ,passed to conversation view
                            TPckgBuf< TIMCVUiParams > params;
                            params().iBuddyId = aContact->UserId();
                            params().iBuddyName = aContact->Name();
                            params().iServiceId = iServiceId;
                            params().iUpdate = ETrue;
                            task.SendMessage( 
                                    TUid::Uid( KUidApaMessageSwitchOpenFileValue ), params );
                            }
                        }
                    } 
                
              }
            if(iProcessObservers )
                {
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, aContactIndex );
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventContactChange End") );
	        break;	
	        }          
        case TVIMPSTEnums::EStorageEventUserIdPostChange:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventUserIdPostChange Start") );
            TRACE( T_LIT("contactindex = %d"),aContactIndex );
            TRACE( T_LIT("itemarraycount = %d"),iItemArray.Count() );
            if( aContactIndex < iItemArray.Count() )
                {
                CVIMPSTProcessContactItem* newItem = CVIMPSTProcessContactItem::NewL(*this, const_cast<TDesC&>(aContact->Name() ),
                        const_cast<TDesC&>(aContact->UserId() ),
                        aContact->ContactLink(),
                        const_cast<TDesC&>(aContact->StatusText()),
                        aContact->OnlineStatus() );
                
                
                MVIMPSTProcessArrayItem* oldItem = iItemArray[ aContactIndex ];
                newItem->SetAvatarIndex(aContact->AvatarIndex()); // copy the avatar index too.
                TRACE( T_LIT("contact removed in item array of index = %d"),aContactIndex );
                iItemArray.Remove(aContactIndex );
                delete oldItem;
                iItemArray.Compress();
                TRACE( T_LIT("Insert at index = %d"), aContactIndex);
                iItemArray.InsertL (newItem, aContactIndex);
                }
            if(iProcessObservers )
                {
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, aContactIndex );
                }
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL EStorageEventUserIdPostChange End") );
            break;
            }
        default:
            {
            TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL Event default") );
            break;
            }
        }
    TRACE( T_LIT("CVIMPSTProcessArray::HandleStorageChangeL Function End") );
    }

// --------------------------------------------------------------------------
// CVIMPSTProcessArray::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTProcessArray::AddObserver( MVIMPSTProcessArrayObserver* aObserver ) 
    {
    iProcessObservers = aObserver;
    }
// --------------------------------------------------------------------------
// CVIMPSTProcessArray::RemoveObserver
// --------------------------------------------------------------------------
//
void CVIMPSTProcessArray::RemoveObserver( ) 
    {
    iProcessObservers = NULL;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::GetItemNameTextL
// -----------------------------------------------------------------------------

TPtrC CVIMPSTProcessArray::GetItemNameText(TInt aIndex)
    {
    TVIMPSTEnums::TItem itemtype = GetType( aIndex );
	MVIMPSTProcessArrayItem* arrayItem = NULL;
    /* Codescanner warning is ignored, since Bound check is done 
     * inside the GetType()
     * method*/
    switch(itemtype)
        {
        case TVIMPSTEnums::EOwnStatusItem:
            {
            MVIMPSTProcessArrayItem *arrayItem = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
            return arrayItem->GetItemUserId(); // no name for own user	
            }
        case TVIMPSTEnums::EContactListItem:
        case TVIMPSTEnums::EContactItem:
            { 
            arrayItem = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
           	break;	
            }
        case TVIMPSTEnums::EFriendRequestItem:
            {
            arrayItem = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
            break;
            }
       case TVIMPSTEnums::EUnknonContactItem:
           {
           arrayItem = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
           break;
           }
        case TVIMPSTEnums::EInvalid:
        default:
            {
            break;
            }
        }
	if( arrayItem )
		{
		return arrayItem->GetItemNameText();	
		}
	return KNullDesC();
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::GetItemUserId
// -----------------------------------------------------------------------------

TPtrC CVIMPSTProcessArray::GetItemUserId(TInt aIndex)
    {
    TVIMPSTEnums::TItem itemtype = GetType( aIndex );

    /* Codescanner warning is ignored, since Bound check is done 
     * inside the GetType()
     * method*/
    switch(itemtype)    
        {
        case TVIMPSTEnums::EContactItem:
        case TVIMPSTEnums::EFriendRequestItem:
        case TVIMPSTEnums::EUnknonContactItem:
        case TVIMPSTEnums::EOwnStatusItem:
            {
            MVIMPSTProcessArrayItem *arrayItem = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
            return arrayItem->GetItemUserId(); // no name for own user	
            }
        default:
            {
            return KNullDesC(); 
            }
        }
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::ContactLink
// -----------------------------------------------------------------------------

MVPbkContactLink* CVIMPSTProcessArray::ContactLink(TInt aIndex)
    {
    TVIMPSTEnums::TItem itemtype = GetType( aIndex );

    /* Codescanner warning is ignored, since Bound check is done 
     * inside the GetType()
     * method*/
    switch(itemtype)
        {
        case TVIMPSTEnums::EOwnStatusItem:
            {
            return NULL;	
            }
        case TVIMPSTEnums::EContactListItem:
        case TVIMPSTEnums::EContactItem:
            { 	
            MVIMPSTProcessArrayItem *arrayItem = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
            return arrayItem->ContactLink();	
            }
        case TVIMPSTEnums::EInvalid:
        default:
            {
            }
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::Count
// -----------------------------------------------------------------------------

TInt CVIMPSTProcessArray::Count() const
	{
	// return the total count here.
	return iItemArray.Count() ;
	}
// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::IsSubServiceSupportedL
// -----------------------------------------------------------------------------

TBool CVIMPSTProcessArray::IsSubServiceSupportedL(TVIMPSTEnums::SubServiceType aType ) const
	{
	return iEngine.IsSubServiceSupportedL(aType );
	}

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::IsIMSupported
// -----------------------------------------------------------------------------
TBool CVIMPSTProcessArray::IsIMSupported()
    {
    return iIMSupported;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::IsPresenceSupported
// -----------------------------------------------------------------------------
TBool CVIMPSTProcessArray::IsPresenceSupported()
    {
    return iPresenceSupported;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::GetType
// Get the type from engine and return the type
// -----------------------------------------------------------------------------

TVIMPSTEnums::TItem CVIMPSTProcessArray::GetType(TInt aIndex) const
	{
	TInt itemArrayCount = iItemArray.Count();
	if(itemArrayCount <=0 || (aIndex < 0 || aIndex >= itemArrayCount) )
	    {
	    return TVIMPSTEnums::EInvalid;
	    }

	else if(itemArrayCount>0)
	    {
	    TInt index = aIndex <= 0 ? 0 : aIndex;
	    MVIMPSTProcessArrayItem* item = iItemArray[ index ];	
	    return item->Type();
	    }
	else
	    {
	    return TVIMPSTEnums::EInvalid;
	    }	
	}
// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::FillItemL
// Fill up the complete array one item at a time
// -----------------------------------------------------------------------------

void CVIMPSTProcessArray::FillItemL()
    {
    TInt count = 0;
    if(iContactListModel)
        {
        count = iContactListModel->Count();
        }

    // the index is starting from 1, because we already added owndata item to the list.......
    for ( TInt index = 0; index < count ; index++)
        {
        // fetch the item and process correct type
        MVIMPSTStorageItemModel::SItem item = iContactListModel->Item( index );
        switch( item.iType )
            {
            case MVIMPSTStorageItemModel::EContactList:
                break;
            case MVIMPSTStorageItemModel::EContactItem:
                {
                // contact item
                // add this items to contact item -- CONTACT 
                // create an object of this type and append this to the rpointerarray
                MVIMPSTStorageContact* contact = item.iContact;							   
                TPtrC userId = contact->UserId();
                CVIMPSTProcessContactItem* contactItem = 
                CVIMPSTProcessContactItem::NewL(*this, contact->Name(),
                								userId ,
                        						contact->ContactLink(),
                        			            const_cast<TDesC&>(contact->StatusText()),
                        			            contact->OnlineStatus() );
                
                contactItem->SetAvatarIndex(contact->AvatarIndex());
                
                TInt otherCount = 1 + iUnKnownContactArray.Count() + iAddRequestArray.Count();
                contactItem->SetItemIndex(index + otherCount );
                //append this to the array
                iItemArray.AppendL(contactItem);
                if( TVIMPSTEnums::ESVCERegistered == iLoginState && userId.Length() )
                    {
                    if( IsUnreadMessageExistsL( userId ) )
                        {
                        contactItem->SetMsgPending(ETrue);
                        }
                    if( IsConversationExistL( userId ) )
                        {
                        contactItem->SetConversationOpen(ETrue);
                        }
                    }
                break;
                }
            default:
                {
                // the call shouldn't be here
                break;
                }
            }
        }

    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::FillOwnDataL
// FillOwnDataL implementation
// -----------------------------------------------------------------------------
// fill the owndata at the begining of array.....
TBool CVIMPSTProcessArray::FillOwnDataL()
    {
    TBool ownDataAdded = EFalse;
    TPtr dataPtr = iData->Des();    
    // Check whether the user has logged in before, if has
    // then dont update anything just return   
    //check if its logged in or not.
    // 1. if not logged in append 
    switch(iLoginState)
        {
        case TVIMPSTEnums::ESVCERegistered:
            {
            TPtrC userId = iContactInterface->OwnContactL().UserId();
            dataPtr.Copy(userId);
            break;
            }
        case TVIMPSTEnums::ESVCENetworkConnecting:
            {
            HBufC* tempStr = VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_LOGGING_LIST_ITEM );
            if(tempStr)
                {
                dataPtr.Copy(*tempStr);     
                delete tempStr; 
                }

            break;
            }
        case TVIMPSTEnums::ESVCEUpdatingContacts:
            {
            HBufC* tempStr = VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_UPDATING_LIST_ITEM );
            if(tempStr)
                {
                dataPtr.Copy(*tempStr);     
                delete tempStr; 
                }

            break;
            }
        case TVIMPSTEnums::ESVCEWaitingForNetwork:
            {
            HBufC* tempStr = VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_WAITING_LIST_ITEM );
            if(tempStr)
                {
                dataPtr.Copy(*tempStr);     
                delete tempStr; 
                }
			break;	
            }
        case TVIMPSTEnums::ESVCENetworkDisConnecting:
            {
            HBufC* tempStr = NULL;
            if ( iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EPresence) ) 
                {
                // if presence enabled use double line listbox string
                tempStr= VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_LOGGINGOUT_LIST_ITEM );
                }
            else
                { 
                // if  presence is not enabled use  single line listbox string
                tempStr= VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SINGLE_LINE_LOGGINGOUT_LIST_ITEM );
                }

            if(tempStr)
                {
                dataPtr.Copy(*tempStr);     
                delete tempStr;
                }			

            break;
            }
        case TVIMPSTEnums::ESVCENotRegistered:
        default:
            {
            HBufC* str = NULL;
            if ( iEngine.SubService( TVIMPSTEnums::EPresence) ) 
                {
                // if presence enabled use double line listbox string
                str = VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_LOGIN_LIST_ITEM );
                }
            else
                { 
                // if  presence is not enabled use  single line listbox string
                // get  the username from settings
                TPtrC ownUserId = iContactInterface->OwnContactL().UserId();
                if ( ownUserId.Length() )
                    { 
                    // if username is available then show with login item
                    // remove domain part and give it to resource laoder
                    str= VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SINGLE_LINE_LOGIN_LIST_ITEM_WITH_USERNAME , VIMPSTUtils::DisplayId( ownUserId ) );
                    }
                else
                    {
                    // this condition occures when there is allocated memory for username with NO data (length =0 )
                    // just make sure "no memory leak" without depending on the Function  LoginUserNameFromSettingsL()
                    // since that would return the allocated memory without Data (length=0 ) 
                    // just show login item since no username is available in the settings
                    str= VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SINGLE_LINE_LOGIN_LIST_ITEM );
                    }
                }
            if( str )
                {
                dataPtr.Copy(*str);		
                delete str;		
                }
            break;
            }
        }
    if( iItemArray.Count() )
	    {
	    // own item is already exist delete it before adding a fresh one
	    MVIMPSTProcessArrayItem* arrayItem = iItemArray[ 0 ]; // should be own data
	    if( arrayItem->Type() == TVIMPSTEnums::EOwnStatusItem )
		    {
	    	iItemArray.Remove(0);   //iItemArray takes ownership of owndataItem, do not delete here
	    	delete arrayItem;
	    	iItemArray.Compress();
	    	}
	    }
    //common code for all the above cases to update the owndata item to list at index 0
    CVIMPSTProcessOwnDataItem* owndataItem = CVIMPSTProcessOwnDataItem::NewL(*this, *iData);
    owndataItem->SetItemIndex(0);
    //append this to the array
    iItemArray.InsertL( owndataItem,0 );   //iItemArray takes ownership of owndataItem, do not delete here
    iContactListIndex = 0;
    ownDataAdded = ETrue;     
    return ownDataAdded;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::FillArrayL
// -----------------------------------------------------------------------------

void CVIMPSTProcessArray::FillArrayL()
    {
    if(FillOwnDataL() )
        {
        // then fill the conversations..or open chats....	
        }
    else
        {
        iContactListIndex = 0; // we have not added any thing to main array..so index is zero...
        }
    FillUnknownContactsL(); //will fill all unknown contact from iUnknownContactArray to iItemArray.
    FillAddRequestDataL();//will fill all requests from iAddRequestArray to iItemArray.
    FillItemL();   
    }


// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::ResetArray
// -----------------------------------------------------------------------------

void CVIMPSTProcessArray::ResetArray()
    {
    RemoveUnKnownContacts(); //will remove all the unknown contacts from iItemArray.
    RemoveAddRequestData();//will remove all the requests from iItemArray.
    iItemArray.ResetAndDestroy();
    TRAPD( err, FillArrayL());
    if ( err != KErrNone )
        {
        CActiveScheduler::Current()->Error( err );
        }
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::SetLoginState
// -----------------------------------------------------------------------------

void CVIMPSTProcessArray::SetLoginStateL(TVIMPSTEnums::TVIMPSTRegistrationState aLoginState)
    {
    iLoginState = aLoginState;
    if(aLoginState != TVIMPSTEnums::ESVCERegistered )
        {
        iFetchCompleted = EFalse;
        RemoveUnKnownContacts();
        RemoveAddRequestData();
        iUnKnownContactArray.ResetAndDestroy();// delete all items
        iUnKnownContactArray.Reset();
        iAddRequestArray.ResetAndDestroy();// delete all items
        iAddRequestArray.Reset();
        }
    FillOwnDataL(); // this will change the own item based on connection status
    }
// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::GetLoginState
// -----------------------------------------------------------------------------

TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTProcessArray::GetLoginState()
    {
    return iLoginState;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::GetOnlineStatus
// -----------------------------------------------------------------------------

TVIMPSTEnums::TOnlineStatus CVIMPSTProcessArray::GetOnlineStatusL(TInt aIndex)
    {
    TVIMPSTEnums::TItem itemtype = GetType( aIndex );
    TVIMPSTEnums::TOnlineStatus status = TVIMPSTEnums::EUnknown;
    switch(itemtype)
        {
        case TVIMPSTEnums::EOwnStatusItem:
            {
            if( iContactInterface )
	            {
                 status = iContactInterface->OwnContactL().OnlineStatus();
	            }
            break;
            }
        case TVIMPSTEnums::EContactItem:
            { 

            status = iItemArray[aIndex]->OnlineStatus();
            break;
            }
        case TVIMPSTEnums::EInvalid:
        default:
            {
            break;
            }
        }
    return status;
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::StatusText
//-----------------------------------------------------------
// 
const TDesC&  CVIMPSTProcessArray::StatusTextL(TInt aIndex ) 
    {
    TVIMPSTEnums::TItem itemtype = GetType( aIndex );
    switch(itemtype)
        {
        case TVIMPSTEnums::EOwnStatusItem:
            {
            if( iContactInterface )
	            {
	            return iContactInterface->OwnContactL().StatusText();	
	            }
            break;
            }
        case TVIMPSTEnums::EContactItem:
            { 
            return iItemArray[aIndex]->StatusText();
            }
        case TVIMPSTEnums::EInvalid:
        default:
            {
            break;
            }
        }
    return KNullDesC;
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::RemoveFromUnknonOrInvitationListL
//-----------------------------------------------------------
//
TBool CVIMPSTProcessArray::RemoveFromUnknonOrInvitationListL( const TDesC& aAddedUserId ,TBool aBlocked )
    {
    TBool found = RemoveFromUnknonListL( aAddedUserId );
    if( !found )
        {
        found = RemoveFromInvitationListL( aAddedUserId );
        }
    //Get IM SubService  
    if( aBlocked && found && aAddedUserId.Length() )
        {
        MVIMPSTEngineSubService* subService =         
        (iEngine.SubService(TVIMPSTEnums::EIM));

        if(subService )
            {
            MVIMPSTEngineIMSubService& imSubService = 
            MVIMPSTEngineIMSubService::Cast (*subService);
            imSubService.CloseConversationL( aAddedUserId );
            }
        }
    return found;
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::RemoveFromUnknonListL
//-----------------------------------------------------------
//
TBool CVIMPSTProcessArray::RemoveFromUnknonListL( const TDesC& aAddedUserId )
    {
    TRACE( T_LIT("CVIMPSTProcessArray::RemoveFromUnknonListL start ") );
    TPtrC addedUserId = VIMPSTUtils::DisplayId( aAddedUserId );
    TBool found = EFalse;
    TInt count = iUnKnownContactArray.Count();
    CVIMPSTProcessUnknownContactItem* unknownItem = NULL;
    TInt index = KErrNotFound;
    TInt itemIndex = KErrNotFound;
    // check in unknon array
    for(index = 0 ; index < count ; index++)
        {
        unknownItem = iUnKnownContactArray[index];
        TPtrC contactId = VIMPSTUtils::DisplayId( unknownItem->GetItemUserId() );
        if( addedUserId.Compare(  contactId ) == 0 )
            {
            itemIndex = iItemArray.Find(unknownItem);
            if( itemIndex > -1 )
                {
                found = ETrue;
                }
            break;
            }
        TRACE( T_LIT("CVIMPSTProcessArray::RemoveFromUnknonListL  for ends") );
        }
    if( found )
        {
        iItemArray.Remove(itemIndex);
        iUnKnownContactArray.Remove( index );
        delete unknownItem;
        unknownItem = NULL;
        iItemArray.Compress();
        iUnKnownContactArray.Compress();
        if( iProcessObservers )
            {
            // this is to refresh the list box
            // KErrNotFound , focus remain at same position
            iProcessObservers->HandleDeletionL(TVIMPSTEnums::EUnknonContactItem, KErrNotFound );
            }
        }
    TRACE( T_LIT("CVIMPSTProcessArray::RemoveFromUnknonListL end ") );
    return found;
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::RemoveFromInvitationListL
//-----------------------------------------------------------
//
TBool CVIMPSTProcessArray::RemoveFromInvitationListL( const TDesC& aAddedUserId )
    {
    TPtrC addedUserId = VIMPSTUtils::DisplayId( aAddedUserId );
    TBool found = EFalse;
    TInt index = KErrNotFound;
    TInt itemIndex = KErrNotFound;
    // item not foun in unknown list
    // check in invitation item array
    TInt count = iAddRequestArray.Count();
    CVIMPSTProcessFriendRequestItem* requestItem = NULL;
    for(index = 0 ; index < count ; index++)
        {
        requestItem = iAddRequestArray[index];
        TPtrC contactId = VIMPSTUtils::DisplayId( requestItem->GetItemUserId() );
        if( addedUserId.Compare( contactId ) == 0 )
            {
            itemIndex = iItemArray.Find(requestItem);
            if( itemIndex > -1 )
                {
                found = ETrue;
                }
            break;
            }
        }
    if( found )
        {
        iItemArray.Remove(itemIndex);
        iAddRequestArray.Remove( index );
        delete requestItem;
        requestItem = NULL;
        iItemArray.Compress();
        iAddRequestArray.Compress();
        if( iProcessObservers )
            {
            // this is to refresh the list box
            // KErrNotFound , focus remain at same position
            iProcessObservers->HandleDeletionL(TVIMPSTEnums::EFriendRequestItem, KErrNotFound );
            }
        }
    return found;
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::MapContactListPositions
//-----------------------------------------------------------
//
void CVIMPSTProcessArray::MapContactListPositions(RArray<TInt>& aPos )
    {
    // add conversation also this list ..and return..
    RArray<TInt> listPos; 

    //	iContactListModel->MapContactListPositions(listPos);
    TInt positionIndex = 0;
    // if count is more than 0..fill the maparray...
    if(iItemArray.Count()  + iContactListIndex > 0) 
        {

        TInt count = listPos.Count();

        for(TInt posIndex = 0; posIndex < count; posIndex++, positionIndex++)
            {
            aPos.Append( listPos[posIndex] + iContactListIndex );
            }
        }
    listPos.Close();	
    }
// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::GetAndCreateOpenChatListL
// return contact index including friend request.
// -----------------------------------------------------------------------------
void CVIMPSTProcessArray::GetAndCreateOpenChatListL() 
    {
    //Get IM SubService     
    MVIMPSTEngineSubService* subService =         
                (iEngine.SubService(TVIMPSTEnums::EIM));
    RArray<SIMCacheChatItem> openChats;
    openChats.Reset();
    if(subService)
        {
        MVIMPSTEngineIMSubService& imSubService = 
            MVIMPSTEngineIMSubService::Cast (*subService);
        openChats = imSubService.GetOpenChatListL();
        }
    while( openChats.Count() )
        {
        SIMCacheChatItem chat = openChats[ 0 ]; // first item 
        TPtrC contactId = *chat.iBuddyId;
        MVIMPSTProcessArrayItem* arrayItem = FindArrayItem( contactId );
        TBool msgPending = EFalse;
        if( contactId.Length() )
            {
            if( IsUnreadMessageExistsL( contactId ) )
                {
                msgPending = ETrue;
                }
            }
        if( arrayItem )
            {
            arrayItem->SetConversationOpen( ETrue );
            arrayItem->SetMsgPending( msgPending );
            }
        else
            {
            // not in buddy List
            DoHandleUnKnownContactMessageL( contactId ,msgPending );
            }
        openChats.Remove( 0 );
        delete chat.iBuddyId;
        chat.iBuddyId = NULL;
        openChats.Compress();
        }
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::FindArrayItem
//-----------------------------------------------------------
// 
MVIMPSTProcessArrayItem* CVIMPSTProcessArray::FindArrayItem( const TDesC& aSenderId )
    {
    TPtrC senderId = VIMPSTUtils::DisplayId( aSenderId );
    MVIMPSTProcessArrayItem* arrayItem = NULL;
    TInt count = iItemArray.Count();
    for( TInt i = 1; i < count; i++ )
        {
        MVIMPSTProcessArrayItem* contactItem = (MVIMPSTProcessArrayItem *)iItemArray[i];
        TPtrC contactId = VIMPSTUtils::DisplayId( contactItem->GetItemUserId() );
        if( senderId.Compare(  contactId ) == 0 )
            {
            arrayItem = contactItem;
            break;
            }
        }
    return arrayItem;
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::HandleChatMessageEventL
//-----------------------------------------------------------
// 
void CVIMPSTProcessArray::HandleChatMessageEventL( TVIMPSTEnums::TIMEventType aEventType ,
                                                    const TDesC& aSender )
    {
    if( !iFetchCompleted )
        {
        // contact matching will not be correct until fetching completed
        // hence return 
        return;
        }
    switch( aEventType )
        {
        case TVIMPSTEnums::EIMUnreadMessage:
            {
            if( FindAndMarkContactAsOpenChat( aSender , ETrue ) )
                {
                if(iProcessObservers)
                    {
                    iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, KErrNotFound );
                    }
                }
            else 
                {
                DoHandleUnKnownContactMessageL( aSender , ETrue );
                }
            break;
            }
        case TVIMPSTEnums::EIMUnreadChange:
            {
            FindAndMarkContactAsOpenChat( aSender , EFalse ) ;
            if(iProcessObservers)
                {
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem,KErrNotFound );
                }
            break;
            }
        case TVIMPSTEnums::EIMChatStarted:
            {
            if( FindAndMarkContactAsOpenChat( aSender , EFalse ) )
                {
                if(iProcessObservers)
                    {
                    iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, KErrNotFound );
                    }
                }
            else 
                {
                DoHandleUnKnownContactMessageL( aSender, EFalse );
                }
            break;
            }
        case TVIMPSTEnums::EIMChatClosed:
            {
            if( !RemoveFromUnknonListL( aSender ) )
                {
                MVIMPSTProcessArrayItem* arrayItem = FindArrayItem( aSender );
                if( arrayItem )
                    {
                    arrayItem->SetConversationOpen( EFalse );
                    arrayItem->SetMsgPending( EFalse );
                    }
                }
            if(iProcessObservers)
                {
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, KErrNotFound );
                }
            break;
            }
        case TVIMPSTEnums::EIMAllChatClosed:
            {
            RemoveUnKnownContacts();
            iUnKnownContactArray.ResetAndDestroy();// delete all items
            ResetOpenConversationPendingMsg();
            if(iProcessObservers)
                {
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, KErrNotFound );
                }
            break;
            }
        case TVIMPSTEnums::EIMRequestCompleted:
            {
            if(iProcessObservers)
                {
                iProcessObservers->HandleAdditionL(TVIMPSTEnums::EContactItem, KErrNotFound );
                }
            break;
            }
        default:
            {
            // do nothing
            }
        }
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::HandleAddRequestEventL
//-----------------------------------------------------------
// 
void  CVIMPSTProcessArray::HandleAddRequestEventL(TVIMPSTEnums::TOperationType aType, const TDesC& aRequesterId,
                                                    const TDesC& aRequestorDisplayName )
    {
  	//add new add request item at top of list(i.e at index 1, as 0 is own id).
    //when request is entertained remove item from list.
    switch( aType )
	    {
		case TVIMPSTEnums::EAddItem :
			{
			if( (!FindAnyAddRequest(aRequesterId ) )&& ( !FindAnyContactServiceField( aRequesterId ) ) && !(IsContactBlocked( aRequesterId )) )
				{
				CVIMPSTProcessFriendRequestItem* addRequestItem = CVIMPSTProcessFriendRequestItem::NewL(*this, aRequesterId,aRequestorDisplayName);
				/* Add it as the first Item of IAddRequestARray as its like stack */
				iAddRequestArray.Insert(addRequestItem,0); 
				/*
				*	always the new friend request will be shown in the beginning, so 0th item is own item, 1st item is new friendrequest
				*	and remaining remains same.. 
				*/
				// ownership is in iItemArray
				TInt invitationIndex = iUnKnownContactArray.Count()+1;
				iItemArray.Insert(addRequestItem, invitationIndex); 
				if(iProcessObservers)
					{
					iProcessObservers->HandleAdditionL( TVIMPSTEnums::EFriendRequestItem ,KErrNotFound );
					}
				}
			break;
			}
		case TVIMPSTEnums::ERemoveItem :
			{
			TInt count = iAddRequestArray.Count();
			CVIMPSTProcessFriendRequestItem* requestItem = NULL;
			for(TInt i = 0 ; i < count ; i++)
				{
				requestItem = iAddRequestArray[i];
				if( aRequesterId.Compare(requestItem->GetItemUserId()) == 0 )
					{
					TInt index = iItemArray.Find(requestItem);
					if(index != KErrNotFound)
						{
						iItemArray.Remove(index);
						iItemArray.Compress();
						}
					iAddRequestArray.Remove(i);        
					delete requestItem;
					iAddRequestArray.Compress();
					break;
					}        
				}    
			if(iProcessObservers)
				{
				iProcessObservers->HandleDeletionL(TVIMPSTEnums::EFriendRequestItem, KErrNotFound );
				}
			break;
			} 
		default :
			break;
		}
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::DoHandleUnKnownContactMessageL
//-----------------------------------------------------------
// 
void CVIMPSTProcessArray::DoHandleUnKnownContactMessageL( const TDesC& aSenderId ,TBool aIsMsgPending )
    {
    TBool contactExist = EFalse ;
    TInt count = iUnKnownContactArray.Count();
    CVIMPSTProcessUnknownContactItem* unknownItem = NULL;
    for( TInt i=0; i<count; i++ )
        {
        unknownItem = iUnKnownContactArray[i];
        if( aSenderId.Compare( unknownItem->GetItemUserId() ) == 0 )
            {
            contactExist = ETrue;
            unknownItem->SetConversationOpen(ETrue);
            unknownItem->SetMsgPending( aIsMsgPending );
            break;
            }
        }
    if( !contactExist )
        {
        CVIMPSTProcessUnknownContactItem* addItem = CVIMPSTProcessUnknownContactItem::NewL(*this, aSenderId);
        /* Add it as the first Item of IAddRequestARray as its like stack */
        iUnKnownContactArray.Insert(addItem,0); 
        // ownership is in iItemArray
        iItemArray.Insert(addItem, 1);  // just after own Item
        addItem->SetMsgPending( aIsMsgPending );
        addItem->SetConversationOpen(ETrue);
        }
    if(iProcessObservers)
        {
        iProcessObservers->HandleAdditionL( TVIMPSTEnums::EUnknonContactItem ,KErrNotFound );
        }
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::IsMsgPending
//-----------------------------------------------------------
// 
TBool CVIMPSTProcessArray::IsMsgPending(TInt aIndex)
    {
    if( aIndex < iItemArray.Count() && aIndex >= 0 )
        {
        MVIMPSTProcessArrayItem* item = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
        if(item)
            {   
            return item->IsMsgPending();
            }   
        }
    return EFalse;
    }

//-----------------------------------------------------------
//CVIMPSTProcessArray::IsConversationExist
//-----------------------------------------------------------
// 
TBool CVIMPSTProcessArray::IsConversationExist(TInt aIndex)
    {
    if( aIndex < iItemArray.Count() && aIndex >= 0 )
        {
        MVIMPSTProcessArrayItem* item = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
        if(item)
            {   
            return item->IsConversationOpen();
            }   
        }
    return EFalse;
    }

//-----------------------------------------------------------
//CVIMPSTProcessArray::ResetPendingMsg
//-----------------------------------------------------------
// 
void CVIMPSTProcessArray::ResetPendingMsg(TInt aIndex)
    {
    if( aIndex < iItemArray.Count() && aIndex >= 0 )
        {
        MVIMPSTProcessArrayItem* item = (MVIMPSTProcessArrayItem *)iItemArray[aIndex];
        if(item)
            {   
            item->SetMsgPending( EFalse );
            }   
        }
    }
//-----------------------------------------------------------
//CVIMPSTProcessArray::ResetOpenConversationPendingMsg
//-----------------------------------------------------------
// 
void CVIMPSTProcessArray::ResetOpenConversationPendingMsg()
    {
    for( TInt index  = 0; index < iItemArray.Count() ;index++ )
        {
        MVIMPSTProcessArrayItem* item = (MVIMPSTProcessArrayItem *)iItemArray[index];
        if(item)
            {   
            item->SetConversationOpen( EFalse );
            item->SetMsgPending( EFalse );
            }   
        }
    }
// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::FindAnyContact
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
TBool CVIMPSTProcessArray::FindAnyContact( const TDesC& aContactId )
    {
    if(aContactId.Length() != 0)
        {
        MVIMPSTStorageContact* contact = iContactInterface->FindContactByUserId(aContactId);
        if( contact )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::FindAnyContactServiceField
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
TBool CVIMPSTProcessArray::FindAnyContactServiceField( const TDesC& aContactId )
    {
    if(aContactId.Length() != 0)
        {
        MVIMPSTStorageContact* contact = iContactInterface->FindContactByUserId(aContactId);
        if( contact )
            {
            return ETrue;
            }
        }
    return EFalse;
    }
// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::FindAndMarkContactAsOpenChat
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
TBool CVIMPSTProcessArray::FindAndMarkContactAsOpenChat( const TDesC& aContactId, TBool aMesssagePending )
    {
    TPtrC senderId = VIMPSTUtils::DisplayId( aContactId );
    TBool ret = EFalse;
    TInt count = iItemArray.Count();
    for( TInt i = 1; i < count; i++ )
        {
        MVIMPSTProcessArrayItem* contactItem = (MVIMPSTProcessArrayItem *)iItemArray[i];
        TPtrC contactId = VIMPSTUtils::DisplayId( contactItem->GetItemUserId() );
        if( senderId.Compare(  contactId ) == 0 )
            {
            contactItem->SetConversationOpen( ETrue );
            contactItem->SetMsgPending( aMesssagePending );
            ret = ETrue;
            break;
            }
        }
    return ret;
    }
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::IsUnreadMessageExists
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TBool CVIMPSTProcessArray::IsUnreadMessageExistsL(const TDesC& aRecipientId) 
    {
    TBool ret = EFalse;
    TInt unreaMsgCount = KErrNone;
    //Get IM SubService     
    MVIMPSTEngineSubService* subService =         
            (iEngine.SubService(TVIMPSTEnums::EIM));
    if(subService)
        {
        MVIMPSTEngineIMSubService& imSubService = 
        MVIMPSTEngineIMSubService::Cast (*subService);
        unreaMsgCount = imSubService.GetUnreadCountL( aRecipientId );
        }
    if( unreaMsgCount )
        {
        ret = ETrue;
        }
    return ret; 
    }
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::IsConversationExistL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TBool CVIMPSTProcessArray::IsConversationExistL(const TDesC& aRecipientId) 
    {
    //Get IM SubService     
    MVIMPSTEngineSubService* subService1 =    	   
                (iEngine.SubService(TVIMPSTEnums::EIM));
    if(subService1)
        {
        MVIMPSTEngineIMSubService& imSubService = 
                    MVIMPSTEngineIMSubService::Cast (*subService1);
        return imSubService.IsConversationExistL( aRecipientId );
        }
    return EFalse; 
    }
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::FindContactIndexL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TInt CVIMPSTProcessArray::FindContactIndexL( const TDesC& aContactId )
    {
    MVIMPSTStorageContact* newContact = iContactInterface->FindContactByUserId( aContactId );
    if(newContact)
        {
        // return the index of contact rather bool value..
        // 1 is for own data item.
        return (iContactListIndex + 1 + iContactListModel->IndexOfContact( newContact ));
        }
    return KErrNotFound;
    }


//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::FillUnknownContactsL
// ( Other items commented in header )
//------------------------------------------------------------------------------
void CVIMPSTProcessArray::FillUnknownContactsL()
    {
    TInt count = iUnKnownContactArray.Count();
    /* 
     * Index = 1 cos 0th index will be OwnData, and 1 - n where n is the number of friend requests, will be friend requests 
     */
    TInt index = 1; 
    for(TInt i = 0 ; i < count ; i++)
        {
        // all friend request item ownership is transfered to iItemArray
        CVIMPSTProcessUnknownContactItem* unknownItem = iUnKnownContactArray[i];
        unknownItem->SetItemIndex(index);        
        iItemArray.Insert(unknownItem, index);        
        index ++;
        }
    }

//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::FillAddRequestDataL
// ( Other items commented in header )
//------------------------------------------------------------------------------
void CVIMPSTProcessArray::FillAddRequestDataL()
    {
    TInt count = iAddRequestArray.Count();
	/* 
	 * Index = unknowncontacts are tops so after that 1 cos 0th index will be OwnData, and 1 - n where n is the number of friend requests, will be friend requests 
	 */
	TInt index = iUnKnownContactArray.Count() +1 ; 
    for(TInt i = 0 ; i < count ; i++)
        {
        // all friend request item ownership is transfered to iItemArray
        CVIMPSTProcessFriendRequestItem* requestItem = iAddRequestArray[i];
        requestItem->SetItemIndex(index );        
        iItemArray.Insert(requestItem, index);        
		index ++;
        }
    }

//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::RemoveUnKnownContacts
// ( Other items commented in header )
//------------------------------------------------------------------------------
void CVIMPSTProcessArray::RemoveUnKnownContacts()
    {
    TInt count = iUnKnownContactArray.Count();
    for(TInt i = 0 ; i < count ; i++)
        {
        CVIMPSTProcessUnknownContactItem* unknownItem = iUnKnownContactArray[i];
        TInt index = iItemArray.Find(unknownItem);
        if(index > -1)
            {            
            iItemArray.Remove(index);
            iItemArray.Compress();
            }       
        }
    }
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::RemoveAddRequestData
// ( Other items commented in header )
//------------------------------------------------------------------------------
void CVIMPSTProcessArray::RemoveAddRequestData()
    {
    TInt count = iAddRequestArray.Count();
    for(TInt i = 0 ; i < count ; i++)
        {
        CVIMPSTProcessFriendRequestItem* requestItem = iAddRequestArray[i];
        TInt index = iItemArray.Find(requestItem);
        if(index > -1)
            {            
            iItemArray.Remove(index);
            iItemArray.Compress();
            }       
        }
    
    }
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::LoginUserIdFromStoreL
// ( Other items commented in header )
//------------------------------------------------------------------------------

const TDesC& CVIMPSTProcessArray::LoginUserIdFromStoreL() const
    {
    if( iContactInterface )
		{
		// most of the time this get called 
		return iContactInterface->OwnContactL().UserId();
		}
    return KNullDesC;
    }

//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::AvatarIndex
// ( Other items commented in header )
//------------------------------------------------------------------------------

TInt CVIMPSTProcessArray::AvatarIndex(TInt aContactIndex)
    {
    TVIMPSTEnums::TItem itemtype = GetType( aContactIndex );
    /* Codescanner warning is ignored, since Bound check is done 
     * inside the GetType()   method*/

    if( TVIMPSTEnums::EContactItem == itemtype )
        {
       // if the type is a contact item then return the index
        MVIMPSTStorageContact* contact = iContactInterface->FindContactByUserId( GetItemUserId(aContactIndex) );
        if(contact)
            {
            // if any contact is found then return the index
            return contact->AvatarIndex();
            }
        }
    return KErrNone;
    }
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::SetAvatarIndex
// ( Other items commented in header )
//------------------------------------------------------------------------------

void CVIMPSTProcessArray::SetAvatarIndex( TInt aContactIndex ,TInt aAvatarIndex )
    {
    TVIMPSTEnums::TItem itemtype = GetType( aContactIndex );
    if( TVIMPSTEnums::EContactItem == itemtype )
        {
        // if the type is a contact item then set the index 
        MVIMPSTStorageContact* contact = iContactInterface->FindContactByUserId(GetItemUserId(aContactIndex));
        if(contact)
            {
            // if any contact is found then set the index
            contact->SetAvatarIndex( aAvatarIndex );
            }           
            
        iItemArray[aContactIndex]->SetAvatarIndex(aAvatarIndex);
        }

    }

//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::AvatarContent
// ( Other items commented in header )
//------------------------------------------------------------------------------

const TDesC8& CVIMPSTProcessArray::AvatarContent(TInt aContactIndex)
    {   
    TVIMPSTEnums::TItem itemtype = GetType( aContactIndex );

    if(  itemtype == TVIMPSTEnums::EContactItem )
        {
        // if the type is a contact item then get the avatar content from strorage
        MVIMPSTStorageContact* contact = iContactInterface->FindContactByUserId(GetItemUserId(aContactIndex));
        if(contact)
            {
            // if any contact is found then get the content
            return contact->AvatarContent();
            }
        }
    return KNullDesC8();
    }
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::OwnAvatarContentL
// ( Other items commented in header )
//------------------------------------------------------------------------------

const TDesC8& CVIMPSTProcessArray::OwnAvatarContentL( ) const
	{   
    MVIMPSTStorageContact& contact = iContactInterface->OwnContactL();             
    // if any contact is found then get the content
    return contact.AvatarContent();
	}
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::OwnAvatarIndexL
// ( Other items commented in header )
//------------------------------------------------------------------------------

TInt CVIMPSTProcessArray::OwnAvatarIndexL( )
    {
    MVIMPSTStorageContact& contact = iContactInterface->OwnContactL();  
    return contact.AvatarIndex();           
    }
//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::SetOwnAvatarIndexL
// ( Other items commented in header )
//------------------------------------------------------------------------------

void CVIMPSTProcessArray::SetOwnAvatarIndexL( TInt aAvatarIndex )
    {
    MVIMPSTStorageContact& contact = iContactInterface->OwnContactL();             
    contact.SetAvatarIndex( aAvatarIndex );
    }

//-----------------------------------------------------------------------------
// CVIMPSTProcessArray::FindAnyAddRequest
// ( Other items commented in header )
//------------------------------------------------------------------------------

TInt CVIMPSTProcessArray::FindAnyAddRequest( const TDesC& aRequesterId )
    {
    TInt count = iAddRequestArray.Count();
    for(TInt i=0; i<count; i++)
        {   
        if((aRequesterId.Compare(iAddRequestArray[i]->GetItemUserId()))==0 )                
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CVIMPSTProcessArray::GetSelectedItemIndex
// return contact index including friend request.
// -----------------------------------------------------------------------------
TInt CVIMPSTProcessArray::GetSelectedItemIndex(const TDesC& aContactId) 
	{
	TInt itemArrayCount = iItemArray.Count();
	for(TInt index=0; index<itemArrayCount; index++)
		{
		MVIMPSTProcessArrayItem* item = iItemArray[ index ];
		if(0 == aContactId.Compare(item->GetItemUserId()))
		    {
		    return index; 
		    }
		}
	return KErrNotFound;
	}

// --------------------------------------------------------------------------
// CVIMPSTProcessArray::IsContactBlocked
// --------------------------------------------------------------------------
TBool CVIMPSTProcessArray::IsContactBlocked(const TDesC& aUserId)
    {
    MVIMPSTEngineSubService* subService =          
                        (iEngine.SubService(TVIMPSTEnums::EPresence));
    if(subService)
        {
        MVIMPSTEnginePresenceSubService& presence = 
                MVIMPSTEnginePresenceSubService::Cast (*subService);
                
        RPointerArray<HBufC> *blockedlist = presence .GetBlockedList();
        
        if(blockedlist)
            {
            for(TInt i=0; i<blockedlist->Count(); ++i)
                {
                if(0 == aUserId.CompareC((*blockedlist)[i]->Des()))
                    {
                    return ETrue;
                    }
                }
            }
        }
    return EFalse;  
    }       


// --------------------------------------------------------------------------
// CVIMPSTProcessArray::GetFormattedString
// --------------------------------------------------------------------------
TPtrC16 CVIMPSTProcessArray::GetFormattedString(TInt aIndex)
    {
    if((aIndex >= 0) && (aIndex< iItemArray.Count()))
        return iItemArray[aIndex]->FormattedItemString();
    else
        return KNullDesC();
    }

//END OF FILE
