@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem
@echo off
rem ** 
rem ** Execute this batch file when you want to reuse same configuration UIDs in other test configurations
rem **

SETLOCAL

set tmp_path="\epoc32\winscw\c\private\200159C0"
echo Processing %tmp_path%...
if exist %tmp_path% rmdir %tmp_path% /s /q

set tmp_path="\epoc32\release\winscw\udeb\z\private\200159C0"
echo Processing %tmp_path%...
if exist %tmp_path% rmdir %tmp_path% /s /q

set tmp_path="\epoc32\release\armv5\urel\z\private\200159C0"
echo Processing %tmp_path%...
if exist %tmp_path% rmdir %tmp_path% /s /q

set tmp_path="\epoc32\data\c\private\200159C0"
echo Processing %tmp_path%...
if exist %tmp_path% rmdir %tmp_path% /s /q

set tmp_path="\epoc32\data\Z\private\200159C0"
echo Processing %tmp_path%...
if exist %tmp_path% rmdir %tmp_path% /s /q

set tmp_path="\epoc32\data\Z\resource\homescreen"
echo Processing %tmp_path%...
if exist %tmp_path% rmdir %tmp_path% /s /q

del \epoc32\winscw\c\private\10202be9\persists\200159c9.cre
del \epoc32\winscw\c\private\10202be9\persists\10275104.cre

echo Done

ENDLOCAL