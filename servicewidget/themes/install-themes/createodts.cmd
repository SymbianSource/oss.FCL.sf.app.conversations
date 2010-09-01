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

REM ################################################################################
REM Xuikon/HSPS install script. Installs Xuikon themes (Creates ODT files)
REM and copies them under the HSPS projects with the same name.
REM 
REM The script will do the following:
REM 1. Go through the directory names under the install\ directory
REM 2. Find .dat files under each directory and call themeinstaller.bat on
REM    each of them.
REM 3. Copy the ODT-files from a temporary location to the widget directory\hsps\
REM
REM The directory structure should look like this:
REM install\
REM 	my_widget\
REM			hsps\     <<-- The HSPS Configuration files should be located here
REM			xuikon\   <<-- The Xuikon theme should be located here
REM ################################################################################

REM If "install" directory is not found, display an error
if not exist install\ (
goto error
)

REM create a directory for temporary files
if not exist install_temp (
mkdir install_temp
)

REM go to install directory and list all directory names (that should contain the widgets)
dir /AD /B install > install_temp\install_temp.txt

REM go through the temporary file and call install on all xuikon themes
FOR /F "tokens=1 delims=" %%i in (install_temp\install_temp.txt) do call installxuikontheme.cmd %%i

REM delete files in temporary location and delete the temporary epoc32 dir
rmdir /S /Q install_temp\

goto end

:error
echo "install" directory not found.

:end
REM call rename_localized_xuikon_odts.pl
echo Done.
