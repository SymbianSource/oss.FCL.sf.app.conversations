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




#include <CVPbkContactViewDefinition.h>

/**
 * Creates a new contact view definition and initializes
 * it from resource. Takes a VPBK_CONTACT_VIEW resource
 * structure as a parameter.
 *
 * @param aReader A resource reader to a VPBK_CONTACT_VIEW
 *                structure.
 * @return A new instance of this class.
 */
EXPORT_C  CVPbkContactViewDefinition* CVPbkContactViewDefinition::NewL(
        TResourceReader& /*aReader*/ ){ return new (ELeave) CVPbkContactViewDefinition; }

/**
 * Creates a new empty contact view definition.
 * Client must then set the needed information.
 *
 * @return A new instance of this class.
 */
EXPORT_C  CVPbkContactViewDefinition* CVPbkContactViewDefinition::NewL(){  return new (ELeave) CVPbkContactViewDefinition;  }

/**
 * Constructs a new contact view definition from an existing one
 * by copying the data.
 *
 * @param aViewDef A view definition to copy.
 * @return A new instance of this class.
 */
EXPORT_C  CVPbkContactViewDefinition* CVPbkContactViewDefinition::NewL(
        const CVPbkContactViewDefinition& /*aViewDef*/ ){  return new (ELeave) CVPbkContactViewDefinition;  }

/**
 * Creates a new contact view definition and initializes
 * it from resource. Takes a VPBK_CONTACT_VIEW resource
 * structure as a parameter.
 *
 * @param aReader A resource reader to a VPBK_CONTACT_VIEW
 *                structure.
 * @return A new instance of this class.
 */
EXPORT_C  CVPbkContactViewDefinition* CVPbkContactViewDefinition::NewLC(
        TResourceReader& /*aReader*/ ){ return new (ELeave) CVPbkContactViewDefinition; }

/**
 * Creates a new empty contact view definition.
 * Client must then set the needed information.
 *
 * @return A new instance of this class.
 */
EXPORT_C  CVPbkContactViewDefinition* CVPbkContactViewDefinition::NewLC()
{ 
CVPbkContactViewDefinition* self = new (ELeave) CVPbkContactViewDefinition;
CleanupStack::PushL(self);
return self;

}

/**
 * Constructs a new contact view definition from an existing one
 * by copying the data.
 *
 * @param aViewDef A view definition to copy.
 * @return A new instance of this class.
 */
EXPORT_C  CVPbkContactViewDefinition* CVPbkContactViewDefinition::NewLC(
        const CVPbkContactViewDefinition& /*aViewDef*/ )
        { 
        CVPbkContactViewDefinition* self = new (ELeave) CVPbkContactViewDefinition;
		CleanupStack::PushL(self);
		return self;
        }

/**
 * Destructor.
 */
CVPbkContactViewDefinition::~CVPbkContactViewDefinition(){ }

/**
 * Returns the number of sub views owned by this view.
 *
 * @return The number of sub views.
 */
EXPORT_C TInt CVPbkContactViewDefinition::SubViewCount() const{ return 0; }

/**
 * Returns the sub view definition at aIndex.
 *
 * @param aIndex The index of the sub view definition.
 * @return The sub view definition at aIndex.
 */
EXPORT_C const CVPbkContactViewDefinition& CVPbkContactViewDefinition::SubViewAt( 
        TInt /*aIndex*/ ) const{ return *this; }

/**
 * Returns the sub view definition at aIndex.
 *
 * @param aIndex    The index to query.
 * @return The sub view definition.
 */
EXPORT_C CVPbkContactViewDefinition& CVPbkContactViewDefinition::SubViewAt( TInt /*aIndex*/ ){ return *this; }

/**
 * Returns the type of this view.
 *
 * @return The type of this view.
 */
EXPORT_C TVPbkContactViewType CVPbkContactViewDefinition::Type() const{ return EVPbkContactsView; }

/**
 * Returns ETrue if aFlag is on, otherwise EFalse.
 *
 * @param aFlag The flag to check.
 * @return A flag state.
 */
EXPORT_C TBool CVPbkContactViewDefinition::FlagIsOn( TVPbkContactViewFlag /*aFlag*/ ) const{ return EFalse;}

/**
 * Returns the URI of this view. KNullDesC if this is not a leaf node.
 *
 * @return The URI of this view or KNullDesC.
 */
EXPORT_C const TDesC& CVPbkContactViewDefinition::Uri() const{ return KNullDesC(); }

/**
 * Returns the sorting policy of this view.
 *
 * @return The sorting policy of this view.
 */
EXPORT_C TVPbkContactViewSortPolicy CVPbkContactViewDefinition::SortPolicy() const{ return EVPbkOrderedContactView; }

/**
 * Returns the name of the view. Default is KNullDesC.
 *
 * In view types EVPbkContactsView and EVPbkGroupsView
 * the name can be used as a shared view identifier by the
 * store. Shared view creation is indicated by
 * TVPbkContactViewSharing. If the name is empty for shared
 * view then store implementation uses its default name.
 * This is a preferred way to use shared views in the platform.
 * 
 * In view type EVPbkFoldingView the name is the label
 * of the only contact in the view.
 *
 * In view type EVPbkCompositeView the name has no meaning.
 *
 * @return The name of the view.
 */
EXPORT_C const TDesC& CVPbkContactViewDefinition::Name() const{ return KNullDesC(); }

/**
 * Returns the view sharing type. The store can support
 * a shared view creation for saving RAM. 
 *
 * Default value for types EVPbkContactsView and EVPbkGroupsView
 * is EVPbkSharedView. Store implementation creates
 * then a shared view if it's possible for the store.
 * Prefer using the default values for saving resources.
 * 
 * For view types EVPbkFoldingView and EVPbkCompositeView the
 * sharing has no effect.
 *
 * See also the documentation of Name()
 *
 * @return The view sharing type.
 * 
 */
EXPORT_C TVPbkContactViewSharing CVPbkContactViewDefinition::Sharing() const{ return EVPbkViewSharingUndefined; }

/**
 * Returns the field type selector used to filter the view or NULL.
 *
 * The view contains only contacts having the fields defined
 * by the field type filter.
 *
 * @return The field type filter.
 */
EXPORT_C CVPbkFieldTypeSelector* CVPbkContactViewDefinition::FieldTypeFilter() const{ return NULL; }

/**
 * Returns the id of this view.
 *
 * @return the id of this view.
 */
EXPORT_C TInt CVPbkContactViewDefinition::Id() const{ return 1; }
/**
 * Adds new view as a subview. This object takes ownership
 * of the subview. If this function leaves ownership is not taken.
 *
 * @param aSubView The new subview to add.
 */
EXPORT_C void CVPbkContactViewDefinition::AddSubViewL( CVPbkContactViewDefinition* /*aSubView*/ ){ }

/**
 * Sets aType as this views type.
 *
 * @param aType The type to set.
 */
EXPORT_C void CVPbkContactViewDefinition::SetType( TVPbkContactViewType /*aType*/ ){ }

/**
 * Sets aId as this views id.
 *
 * @param aId the id to set.
 */
EXPORT_C void CVPbkContactViewDefinition::SetId( TInt /*aId*/ ){ }
/**
 * Sets the saFlag's state as aState.
 *
 * @param aFlag The flag to modify.
 * @param aState The state to set.
 */
EXPORT_C void CVPbkContactViewDefinition::SetFlag( TVPbkContactViewFlag /*aFlag*/, TBool /*aState*/ ){ }

/**
 * Sets aUri as this views URI. This function takes a copy of aUri.
 *
 * @param aUri The URI to set.
 */
EXPORT_C void CVPbkContactViewDefinition::SetUriL( const TDesC& /*aUri*/ ){ }

/**
 * Sets aSortPolicy as this views sorting policy.
 *
 * @param aSortPolicy The sort policy to set.
 */
EXPORT_C void CVPbkContactViewDefinition::SetSortPolicy( TVPbkContactViewSortPolicy /*aSortPolicy*/ ){ }

/**
 * Sets aName as this views Name. This function takes a copy of aName.
 *
 * @param aName The name to set.
 */
EXPORT_C void CVPbkContactViewDefinition::SetNameL( const TDesC& /*aName*/ ){ }

/**
 * Sets the view sharing type.
 *
 * @param aViewSharing One of the values defined in
 *                     VPbkContactView.hrh.
 */
EXPORT_C void CVPbkContactViewDefinition::SetSharing( TVPbkContactViewSharing /*aViewSharing*/ ){ }

/**
 * Sets the field type selector used to filter the view.
 *
 * The view contains only contacts having the fields defined
 * by the field type filter.
 *
 * @param aFilter The field type filter. Use NULL to
 *                filter nothing.
 */
EXPORT_C void CVPbkContactViewDefinition::SetFieldTypeFilterL( CVPbkFieldTypeSelector* /*aFilter*/ ){ }

/**
 * Sets the contact selector used to filter the view.
 *
 * The view contains only contacts accepted by the selector.
 *
 * @param aContactSelector The contact selector. Use NULL to
 *                filter nothing. Ownership not transferred.
 */
EXPORT_C void CVPbkContactViewDefinition::SetContactSelector( MVPbkContactSelector* /*aContactSelector*/ ){ }

/**
 * Gives either the contact selector or NULL. Ownership is not transferred.
 */
EXPORT_C MVPbkContactSelector* CVPbkContactViewDefinition::ContactSelector() const{ return NULL; }

CVPbkContactViewDefinition::CVPbkContactViewDefinition(){ }
void CVPbkContactViewDefinition::ConstructL(){ }
void CVPbkContactViewDefinition::ConstructL(
        const CVPbkContactViewDefinition& /*aViewDef*/ ){ }
void CVPbkContactViewDefinition::ConstructFromResourceL(
        TResourceReader& /*aReader*/ ){ }
void CVPbkContactViewDefinition::InitializeSharing(){ }

// End of File
