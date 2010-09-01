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

#ifndef MyXMPPSettingApiStub_H
#define MyXMPPSettingApiStub_H
#include <e32base.h>

class CXmppSettingsApi : public CBase
    {

public:
	
	    /**
	    * Two-phased constructor.
	    */
	    static CXmppSettingsApi* NewL();

	    /**
	    * Two-phased constructor.
	    */
	    static CXmppSettingsApi* NewLC();

	    /**
	    * Destructors.
	    */
	    virtual ~CXmppSettingsApi();


public:

    /**
     * Creates new settings record.
     *
     * @since S60 5.0
     * @param aSettingsRecordId On return contains the new setting id.
     */
    IMPORT_C void CreateSettingsRecordL( TUint32& aSettingsRecordId );

    /**
     * Removes Settings record using given settings record id.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id to be removed.
     */
    IMPORT_C void RemoveSettingsRecordL( TUint32 aSettingsRecordId );

    /**
     * Returns default settings record id.
     *
     * @since S60 5.0
     * @param aSettingsRecordId On return contains the setting id.
     */
    IMPORT_C void DefaultSettingsRecordL( TUint32& aSettingsRecordId );

    /**
     * Fills the array with settings ids. If there is none, array
     * is zero length.
     *
     * @since S60 5.0
     * @param aArray Array is filled with setting ids.
     */
    IMPORT_C void GetSettingsRecordIdArrayL( RArray<TUint32>& aArray );

    /**
     * Sets value of the param. If ParamName is not found, it creates new.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id of interest.
     * @param aAttrName Parameters name.
     * @param aAttrValue Parameters value.
     */
    IMPORT_C void SetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             const TDesC& aAttrValue );

    /**
     * Sets value of the param. If ParamName is not found, it creates new.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id of interest.
     * @param aAttrName Parameters name.
     * @param aAttrValue Parameters value.
     */
    IMPORT_C void SetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             TInt aAttrValue );

    /**
     * Gets value of the param.
     * - KErrNotFound if setting collection or Param is not found.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id of interest.
     * @param aAttrName Parameters name.
     * @param aAttrValue On return, contains the param value.
     */
    IMPORT_C void GetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             TDes& aAttrValue );

    /**
     * Gets value of the param.
     * - KErrNotFound if setting collection or Param is not found.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id of interest.
     * @param aAttrName Parameters name.
     * @param aAttrValue On return, contains the param value.
     */
    IMPORT_C void GetParamL( TUint32 aSettingsRecordId,
                                const TDesC& aAttrName,
                                TInt& aAttrValue );

    /**
     * Removes this param from this Settings record.
     * - KErrNotFound If Settings record or param is not found.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id.
     * @param aAttrName Parameters name.
     */
    IMPORT_C void RemoveParamL( TUint32 aSettingsRecordId,
                                const TDesC& aAttrName );

	
	 /**
     * This method does checks before setting or getting values.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id to be changed.
     * @param aAttrName Parameters name.
     * @param aCreateColumnIfNotExist Should new column be created if it is not
     *                         founded (by aAttrName).
     * @param aTheKey On return contains key to be created/changed.
     */
    IMPORT_C void GetRepositoryKeyL( TUint32 aSettingsRecordId,
                       const TDesC& aAttrName,
                       TBool aCreateColumnIfNotExist,
                       TUint32& aTheKey );
 

private:
	
};


#endif // MyXMPPSettingApiStub_H
