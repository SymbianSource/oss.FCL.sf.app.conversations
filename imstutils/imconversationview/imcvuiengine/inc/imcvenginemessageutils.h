/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  message utils class
*
*/


#ifndef IMCVENGINEMESSAGEUTILS_H
#define IMCVENGINEMESSAGEUTILS_H
#include <e32base.h>
// FORWARD DECLARATIONS
class TTime;
class MIMCVEngineMessage;
class MIMCVEngineMessageCreator;
class MIMCVEngineMessageWriteInterface;

/**
 *  General message handling methods.
 *
 *  General message handling methods which are to be used in both 
 *  messagehandler and chatgroupmanager, to avoid duplicate code.
 *
 *  @lib imcvengine.dll
 *  @since S60 v5.0
 */
class IMCVEngineMessageUtils 
    {
public:

    /**
     * Appends a datestamp to writeinterface if needed.
     * New time is compared to the previous message in the interface.
     * If the time values are from different date then datestamp is created
     * Otherwise this does nothing.
     *
     * @since S60 v5.
     * @param aNewTime Time of the new message
     * @param aWriteInterface Interface to write the date message
     * @param aMessageCreator for creating the date message
     * @param aSettingsAPI Settings interface to determine if datestamps
     *                     are required.
     */
    static void AppendDateStampL( TTime aNewTime,
                                  MIMCVEngineMessageWriteInterface& aWriteInterface,
                                  MIMCVEngineMessageCreator& aMessageCreator );


    /**
     * Appends a datestamp and message to interface. Datestamp is added only 
     * if the date has changed between previous and current message.
     * Ownership of the message is transferred or shared, so it doesn't 
     * need to be in cleanupstack.
     *
     * @since S60 v5.0
     * @param aNewMessage New message
     * @param aWriteInterface Interface to write the date message
     * @param aMessageCreator for creating the date message
     * @param aSettingsAPI Settings interface to determine if datestamps
     *                     are required.
     * @param aSharedOwnership EFalse - ownership is transferred
     *                         ETrue - ownership is shared
     */
    static void AppendMessageWithDateStampL( 
                              MIMCVEngineMessage& aNewMessage,
                              MIMCVEngineMessageWriteInterface& aWriteInterface,
                              MIMCVEngineMessageCreator& aMessageCreator,
                              TBool aSharedOwnership = EFalse );
                              
	/**
	 * used to load the resource
	 * @param aResourceId The id to read
	 * @return The string for given resource id 
	 */
     static HBufC* LoadResourceL( TInt aResourceId ,  const TDesC& aText );     
    };


#endif // IMCVENGINEMESSAGEUTILS_H
