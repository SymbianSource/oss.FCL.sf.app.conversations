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
* Description:  class to publish the data into widget
 *                
 *
*/

 
#ifndef CSERVICEWIDGETCONTENTPUBLISHER_H
#define CSERVICEWIDGETCONTENTPUBLISHER_H

#include <liwservicehandler.h>
#include <LiwVariant.h>
#include <LiwGenericParam.h>
#include "servicewidgetcpglobals.h"
#include "mservicewidgetobservers.h"

class MServiceWidgetEventObserver;

/**
 * @class servicewidgetdatapubliser
 * @since 5.0
 */
class CServiceWidgetContentPublisher: public CBase,
                                      public MLiwNotifyCallback
	{


	public:

		/**
		* Two-phased constructor.
		*/
		static CServiceWidgetContentPublisher* NewL(MServiceWidgetEventObserver& aObserver);

		/**
		* Two-phased constructor.
		*/
		static CServiceWidgetContentPublisher* NewLC(MServiceWidgetEventObserver& aObserver);

		/**
		* @function ~CServiceWidgetContentPublisher
		* @discussion Destroy the object and release all memory objects
		*/
		~CServiceWidgetContentPublisher();

		

	public:

		/**
		* publish the text with given details and actions
		* @param aPublisherId,  publisher id
		* @param aInstId,       Instance id
		* @paramconst aOwnTextToAdd Own text (updated in first row)
        * @paramconst aStatusTextToAdd status text (updated in second row)
        * @param      aBitmapHandle    image handle to add
        * @param      aMaskHandle,     image mask handle to add
		*/
		void PublishDataWithActionL( const TDesC& aPublisherId, 
		                             const TDesC& aInstId,			  
		                             const TDesC& aOwnTextToAdd,
							         const TDesC& aStatusTextToAdd,
							         TInt aBitmapHandle,
							         TInt aMaskHandle,
							         TInt sericonbitmapHandle, 
							         TInt sericonmaskHandle
							         );
							      
		                  
		/**
		           * Notifies event from widget (from CPS)
		           * @param aCmdId,  cmd id
		           * @param aEventParamList
		           * @param aInParamList
		           */
		  virtual TInt HandleNotifyL(TInt aCmdId, TInt /*aEventId*/, 
		            CLiwGenericParamList& aEventParamList,
		            const CLiwGenericParamList& aInParamList);
		  
		  
		  /**
		   * Unregister widget from CPS
		   * @param aPublisherId,  publisher id
		   */
		  void UnregisterWidgetL(const TDesC& aPublisherId );

		  /**
		   * Register widget from CPS
		   * @param aPublisherId,  publisher id
		   */
		  void RegisterWidgetL(const TDesC& aPublisherId,
		                             TInt aBitmapHandle,
                                     TInt aMaskHandle  );

		  /**
		   * Remove data from widget
		   * @param aPublisherId,  publisher id
		   */
		  void RemoveWidgetDataL(const TDesC& aPublisherId,const TDesC& aInstId );
									       
	private:

		/**
		* @function ConstructL
		* @discussion Perform the second phase construction of a CServiceWidgetContentPublisher object
		* this needs to be public due to the way the framework constructs the AppUi 
		*/
		void ConstructL();

		/**
		*@function CServiceWidgetContentPublisher
		* @discussion Perform the first phase of two phase construction.
		* This needs to be public due to the way the framework constructs the AppUi 
		*/
		CServiceWidgetContentPublisher(MServiceWidgetEventObserver& aObserver);

		/**
		* GetMessagingInterfaceL get interface from liw
		*/
		MLiwInterface* GetMessagingInterfaceL( );

	private: // data
	    
	    MServiceWidgetEventObserver& iObserver;
		// owns : message interface to liw
		MLiwInterface* iMsgInterface;
		// owns : liw service handler
		CLiwServiceHandler* iServiceHandler;
	};
 
 #endif // CSERVICEWIDGETCONTENTPUBLISHER_H
 
