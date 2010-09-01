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
* Description:  Settings Store implementation.
*
*/


#ifndef __CVIMPSTSETTINGSCENREP_H
#define __CVIMPSTSETTINGSCENREP_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CRepository;
class CVIMPSTSettings;


// CLASS DECLARATION

NONSHARABLE_CLASS( CVIMPSTSettingsCenRep ): public CBase
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         *          
         */
        static CVIMPSTSettingsCenRep* NewL( );
        static CVIMPSTSettingsCenRep* NewLC( );

        /**
         * Destructor.
         */
        virtual ~CVIMPSTSettingsCenRep();


    public:        
        /**
         * UpdateOldSAPL
         * @param aSAPSettings the pointer to respective sap
         */
        void UpdateOldSettingsL( TUint32& aServiceId, CVIMPSTSettings* aSettings );
        /**
         * GetSAPL
         * @param aSAPSettings the pointer to respective sap
         */
        void GetSettingsL( TUint32& aServiceId, CVIMPSTSettings* aSettings );
        /**
         * DoGetSAPL
         * @param aSAPSettings the pointer to respective sap
         */
        void DoGetSettingsL( TUint32& aServiceId, CVIMPSTSettings* aSettings );
        /**
         * StartOwnTransaction
         * @param aMode updates the store
         */
        TBool StartOwnTransaction( TInt aMode );
        /**
         * StartOwnTransaction
         * returns current repository
         */
        CRepository* Repository() const;  

    private: //Constructors

        /**
         * C++ default constructor.
         */
        CVIMPSTSettingsCenRep();

        /**
         * By default Symbian 2nd phase constructor is private.
         *
         * @param aPriority Priority for active object based store event
         *        notifier. This determines priority of notifying store
         *        event observers.
         */
        void ConstructL( );


    private:    // Data
        CRepository* iRepository; // Handle to central repository session (own)  
    };

#endif      // __CVIMPSTSETTINGSCENREP_H

// End of File
