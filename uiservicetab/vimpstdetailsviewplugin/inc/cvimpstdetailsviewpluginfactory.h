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
* Description:  View plugin base class of ECE's CCA-application
 *
*/
#ifndef CVIMPSTDETAILSVIEWPLUGINFACTORY_H_
#define CVIMPSTDETAILSVIEWPLUGINFACTORY_H_

#include <e32base.h>
#include <mccapluginfactory.h>

/**
 *  Class implementing MCcaPluginFactory interface.
 */

// High codescanner warning ignored. do not derive this class from CBase
// deriving this class from CBase will cause a crash. to be analysed
class CVIMPSTDetailsViewPluginFactory:  public MCcaPluginFactory,
                                        public CBase
                                        
        {
public:

    /**
     * Create interface
     */
    //static MCcaPluginFactory* NewL();
    static CVIMPSTDetailsViewPluginFactory* NewL();

    /**
     * From MCcaPluginFactory
     */
    TInt TabViewCount();

    /**
     * From MCcaPluginFactory
     */
    CCCAppViewPluginBase* CreateTabViewL( TInt aIndex );
    
    /*
		* destructor
		*/	
    ~CVIMPSTDetailsViewPluginFactory();  
 

private:

    /*
     * constructor
     */
    CVIMPSTDetailsViewPluginFactory();
    

    /*
     * 2- Phase constructor
     */
    void ConstructL();

    /**
     * Helper function for reading the service id from sttings
     * @since S60 v5.0
     */
    TInt ReadServiceIdL();

private:
    /**
     * Owns : service id to use
     */
    RArray< TInt32 > iServiceIds;
    TInt iServiceId;
        };
  


#endif /* CVIMPSTDETAILSVIEWPLUGINFACTORY_H_ */
