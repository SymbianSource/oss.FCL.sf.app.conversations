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
* Description:  base class for messages
*
*/


#ifndef CIMCVENGINEMESSAGEBASE_H
#define CIMCVENGINEMESSAGEBASE_H

#include <e32base.h>
#include <badesca.h>

#include "mimcvenginemessage.h"
#include "mimcvenginemessagecreator.h"
#include <ximpbase.h>
//	FORWARD CLASS DECLERATIONS
class MIMCVEngineMessageContainerInfo;

// CLASS DECLARATION

/**
 *  Message Base class
 */
class CIMCVEngineMessageBase : public CBase, 
                       public MIMCVEngineMessage
	{
                      	
    public: // Construction

		/**
		 * Destruction
		 */
		~CIMCVEngineMessageBase();
		
	protected: // Construction
	
		/**
		 * Constructor
		 */
		CIMCVEngineMessageBase();
		
		/**
		 * Remember to call this one as last line of child ConstructL
		 * @param aSender, sender user id
		 * @param aRecipient, recipient id
		 */
		void ConstructL( const TDesC& aSender, 
		                 const TDesC& aRecipient );
		
	protected: // From MIMCVEngineMessage
		
		/**
		 * @see MIMCVEngineMessage
		 */
		const TTime& TimeStamp() const;
		/**
		 * @see MIMCVEngineMessage
		 */
		void SetTimeStamp(TTime aTime ) ;

		/**
		 * @see MIMCVEngineMessage
		 */
		TMessageType MessageType() const;
		
		/**
		 * @see MIMCVEngineMessage
		 */
		TMessagerType MessagerType() const;
		
		/**
		 * @see MIMCVEngineMessage
		 */
		TXIMPRequestId OperationCode() const;
		
		/**
		 * @see MIMCVEngineMessage
		 * @param aOpId, operation code
		 */
	    void SetOperationCode( TXIMPRequestId aRequestId );
		
		/**
		 * @see MIMCVEngineMessage
		 */
        const TDesC& Recipient() const;	
        
		/**
		 * @see MIMCVEngineMessage
		 */
        const TDesC& Sender() const;
        
		                
		/**
		 * @see MIMCVEngineMessage
		 */
        const TDesC& Text() const;
        
	
		/**
		 * @see MIMCVEngineMessage
		 */
        //const TDesC& UserId() const;

	        
        /**
         * @see MIMCVEngineMessage
         * @param aInfo, message container info
         */
        void SetContainerInfo( MIMCVEngineMessageContainerInfo* aInfo );
                   

                
		/**
		 * @see MIMCVEngineMessage
		 */
		TInt SizeInBytes() const;
		
		/**
		 * @see MIMCVEngineMessage
		 */
		TBool FailedMessage() const;
		

	    /**
	     * @see MIMCVEngineMessage
	     */
        TSystemMessage SystemMessageType();
              
      	
      	/**
		 * @see MIMCVEngineMessage
		 */
        void IncreaseOwnerCount();
        
		/**
		 * @see MIMCVEngineMessage
		 */
        TBool DecreaseOwnerCount();
   
    
    public: // New methods
    
        
        
        /**
         * Set message type
         * @param aNewType New message type.
         */
        virtual void SetMessageType( TMessageType aNewType );
        
        /**
         * Set messager type
         * @param aNewType New messager type.
         */
        virtual void SetMessagerType( TMessagerType aNewType );

		/**
		 * Set message as system message
		 * @param aType Type of system message
		 */ 
        void SetSystem( TSystemMessage aType );
        
        
       
    protected:
    
        /**
         * Set failed message
         * @param aFailed, by default it  will set ETrue
         */
        virtual void SetFailed( TBool aFailed = ETrue );
       
        
                                            
	protected: // Member variables
	
	    /// Type of message
	    TMessageType iMessageType;
	    // messager type
	    TMessagerType iMessagerType;
	       
		/// Owns Message sender
 		HBufC* iSender;
 		
	    /// Owns Recipient of message
	    HBufC* iRecipient;	 

 		
 		/// Timestamp.
 		TTime iTime;
 		
	    /// Operation code
	    TXIMPRequestId iOperationCode;
	    	    
	    
	    /// Owns. Textual content.
	    HBufC* iText;
	    
	    /// Does not own. Container information
	    MIMCVEngineMessageContainerInfo* iInfo;
	    
        
        /// Is message failed message or not.
        TBool iFailed;
                
        /// If this is a system message, this defines the type
        TSystemMessage iSystemMessageType;
        
        // Message's version used in internalizing/externalizing
        TUint16 iMessageVersion;
        
        /// Ownercount to count owners.
	    TInt iOwnerCount;
	}; 

#endif      // CIMCVENGINEMESSAGEBASE_H

// End of File
