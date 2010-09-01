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
* Description: Header for Extension View info
*
*/


#ifndef C_VIMPSTUIEXTENSIONVIEWINFO_H
#define C_VIMPSTUIEXTENSIONVIEWINFO_H

#include <e32base.h>
#include <MxSPViewInfo.h>

/**
 *  Extension View Info
 *
 *  This class encapsulates xSP Extension View Info data
 *
 *  @lib vimpstui.lib
 *  @since S60 v5.0
 */
class CVIMPSTUIExtensionViewInfo : public CBase,   
                                    public MxSPViewInfo
    {

public:

    /**
     * Two-phased constructor.
     * @param aViewId, view id.
     * @param aIsTabbedView, ETrue if its a tabbed view, else EFlase.
     * @param aIconFile, reference to icon file used to display icon in the tab.
     * @param aIconId, icon id.
     * @param aMaskId, mask id.
     * @param aServiceName, name of the service.
     */
    static CVIMPSTUIExtensionViewInfo* NewL(
        TUint32 aViewId,
        TBool aIsTabbedView,
        const TDesC& aIconFile,
        TUint aIconId,
        TUint aMaskId,
        const TDesC& aServiceName );

    /**
     * Two-phased constructor.
     * @param aViewId, view id.
     * @param aIsTabbedView, ETrue if its a tabbed view, else EFlase.
     * @param aIconFile, reference to icon file used to display icon in the tab.
     * @param aIconId, icon id.
     * @param aMaskId, mask id.
     * @param aServiceName, name of the service.
     */
    static CVIMPSTUIExtensionViewInfo* NewLC(
        TUint32 aViewId,
        TBool aIsTabbedView,
        const TDesC& aIconFile,
        TUint aIconId,
        TUint aMaskId,
        const TDesC& aServiceName );
    
    /**
     * Standard C++ destructor
     */    
    ~CVIMPSTUIExtensionViewInfo();
    
public: //MxSPViewInfo

    /**
     * @see MxSPViewInfo
     * Returns old view id.
     *
     * @return, TInt32, old view id
     */    
    TInt32 OldViewId() const;

    /**
     * @see MxSPViewInfo
     * Returns ETrue if this is tabbed view
     *
     * @return, TBool, tabbed or not
     */    
    TBool TabView() const;
    
    /**
     * @see MxSPViewInfo
     * Returns tab icon id
     *
     * @return, TInt32, tab icon id
     */            
    TInt32 TabIconId() const;
    
    /**
     * @see MxSPViewInfo
     * Returns tab mask id
     *
     * @return, TInt32, tab mask id
     */                    
    TInt32 TabMaskId() const;

    /**
     * @see MxSPViewInfo
     * Returns tab icon file
     *
     * @return, TDesC&, file and path of icon file
     */                    
    const TDesC& TabIconFile() const;
    
    /**
     * @see MxSPViewInfo
     * Returns tab/service name
     *
     * @return, TDesC&, tab/service name
     */                            
    const TDesC& Name() const;
    
    /**
     * @see MxSPViewInfo
     * Returns sort icon id
     *
     * @return, TInt32, sort icon id
     */                                    
    TInt32 SortIconId() const;
    
    /**
     * @see MxSPViewInfo
     * Returns sort mask id
     *
     * @return, TInt32, sort mask id
     */                                     
    TInt32 SortMaskId() const;
    
    /**
     * @see MxSPViewInfo
     * Returns sort icon file.
     *
     * @return, TDesC&, sort icon file name/path
     */                                  
    const TDesC& SortIconFile() const;       

private: // Implementation

    /**
     * Standard C++ constructor.
     * @param aViewId, view id.
     * @param aIsTabbedView, ETrue if ita a tabbed view, else EFalse.
     * @parma aIconId, icon id.
     * @parma aMaskId, mask id.
     */    
    CVIMPSTUIExtensionViewInfo( TUint32 aViewId,
        TBool aIsTabbedView, TUint aIconId, TUint aMaskId );
    
    /**
     * Performs the 2nd phase of construction.
     * @param aIconFile, icon file path
     * @param aServiceName, name of the service
     */        
    void ConstructL( const TDesC& aIconFile,
        const TDesC& aServiceName );

private: // data

    // own, icon file path
    HBufC*  iIconFile;
    
    // Own, service name
    HBufC*  iServiceName;
    
    // View id
    TUint32 iViewId;
    
    // Is tabbed view info or not
    TBool   iIsTabbedView;
    
    // Icon id
    TUint   iIconId;
    
    // Mask id
    TUint   iMaskId;
    };

#endif // C_VIMPSTUIEXTENSIONVIEWINFO_H
