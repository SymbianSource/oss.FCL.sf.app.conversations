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
* Description:  Contact sorter
*
*/


// INCLUDE FILES
#include    "cvimpststoragecontactsorter.h"
#include    "mvimpststoragecontact.h"
#include    "tvimpstenums.h"
#include    "cvimpststoragedefs.h"
#include "uiservicetabtracer.h"

// The order of contacts if sorted by status
const TInt KConstOnline  = 1;
const TInt KConstBusy	= 2;
const TInt KConstAway    = 3;
const TInt KConstServiceOut = 4;
const TInt KConstOnPhone = 5;
const TInt KConstInvisible = 6;
const TInt KConstOffline = 7;
const TInt KConstCallForward = 8;
const TInt KConstUnknown = 9;

// local functions 
// sorting algorithms, you can add new one to extend functionality


// Compares alphabetically using MVIMPSTStorageContact::Identification and 
// TDesC::CompareC
TInt CompareAlphabetically( const MVIMPSTStorageContact& aFirst, 
                           const MVIMPSTStorageContact& aSecond )
    {
	TRACER_AUTO;
    return aFirst.Name().CompareC( aSecond.Name(),
                                             KCollationLevel, NULL );
    }


// Group the contact by it's status. 
TInt OrderContact( const MVIMPSTStorageContact& aContact )
    {
	TRACER_AUTO;
     // Contact is not blocked, group by presence status
      switch( aContact.OnlineStatus() )
            {
            case TVIMPSTEnums::EOnline:
                {
                return KConstOnline;  // First group, contact is online
                }
            case TVIMPSTEnums::EDoNotDisturb:
			case TVIMPSTEnums::EBusy:
                {
                return KConstBusy;  // second group, contact is busy/donotdisturb
                }
			case TVIMPSTEnums::EOnPhone:
                {
                return KConstOnPhone;  //fifth group, contact is onphone
                }
			case TVIMPSTEnums::EInvisible:
                {
                return KConstInvisible;  // sixth group, contact is invisible
                }
            case TVIMPSTEnums::EAway:
                {
                return KConstAway;    // third group, contact is away
                }
            case TVIMPSTEnums::EOffline:
                {
                return KConstOffline; // seventh group, contact is offline
                }
			case TVIMPSTEnums::ECallForward:
			    {
			    return KConstCallForward; // eight group, contact state is callforward
			    }
            case TVIMPSTEnums::EServiceOut:
                {
                return KConstServiceOut;  // fourth group, contact is serviceout(has ony pstn or msisdn number)
                }
            default:
                {
                return KConstUnknown; //  unknown status
                }
            }
    }

// Compares by contact status, -1 = aFirst is greater, 1 = aSecond is greater.
TInt CompareByPresence( const MVIMPSTStorageContact& aFirst, 
                        const MVIMPSTStorageContact& aSecond )
    {
	TRACER_AUTO;
    TInt firstGroup( OrderContact( aFirst ) );
    TInt secondGroup( OrderContact( aSecond ) );

    // If both contacts belong to same group, sort alphabetically
    if( firstGroup == secondGroup )
        {
        return CompareAlphabetically( aFirst, aSecond );
        }
    
    // Otherwise return the difference of groups
    return firstGroup - secondGroup;
    }

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CVIMPSTStorageContactSorter* CVIMPSTStorageContactSorter::NewL() 
    {
	TRACER_AUTO;
    CVIMPSTStorageContactSorter* self = new( ELeave ) CVIMPSTStorageContactSorter();
    return self;
    }

// Destructor
CVIMPSTStorageContactSorter::~CVIMPSTStorageContactSorter()
    {
	TRACER_AUTO;
    }

// ---------------------------------------------------------
// CVIMPSTStorageContactSorter::Sort( RPointerArray<MVIMPSTStorageContact>& aList )
// Sorts given array.
// ---------------------------------------------------------
//
void CVIMPSTStorageContactSorter::Sort( RPointerArray< MVIMPSTStorageContact >& aList )
    {
	TRACER_AUTO;
    aList.Sort( iAlgorithm );
    }

// ---------------------------------------------------------
// CVIMPSTStorageContactSorter::Compare( const MVIMPSTStorageContact& aFirst, 
//                                  const MVIMPSTStorageContact& aSecond ) const
// Get function pointer from inside of TLinearOrder and 
// execute the function.
// ---------------------------------------------------------
//
TInt CVIMPSTStorageContactSorter::Compare( const MVIMPSTStorageContact& aFirst, 
                                    const MVIMPSTStorageContact& aSecond ) const
    {
	TRACER_AUTO;
    return iAlgorithm.operator TGeneralLinearOrder()( &aFirst, &aSecond );
    }

// C++ default constructor can NOT contain any code, that
// might leave.
// Default algorithm is alphabetic. It is changed later in ConstructL.
//
CVIMPSTStorageContactSorter::CVIMPSTStorageContactSorter() : 
    iAlgorithm( *CompareByPresence )
    {
    }
    
// ---------------------------------------------------------
// CVIMPSTStorageContactSorter::SetSortAlgorithm()
// Get setting item value and choose algorithm.
// New algorithms can be added to this method.
// ---------------------------------------------------------
//
void CVIMPSTStorageContactSorter::SetSortAlgorithm( 
                            MVIMPSTStorageContact::TSortAlgorithm aAlgorithm )
    {    
	TRACER_AUTO;
    switch( aAlgorithm )
            {
        case MVIMPSTStorageContact::ECompareByPresence: 
            {
            iAlgorithm = 
                  TLinearOrder< MVIMPSTStorageContact > ( *CompareByPresence );
            break;
            }

        case MVIMPSTStorageContact::ECompareAlphabetically: // flowthrough
        default : 
            {
            iAlgorithm = 
                  TLinearOrder< MVIMPSTStorageContact > ( *CompareAlphabetically );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CVIMPSTStorageContactSorter::InsertOrder()
// Get setting item value and choose algorithm.
// New algorithms can be added to this method.
// ---------------------------------------------------------
//    
TLinearOrder< MVIMPSTStorageContact >& CVIMPSTStorageContactSorter::InsertOrder()
    {
    return iAlgorithm;
    }

// End of file
