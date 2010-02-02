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
* Description:  Interface for presence context.
*
*/


#ifndef S_VIMPSTSTORAGESERVICEVIEW_H
#define S_VIMPSTSTORAGESERVICEVIEW_H

#include "mvimpststorageserviceview.h"

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

class MVIMPSTStorageContact;
class vimpstserviceview_stub;

class vimpstserviceview_stub : public MVIMPSTStorageServiceView
    {
    public :
        static MVIMPSTStorageServiceView* NewL();
        void AddObserverL( MVIMPSTStorageContactsObserver* aObserver ) ;

        void RemoveObserver( MVIMPSTStorageContactsObserver* aObserver );


        MVIMPSTStorageContact* FindContactByUserId( const TDesC& aUserId );


        TInt ListCount() const;

         MVIMPSTStorageContactList& ListAt( TInt aIndex ) const ;


         MVIMPSTStorageContactList* FindContactList( const TDesC& aListId );

         MVIMPSTStorageContactList* CreateContactListL( const TDesC& aContactListId, 
                const TDesC& aDisplayName ) ;

         void RemoveContactList( const TDesC& aContactListId );


         TInt ContactCount(TBool aSkipOfflineContacts = EFalse ) const;


        
         MVIMPSTStorageContact* UpdatePresenceL(const TDesC& aContactId, 
                TVIMPSTEnums::TOnlineStatus aStatus, 
                const TDesC& aStatusText,
                const TDesC8& aAvatarData,
                TBool aIsClearingAvatar = EFalse) ;

        MVIMPSTStorageContact* UpdateAvatarL(const TDesC& aContactId,
                const TDesC8& aAvatarData );

         MVIMPSTStorageContact* CreateNewContactL(const TDesC& aUserId, 
                const TDesC& aDisplayName = KNullDesC,
                TBool aIsInvitationItem = EFalse ,
                TBool aInvitationAutoAccept = EFalse);

        TInt CreateNewContactFromRetrivedIdL(  TInt aIndexToUse  );

         TInt DeleteNewContactFromRetrivedIdL(  TInt aIndexToUse  );

         TInt RemoveContactL( MVIMPSTStorageContact* aContact  );


         void CreateNewFetchContactsL( RArray <TPtrC> &aFirstNameList, 
                RArray <TPtrC> &aServiceField );


         TBool  IsLocalStore() const;


         TInt RetriveLinkXSPIdsL(const TDesC8& aContactPackLink );
         const TDesC& GetRetrieveXSPIdL(TInt aIndex );

         MVIMPSTStorageContact* FindContactByLink(const MVPbkContactLink& aContactLink );


         void Sort( const TDesC& aContactListId  = KNullDesC );

         MVIMPSTStorageContact& OwnContactL() ;

         void DeleteDatabaseL() ;

         void SetOwnUserIdL(const TDesC& aUserId ) ; 

         void SetUnnamedTextL(HBufC* aUnnamedText );

   
    };




#endif // S_VIMPSTSTORAGESERVICEVIEW_H



// End of File
