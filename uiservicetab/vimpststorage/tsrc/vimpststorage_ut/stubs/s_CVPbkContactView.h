/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
:                
*
*/



#ifndef SCVPBKCONTACTVIEW_H
#define SCVPBKCONTACTVIEW_H

// INCLUDES
#include <e32base.h>
#include <MVPbkContactView.h>


// CLASS DECLARATIONS

/**
 * An interface for contact store views.
 *
 * The view contains contacts from a single contact store.
 * MVPbkObjectHierarchy interface can be used to navigate to the store.
 */
class CVPbkContactViewStub : public CBase, 
        public MVPbkContactView
    {
    public:  // Destructor
        /**
         * Destructor.
         */
        ~CVPbkContactViewStub() { };
        
        CVPbkContactViewStub()  { };
   		
   		/**
         * Returns the parent object of this object. For the root of the 
         * hierarchy returns self.
         * @return The parent object.
         */
        MVPbkObjectHierarchy& ParentObject() const;


        /**
         * Returns type of this contact view.
         *
         * @return A contact view type.
         */
        TVPbkContactViewType Type() const ;

        /**
         * Changes sort order of the view asynchronously.
         *
         * Clients of this view will get an event via MVPbkContactViewObserver
         * interface when the order has been updated. The leaf view sends
         * a pair of events. First it sends ContactViewUnavailable and then
         * after the order has been changed ContactViewReady. However if
         * the view is of type EVPbkCompositeView the client doesn't necessary
         * receive ContactViewUnavailable at all because it might be that
         * there is always some subview ready in the composite.
         *
         * @param aSortOrder A new sort order for this view.
         * @exception KErrArgument Possible reasons: a client tries to change
         *                         a sort order of platform defined shared
         *                         view against the platform setting.
         * @see CVPbkSortOrder
         */
        void ChangeSortOrderL(
                const MVPbkFieldTypeList& aSortOrder ) ;

        /**
         * Returns the current sort order of the view.
         *
         * The sort order is a sub set of master field types from
         * CVPbkContactManager.
         *
         * @return The sort order of the view.
         */
        const MVPbkFieldTypeList& SortOrder() const ;

        /**
         * Refreshes the view contents asynchronously.
         * 
         * All handles to this view's contacts are invalidated.
         * Clients of this view will get an event via MVPbkContactViewObserver
         * interface when the view has been refreshed.
         */
        void RefreshL() ;
        
        /**
         * Returns the number of contacts in this view.
         *
         * @return The number of contacts.
         */
        TInt ContactCountL() const ;

        /**
         * Returns a contact in this view. 
         *
         * The returned reference may be invalidated when this 
         * function is called again. For that reason clients should prefer
         * not to save references to the contacts retrieved from this function.
         *
         * @param aIndex An index of the contact in this view.
         * @return A reference to a contact in this view at aIndex.
         * @precond aIndex >
         *              VPbkError::Panic(VPbkError::EInvalidContactIndex)
         *              is raised if the precondition does not hold.
         * @exception KErrArgument if aIndex >= ContactCountL()
         */
        const MVPbkViewContact& ContactAtL(
                TInt aIndex ) const ;

        /**
         * Creates and returns a link that points to contact at aIndex.
         *
         * NOTE: If the view contact is not from any store it can return
         *       also NULL. E.g. if the view contact is a folding 
         *       contact view that it's not saved to any store.
         *       The NULL is not pushed onto the cleanup stack.
         *
         * @param aIndex An index of the contact for which the link
         *               is created.
         * @return A new link object pointing to contact at aIndex or NULL if
         *          the link can't be created (e.g. a folder).
         * @precond aIndex >
         *          VPbkError::Panic(VPbkError::EInvalidContactIndex)
         *          is raised if the precondition does not hold.
         * @exception KErrArgument if aIndex >= ContactCountL()
         */
        MVPbkContactLink* CreateLinkLC(
                TInt aIndex ) const ;

        /**
         * Returns The index of the aContactLink in this view.
         *
         * If the identifier is not found from the view then
         * KErrNotFound is returned. If the view is not from
         * any store (e.g. a folding view) then it will also
         * return KErrNotFound.
         *
         * NOTE: the implementation of this function probably
         *       must loop the view which means that calling
         *       this function frequently or in a loop can
         *       be slow when there are lots of contacts.
         *
         * @param aContactLink A link whose index is searched for.
         * @return The index of the link or KErrNotFound.
         */
        TInt IndexOfLinkL(
                const MVPbkContactLink& aContactLink ) const ;

        /**
         * Adds an observer to this contact view asynchronously. 
         *
         * The observer will be notified after it has been added
         * to the view.
         *
         * @param aObserver A new observer for the view.
         */
        void AddObserverL(
                MVPbkContactViewObserver& aObserver ) ;

        /**
         * Removes an observer from this contact view.
         *
         * This method can be called even if aObserver has not been
         * previously added. In that case, no observers are removed.
         *
         * @param aObserver The observer to be removed.
         */
        void RemoveObserver(
                MVPbkContactViewObserver& aObserver ) ;
        
        /**
         * Returns ETrue if this view is from a store identified 
         * by aContactStoreUri.
         *
         * @param aContactStoreUri The whole URI of the contact store.
         * @return ETrue if the view was from the given store.
         *
         * @see TVPbkContactStoreUriPtr::UriDes
         */
        TBool MatchContactStore(
                const TDesC& aContactStoreUri ) const ;
        
        /**
         * Returns ETrue if this view is from a store domain identified 
         * by aContactStoreDomain.
         *
         * @param aContactStoreDomain The domain part of the contact store URI.
         *                            The domain can be retrieved from the 
         *                            whole contact store URI using class 
         *                            TVPbkContactStoreUriPtr and 
         *                            EContactStoreUriStoreType.
         *                            An implementation compares the
         *                            EContactStoreUriStoreType part of 
         *                            its own URI to aContactStoreDomain.
         *                            
         * @return ETrue if the view was from the same store domain.
         * @see TVPbkContactStoreUriPtr
         */
        TBool MatchContactStoreDomain(
                const TDesC& aContactStoreDomain ) const ;
        
        /**
         * Creates and returns a bookmark that points to the
         * view contact at aIndex.
         *
         * @param aIndex An index of the contact in the view.
         * @return A new bookmark to the view item.
         * @precond aIndex >
         *          VPbkError::Panic(VPbkError::EInvalidContactIndex)
         *          is raised if the precondition does not hold.
         */
        MVPbkContactBookmark* CreateBookmarkLC(
                TInt aIndex ) const ;

        /**
         * Returns an index of the contact in the view.
         *
         * If the identifier is not found from the view then
         * KErrNotFound is returned.
         *
         * NOTE: the implementation of this function probably
         *       must loop the view which means that calling
         *       this function frequently or in a loop can
         *       be slow when there are lots of contacts.
         *
         * @param aContactBookmark A bookmark that identifies
         *                         a contact in the view.
         * @return An index of the contact in the view or KErrNotFound.
         */
        TInt IndexOfBookmarkL(
                const MVPbkContactBookmark& aContactBookmark ) const ;
        
        /**
         * Returns an interface for text based contact filtering support.
         *
         * If the view doesn't support filtering then this returns NULL. 
         * Filtering must be supported in all views created from a contact
         * store. However, it's possible to implement a view that doesn't
         * need a filtering support and therefore clients must always check
         * the returned pointer.
         *
         * @return A filtering interface or NULL
         */
        MVPbkContactViewFiltering* ViewFiltering() ;
        
        
        
    };

#endif // SCVPBKCONTACTVIEW_H

//End of file
