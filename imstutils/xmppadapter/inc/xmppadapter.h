/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Handles Meco IM settings in OMA provisioning.
*
*/


#ifndef C_XMPPADAPTER_H
#define C_XMPPADAPTER_H

// INCLUDES
#include    <CWPAdapter.h>
#include <barsc.h>
class CCoeEnv;


// CLASS DECLARATION

/**
 *  CXMPPAdapter handles XMPP conifurations.
 *  The processes configurations are stored inside VENDORCONFIG element.
 *
 *  @lib xmppadapter.lib
 *  @since S60 v5.0
 */
class CXMPPAdapter : public CWPAdapter
    {
    
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aStorage Storage instance
         * @param aObserver contact data container observer
         * 
         */

         static CXMPPAdapter* NewL();

        /**
        * Destructor
        */
        virtual ~CXMPPAdapter();

    public: // Functions from base classes
        /**
         * Reads resource corresponding to the aId 
         *
         * @return Resource buffer
         */

        HBufC* ReadResourceL( TInt aId );
        
        /**
         * From CWPAdapter.
         * Checks the number of settings items.
         *
         * @return Quantity of items (always one).
         */
        TInt ItemCount() const;

        /**
         * From CWPAdapter.
         * Getter for the summary title of general IM settings item. Text is
         * shown to the user in an opened configuration message.
         *
         * @param aIndex Not used.
         * @return Summary title.
         */
        const TDesC16& SummaryTitle( TInt aIndex ) const;

       
        /**
         * From CWPAdapter.
         * Getter for the summary text of general IM settings. Text is shown
         * to the user in opened configuration message.
         *
         * @param aIndex Not used.
         * @return Summary text.
         */
        const TDesC16& SummaryText( TInt aIndex ) const;

        /**
         * From CWPAdapter.
         * Query for detailed information about the general IM settings.
         * This is not supported feature as in other adapters in the framework.
         *
         * @param aItem Not used but here because of inheritance.
         * @param aVisitor Object for retrieveing details of a setting entry.
         * @return KErrNotSupported if not supported.
         */
        TInt DetailsL( TInt aItem, MWPPairVisitor& aVisitor );

        /**
         * From CWPAdapter.
         * Saves general IM settings.
         *
         * @param aIndex Not used.
         */
        void SaveL( TInt aItem );
		
		/**
         * From CWPAdapter.
         * Returns EFalse since there can only be one set of general VoIP
         * settings.
         *
         * @param aItem Location of the general IM settings item to
         *        be queried. Not used.
         * @return Information whether these settings can be set as default.
         */
        TBool CanSetAsDefault( TInt aItem ) const;

        /**
         * From CWPAdapter.
         * Adapter sets the settings as default. Not supported.
         *
         * @param aItem Location of the general IM settings item to be
         *        set as default.
         */
        void SetAsDefaultL( TInt aItem );

        /**
         * From CWPAdapter.
         * Visit method. Visits the adapter. Adapter checks from 
         * aCharacteristic if the data is targetet to it and acts according to
         * that.
         *
         * @param aCharacteristic The characteristic found.
         */
        void VisitL( CWPCharacteristic& aCharacteristic );

         /**
         * From CWPAdapter.
         * Visit method. Visits the adapter. Method sets the iAppId value.
         *
         * @param aParameter The parameter found.
         */
        void VisitL( CWPParameter& aParameter );

        /**
         * From CWPAdapter.
         * Visit method. Not supported.
         *
         * @param aLink Link to the characteristic.
         */
        void VisitLinkL( CWPCharacteristic& /*aLink*/ );

    private:  // Default constructors

        /**
        * C++ default constructor.
        */
        CXMPPAdapter();

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();

    private: // New functions
		
		/**
         * Creates the settings from the received provisioning
         */
		void CreateSettingsRecordL();
		
		/**
         * Updates the existing settings
         */
		void UpdateServiceSettingsL();
  	
        /**
         * Parses integer form Descriptor
         *
         * @param aPtr to be parsed.
         * @param aInt parsed integer value is stored..
         */
        void ParseIntegerL( const TDesC& aPtr, TInt& aInt );
        /**
         * writes the application specific settings into cenrep
         *
         * e.g.  cancel login support : This writes if cancel lgin not supported.
         */
        void WriteAppSettingsL();
        
        /**
        * This will check for the IM subservice in sp settings table
        * if any service in spsettings is having IM Subservice
        * this will return Etrue else it will return EFalse
        * @Return - ETrue - if IM Subservice Exists else EFalse
        **/
        TBool CheckforXmppserviceL();

    private: // Data
        /**
         * user name for the current characteristic.
         * Owned
         */
        HBufC* iUserName;
        /**
         * password for the current characteristic.
         * Owned
         */
        HBufC* iPassword;
        
        /**
         * user name the current characteristic.
         * Owned
         */
        HBufC* iProviderId;        
        
    	/**
         * settingsrecordid  for the current characteristic.
         * Owned
         */
        TUint32    iSettingsRecordId;
        
       	/**
         * service id for the current characteristic.
         */
     	TInt       iServiceProviderId;
     	
        /**
         * Application ID of the current characteristic.
         * Owned
         */
        HBufC* iAppId;
        
         /**
         * pointer to Application ID of the current characteristic.
         */
        TPtrC iAppIdPtr;
        
        /**
         * xmppserveraddress of the current characteristic.
         * Owned
         */
        HBufC* iXmppServerAddress;
        /**
         * protocol id of the current characteristic.
         * Owned
         */
        HBufC* iProtocol;
        /**
         * Sanp id of the current characteristic.
         */
        TInt iSnapId;
        
        /**
         * ssl of the  current characteristic.
         */
        TInt iSSL;
        
        /**
         * port number of the current characteristic.
         */
        TInt iPortNbr;
        
        /**
         * Domain name the current characteristic.
         * Owned
         */
        HBufC* iDomainName;
        
        /**
         * BranId for the service specific icons.
        */
        HBufC* iBrandId;
        
        /**
         * Reference Name.
        */
        HBufC* iAppref;
        
        /**
         * Default text 
         */ 
        HBufC* iDefaultName;
        
        /**
         * Title 
         */ 
        HBufC* iTitle;
        /**
         * Confirms whether APPID is correct (w9033).
         */
        TBool iIsAppIdOk;
        
        /*
         * the user viewable name of general XMPP settings.
         */ 
        HBufC* iAppName;
        
        /*
         * Resource file to read
         */ 
        RResourceFile iResFile;

    };

#endif  // C_XMPPADAPTER_H

// End of File
