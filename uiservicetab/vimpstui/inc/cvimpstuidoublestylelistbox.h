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
* Description:  Custom listbox component for the contact list
*
*/


#ifndef _CVIMPSTUIDOUBLESTYLELISTBOX_H
#define _CVIMPSTUIDOUBLESTYLELISTBOX_H

#include <aknjavalists.h>

// FORWARD DECLARATIONS
class MVIMPSTProcessArray;
class CVIMPSTUiListBoxModel;

//CLASS DECLARATION
/**
 *   double style / 2 line Custom listbox component for the contact list
 *
 *  Allows to change the model for the listbox.
 *  This is done because we have to customize the behaviour of the find pane,
 *  so that filtered list shows also the contact lists where the searched 
 *  contacts exist.
 *
 *  @lib vimpstui.dll
 *  @since S60 v5.0
 */
class CVIMPSTUiDoubleStyleListBox : public CAknDoubleLargeStyleListBox
                        
    {

public:

    /**
     * Creates the model
     * 
     */
    void CreateModelL();

    ~CVIMPSTUiDoubleStyleListBox();

public: // New functions

    /**
    * Give listbox a direct access to contact list model. 
    * @param aContactListModel Model of this listbox
    */
    void SetContactListModelL(MVIMPSTProcessArray& aItemModel );
private:   
	CVIMPSTUiListBoxModel* iListBoxModel;

    };


#endif // _CVIMPSTUIDOUBLESTYLELISTBOX_H
