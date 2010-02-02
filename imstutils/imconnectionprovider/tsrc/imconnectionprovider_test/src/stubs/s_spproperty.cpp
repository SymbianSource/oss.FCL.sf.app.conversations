/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_spproperty.cpp
*
*/

#include <spproperty.h>

TServicePropertyName gPropertyName = EPropertyUnknown;
TInt gContactViewPluginId = 0;
HBufC* gPropertyContactStoreId = 0;
HBufC* gPropertyBrandId = 0;
TBool gPropertyInvalid = EFalse;
TInt gPropertyBrandLanguage = 0;
TInt gPropertyBrandVersion = 0;
TInt gPropertyXIMP = 0;
TOnOff gPropertyPresenceRequestPreference = EOff;
TInt gContactViewId = 0;
TInt gESubPropertyIMSettingsId = 0;

CSPProperty* CSPProperty::NewL()
    {
    return new ( ELeave ) CSPProperty;
    }


CSPProperty* CSPProperty::NewLC()
    {

    CSPProperty* self = new ( ELeave ) CSPProperty;
    CleanupStack::PushL( self );
    return self;
    }

CSPProperty::~CSPProperty(){}


TServicePropertyName CSPProperty::GetName() const
    {
    return iPropertyName;
    }


TInt CSPProperty::SetName( TServicePropertyName aPropertyname )
    {
    iPropertyName = aPropertyname;
    return KErrNone;
    }


TSPItemType CSPProperty::GetPropertyType() const{}


TInt CSPProperty::SetPropertyType( TSPItemType aPropertyType ){}


TPropertyDataType CSPProperty::GetDataType() const{}


void CSPProperty::SetDataType( TPropertyDataType aDataType ){}


TInt CSPProperty::GetValue( TInt& aValue ) const
    {
    if( gPropertyInvalid )
        {
        return KErrNotFound;
        }

    if( GetName() == EPropertyContactViewPluginId )
        {
        aValue = gContactViewPluginId;
        } 
	else if( GetName() == EPropertyBrandLanguage )
        {
        aValue = gPropertyBrandLanguage;
        } 
	else if( GetName() == EPropertyBrandVersion )
        {
        aValue = gPropertyBrandVersion;
        }
	else if( GetName() == EPropertyPCSPluginId )
        {
        aValue = gPropertyXIMP;
        }  
	else if( GetName() == EPropertyContactViewId )
        {
        aValue = gContactViewId;
        }  
	else if( GetName() == ESubPropertyIMSettingsId )
        {
        aValue = gESubPropertyIMSettingsId;
        }  
        
    return KErrNone;
    }

TInt CSPProperty::SetValue( TInt aValue )
    {
    if( GetName() == EPropertyContactViewPluginId )
        {
        gContactViewPluginId = aValue;
        }    
	else if( GetName() == EPropertyBrandLanguage )
        {
        gPropertyBrandLanguage = aValue;
        } 
	else if( GetName() == EPropertyBrandVersion )
        {
        gPropertyBrandVersion = aValue;
        } 
	else if( GetName() == EPropertyPCSPluginId )
        {
        gPropertyXIMP = aValue;
        } 
	else if( GetName() == EPropertyContactViewId )
        {
        gContactViewId = aValue ;
        }
	else if( GetName() == ESubPropertyIMSettingsId )
        {
        gESubPropertyIMSettingsId = aValue;
        }                          
    return KErrNone;
    }


TInt CSPProperty::GetValue( TDes& aValue ) const
	{
	if( gPropertyInvalid )
        {
        return KErrNotFound;
        }
	if( GetName() == EPropertyContactStoreId )
        {
        if (gPropertyContactStoreId)
	        {
	        aValue.Copy(*gPropertyContactStoreId);
	        }
        }    
    else if( GetName() == EPropertyBrandId )
        {
        if (gPropertyBrandId)
	        {
	        aValue.Copy(*gPropertyBrandId);
	        }
        }   
	
	return KErrNone;        
	}

TInt CSPProperty::SetValue( const TDesC& aValue )
	{
	if( GetName() == EPropertyContactStoreId )
        {
		
        if (gPropertyContactStoreId)
	        {
	        delete gPropertyContactStoreId;
	        gPropertyContactStoreId = NULL;	
	        }
	    
	    gPropertyContactStoreId = aValue.Alloc();
        }    
    else if( GetName() == EPropertyBrandId )
        {
        if (gPropertyBrandId)
	        {
	        delete gPropertyBrandId;
	        gPropertyBrandId = NULL;	
	        }
        gPropertyBrandId = aValue.Alloc();        
        }   
	
	return KErrNone;
	}

TInt CSPProperty::GetValue( TOnOff& aValue ) const
	{
	if( gPropertyInvalid )
        {
        return KErrNotFound;
        }

    if( GetName() == ESubPropertyPresenceRequestPreference )
        {
        aValue = gPropertyPresenceRequestPreference;
        }	    
	
	return KErrNone;
	}


TInt CSPProperty::SetValue( TOnOff aValue )
	{
	if( gPropertyInvalid )
        {
        return KErrNotFound;
        }

    if( GetName() == ESubPropertyPresenceRequestPreference )
        {
        gPropertyPresenceRequestPreference = aValue;
        }	    
	
	return KErrNone;
	}


TInt CSPProperty::SetValueNoDataTypeCheck( const TDesC& aValue ){}


TPropertyDataType CSPProperty::DataType( TServicePropertyName aProperty ){}


TSPItemType CSPProperty::PropertyType( TServicePropertyName aProperty ){}


void CSPProperty::CopyL( const CSPProperty& aSource )
    {
    iPropertyName = aSource.GetName();    
    
    if( GetName() == EPropertyContactStoreId )
        {
        HBufC* value = HBufC::NewLC(500);        
        TPtr valuePtr = value->Des();
        aSource.GetValue(valuePtr);
        SetValue( valuePtr );
        
        CleanupStack::PopAndDestroy(value);        
        }    
    else if( GetName() == EPropertyBrandId )
        {
        HBufC* value = HBufC::NewLC(500);        
        TPtr valuePtr = value->Des();
        aSource.GetValue(valuePtr);
        SetValue( valuePtr );
        
        CleanupStack::PopAndDestroy(value); 
        }   
    else  if( GetName() == EPropertyContactViewPluginId )
	    {
	    TInt value = 0;
	    aSource.GetValue( value );
	    SetValue( value );	
	    }   
    
    }


TBool CSPProperty::CSPProperty::operator==( 
            const CSPProperty& aProperty ) const{}

CSPProperty::CSPProperty(){}

void CSPProperty::ConstructL(){}


TInt SetDefaultValue( TServicePropertyName aProperty ){}
