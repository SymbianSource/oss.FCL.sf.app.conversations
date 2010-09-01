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

@ECHO OFF
SETLOCAL

SET INPUT_FILE=doinstall_install.dat
SET TEMP_DIR=%EPOCROOT%epoc32\winscw\c\data\Installs\test\
SET INSTALLER_BIN=%EPOCROOT%epoc32\release\winscw\udeb\hspsThemeInstallerCons.exe
SET INSTALLER_LOG=%EPOCROOT%epoc32\winscw\c\Logs\hspsThemeInstallerCons\hspsThemeInstallerCons.log
SET ERR=0

if exist %TEMP_DIR% rmdir %TEMP_DIR% /s /q
xcopy * %TEMP_DIR% /s /q
%INSTALLER_BIN% %INPUT_FILE%
if %ERRORLEVEL% NEQ 0 SET ERR=1
if exist %INSTALLER_LOG% type %INSTALLER_LOG%
if %ERR% NEQ 0 echo Failed!
echo Please move generated files from \epoc32\winscw\c\private\200159c0\themes\ path to the ROM drive!

:end
ENDLOCAL
