/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

#include "mvimpstenginecontactmgmtextention.h"

class contactmanagementextention_stub : public MVIMPSTEngineContactMgmtExtention
	{
	public:
	
	static MVIMPSTEngineExtentionFeatures* NewL();
	
	TVIMPSTEnums::FeatureSupport IsSupported() const ;

	/**
	* SetSupported
	* @param TVIMPSTEnums::FeatureSupport defined in tvimpstenums.h
	*/
	void SetSupported(TVIMPSTEnums::FeatureSupport aSupported );


	/**
	* Type of the Extention Feature
	* @return TVIMPSTEnums::ExtentionType defined in tvimpstenums.h
	*/
	TVIMPSTEnums::ExtentionType Type() const ;
	

    TInt AddServerContactL(const TDesC& aContactList, 
         						const TDesC& aUserId,const TDesC& aNickname );
        
        
    TInt DeleteServerContactL( const TDesC& aContactListId, 
         							const TDesC& aUserId ) ;
	


	};

//end of file
