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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

#ifndef S_MCCAPARAMETER_H_
#define S_MCCAPARAMETER_H_


#include <e32std.h>
#include <cntdef.h>
#include <MVPbkContactLink.h>
#include <mccaparameter.h>

/** @file mccaparameter.h
 *
 *  MCCAParameter class is used to store, control and define how the CCA application is launched
 *  with the contact specified data.
 *
 *  Example usage of the API:
 *  @code
 *   MCCAParameter* aParameter = TCCAFactory::NewParameterL()
 *   aParameter->SetConnectionFlag(ENormal);
 *   aParameter->SetContactDataFlag(EContactId);
 *   aParameter->SetContactDataL(aString);
 *
 *   aParameter->SetLaunchedViewUid(aUid);
 *
 *  @endcode
 *
 *  @lib ccaclient
 *  @since S60 v5.0
 */
class MCCAParameterstub:public MCCAParameter
    {

public:
    static MCCAParameter* MCCAParameterstub::NewL();
    /**
     * Destroy this parameter entity and all its data.
     * @since S60 5.0
     */
     void Close() ;

    /**
     * Getter for the flags of connection.
     * @since S60 5.0
     * @return the flags.
     */
     MCCAParameter::TConnectionFlags ConnectionFlag() ;

    /**
     * Getter for the contact data flags.
     * @since S60 5.0
     * @return the flags.
     */
     MCCAParameter::TContactDataFlags ContactDataFlag() ;

    /**
     * Getter for the contact data.
     * @leave KErrArgument if data not set.
     * @since S60 5.0
     * @return the reference of contact data.
     */
     HBufC& ContactDataL() ;

    /**
     * Getter for the view uid.
     * @since S60 5.0
     * @return the Uid.
     */
     TUid LaunchedViewUid() ;

    /**
     * Getter for the version.
     * @since S60 5.0
     * @return the version.
     */
     TInt32 Version() ;

    /**
     * Setter for the flag of connection.
     *
     * @see TConnectionFlags
     * @since S60 5.0
     * @param aFlags: flags of connection
     */
     void
        SetConnectionFlag(const MCCAParameter::TConnectionFlags aConnectionFlags) ;

    /**
     * Setter for the contact data flag.
     *
     * @see TContactDataFlags
     * @since S60 5.0
     * @param aFlags: flags of connection
     */
     void
        SetContactDataFlag(const MCCAParameter::TContactDataFlags aContactDataFlags) ;

    /**
     * Setter for the contact data.
     * If you are using TContactDataFlags::EContactLink, the MVPbkContactLink
     * should be streamed with PackLC before given for this method.
     * Ownership is NOT transferred.
     * @see MVPbkContactLink from  phonebook.
     * @leave KErrNotFound if contact data flag is not
     *        set before calling this method.
     * @see MCCAParameter::SetContactDataFlags
     * @since S60 5.0
     * @param aContactData: given contact data.
     */
     void SetContactDataL(const TDesC& aContactData) ;

    /**
     * Setter for the launched view uid. Use this when there
     * is need to get a particular view opened first on launch of CCA.
     * If view uid is not set, CCA uses the default view.
     * NOT IMPLEMENTED YET!
     * @since S60 5.0
     * @param aContactData: given contact data.
     */
     void SetLaunchedViewUid(const TUid aUid) ;

    /**
     * Returns an extension point for this interface or NULL.
     * @param aExtensionUid Uid of extension
     * @return Extension point or NULL
     */
     TAny* CcaParameterExtension(
        TUid /*aExtensionUid*/ ) { return NULL; }

    };


#endif /* S_MCCAPARAMETER_H_ */
