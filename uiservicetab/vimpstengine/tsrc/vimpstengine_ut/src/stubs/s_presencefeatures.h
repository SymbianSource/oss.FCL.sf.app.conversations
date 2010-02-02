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
* Description:  Interface for presence context.
*
*/
#ifndef MyMPresenceFeatureStub_H
#define MyMPresenceFeatureStub_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpcontext.h>

#include <presencefeatures.h>

class MyPresentityGroupsStub;

/**
 * Interface for presence features.
 * This interface is implemented by the XIMP Framework Presence Feature plug-in.
 *
 * MPresenceFeatures is root interface for accessing
 * and updating presence data to remote presence service.
 * XIMP FW client binds MXIMPContext interface
 * to desired remote presence service and creates
 * MPresenceFeatures interface to access the XIMP FW presence related 
 * methods to publish and access presence data.
 *
 * Following sub interfaces can be accessed from
 * from MPresenceFeatures:
 *   - MPresenceObjectFactory
 *   - MPresenceWatching
 *   - MPresencePublishing
 *   - MPresentityGroups
 *   - MPresenceAuthorization
 *
 * All Presence events (context state, presence data,
 * request completion etc. ones) are delivered through
 * MPresenceContextObserver interface.
 *
 * @ingroup ximppresmanagementapi
 * @since S60 v3.2
 */
class MyMPresenceFeatureStub : public CBase, public MPresenceFeatures
    {

public:

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
    static MPresenceFeatures* NewStubL( MXIMPContext* aContext );

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    ~MyMPresenceFeatureStub();
    
protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    MyMPresenceFeatureStub();
  
private:
	  
    void ContructL();

public: //Sub interfaces for presence context


    /**
     * Gets reference to object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate presence objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    MPresenceObjectFactory& PresenceObjectFactory() const ;


    /**
     * Gets reference to presence watching interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    MPresenceWatching& PresenceWatching() const ;



    /**
     * Gets reference to presence publishing interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    MPresencePublishing& PresencePublishing() const ;



    /**
     * Gets reference to presentity groups interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    MPresentityGroups& PresentityGroups() const ;


    /**
     * Gets reference to presence authorization interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    MPresenceAuthorization& PresenceAuthorization() const ;
    
    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;


	private:
		MyPresentityGroupsStub* iGroups;
		
    };




#endif // MyMPresenceFeatureStub_H



