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

REM go to widget directory and store a list of widget .dat files
dir /B install\%1\xuikon\00\*.dat > install_temp\widget_temp.txt

REM go through the temporary file and call install on all xuikon themes
FOR /F "tokens=1 delims=" %%i in (install_temp\widget_temp.txt) do call themeinstaller.bat install\%1\xuikon\00\%%i install_temp

REM find odt files+resources <and copy them to the hsps directory
dir /s /B /A-D install_temp\epoc32\*.* > install_temp\odt_temp.txt

REM copy odt files to the widget\hsps directory
FOR /F "tokens=1 delims=" %%j in (install_temp\odt_temp.txt) do xcopy /F /R %%j install\%1\xuikon\00\

REM delete the odt files from the temporary location
FOR /F "tokens=1 delims=" %%j in (install_temp\odt_temp.txt) do del %%j

del install_temp\widget_temp.txt
del install_temp\odt_temp.txt
