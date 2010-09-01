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
* Description:  This plugin is loaded by the Communication Launcher
*
*/


#ifndef CIMCMSLAUNCHERPLUGIN_H
#define CIMCMSLAUNCHERPLUGIN_H

#include <e32std.h>
#include <cmscontactorimpluginbase.h>


/**
 * CImCmsLauncherPlugin
 *
 * Implementation of Communication Launcher Plugin
 *
 * @lib imcmslauncherplugin
 * @since s60 v5.0
 */
class CImCmsLauncherPlugin: public CCmsContactorImPluginBase
    {
public:
	
		/**
         * Two-phased constructor.         
         * @return Instance of this class
         */         
		 static CImCmsLauncherPlugin* NewL();
		 

		/**
		 * C++ default destructor.
		 */
		 virtual ~CImCmsLauncherPlugin();

private:
	
		/**
		 * C++ default constructor.
		 */
    	 CImCmsLauncherPlugin();    
    

public:

		// from base class MCmsContactorImPluginApi

	    /**
	     * Defined in a base class
	     */
	      
	    void ExecuteL( const TDesC& aXspId );
	    
    };

#endif // CIMCMSLAUNCHERPLUGIN_H
