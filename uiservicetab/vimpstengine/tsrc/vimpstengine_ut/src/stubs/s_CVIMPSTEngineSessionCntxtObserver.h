/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class handling connection to precense framework
*
*/


#ifndef     s_CVIMPSTEngineSessionCntxtObserver_H
#define     s_CVIMPSTEngineSessionCntxtObserver_H

// INCLUDES
#include <e32base.h>








NONSHARABLE_CLASS( CVIMPSTEngineSessionCntxtObserver_Stub ): public CBase 
	                                                           
						  
                         
    {
    public:

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
        static CVIMPSTEngineSessionCntxtObserver_Stub* NewL( TUint32 aServiceId );

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
        static CVIMPSTEngineSessionCntxtObserver_Stub* NewLC(TUint32 aServiceId );

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTEngineSessionCntxtObserver_Stub();        
    
     	
    
    private: // Data
                                
        // Service identifier
        TUint32 iServiceId; 
        
       
    }; 

#endif //s_CVIMPSTEngineSessionCntxtObserver_H

