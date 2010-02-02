/*
* s_presencefeatures.cpp Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence context.
*
*/


#include "s_presencefeatures.h"
#include "s_presentitygroups.h"
#include "s_ximppresenceauthorization.h"
#include "s_presencepublishing.h"
#include "s_presenceobjectfactory.h"
#include "s_presencewatching.h"

MPresenceAuthorizationStub gPresenceAuthorizationStub;
MPresencePublishingStub gMPresenceFeaturesStubPublishing;
MPresenceObjectFactoryStub gMPresenceFeaturesStubObjectFactory;
MPresenceWatchingStub gMPresenceFeaturesStubPresenceWatching;

/**
 * Factory method to instantiate MPresenceFeatures.
 *
 * Factory method to instantiate platform default
 * MXIMPClient implementation through the ECom.
 * If the default presence framework isn't supported
 * in the platform, leaves with errorcode signalled
 * from ECom.
 *
 * @return The new presence client object. Object
 *         ownership is returned to caller.
 */
MPresenceFeatures* MyMPresenceFeatureStub::NewStubL( MXIMPContext* /*aContext*/ )
	{
	
	MyMPresenceFeatureStub* ret = new (ELeave) MyMPresenceFeatureStub();
	CleanupStack::PushL(ret);
	ret->ContructL();
	CleanupStack::Pop();
	return ret;	
	}

void MyMPresenceFeatureStub::ContructL()
	{
	iGroups = new (ELeave) MyPresentityGroupsStub();	
	}

/**
 * Public destructor.
 * Objects can be deleted through this interface.
 */
MyMPresenceFeatureStub::~MyMPresenceFeatureStub()
	{
	delete iGroups;	
	};


/**
 * Default constructor to zero initialize
 * the iEcomDtorID member.
 */
MyMPresenceFeatureStub::MyMPresenceFeatureStub()
	{
		
	}


/**
 * Gets reference to object factory interface.
 *
 * Returned object factory interface is used
 * to instantiate presence objects.
 *
 * @return Object factory interface.
 *         No ownership is returned caller.
 */
MPresenceObjectFactory& MyMPresenceFeatureStub::PresenceObjectFactory() const
	{
//	MPresenceObjectFactory* ret = NULL;
//	return *ret;
	return gMPresenceFeaturesStubObjectFactory;
	};


/**
 * Gets reference to presence watching interface.
 *
 * @return Requested interface.
 *         No ownership is returned caller.
 */
MPresenceWatching& MyMPresenceFeatureStub::PresenceWatching() const
	{
	//MPresenceWatching* ret = NULL;
	//return *ret;	
	return gMPresenceFeaturesStubPresenceWatching;
	};



/**
 * Gets reference to presence publishing interface.
 *
 * @return Requested interface.
 *         No ownership is returned caller.
 */
MPresencePublishing& MyMPresenceFeatureStub::PresencePublishing() const
	{
//	MPresencePublishing* ret = NULL;
//	return *ret;	
	return gMPresenceFeaturesStubPublishing;
	};



/**
 * Gets reference to presentity groups interface.
 *
 * @return Requested interface.
 *         No ownership is returned caller.
 */
MPresentityGroups& MyMPresenceFeatureStub::PresentityGroups() const
	{
	return *iGroups;	
	};


/**
 * Gets reference to presence authorization interface.
 *
 * @return Requested interface.
 *         No ownership is returned caller.
 */
MPresenceAuthorization& MyMPresenceFeatureStub::PresenceAuthorization() const
	{
	return gPresenceAuthorizationStub;
	};


const TAny* MyMPresenceFeatureStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MyMPresenceFeatureStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MyMPresenceFeatureStub::GetInterfaceId() const
    {
    return 0;
    }


