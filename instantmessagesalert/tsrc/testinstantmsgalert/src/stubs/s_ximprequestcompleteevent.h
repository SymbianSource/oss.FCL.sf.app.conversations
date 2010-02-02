/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_ximprequestcompleteevent.h
*
*/
#ifndef MXIMPFWREQUESTCOMPLETEEVENTSTUB_H
#define MXIMPFWREQUESTCOMPLETEEVENTSTUB_H

#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>

class MXIMPStatus;

class MXIMPRequestCompleteEventStub : public MXIMPRequestCompleteEvent
    {
    const TXIMPRequestId& RequestId() const;
    const MXIMPStatus& CompletionResult() const;
    const MXIMPBase* LookupCompletionDataByType( TInt32 aInterfaceId ) const;
        
    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;

    };


/**
 * Interface for status object.
 *
 * Status object is used in XIMP Framework API
 * to present result code and additional textual
 * details from request completions, context
 * state changes and similar events.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MyXIMPStatusStub : public MXIMPStatus
    {

public:

    /**
     * Gets request completion code.
     *
     * Result code can be:
     *  - one of the system wide errors
     *  - one of the ximp core specific errors defined in in ximperrors.hrh
     *  - one of the presence specific errors defined in in presenceerrors.hrh
     *  - error specific to the underlying communication channel
     *
     * @return The result code.
     */
    TInt ResultCode() const { return iCode; };


    /**
     * Gets the protocol level response code.
     *
     *
     * @return Protocol level response code.
     *         KErrNone if no protocol level response code available.
     */
    TInt ProtocolResultCode() const { return KErrNone; }



    /**
     * Get the textual result description.
     * Textual descriptions are protocol specific.
     *
     * @return The textual result description.
     *         Empty descriptor if no description available.
     */
    const TDesC16& ProtocolResultDescription() const { return KNullDesC; }



    /**
     * Sets request completion code.
     *
     * @param [in] aCode
     *        The result code.
     */
    void SetResultCode(
                TInt aCode ) { iCode = aCode; };



    /**
     * Sets textual result description.
     *
     * @param [in] aCode
     */
    void SetProtocolResultCode(
                TInt /*aCode*/ ) {};



    /**
     * Sets textual result description.
     *
     * @param [in] aDescription
     *        The textual result description.
     */
    void SetProtocolResultDescriptionL(
                const TDesC16& /*aDescription*/ ) {};
                
	const TAny* GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ ) const { return NULL; }

    TAny* GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ ) { return NULL; }

    TInt32 GetInterfaceId() const { return KErrNone; }   
    
    private:
     TInt iCode;   

    };
    
#endif // MXIMPFWREQUESTCOMPLETEEVENT_H



