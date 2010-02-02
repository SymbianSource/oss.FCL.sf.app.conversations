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



#include <CPbk2StoreConfiguration.h>
#include <TVPbkContactStoreUriPtr.h>
/**
 * Creates a new instance of this class.
 *
 * @return  A new instance of this class.
 */
  EXPORT_C /*static*/ CPbk2StoreConfiguration* CPbk2StoreConfiguration::NewL(){ }

/**
 * Destructor.
 */
  CPbk2StoreConfiguration::~CPbk2StoreConfiguration(){ }



/**
 * Fetches an array of configured contact store URIs.
 *
 * @return  Array of URIs that is the current configuration.
 */
  CVPbkContactStoreUriArray* CPbk2StoreConfiguration::CurrentConfigurationL() const{ }

/**
 * Fetches an array of contact store URIs to be used
 * in searching, for example in phone number matching.
 *
 * @return  Array of URIs to be used for searching.
 */
  CVPbkContactStoreUriArray* CPbk2StoreConfiguration::SearchStoreConfigurationL(){ }

/**
 * Fetches an array of supported contact stores URIs.
 * NOTE: All returned stores are not necessarily supported by
 * the current HW / SW configuration of the phone.
 *
 * @return  Array of URIs that is the supported store configuration.
 */
  CVPbkContactStoreUriArray* CPbk2StoreConfiguration::SupportedStoreConfigurationL() const{ }

/**
 * Returns the URI of the default store for saving contacts.
 *
 * @return  URI of the default store.
 */
  TVPbkContactStoreUriPtr CPbk2StoreConfiguration::DefaultSavingStoreL() const{ }

/**
 * Adds a new contact store URI to current configuration.
 * Then use AddSupportedContactStoreUriL to add the store to
 * supported configuration if desired.
 *
 * @param aURI      The URI to add.
 */
  void CPbk2StoreConfiguration::AddContactStoreURIL(
        TVPbkContactStoreUriPtr aURI ){ }

/**
 * Removes a contact store URI from current configuration.
 * Then use RemoveSupportedContactStoreUriL to remove the store from
 * supported configuration if desired.
 *
 * @param aURI      The URI to remove.
 */
  void CPbk2StoreConfiguration::RemoveContactStoreURIL(
        TVPbkContactStoreUriPtr aURI ){ }

/**
 * Adds a new configuration observer.
 *
 * @param aObserver     The observer to add.
 */
  void CPbk2StoreConfiguration::AddObserverL(
        MPbk2StoreConfigurationObserver& aObserver ){ }

/**
 * Removes a configuration observer.
 *
 * @param aObserver     The observer to remove.
 */
  void CPbk2StoreConfiguration::RemoveObserver(
        MPbk2StoreConfigurationObserver& aObserver ){ }

/**
 * Adds a default saving store observer.
 *
 * @param aObserver     The observer to add.
 */
  void CPbk2StoreConfiguration::AddDefaultSavingStoreObserverL(
        MPbk2DefaultSavingStoreObserver& aObserver ){ }

/**
 * Removes a default saving store observer.
 *
 * @param aObserver     The observer to remove.
 */
  void CPbk2StoreConfiguration::RemoveDefaultSavingStoreObserver(
        MPbk2DefaultSavingStoreObserver& aObserver ){ }

/**
 * Adds a new contact store URI to supported configuration.
 * Use AddContactStoreURIL to add the store to current
 * configuration if desired.
 *
 * @param aURI      The URI to add.
 */
  void CPbk2StoreConfiguration::AddSupportedContactStoreURIL(
        TVPbkContactStoreUriPtr aURI ){ }

/**
 * Removes a contact store URI from supported configuration.
 * Use RemoveContactStoreUriL to remove the store from
 * current configuration if desired.
 *
 * @param aURI      The URI to remove.
 */
  void CPbk2StoreConfiguration::RemoveSupportedContactStoreURIL(
        TVPbkContactStoreUriPtr aURI ){ }


  CPbk2StoreConfiguration::CPbk2StoreConfiguration(){ }
void CPbk2StoreConfiguration::ConstructL(){ }

   

// End of File
