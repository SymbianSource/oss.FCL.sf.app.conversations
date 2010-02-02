/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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



// INCLUDE FILES
#include <CPbk2SortOrderManager.h>


/**
 * Creates a new instance of this class.
 *
 * @param aMasterFieldTypeList      Master field type list.
 *                                  Can be retrieved from Virtual
 *                                  Phonebook contact manager.
 * @param aFs                       A file system session
 *                                  reference for resource file
 *                                  handling. If NULL then
 *                                  an own session is created.
 * @return  A new instance of this class.
 */
EXPORT_C CPbk2SortOrderManager* CPbk2SortOrderManager::NewL(
        const MVPbkFieldTypeList& /*aMasterFieldTypeList*/,
        RFs* /*aFs = NULL*/ )
	{
	return new (ELeave) CPbk2SortOrderManager;
	}

/**
 * Destructor.
 */
CPbk2SortOrderManager::~CPbk2SortOrderManager(){ }

/**
 * Sets the contact view that this manager manages.
 * Takes in use the sort order from the given view.
 *
 * @param aContactView  The contact view to manage.
 */
EXPORT_C void CPbk2SortOrderManager::SetContactViewL(
        MVPbkContactViewBase& /*aContactView*/ ){ }

/**
 * Adds an observer.
 *
 * @param aObserver     The observer to add.
 */
EXPORT_C void CPbk2SortOrderManager::AddObserverL(
        MPbk2SortOrderObserver& /*aObserver*/ ){ }

/**
 * Removes an observer.
 *
 * @param aObserver     The observer to remove.
 */
EXPORT_C void CPbk2SortOrderManager::RemoveObserver(
        MPbk2SortOrderObserver& /*aObserver*/ ){ }

/**
 * Sets the name displaying order for the managed contact view.
 *
 * @param aNameDisplayOrder     New name display order for
 *                              the managed contact view.
 * @param aSeparator            Custom separator to be used between
 *                              lastname and firstname if it exists.
 *                              If KNullDesC is given, space is used
 *                              as separator.
 */
EXPORT_C void CPbk2SortOrderManager::SetNameDisplayOrderL(
        TPbk2NameDisplayOrder /*aNameDisplayOrder*/,
        const TDesC& /*aSeparator = KNullDesC*/ ) 
{
	
}

/**
 * Returns the current name display order.
 *
 * @return  Current name display order.
 */
EXPORT_C CPbk2SortOrderManager::TPbk2NameDisplayOrder CPbk2SortOrderManager::NameDisplayOrder() const
{

return EPbk2NameDisplayOrderLastNameFirstName; 
}

/**
 * Returns the current sort order.
 *
 * @return  Current sort order.
 */
EXPORT_C const MVPbkFieldTypeList& CPbk2SortOrderManager::SortOrder() const
{ 
MVPbkFieldTypeList* ret = NULL;
return *ret;
}

/**
 * Returns the language specific default separator.
 * This can not be changed run time.
 *
 * @return the language specific default separator.
 */
EXPORT_C const TDesC& CPbk2SortOrderManager::DefaultSeparator(){ return KNullDesC(); }

/**
 * Returns the current separator. This the separator that
 * can be changed run time and saved to Central Repository.
 *
 * @return A current separator.
 */
EXPORT_C const TDesC& CPbk2SortOrderManager::CurrentSeparator() const{ return KNullDesC(); }


void CPbk2SortOrderManager::ConstructL(
        const MVPbkFieldTypeList& /*aMasterFieldTypeList*/,
        RFs* /*aFs*/ ){ }

    
// End of File
