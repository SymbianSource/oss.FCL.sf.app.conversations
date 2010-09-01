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
* Description:  Icon file provider.
 *
*/



#ifndef CIMCVUIAPPICONFILEPROVIDER_H
#define CIMCVUIAPPICONFILEPROVIDER_H

//  INCLUDES
#include <e32base.h>
#include <AknsItemID.h>
#include <AknIconUtils.h>


// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  Icon file provider.
 *  See MAknIconFileProvider.h for comments.
 *
 *  @lib chat.app
 *  @since 3.0
 */
class CIMCVUiAppIconFileProvider : public CBase, public MAknIconFileProvider
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CIMCVUiAppIconFileProvider* NewL( RFile& aFile );

        /**
         * Destructor.
         */
        virtual ~CIMCVUiAppIconFileProvider();

    public: // Functions from base classes

        /**
         * @see MAknIconFileProvider
         */		                       
        void RetrieveIconFileHandleL(
                RFile& aFile, const TIconFileType aType );

        /**
         * @see MAknIconFileProvider
         */
        void Finished();

    private:

        /**
         * C++ default constructor.
         */
        CIMCVUiAppIconFileProvider( );

        /**
         * By default Symbian 2nd phase constructor is private.
         */
		void ConstructL(RFile& aFile);

    private:    // Data
               
        RFile iFile;
    };

#endif      // CIMCVUIAPPICONFILEPROVIDER_H

// End of File
