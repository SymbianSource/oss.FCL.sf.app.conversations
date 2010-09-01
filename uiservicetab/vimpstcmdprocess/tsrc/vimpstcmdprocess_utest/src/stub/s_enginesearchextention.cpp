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

#include "s_enginesearchextention.h"
#include "tvimpstenums.h"

MVIMPSTEngineSearchMgrExtention* searchextention_stub::NewL()
{
	MVIMPSTEngineSearchMgrExtention* extFeature = new(ELeave) searchextention_stub;
	return extFeature ;
}

TVIMPSTEnums::FeatureSupport searchextention_stub::IsSupported() const 
{
	return TVIMPSTEnums::ESupported;
}

/**
* SetSupported
* @param TVIMPSTEnums::FeatureSupport defined in tvimpstenums.h
*/
void searchextention_stub::SetSupported(TVIMPSTEnums::FeatureSupport /*aSupported*/ )
{
	
}


/**
* Type of the Extention Feature
* @return TVIMPSTEnums::ExtentionType defined in tvimpstenums.h
*/

TVIMPSTEnums::ExtentionType searchextention_stub::Type() const
{
	return TVIMPSTEnums::ESearch;
}

void searchextention_stub::RegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* /*aObserver*/)
{
	
}


void  searchextention_stub::UnRegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* /*aObserver*/ ) 
{
	
}

TInt searchextention_stub::SearchContactsL( RArray<TVIMPSTSearchKeyData>& /*aKeyDataArray*/ ) 
{
	return 1;
}
 
TInt searchextention_stub::SubscribeForSearchKeysL() 
{
	return 1;
}


//end of file
