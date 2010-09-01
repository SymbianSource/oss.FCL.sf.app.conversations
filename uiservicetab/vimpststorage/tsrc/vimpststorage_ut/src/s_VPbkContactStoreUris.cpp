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




//  INCLUDES
#include <VPbkContactStoreUris.h>

_LIT( KPhoneBookDbURI, "cntdb://c:contacts.cdb");

/**
 * Returns the default Contacts Model database store URI.
 *
 * @return The default Contacts Model database store URI.
 */
EXPORT_C const TDesC& VPbkContactStoreUris::DefaultCntDbUri(){ return KPhoneBookDbURI(); }

/**
 * Returns the global ADN SIM store URI.
 * 
 * Global means the ADN store that can be accessed by both GSM and 3G phone.
 * 
 * @return The global ADN SIM store URI
 */
EXPORT_C const TDesC& VPbkContactStoreUris::SimGlobalAdnUri(){ return KPhoneBookDbURI(); }

/**
 * Returns the global FDN SIM store URI.
 *
 * Global means the FDN store that can be accessed by both GSM and 3G phone.
 *
 * @return The global FDN SIM store URI
 */
EXPORT_C const TDesC& VPbkContactStoreUris::SimGlobalFdnUri(){ return KPhoneBookDbURI(); }

/**
 * Returns the global SDN SIM store URI. 
 *
 * Global means the SDN store that can be accessed by both GSM and 3G phone.
 * 
 * @return The global SDN SIM store URI
 */
EXPORT_C const TDesC& VPbkContactStoreUris::SimGlobalSdnUri(){ return KPhoneBookDbURI(); }

/**
 * Returns the MSISDN SIM store URI.
 *
 * Global means the MSISDN store that can be accessed by both GSM and
 * 3G phone.
 *
 * NOTE: Though the name of the store suggests that the store will contain
 *       the caller's own phone number it's not exactly the case. It can
 *       contain the number but only if it has been set.
 *
 * @return The MSISDN SIM store URI.
 */
EXPORT_C const TDesC& VPbkContactStoreUris::SimGlobalOwnNumberUri(){ return KPhoneBookDbURI(); }
    
            
// End of File
