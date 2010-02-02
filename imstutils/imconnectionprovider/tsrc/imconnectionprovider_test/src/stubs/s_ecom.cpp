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
* Description:  Interface for identity object.
*
*/


#include <ECom.h>
#include "s_ximpclient.h"

TAny* REComSession::CreateImplementationL(
                            TUid aImplementationUid, 
                            TUid& aDtorIDKey)
      {
      	//return NULL;
      	return MXIMPClientStub::NewClientL();
      }
                            
	// CreateImplementationL - CI2
	TAny* REComSession::CreateImplementationL(
                            TUid aImplementationUid, 
														TInt32 aKeyOffset)
														{
												      	//return NULL;
												      	return MXIMPClientStub::NewClientL();
														}
							
	// CreateImplementationL - CI3
TAny* REComSession::CreateImplementationL(
                            TUid aImplementationUid, 
							TUid& aDtorIDKey, 
							TAny* aConstructionParameters)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
							
	// CreateImplementationL - CI4
TAny* REComSession::CreateImplementationL(
                            TUid aImplementationUid, 
							TInt32 aKeyOffset,
							TAny* aConstructionParameters)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
							
	// CreateImplementationL - CI5
TAny* REComSession::CreateImplementationL(
                            TUid aInterfaceUid, 
							TUid& aDtorIDKey,
							const TEComResolverParams& aResolutionParameters)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
							
	// CreateImplementationL - CI6
TAny* REComSession::CreateImplementationL(
                            TUid aInterfaceUid, 
							TInt32 aKeyOffset, 
							const TEComResolverParams& aResolutionParameters)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
							
	// CreateImplementationL - CI7
TAny* CreateImplementationL(
                            TUid aInterfaceUid, 
														TUid& aDtorIDKey, 
														TAny* aConstructionParameters, 
														const TEComResolverParams& aResolutionParameters)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
	// CreateImplementationL - CI8
TAny* REComSession::CreateImplementationL(
                            TUid aInterfaceUid, 
							TInt32 aKeyOffset, 
							TAny* aConstructionParameters, 
							const TEComResolverParams& aResolutionParameters)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
							
	// CreateImplementationL - CI9
TAny* REComSession::CreateImplementationL(
                            TUid aInterfaceUid, 
														TUid& aDtorIDKey, 
														const TEComResolverParams& aResolutionParameters,
														TUid aResolverUid)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
												
	// CreateImplementationL - CI10
TAny* REComSession::CreateImplementationL(
                            TUid aInterfaceUid, 
							TInt32 aKeyOffset,
							const TEComResolverParams& aResolutionParameters, 
							TUid aResolverUid)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
												
	// CreateImplementationL - CI11
TAny* CreateImplementationL(
                            TUid aInterfaceUid, 
							TUid& aDtorIDKey, 
							TAny* aConstructionParameters,
							const TEComResolverParams& aResolutionParameters, 
							TUid aResolverUid)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
												
	// CreateImplementationL - CI12
TAny* REComSession::CreateImplementationL(
                            TUid aInterfaceUid, 
							TInt32 aKeyOffset, 
							TAny* aConstructionParameters,
							const TEComResolverParams& aResolutionParameters,
							TUid aResolverUid)
							{
					      	//return NULL;
					      	return MXIMPClientStub::NewClientL();
							}
	
    // Destroy Implementation API declaration											
void REComSession::DestroyedImplementation(
                            TUid aDtorIDKey)
                            {
                            	
                            }



