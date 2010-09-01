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
* Description:  Implementation for cvimpstuicontactselection ,a contact selection class from pbk
*
*/


// INCLUDE FILES

#ifndef CVIMPSTUICONTACTSELECTION_H_
#define CVIMPSTUICONTACTSELECTION_H_

#include <e32base.h>

//forward declaration.
class CVIMPSTUiTabbedView;

// CLASS DECLARATION
/**
 * contacselection implementation 
 *  @lib vimpstui.lib
 *  @since S60 v5.0 
 */

class CVIMPSTUiContactSelection : public CActive
{
public:
    /**
     * Creates a new instance of this class.
     * @param  iViewObserver ,reference to tabbed view caller class      
     * @return  A new instance of this class.
     */
    static CVIMPSTUiContactSelection* NewL(CVIMPSTUiTabbedView& iViewObserver );
    /**
     * destructor
     */
    ~CVIMPSTUiContactSelection();
    
     /**
     * helper method to issue request.
     */
    void IssueRequest();
    
    
private:
    
    /**
     * default constructor.
     * @param  iViewObserver ,reference to tabbed view caller class   
     */
    CVIMPSTUiContactSelection(CVIMPSTUiTabbedView& iViewObserver );
      
private: // From CActive
    void RunL();
    void DoCancel();
    TInt RunError( TInt aError );
    
private://member variables
    
    //doesnt own - reference to tabbed view 
    // for notification about request completion
    CVIMPSTUiTabbedView& iViewObserver;
    
};


#endif /* CVIMPSTUICONTACTSELECTION_H_ */
