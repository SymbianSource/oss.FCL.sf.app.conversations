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
* Description: inf file for conversations
*
*/

#ifndef VIMPSTCUSTOMCLEANUPAPI_H
#define VIMPSTCUSTOMCLEANUPAPI_H


/** 
* @publishedAll
* @released
* Template class CleanupResetAndDestroy to clean up the array
* of implementation information from the cleanup stack.
*/

template <class T>
class CustomCleanupResetAndDestroy
    {
 public:
    /**
     * Puts an item on the cleanup stack.
     * @param  aRef 
     * The implementation information to be put on the cleanup stack.
     */
   inline static void PushL(T& aRef);
private:
    static void ResetAndDestroy(TAny *aPtr);
    };

template <class T>
        inline void CleanupResetAndDestroyPushL(T& aRef);

template <class T>
         inline void CustomCleanupResetAndDestroy<T>::PushL(T& aRef)
    { CleanupStack::PushL(TCleanupItem(&ResetAndDestroy,&aRef));}

template <class T>
         void CustomCleanupResetAndDestroy<T>::ResetAndDestroy(TAny *aPtr)
    {(STATIC_CAST(T*,aPtr))->ResetAndDestroy();}

template <class T>
         inline void CustomCleanupResetAndDestroyPushL(T& aRef)
    {CustomCleanupResetAndDestroy<T>::PushL(aRef);}

#endif /*VIMPSTCUSTOMCLEANUPAPI_H*/
