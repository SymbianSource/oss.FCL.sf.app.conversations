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

#include "s_engineextentionfeatures.h"
#include "tvimpstenums.h"

MVIMPSTEngineExtentionFeatures* extentionfeatures_stub::NewL()
{
	MVIMPSTEngineExtentionFeatures * extFeature = new(ELeave) extentionfeatures_stub;
	return extFeature ;
}

TVIMPSTEnums::FeatureSupport extentionfeatures_stub::IsSupported() const 
{
	return TVIMPSTEnums::ESupported;
}

/**
* SetSupported
* @param TVIMPSTEnums::FeatureSupport defined in tvimpstenums.h
*/
void extentionfeatures_stub::SetSupported(TVIMPSTEnums::FeatureSupport /*aSupported*/ )
{
	
}


/**
* Type of the Extention Feature
* @return TVIMPSTEnums::ExtentionType defined in tvimpstenums.h
*/

TVIMPSTEnums::ExtentionType extentionfeatures_stub::Type() const
{
	return TVIMPSTEnums::EContactManagement;
}

/*
MVIMPSTEngineContactMgmtExtention& extentionfeatures_stub::Cast(
            MVIMPSTEngineExtentionFeatures& aExtentionFeature)
{
    __ASSERT_ALWAYS( aExtentionFeature.Type() == TVIMPSTEnums::EServerContactManagement,
        User::Panic(KErrNotFound) );
    return static_cast<MVIMPSTEngineContactMgmtExtention&>(aExtentionFeature);
}

    
const MVIMPSTEngineContactMgmtExtention& extentionfeatures_stub::Cast(
        const MVIMPSTEngineExtentionFeatures& aExtentionFeature)
{
    __ASSERT_ALWAYS( aExtentionFeature.Type() == TVIMPSTEnums::EServerContactManagement,
        User::Panic(KErrNotFound) );
    return static_cast<const MVIMPSTEngineContactMgmtExtention&>(aExtentionFeature);
}
  */    


TInt extentionfeatures_stub::AddServerContactL(const TDesC& /*aContactList*/, 
     						const TDesC& aUserId,const TDesC& /*aNickname*/,TBool /*aWaitToComplete */  ) 
{
	return KErrNotFound;
}
    
    
TInt extentionfeatures_stub::DeleteServerContactL( const TDesC& /*aContactListId*/, 
     							const TDesC& /*aUserId*/ ) 
{
	return KErrNotFound;
}


//end of file
