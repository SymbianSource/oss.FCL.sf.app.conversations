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

SET PATH_LINT=C:\Apps\libxml2-2.6.23.win32\bin\xmllint.exe
SET PATH_MANIFEST_XSD=hsps_manifest.xsd
SET PATH_DEFINITION_XSD=hsps_definition.xsd

IF NOT EXIST %PATH_MANIFEST_XSD% GOTO ERR_XSD
IF NOT EXIST %PATH_DEFINITION_XSD% GOTO ERR_XSD

SET ARGS_MANIFEST=--schema %PATH_MANIFEST_XSD% --noout
SET ARGS_DEFINITION=--schema %PATH_DEFINITION_XSD% --noout

echo.
echo Validating AI3 against the HSPS schema files...
echo.
%PATH_LINT% .\install\wideimage\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\wideimage\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\onerow\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\onerow\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\tworows\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\tworows\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\threerows\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\threerows\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\threetextrows\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\threetextrows\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\organizer\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\organizer\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\root\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\root\hsps\rootconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\shortcut\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\shortcut\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\shortcut2\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\shortcut2\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\view\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\view\hsps\viewconfiguration.xml %ARGS_DEFINITION%

%PATH_LINT% .\install\empty\hsps\manifest.dat %ARGS_MANIFEST%
%PATH_LINT% .\install\empty\hsps\widgetconfiguration.xml %ARGS_DEFINITION%

GOTO END

:ERR_LINT
ECHO XMLLint was not found! Personlize the PATH_LINT variable in %0 and download the tool from HSPS's support pages.
GOTO END

:ERR_XSD
ECHO Schemas were not found at %PATH_MANIFEST_XSD% and %PATH_DEFINITION_XSD%!
GOTO END

:END

ENDLOCAL