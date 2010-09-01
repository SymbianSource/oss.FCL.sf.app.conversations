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
* Description: implements the request mapper
*
*/

// INCLUDE FILES

#ifndef CVIMPSTENGINEREQUESTMAPPER_H__
#define CVIMPSTENGINEREQUESTMAPPER_H__

#include <e32base.h>
#include <ximpbase.h>
#include "tvimpstenums.h"

// farword declaration
class CVIMPSTEngineRequest;

/**
 *  This class implements the request mapper
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS(CVIMPSTEngineRequestMapper) :public CBase
	{

	public:  // Two-phased constructors and destructor

        
       /**
		* Two-phased constructor.
		*/
		static CVIMPSTEngineRequestMapper* NewL();


       /**
		* Two-phased constructor.
		*/
		static CVIMPSTEngineRequestMapper* NewLC();

		/**
		* destructor
		*/
		virtual ~CVIMPSTEngineRequestMapper();

		/**
		* CreateRequestL  
		* create request and start wait if required 
		* @param aRequestId for request
		* @param aIsWait, wait or not
		* @param aType - Request Type defined in tvimpstenums.h
		* @return pointer to created CVIMPSTEngineRequest
		*/
		CVIMPSTEngineRequest* CreateRequestL(TXIMPRequestId& aRequestId, 
										TBool aIsWait,
										TXimpOperation aType) ;


		/**
		* FindRequestId   
		* find the request id
		* @param aRequestId to find
		* @return the object of found CVIMPSTEngineRequest. If request not found
		*  this return value contains NULL. The ownership is not transferred to the caller
		*/
		CVIMPSTEngineRequest* FindRequestId(const TXIMPRequestId& aRequestId ) ;

		/**
		* RemoveRequestId 
		* Removes the object identified by aRequestId from request array and deletes the handle.
		* Client must call this method every time XIMP operation has finished.
		* If matching object is not found, this function does nothing.
		* @param aRequestId, id to be matched to request handles.
		* @return None
		*/      
		void RemoveRequestId(const TXIMPRequestId& aRequestId) ;

	private:

		/**
		* constructor
		*/
		CVIMPSTEngineRequestMapper();

	private:
		// own , array of CVIMPSTEngineRequest
		RPointerArray< CVIMPSTEngineRequest > iRequestArray;

};

#endif //CVIMPSTENGINEREQUESTMAPPER_H__
// end of file
