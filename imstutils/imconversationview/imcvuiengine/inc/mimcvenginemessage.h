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
* Description:  message interface
*
*/


#ifndef MIMCVENGINEMESSAGE_H
#define MIMCVENGINEMESSAGE_H

#include <e32base.h>
#include <badesca.h>

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineMessageContainerInfo;
class TXIMPRequestId;
// MDesCArray must not be fwd declared, or compiling test code will break
// since it's a MACRO. hence the include to badesca.h

// CLASS DECLARATION

/**
 *  Interface for handling one message
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineMessage
	{
	public: // Definitions
	
		/** 
		 * Different message types
		 */
		enum TMessageType
			{
			EMessagePTOP,
			EMessageWhisper,
			EMessageGroup,
			EMessageSystem,
			EMessageInvalid
			};
	
        /**
         * System message enumerations.
         */			
	    enum TSystemMessage
	        {
	        ESystemMessageNone,
	        ESystemMessageWelcome,
	        ESystemMessageNewUsers,
	        ESystemMessageTopicChanged,
	        ESystemMessagePrivatChanged,
	        ESystemMessageLeftUsers,
	        ESystemMessageDateChange
	        };

		/**
		 * Direction of message. Coming or going or other.
		 */
		enum TMessagerType
			{
			EMessageSent,
			EMessageReceived,
			EMessageOther,
			EMessageStarted
			};
		
		/**
		 * Different message content types
		 */
		enum TContentType
			{
			EContentText,
			EContentPicture,
			EContentOther,
			EContentInvalid
			};
			
	    /**
	     * Content processing states
	     */
	    enum TContentProcessState
	        {
	        EContentNotProcessed = 0,
	        EContentNotSupported,
	        EContentNotSupportedDrm,
	        EContentCorrupted,
	        EBitmapDecoded,
	        EBitmapScaled,
	        EContentReady,
	        EContentDecoded,
	        EThumbnailReady
	        };
	
	public: // General information
	
		/**
		 * Get timestamp when this message has been created.
		 * For prioritization of message deletion and timestamping
		 * chat messages
		 */
		virtual const TTime& TimeStamp() const = 0;

		/**
		 * @see MIMMessageBase
		 */
		virtual void SetTimeStamp(TTime aTime ) = 0 ;
		
		/**
		 * Message size in bytes.
		 * @return Size of message in bytes.
		 */
		virtual TInt SizeInBytes() const = 0;

    public: // Type of message
		/**
		 * Type of message
		 * @return Type of message. EMessageGroup, EMessagePersonal, 
		 *         EMessageSystem
		 */
		virtual TMessageType MessageType() const = 0;

		/**
		 * Type of messager
		 * @return Type of message. EMessageFrom, EMessageTo, EMessageOther 
		 */
		virtual TMessagerType MessagerType() const = 0;

		/**
		 * Type of content of message
		 * @return Type of message. EMessageText, EMessagePicture
		 */
		virtual TContentType ContentType() const = 0;
		
		/**
		 * Is message failed or not
		 * @return ETrue if is, EFalse if not.
		 */ 
        virtual TBool FailedMessage() const = 0;
				
		/**
		 * Returns the type of system message.
		 * @return type of system message or
		 *         TSystemMessageNone if not system message
		 */ 
		virtual TSystemMessage SystemMessageType() = 0;
		
		/**
	     * Increase owner count. There can be multiply owners for one message. 
	     * This method is for increasing the count. Every owner class
	     * must use also DecreaseOwnerCount.
	     */
	    virtual void IncreaseOwnerCount() = 0;
	    
	    /**
	     * Decrease owner count. There can be multiply owners for one message.
	     * This method is for decreasing the count. Every owner class
	     * must have called IncreaseOwnerCount before calling this method. 
	     * @return ETrue if owner can delete message. EFalse if not.
	     */ 
	    virtual TBool DecreaseOwnerCount() = 0;
	    
		
	public: // Identification of message
		/**
		 * Operation code of message. One way to identify message.
		 * @return Operation code of message.
		 */
	    virtual TXIMPRequestId OperationCode() const = 0;
	    
	    /**
	     * Set operation code of message.
	     * @param aOpId, new operation code for message
	     */
	    virtual void SetOperationCode(TXIMPRequestId aRequestId ) = 0;
	    
	    

	    /**
	     * Part of container identifier which contains this message
	     * @return User id to identify container which contains this message
	     */
	    //virtual const TDesC& UserId() const = 0;
	    
	   	    
	    /**
	     * Set containerinfo of message. Needed for identifying of message.
	     * @param aInfo. ContainerInfo.
	     */
        virtual void SetContainerInfo( MIMCVEngineMessageContainerInfo* aInfo ) = 0;
	    	    
    public: // Content of message	    
	    
	    /**
	     * Recipient of message.
	     * @return Recipient of message. To whom the message is sent.
	     */
	    virtual const TDesC& Recipient() const = 0;
	    
	    /**
	     * Sender of message
	     * @return Sender of message. To who has sent the message.
	     */
	    virtual const TDesC& Sender() const = 0;

	    
	    /**
	     * Textual content of message
	     * @return Textual content.
	     */
	    virtual const TDesC& Text() const = 0;
	    
               
       
    public: 
     
		/**
		 * Virtual destructor
		 */
		virtual ~MIMCVEngineMessage(){}
	}; 

#endif      // MIMCVENGINEMESSAGE_H

// End of File

