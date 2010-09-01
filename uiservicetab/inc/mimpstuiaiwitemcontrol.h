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
* Description: Interface to hide/unhide AIW menu in service tab
 *
*/

#ifndef MVIMPSTUIAIWITEMCONTROL_H
#define MVIMPSTUIAIWITEMCONTROL_H


class MVIMPSTUiAIWControl
    {
public:
    virtual void SetAIWItemDimmed(TInt aCommand, TBool aHide) = 0; 
    };

#endif // MVIMPSTUIAIWITEMCONTROL_H
