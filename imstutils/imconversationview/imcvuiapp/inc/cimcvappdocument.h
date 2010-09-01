/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  document part of avkon
*
*/

#ifndef CIMCVAPPDOCUMENT_H
#define CIMCVAPPDOCUMENT_H


#include <AknDoc.h>
#include <apgwgnam.h>
// Forward references
class CIMCVAppUi;
class CEikApplication;
class CEikAppUi;

/*! 
  @class CIMCVAppDocument
  
  @discussion An instance of class CIMCVAppDocument is the Document part of the AVKON
  application framework for the conversationview example application
  */
class CIMCVAppDocument : public CAknDocument
    {
public:

/*!
  @function NewL
  
  @discussion Construct a CIMCVAppDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CIMCVAppDocument
  */
    static CIMCVAppDocument* NewL(CEikApplication& aApp);

/*!
  @function NewLC
  
  @discussion Construct a CIMCVAppDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CIMCVAppDocument
  */
    static CIMCVAppDocument* NewLC(CEikApplication& aApp);

/*!
  @function ~CIMCVAppDocument
  
  @discussion Destroy the object and release all memory objects
  */
    ~CIMCVAppDocument();

public: // from CAknDocument
/*!
  @function CreateAppUiL 
  
  @discussion Create a CIMCVAppUi object and return a pointer to it
  @result a pointer to the created instance of the AppUi created
  */
    CEikAppUi* CreateAppUiL();
    
 /**
    * This method makes an application hidden so that it is not visible
    * for example in Applications list and FastSwap window.
    * @param aWgName ,aWgName
    * @return void        
    */
    void UpdateTaskNameL( CApaWindowGroupName* aWgName );

private:

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CIMCVAppDocument object
  */
    void ConstructL();

/*!
  @function CIMCVAppDocument
  
  @discussion Perform the first phase of two phase construction 
  @param aApp application creating this document
  */
    CIMCVAppDocument(CEikApplication& aApp);

    };


#endif // CIMCVAPPDOCUMENT_H
