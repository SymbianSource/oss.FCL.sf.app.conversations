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
* Description:  Ccommand info
*
*/
#ifndef __CIMCVCOMMANDINFO_H__
#define __CIMCVCOMMANDINFO_H__

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class TResourceReader;

/**
 *  command info
 */
class CIMCVCommandInfo : public CBase
    {
    	
    public:    
        /**
         * Static factory function that performs the 2-phased construction.
         *
         * @param aReader Reference to resource reader to be used
         * @param aId Id of the component owning the command IDs
         * @param anewCommandIds New command IDs
         * @return New instance of this class
         */
        static CIMCVCommandInfo* NewLC( TResourceReader& aReader,
                                    TInt32 aPluginId,
        							TInt& aNewCommandIds );
        
        /**
         * Destructor.
         */
        ~CIMCVCommandInfo();
        
    public:
    
    	/**
    	 * Returns the plugin ID from command info
    	 *
    	 * @return The plugin ID related to this command info
    	 */
        TInt32 PluginId() const;
    	
    	/**
    	 * Gets the new command ID
    	 *
    	 * @return The new command ID
    	 */    	
    	TInt32 NewCommandId() const;

    	/**
    	 * Gets the old/original command ID
    	 *
    	 * @return The old/original command ID
    	 */
    	TInt32 OldCommandId() const;
		
    private:
    
    	/**
         * Constructor.
         */
        CIMCVCommandInfo( TInt32 aPluginId );
        
        /**
         * 2nd phase constructor.
         */
        void ConstructL( TResourceReader& aReader,
        					TInt& aNewCommandIds );        					        

    private: // Data
        // plugin id
        TInt32 iPluginId;
        
        //new command id form the command pool
		TInt32 iNewCommandId;	
		
		//old command id from the plugin
		TInt32 iOldCommandId;
    };
        

#endif // __CIMCVCOMMANDINFO_H__

// End of File
