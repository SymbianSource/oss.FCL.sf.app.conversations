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
* Description: Abstract interface for getting mapped command ids
*
*/

#ifndef __MIMCVCOMMANDMAPPER_H__
#define __MIMCVCOMMANDMAPPER_H__


// INCLUDES
#include <e32base.h>

// CLASS DECLARATION


/**
 *  Abstract interface for getting mapped command IDs. Command ID mapping
 *  means that xSP's own "Names List" command IDs ( constants that are usually 
 *  defined in a *.hrh file ) are substituted with IDs provided by Extension
 *  Manager. This is needed because there may be extensions using same 
 *  "Names List" command IDs. After the substitution, the original IDs are 
 *  not valid anymore. Still, in order to allow original "Names List"
 *  command IDs usage inside the xSP's own code, MxSPCommandMapper provides the
 *  mechanism for retrieving new IDs which correspond to the old original
 *  ones and vice versa.
 */
class MIMCVCommandMapper
    {
    public:
        /**
         *  Gets old original command ID which corresponds
         *  to the given new one
         *
         *  @param aPluginId  implementation ID for identification
         *  @param aNewCommand  New mapped command ID
         *  @param aOldCommand  On return old/original command ID
         *  @return KErrNone if mapping exist, otherwise KErrNotFound
         */
        virtual TInt GetOldCommand( TInt32 aPluginId, 
                                   TInt aNewCommand,
                                   TInt& aOldCommand ) const = 0;
                            
        /**
         *  Gets new mapped command ID which corresponds
         *  to the given old one
         *
         *  @param aPluginId  implementation ID for identification
         *  @param aOldCommand Old/original command ID
         *  @param aNewCommand On return new mapped command ID
         *  @return KErrNone if mapping exist, otherwise KErrNotFound
         */
        virtual TInt GetNewCommand( TInt32 aPluginId, 
                                    TInt aOldCommand,
                                    TInt& aNewCommand ) const = 0;
                                    
    protected: // Protected interface

        /**
         * Destructor.
         */
        virtual ~MIMCVCommandMapper()
                {};
                                    
    };


#endif // __MIMCVCOMMANDMAPPER_H__
