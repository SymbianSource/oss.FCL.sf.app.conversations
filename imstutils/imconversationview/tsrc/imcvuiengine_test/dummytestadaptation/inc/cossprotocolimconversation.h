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
* Description:  Oss protocol im conversation 
*
*/

#ifndef COSSPROTOCOLIMCONVERSATION_H__
#define COSSPROTOCOLIMCONVERSATION_H__


#include <e32base.h>
#include <protocolimconversation.h>


class MOSSProtocolConnectionManager;


/**
 * COSSProtocolImConversation
 *
 * ?more_complete_description
 *
 * @lib ?library
 * @since S60 v4.0
 */
class COSSProtocolImConversation :   public CBase,
                                        public MProtocolImConversation
    {

public:

    static COSSProtocolImConversation* NewL( MOSSProtocolConnectionManager& aConnManager );
    ~COSSProtocolImConversation();


private:

    COSSProtocolImConversation( MOSSProtocolConnectionManager& aConnManager );
    void ConstructL();

public:

    // MXIMPBase Start

    TAny* GetInterface( TInt32 aInterfaceId,
                        TIfGetOps  aOptions );

    const TAny* GetInterface( TInt32 aInterfaceId,
                              TIfGetOps aOptions ) const;


    TInt32 GetInterfaceId() const;

    // MXIMPBase End

public: // From MProtocolImConversation
    // Manage Im operations.For comments,see MProtocolImConversation 
    void DoSendMessageL( 
              const MImConversationInfo& aImMessage,
              TXIMPRequestId aReqId );
    void DoForwardMessageL(
              const MImConversationInfo& aImMessage,
              TXIMPRequestId aReqId);
    void DoSendMessageToGroupL( 
    					const MImConversationInfo& aImMessage,
    					const TDesC* aGroupId,TXIMPRequestId aReqId );
		TXIMPRequestId DoBlockUsersL(
	        		const MDesCArray* aUserIds,
	        		TXIMPRequestId aReqId);
		TXIMPRequestId DoUnBlockUsersL(
            	const MDesCArray* aUserIds,
            	TXIMPRequestId aReqId );
    void DoGetBlockedListRequestL( 
    					const MXIMPIdentity& aImMessageId,
	    				TXIMPRequestId aReqId );	        
		void DoGetMessageListL( 
							const MXIMPIdentity& aImMessageId,
		 					const TDesC* aGroupId,
							const TInt aMessageCount,
							TBool aDeliveryReportWanted,
							TXIMPRequestId aReqId);
		void DoRejectMessageL( 
							const MXIMPIdentity& aImMessageId,
							const TDesC& aMessageID,
							TXIMPRequestId aReqId );			
private:
		/**
     * Creates data handler
     */
		void CreateDataHandlerL();
	
private:    // data members
		/**
     * Oss Protocol Plugin Connection Manager
     * Not own.  *** Write "Not own" if some other class owns this object.
     */
    MOSSProtocolConnectionManager& iConnMan;

    };

#endif // COSSPROTOCOLIMCONVERSATION_H__
