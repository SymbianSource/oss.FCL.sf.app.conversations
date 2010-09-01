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

#include "mvimpstenginesearchmgrextention.h"

class searchextention_stub : public MVIMPSTEngineSearchMgrExtention
	{
	public:

	static MVIMPSTEngineSearchMgrExtention* NewL();	
	
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
	

 /*  	static MVIMPSTEngineSearchMgrExtention& Cast(
                MVIMPSTEngineExtentionFeatures& aExtentionFeature);
                
    static const MVIMPSTEngineSearchMgrExtention& Cast(
                const MVIMPSTEngineExtentionFeatures& aExtentionFeature);   */


	void RegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* aObserver) ;

	void  UnRegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* aObserver ) ;

    TInt SearchContactsL( RArray<TVIMPSTSearchKeyData>& aKeyDataArray ) ;
     
    TInt SubscribeForSearchKeysL()  ;         
	


	};

//end of file
