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
* Description:  Con file provider for VIMPSTUI
*
*/

#include "cvimpstuiiconfileprovider.h"     

   
// -----------------------------------------------------------------------------
// CVIMPSTUiconFileProvider::CVIMPSTUiconFileProvider
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        
CVIMPSTUiconFileProvider::CVIMPSTUiconFileProvider( RFile& aFile ) 
: iFile( aFile )
    {
    iFile.Duplicate( aFile );
    }
// -----------------------------------------------------------------------------
// CVIMPSTUiconFileProvider::~CVIMPSTUiconFileProvider
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
  
CVIMPSTUiconFileProvider::~CVIMPSTUiconFileProvider()
    {
    iFile.Close();
    }
// -----------------------------------------------------------------------------
// CVIMPSTUiconFileProvider::RetrieveIconFileHandleL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        
void CVIMPSTUiconFileProvider::RetrieveIconFileHandleL( RFile& aFile, const TIconFileType /*aType*/ )
    {
    aFile.Duplicate( iFile );
    }
// -----------------------------------------------------------------------------
// CVIMPSTUiconFileProvider::Finished
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CImContactsIconFileProvider::Finished()
    {
    delete this;
    }
    
