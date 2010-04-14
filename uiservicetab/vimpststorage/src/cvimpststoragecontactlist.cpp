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
* Description:  Contact list container implementation
*
*/



// INCLUDE FILES
#include "cvimpststoragecontactlist.h"
#include "vimpststorageutils.h"
#include "cvimpststoragecontactsorter.h"
#include "mvimpststoragecontact.h"
#include "mvimpststoragecontactsobserver.h"
#include "tvimpststoragepanics.h"
#include "uiservicetabtracer.h"
#include "vimpstdebugassert.h"
#include "cvimpststoragecontact.h"

#include <MVPbkContactLink.h> //vpbk

// ============================ MEMBER FUNCTIONS ===============================

// Compares two contacts
TInt CompareUserIdAlphabetically(
                           const MVIMPSTStorageContact& aFirst,
                           const MVIMPSTStorageContact& aSecond )
    {
    return VIMPSTStorageUtils::NeutralCompare( aFirst.UserId(), aSecond.UserId() );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::CVIMPSTStorageContactList
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContactList::CVIMPSTStorageContactList( CVIMPSTStorageContactSorter& aSorter ) 
: iSorter( aSorter )
    {
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContactList::ConstructL( const TDesC& aContactListId, 
                                 const TDesC& aDisplayName )
    {
	TRACER_AUTO;
    iListId = aContactListId.AllocL();  
    iDisplayName = aDisplayName.AllocL();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContactList* CVIMPSTStorageContactList::NewL(CVIMPSTStorageContactSorter& aSorter,
					                                      const TDesC& aContactListId, 
					                                      const TDesC& aDisplayName )
    {
	TRACER_AUTO;
    CVIMPSTStorageContactList* self = NewLC(  aSorter,
			                                  aContactListId, 
			                                  aDisplayName );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContactList* CVIMPSTStorageContactList::NewLC(CVIMPSTStorageContactSorter& aSorter,
					                                       const TDesC& aContactListId, 
					                                       const TDesC& aDisplayName )
    {
	TRACER_AUTO;
    CVIMPSTStorageContactList* self = new( ELeave ) CVIMPSTStorageContactList(aSorter );
    CleanupStack::PushL( self );
    self->ConstructL( aContactListId, aDisplayName );
    return self;
    }


// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::destructor
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContactList::~CVIMPSTStorageContactList()
    {
    iContactArray.ResetAndDestroy();
    iContactArray.Close();
    iOrderedContacts.Reset();
    delete iListId;
    delete iDisplayName;
    }
    
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::ListId
// -----------------------------------------------------------------------------
//
const TDesC& CVIMPSTStorageContactList::ListId() const
    {
    if( iListId )
        {
        return *iListId;
        }
    return KNullDesC;        
    }
    
       
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::DisplayName
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTStorageContactList::DisplayName() const
    {
    if( iDisplayName )
        {
        return *iDisplayName;
        }
    return KNullDesC();
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::AddStorageContactToCacheL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::AddStorageContactToCacheL( MVIMPSTStorageContact* aContact, TInt& aIndex )
    {
	TRACER_AUTO;
    
    if( !aContact )
	    {
	    return KErrArgument;	
	    }
    TPtrC userID = aContact->UserId();
    TRACE( " userId = %S", &userID ); 
    // optimize insert for inserting of reverse alphabetical order
    TInt idIndex( 0 );
    TInt count( iOrderedContacts.Count() );
    TRACE("count = %d",count ); 
    if( count && aContact->UserId().Length() &&
    	 ( 0 <= VIMPSTStorageUtils::NeutralCompare( aContact->UserId(),
    								  iOrderedContacts[ 0 ]->UserId() ) ) )
    	{
    TRACE( "inside if" );
    	// there are items and the contact should not be inserted to beginning
    	// => find the correct place
	    TLinearOrder< MVIMPSTStorageContact > userIdOrder( *CompareUserIdAlphabetically );
	    if( KErrNone == iOrderedContacts.FindInOrder( aContact, idIndex, userIdOrder ) )
    	    {
	    TRACE( "contact already exists." ); 
            // this contact already exists, return it
            return KErrAlreadyExists;
            }
    	}
    TRACE( "inserting the contact" ); 
    // the position is now correct, insert the contact
    iOrderedContacts.InsertL( aContact, idIndex );

    // insert also to list sorted by contact "identification"
    TInt err( KErrNone );
    count = iContactArray.Count();  
    TRACE( "iContactsArray count %d",count ); 
    TLinearOrder< MVIMPSTStorageContact >& order = iSorter.InsertOrder();
    if( count &&  0 > (*order)( aContact, iContactArray[ 0 ] ) )
    	{
    	// the item should be inserted in the beginning of the array
		err = iContactArray.Insert( aContact, 0 );
    	}
    else
       	{
       	// insert in correct position
       	err = iContactArray.InsertInOrderAllowRepeats( aContact, order );
       	}
    TRACE( "iContactsArray inserting error %d",err ); 
    if( err != KErrNone )
        {
        // appending to second array did not succeed, so remove from first and leave
        iOrderedContacts.Remove( idIndex );
        User::Leave( err );
        }
    aIndex = idIndex;
    return err;
    }
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::RemoveContactFromCacheL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::RemoveContactFromCacheL( const TDesC& aContactId, TInt& aIndex )
    {
	TRACER_AUTO;
    TInt orderedIndex(0);
    TInt contactIndex = FindContactIndex( aContactId, orderedIndex );
	aIndex = contactIndex;
    if( KErrNotFound != contactIndex )
        {
        delete iContactArray[ contactIndex ];
        iContactArray.Remove( contactIndex );
        iOrderedContacts.Remove( orderedIndex );
        iContactArray.Compress();
        iOrderedContacts.Compress();
        return KErrNone;
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::RemoveContactFromCacheL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::RemoveContactFromCacheL( const MVPbkContactLink& aContactLink, TInt& index )
    {
	TRACER_AUTO;
    TInt error = KErrNotFound;
    TInt orderedIndex(0);
    TInt contactIndex = FindContactByLinkL( aContactLink, orderedIndex );
	index = contactIndex;
    if( KErrNotFound != contactIndex )
        {
        MVIMPSTStorageContact* contact = iContactArray[ contactIndex ];
        iContactArray.Remove( contactIndex );
        iOrderedContacts.Remove( orderedIndex );
        delete contact;
        iContactArray.Compress();
        iOrderedContacts.Compress();
        error = KErrNone;
        }
    return error;
    }    
 // -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContact
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::ContactCount( TBool aSkipOfflineContacts,
						   TBool aSkipBlocekedContacts ) const
    {
	TRACER_AUTO;
    if( !aSkipOfflineContacts && !aSkipBlocekedContacts )
        {
        return iContactArray.Count();
        }
    TInt countOfContacts( 0 );
    TInt count( iContactArray.Count() );
    for( TInt a( 0 ); a < count; ++a )
        {
        TVIMPSTEnums::TOnlineStatus status(
                                    iContactArray[ a ]->OnlineStatus() );
        if( status == TVIMPSTEnums::EOnline ||
            status == TVIMPSTEnums::EAway ||
            status == TVIMPSTEnums::EBusy ||
            status == TVIMPSTEnums::EDoNotDisturb )
            {
	        ++countOfContacts;
            }
        }
    return countOfContacts;
    }
            
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContactIndex
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::FindContactIndex( const TDesC& aUserId,
                                       TInt& aOrderedIndex ) const
    {
	TRACER_AUTO;
     TInt low( 0 );
    TInt high( iOrderedContacts.Count() );
    while( high > low )
        {
        TInt m( ( low + high ) / 2 );
        TInt compare( VIMPSTStorageUtils::NeutralCompare( aUserId, iOrderedContacts[ m ]->UserId() ) );
        if ( compare == KErrNone )
            {
            aOrderedIndex = m;
            return FindContactEntry( iOrderedContacts[ m ] );
            }
        else if ( compare > 0 )
            {
            low = m + 1;
            }
        else
            {
            high = m;
            }
        }
    aOrderedIndex = KErrNotFound;
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContactEntry
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::FindContactEntry( const MVIMPSTStorageContact* aContact ) const
    {
    TInt index( iContactArray.Find( aContact ) );
    __ASSERT_DEBUG( index != KErrNotFound , Panic( EContactsArrayOutOfSync ));
    return index;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::Sort
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContactList::Sort()
    {
    iSorter.Sort( iContactArray );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::ResortContact
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContactList::ResortContact( MVIMPSTStorageContact* aContact )
    {
	TRACER_AUTO;
    // let it panic, if index is not found, which would be bad
    TInt index( iContactArray.Find( aContact ) );
    __ASSERT_DEBUG( index != KErrNotFound , Panic( ESortingCorupted ));
    // Check if resort is even needed for the contact
    TLinearOrder< MVIMPSTStorageContact >& order = iSorter.InsertOrder();
    if(
        (
          ( index == 0 )
          ||
          ( 0 >= (*order)( iContactArray[ index - 1], aContact ) )
        )
        &&
        (
          ( index == ( iContactArray.Count() - 1 ) )
          ||
          ( 0 <= (*order)( iContactArray[ index + 1], aContact ) )
        )
      )
        {
        // the item is in correct position
        // => no need to resort
        return;
        }
    
    iContactArray.Remove( index );
    // this should alway succeed, since we did not actually add anything to array
    // no need to add contact to the ordered array, should be there already
    TInt err( iContactArray.InsertInOrderAllowRepeats( aContact, order ) );
    __ASSERT_DEBUG( err == KErrNone , Panic( ESortingCorupted ));
    }
   
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindIndexOfContact
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::FindIndexOfContact( const MVIMPSTStorageContact* aContact,
                            TVIMPSTEnums::TFilterType aFilter  ) const
    {    
	TRACER_AUTO;
    TInt count( iContactArray.Count() ); 
    if ( aFilter == TVIMPSTEnums::EFilterAll )
		{
        for( TInt a( 0 ); a < count; ++a )
            {
            const MVIMPSTStorageContact* contact = iContactArray[ a ];
            if( aContact == contact )
                {
                return a;
                }
            }		
		}
	else
	    {
	    TInt filteredIndex( -1 );
	    for( TInt i( 0 ); i < count; ++i )
            {
            MVIMPSTStorageContact* contact ( iContactArray[i] );
            if ( FilterAllowsContact( contact, aFilter ) )
                {
                // contact was in the correct state
                filteredIndex++; 
                if( contact == aContact)
                    {
                    // Contact was found, return the index
                    return filteredIndex;
                    }                  
                }        
            }    
	    }
    return KErrNotFound;
    }
       
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::Count
// From MVIMPSTStorageContactList
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::Count() const
    {
    return ContactCount( EFalse, EFalse );
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::OnlineCount
// From MVIMPSTStorageContactList
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::OnlineCount() const
    {
    return FilteredCount( TVIMPSTEnums::EFilterNonOffline );
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::operator[]
// From MVIMPSTStorageContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& CVIMPSTStorageContactList::operator[]( TInt aIndex ) const
    {
	TRACER_AUTO;
    __CHAT_ASSERT_DEBUG( aIndex < iContactArray.Count() )
    return *iContactArray[ aIndex ];
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::OnlineContact
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& CVIMPSTStorageContactList::OnlineContact( TInt aIndex ) const
    {
    return FilteredContact( 
                aIndex, TVIMPSTEnums::EFilterNonOffline );
    }
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FilteredContact
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& CVIMPSTStorageContactList::FilteredContact( 
                TInt aIndex, TVIMPSTEnums::TFilterType aFilter ) const
	{
	TRACER_AUTO;
    if ( aFilter == TVIMPSTEnums::EFilterAll )
		{
		return (*this)[aIndex];
		}
	else
        {      
        TInt filteredIndex( -1 );
        TInt count( iContactArray.Count() );
        for( TInt i(0); i < count; ++i )
            {
            MVIMPSTStorageContact* contact = iContactArray[i];
            if ( FilterAllowsContact( contact, aFilter ) )
                {
                // Contact was in correct state
                filteredIndex++;
                if ( aIndex == filteredIndex )
                    {
                    // index was also correct
                    return *iContactArray[i];                   
                    } 
                }
            }
	    __CHAT_ASSERT_DEBUG( EFalse );
        // prevent compiler warning
        return ( MVIMPSTStorageContact& )KNullDesC;
        }
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FilteredCount
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::FilteredCount( 
                            TVIMPSTEnums::TFilterType aFilter ) const
	{
	TRACER_AUTO;
    if ( aFilter == TVIMPSTEnums::EFilterAll )
		{
		return Count();
		}
	TInt filtered( 0 );
	TInt count( iContactArray.Count() );
	for( TInt i( 0 ); i<count; ++i )
		{
		if ( FilterAllowsContact( iContactArray[i], aFilter ) )
			{
			filtered++;                
			}
		}
	return filtered;
  	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FilterAllowsContact
// -----------------------------------------------------------------------------
//
TBool CVIMPSTStorageContactList::FilterAllowsContact( const MVIMPSTStorageContact* aContact,
                             TVIMPSTEnums::TFilterType aFilter ) const
    {
	TRACER_AUTO;
    TBool showContact( EFalse );
    TInt myStatus( KErrNone );          
    TVIMPSTEnums::TOnlineStatus status(
                            aContact->OnlineStatus() );                                          

    if( aFilter & TVIMPSTEnums::EFilterAndOperation )
        {                
        myStatus = myStatus | TVIMPSTEnums::EFilterAndOperation;
        }
     
    if ( status == TVIMPSTEnums::EOnline )
        {
        myStatus = myStatus | TVIMPSTEnums::EFilterOnline;
        }

    if ( status == TVIMPSTEnums::EOnline || 
         status == TVIMPSTEnums::EAway ||
         status == TVIMPSTEnums::EBusy ||
         status == TVIMPSTEnums::EDoNotDisturb)
        {
        myStatus = myStatus | TVIMPSTEnums::EFilterNonOffline;
        }

    if ( status == TVIMPSTEnums::EAway)
        {
        myStatus |= TVIMPSTEnums::EFilterAway;
        }
    
    if( aFilter & TVIMPSTEnums::EFilterAndOperation )
        {
        if( ( myStatus & aFilter ) == aFilter )
            {
            showContact = ETrue;
            }
        }
    else
        {
        if( myStatus & aFilter )
            {
            showContact = ETrue;
            }
        } 
       
    return showContact;   
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContact
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* CVIMPSTStorageContactList::FindContact( const TDesC& aContactId )
    {
	TRACER_AUTO;
    TInt orderedIndex(0);
    TInt contactIndex = FindContactIndex( aContactId, orderedIndex );
    return ( contactIndex >= 0 ? iContactArray[ contactIndex ] : NULL );
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContactByContactLinkL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* CVIMPSTStorageContactList::FindContactByContactLink( const MVPbkContactLink& aContactLink )
    {
	TRACER_AUTO;
    TInt orderedIndex(0);
    TInt contactIndex = -1;
    TRAP_IGNORE( contactIndex = FindContactByLinkL( aContactLink, orderedIndex );)
    return ( contactIndex >= 0 ? iContactArray[ contactIndex ] : NULL );
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContactByLinkL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageContactList::FindContactByLinkL( const MVPbkContactLink& aContactLink,
                                          TInt& aIndexOrderedArray ) const
    {
	TRACER_AUTO;
    TInt count( iOrderedContacts.Count() );
    TInt contactIndex = KErrNotFound;
     //Does a Linear Search
    for (TInt index = 0; index<count; index++)
		{
		MVPbkContactLink* contactLink = iOrderedContacts[ index ]->ContactLink();
		if( contactLink && contactLink->IsSame( aContactLink ) )
			{
			aIndexOrderedArray = index;
		    contactIndex =  FindContactEntry( iOrderedContacts[ index ] );
		    break;	
			}
		} 
    return contactIndex;
    }        
    
//  End of File
