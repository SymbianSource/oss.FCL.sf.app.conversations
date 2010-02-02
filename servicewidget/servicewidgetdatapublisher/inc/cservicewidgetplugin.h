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
* Description:  This is a plugin implementation
 *                
 *
*/

#ifndef CSERVICEWIDGETPLUGIN_H
#define CSERVICEWIDGETPLUGIN_H

//content harvester
#include <contentharvesterplugin.h>
#include <contentharvesterpluginuids.hrh>
#include <mspnotifychangeobserver.h>
#include <aipropertyextension.h>
#include "mservicewidgetobservers.h"
#include<badesca.h>
// FORWARD DECLARATIONS
class CServiceWidgetServiceManager;
class CServiceWidgetContentPublisher;
class CCoeEnv;
class MVIMPSTSettingsStore;
class CSPNotifyChange;

/**
 *  
 *  Plugin implementation
 * 
 *  @servicewidgetdatapublisher.dll
 *  @since S60 5.0
 */
 
class CServiceWidgetPlugin : public CContentHarvesterPlugin,
							 public MSPNotifyChangeObserver,
							 public MServiceWidgetEventObserver,
							 public MAiPropertyExtension
	{
	friend class Cservicewidgetdatapublisher_ut;
	public:

		/**
		* Two-phased constructor.
		* @return the instance of same class
		*/
		static CServiceWidgetPlugin* NewL();

		/**
		* Standard C++ destructor
		*/    
		~CServiceWidgetPlugin();


	private:

		/**
		* Standard C++ constructor
		*/  
		CServiceWidgetPlugin();
		
		/**
		* 2phase construction 
		*/ 
		void ConstructL();
		
		/**
		* helper method for reading the service ids from cspsettings
		*/ 
		void ReadServiceIdsL();
		/**
		* helper method for reading resource file
		*/ 
		void LoadResourceFileL();
		/**
		  * helper method for validating Service and creating/deleting service managers 
		  * as per service
		  */ 
		void ValidateAllServicesL();

	public : //CContentHarvesterPlugin	
		/** 
		* Triggers update process.
		* all the operation must start here
		*/
		void UpdateL() ;
		
    public : // from  MSPNotifyChangeObserver
	
		/**
		 * From MSPNotifyChangeObserver 
		 * Callback when service has changed
		 *
		 * @since S60 5.0
		 * @param aServiceId changed service
		 * @return None
		 */
		void HandleNotifyChange( TServiceId aServiceId );

		/**
		 * From MSPNotifyChangeObserver 
		 * Callback when service error occures
		 *
		 * @since S60 5.0
		 * @param aError error code
		 * @return None
		 */
		void HandleError( TInt aError );
	
	public: // 	from MServiceWidgetEventObserver
		/** 
		* @param aPublisherId publisher id for Theme (it is service name).
		* @param aEvent Event name (i.e. Active,Suspend,Resume,Deactivate,Selected)
		*This function handles all homescreen callbacks (publisher received callback events 
        * from homescreen and direct them to plugin to take appropriate action.)
		*/
		void HandleServiceEventL(const TDesC& aPublisherId, const TDesC8& aEvent,TDesC& aInstId );
		
	public: //From MAiPropertyExtension.
		    /**
		    * From MAiPropertyExtension.
		    * Read property of publisher plug-in.
		    *
		    * @param aProperty - identification of property.
		    * @return pointer to property value.
		    */
		    TAny* GetPropertyL( TInt aProperty );

		    /**
		    * From MAiPropertyExtension.
		    * Write property value.
		    *
		    * @param aProperty - identification of property.
		    * @param aValue - contains pointer to property value.
		    */
		    void SetPropertyL( TInt aProperty, TAny* aValue );
		
		
	private:  // Data members
		// owns : content publishing client to hsps
		CServiceWidgetContentPublisher* iContentPublisher;
		// owns : list of all service ids interested in service tab
		RArray<TInt> iServiceIds;
		// owns : content prioritizer 
		RPointerArray<CServiceWidgetServiceManager> iServiceManagers;
		 // Not owns : A static CEikonEnv
	    CCoeEnv* iEnv;
	    //  owns : a resource offset
	    TInt iResourceOffset;
	    // owns, retrive settings from cenrep store
	    MVIMPSTSettingsStore* iSettingsStore;
	    // Own: service observer
	    CSPNotifyChange* iSpNotifyChange; 
	    // Information about the content publisher (this plug-in)
	    TAiPublisherInfo iInfo;
	};

#endif  // CSERVICEWIDGETPLUGIN_H
// End of File
