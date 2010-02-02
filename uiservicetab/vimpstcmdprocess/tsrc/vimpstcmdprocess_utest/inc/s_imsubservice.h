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
#ifndef S_IMSUBSERVICE_CPP_
#define S_IMSUBSERVICE_CPP_

//  INCLUDES
#include <e32base.h>
#include "tvimpstenums.h" 

//base

#include "mvimpstenginesubservice.h"
#include "mvimpstengineimsubservice.h"


// FORWARD DECLARATIONS

class MVIMPSTEngineIMSubServiceEventObserver;




//CLASS DECLARATION

/**
 * class handling im sub service
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CVIMPSTEngineImSubService_Stub ): public CBase, 
                          public MVIMPSTEngineIMSubService
    {
    //for testcases


    public:  // Two-phased constructors and destructor
        /**
               * Two-phased constructor.
               *
               * @since s60 5.0
               * @param aServiceId, service id
               * @param aCchHandler, reference to cchhandler.
               * @param aTableFetcher, reference to settingstable.
               * @param aXimpEventObserver, reference to ximpevent observer.
               * @param aServiceStateListener, reference to servicestatelistener.
               */
              static CVIMPSTEngineImSubService_Stub* NewL( );

              /**
               * Two-phased constructor.
               *
               * @since s60 5.0
               * @param aServiceId, service id
               * @param aCchHandler, reference to cchhandler.
               * @param aTableFetcher, reference to settingstable.
               * @param aXimpEventObserver, reference to ximpevent observer.
               * @param aServiceStateListener, reference to servicestatelistener.
               */
              static CVIMPSTEngineImSubService_Stub* NewLC();

              /**
               * Standard C++ destructor
               */    
              ~CVIMPSTEngineImSubService_Stub();
        

    public: //From MVIMPSTEngineSubService

        /**
        * @see MVIMPSTEngineSubService
        */
        TVIMPSTEnums::TVIMPSTRegistrationState SubServiceState() const;     

        /**
        * @see MVIMPSTEngineSubService
        */
        TVIMPSTEnums::SubServiceType Type() const;  



    public: //From MVIMPSTIMSubService

        /**
        * @see MVIMPSTIMSubService
        */
        void RegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver);

        /**
        * @see MVIMPSTIMSubService
        */
        void UnRegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver);

        /**
        * @see MVIMPSTIMSubService
        */
        TInt GetUnreadCountL(const TDesC& aSender);


        /**
        * @see MVIMPSTIMSubService
        */
        TBool IsConversationExistL(const TDesC& aRecipientId) const ;

        /**
        * @see MVIMPSTIMSubService
        */
        void CloseConversationL( const TDesC& aContactId );  
        
        /**
        * @see MVIMPSTIMSubService
        */
        
        RArray<SIMCacheChatItem> GetOpenChatListL();

      

   
private:
    /**
           * Standard C++ constructor
           * 
           * @since s60 5.0
           * @param aServiceId, service id
           * @param aCchHandler, reference to cchhandler.
           * @param aTableFetcher, reference to settingstable.
           * @param aXimpEventObserver, reference to ximpevent observer.
           * @param aServiceStateListener, reference to servicestatelistener.
           */   
    CVIMPSTEngineImSubService_Stub(  );
          
          /**
           * Performs the 2nd phase of construction.
           */        
          void ConstructL();

    private :
        
       
       

    };


#endif /* S_IMSUBSERVICE_CPP_ */
