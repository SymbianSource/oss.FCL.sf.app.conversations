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
* Description:  Custom listbox model for the contact list
*
*/


#include "cvimpstuilistboxmodel.h"
#include "mvimpstprocessarray.h"
#include "uiservicetabtracer.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::CVIMPSTUiListBoxModel
// ---------------------------------------------------------------------------
//
CVIMPSTUiListBoxModel::CVIMPSTUiListBoxModel()
    {
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::~CVIMPSTUiListBoxModel
// ---------------------------------------------------------------------------
//
CVIMPSTUiListBoxModel::~CVIMPSTUiListBoxModel()
    {
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::NumberOfItems
// ---------------------------------------------------------------------------
//
TInt CVIMPSTUiListBoxModel::NumberOfItems() const
    {
	TRACER_AUTO;
    if( IsFilterActive() )
        {
        // Something is filtered
        return CountItems();
        }
    if( iItemTextArray )
        {
        // Fast
        return iItemTextArray->MdcaCount();
        }

    return 0;
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::ItemText
// ---------------------------------------------------------------------------
//
TPtrC CVIMPSTUiListBoxModel::ItemText( TInt aItemIndex ) const
    {
    if( IsFilterActive() )
        {
        // Slow
        return ItemAt( aItemIndex );
        }

    // Fast
    return iItemTextArray->MdcaPoint( aItemIndex );
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::FilteredItemIndex
// ---------------------------------------------------------------------------
//
TInt CVIMPSTUiListBoxModel::FilteredItemIndex( TInt aVisibleItemIndex ) const
    {
	TRACER_AUTO;
    if( !IsFilterActive() )
        {
        // Nothing is filtered
        return aVisibleItemIndex;
        }
      if( Filter() )
        {
        // Get positions of lists
        RArray<TInt> pos;
       // iItemModel->MapContactListPositions( pos );
               
        // Count visible items until we reach the wanted one
        TInt itemcount = -1;
        TInt count = iItemTextArray->MdcaCount();
        for( TInt i = 0; i < count; ++i )
            {
            if( ( pos.Find( i ) != KErrNotFound && 
                IsContactList( i, pos ) ) ||
                Filter()->VisibleItemIndex( i ) != KErrNotFound )
                {                
                // this item is either visible by filter or
                // it's our dynamically added contact list
                itemcount++;
                if( itemcount == aVisibleItemIndex )
                    {
                    pos.Close();
                    return i;
                    }
               }
            }
        pos.Close();
        } 
    return 0;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::VisibleItemIndex
// ---------------------------------------------------------------------------
//
TInt CVIMPSTUiListBoxModel::VisibleItemIndex( TInt aOriginalIndex ) const
    {
	TRACER_AUTO;
    if( !IsFilterActive() )
        {
        // Nothing is filtered
        return aOriginalIndex;
        }
    
    // Get positions of contact lists
    RArray<TInt> pos;
    //iItemModel->MapContactListPositions( pos );
    
    // Check if it's even visible
    TInt index = Filter()->VisibleItemIndex( aOriginalIndex );
    TBool visibleList = IsContactList( aOriginalIndex, pos );
    if( index == KErrNotFound && !visibleList )
        {
        pos.Close();
        return KErrNotFound;
        }
    
    // Count up to original index to find out the visible position
    TInt visibleIndex = -1;
    for( TInt i = 0; i <= aOriginalIndex; ++i )
        {
        if( ( pos.Find( i ) != KErrNotFound && 
            IsContactList( i, pos ) ) ||
            Filter()->VisibleItemIndex( i ) != KErrNotFound )
            {
            // this item is either visible by filter or
            // it's our dynamically added contact list
            visibleIndex++;
            }
        }
    pos.Close();
    return visibleIndex;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::CountItems
// ---------------------------------------------------------------------------
//
TInt CVIMPSTUiListBoxModel::CountItems() const
    {
	TRACER_AUTO;
    TInt itemcount = 0;
    if( Filter() )
        {
        // Count of visible items by filter
        itemcount = Filter()->FilteredNumberOfItems();
        if( itemcount == 0 )
            {
            return itemcount;
            }

        // Get positions of contact lists
        RArray<TInt> pos;
        //iItemModel->MapContactListPositions( pos );
                       
        // Scan through list positions and count the additional visible items.
        TInt count = pos.Count();
        for( TInt i = 0; i < count; ++i )
            {
            if( IsContactList( pos[i], pos ) && 
                Filter()->VisibleItemIndex( pos[i] ) == KErrNotFound )
                {
                // List should be visible because it has found contacts in it
                // but it's filtered out by searchfield. Add it dynamically.
                itemcount++;
                }
            }
        pos.Close();
        }
    return itemcount;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::ItemAt
// ---------------------------------------------------------------------------
//
TPtrC CVIMPSTUiListBoxModel::ItemAt( TInt aIndex ) const
    {
	TRACER_AUTO;
    TInt itemcount = -1;
    if( Filter() )
        {
        // Get positions of contact lists
        RArray<TInt> pos;       
        
        // Scan through visible items until we reach the wanted one
        TInt count = iItemTextArray->MdcaCount();
        for( TInt i = 0; i < count; ++i )
            {
            if( ( pos.Find( i ) != KErrNotFound && 
                IsContactList( i, pos ) ) ||
                Filter()->VisibleItemIndex( i ) != KErrNotFound )
                {
                // this item is either visible by filter or
                // it's our dynamically added contact list
                itemcount++;
                if( itemcount == aIndex )
                    {
                    // Found it
                    pos.Close();
                    return iItemTextArray->MdcaPoint( i );
                    }
                }
            }
        pos.Close();
        }
    return KNullDesC();
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::IsContactList
// ---------------------------------------------------------------------------
//
TBool CVIMPSTUiListBoxModel::IsContactList( TInt aIndex, RArray<TInt>& aPosArray ) const
    {
	TRACER_AUTO;
    TInt startInd = aPosArray.Find( aIndex );
    if( KErrNotFound == startInd )
        {
        // aIndex not found in array -> not even list
        return EFalse;
        }

    TInt count = aPosArray.Count();
    TInt end = 0;
    if( startInd >= count - 1 )
        {
        // aIndex is the last list:
        // Search from aIndex to last item
        end = iItemTextArray->MdcaCount();
        }
    else
        {
        // Search from aInded to next item in array
        end = aPosArray[ startInd + 1 ];
        }
    
    // Try to find visible contacts for this list
    for ( TInt i = aIndex + 1; i < end; ++i )
        {
        if( Filter()->VisibleItemIndex( i ) != KErrNotFound )
            {
            // found one
            return ETrue;
            }
        }
    // not found
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::IsFilterActive
// ---------------------------------------------------------------------------
//
TBool CVIMPSTUiListBoxModel::IsFilterActive() const
    {
    if( Filter() )
        {
        return iItemTextArray->MdcaCount() != 
               Filter()->FilteredNumberOfItems();
        }
    return EFalse;
    }
// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::MatchableTextArray
// ---------------------------------------------------------------------------
//
const MDesCArray* CVIMPSTUiListBoxModel::MatchableTextArray() const
    {
    return this;
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::MdcaCount
// ---------------------------------------------------------------------------
//
TInt CVIMPSTUiListBoxModel::MdcaCount() const
    {
    return iItemModel->Count();
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::MdcaPoint
// ---------------------------------------------------------------------------
//
TPtrC CVIMPSTUiListBoxModel::MdcaPoint( TInt aIndex ) const
    {
    return iItemModel->GetItemNameText( aIndex );
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiListBoxModel::SetContactListModel
// ---------------------------------------------------------------------------
//
void CVIMPSTUiListBoxModel::SetContactListModel( MVIMPSTProcessArray& 
                                                  aContactListModel )
    {
    iItemModel = &aContactListModel;
    }

// End of file
