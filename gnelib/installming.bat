@echo off
REM ----------------------------------------------------------------------------
echo.
echo During this installer, when asked a question, press y or n to respond.
echo Any response besides y will be counted as a negative response.
echo You may press CTRL-C to abort the installer at any time.
echo The readme.vc file contains information about the MSVC install process.
echo.

:checkvcvars
if "%MINGDIR%"=="" goto mingvarserr
call fixming
echo You may use these commands outside of the installer if you prefer to use
echo  only the traditional makefile system.
echo.

REM **** CHECK FOR HAWKNL INSTALL ****

if NOT exist "%MINGDIR%\include\nl.h" goto hawknotfound
if NOT exist "%MINGDIR%\lib\libNL.a" goto hawknotfound

echo I have found an installation of HawkNL. The version is:
find "NL_VERSION_STRING" "%MINGDIR%\include\nl.h" | find "NL_VERSION_STRING"
if errorlevel 1 goto othererr
echo.
echo GNE requires HawkNL version 1.6 or later.  A GNE
echo distribution comes with a precompiled binary version of HawkNL.  Would you
echo like to see more info about the precompiled version?
yesno
if errorlevel 2 goto userabort
if errorlevel 1 goto askreinsthawk
@echo on
notepad hawk_bin\HawkNL.txt
@echo off
:askreinsthawk
echo Would you like to reinstall HawkNL from GNE's version, overwriting your
echo old HawkNL installation?
yesno
if errorlevel 2 goto userabort
if errorlevel 1 goto hawkcont
echo.
goto insthawk

:hawkcont
echo.
REM **** CHECK TO SEE IF GNE IS COMPILED ****
if exist "lib\mingw\libgne.a" goto instgne
if NOT exist "lib\mingw\libgned.a" goto compilegne

:instgne
echo.
dir lib\mingw\libgne.a | find "libgne.a"
if exist "lib\mingw\libgne.a" echo release version of GNE found in lib\mingw!
dir lib\mingw\libgned.a | find "libgned.a"
if exist "lib\mingw\libgned.a" echo debug version of GNE found in lib\mingw!
echo.
echo I see you have a compiled version of GNE.  If you wish, you may now
echo recompile your version of GNE.  Would you like to do this?
yesno
echo.
if errorlevel 2 goto userabort
if NOT errorlevel 1 goto recompilegne

REM **** INSTALL GNE ****
echo I can now install GNE into your mingw directory.  I see your ming root is:
echo %MINGDIR%
echo If the root is incorrect, or you do not want to install, press ctrl-c now.
pause

if NOT exist "lib\mingw\libgne.a" goto instdebug
@echo on
copy /-Y "lib\mingw\libgne.a" "%MINGDIR%\lib"
@echo off
if errorlevel 1 goto copyerr

if NOT exist "lib\mingw\libgned.a" goto instinclude
:instdebug
@echo on
copy /-Y "lib\mingw\libgned.a" "%MINGDIR%\lib"
@echo off
if errorlevel 1 goto copyerr

:instinclude
@echo on
copy include\gnelib.h "%MINGDIR%\include"
@echo off
if errorlevel 1 goto copyerr
@echo on
mkdir "%MINGDIR%\include\gnelib"
copy include\gnelib\*.* "%MINGDIR%\include\gnelib"
@echo off
if errorlevel 1 goto copyerr

REM **** INSTALL DOCS ****
echo.
echo Looking for doxygen.  If you see a short error below, disregard it.
doxygen > nul
if errorlevel 255 goto explaindoxy
echo.
echo Doxygen successfully located.
echo Using Doxygen, you can (re)generate the GNE documentation and beginner's
echo guide in HTML (and/or other formats, by modifying the doxygen.cfg).
echo Do you wish to (re)generate the documentation?
yesno
if errorlevel 1 goto installfinished
call udocs.bat
goto installfinished

:installfinished
echo.
echo GNE has now been successfully installed!  You may now use GNE.  See the
echo readme.mingw file for more information.  This installer may be run again
echo at ANY time to install or reinstall GNE and/or HawkNL.
goto end

REM ----------------------------------------------------------------------------
REM ----------- PROCEDURES -----------
:insthawk
echo I am now going to install HawkNL for you into your mingw directories,
echo which consists of nl.h and libNL.a.  I see that your mingw root is:
echo %MINGDIR%
echo If the root is incorrect, or you do not want to install, press ctrl-c now.
pause
echo.
@echo on
copy /-Y hawk_bin\nl.h "%MINGDIR%\include"
@echo off
if errorlevel 1 goto copyerr
@echo on
copy /-Y hawk_bin\mingw1\libNL.a "%MINGDIR%\lib"
@echo off
if errorlevel 1 goto copyerr
echo.
echo I can also install the NL.dll file into your system directory if you
echo desire.  This can make development easier by not having to copy the DLL
echo into every project you create.  Note that when you distribute your final
echo product, the DLL should probably go into the directory of your program.
echo Would you like to install the HawkNL.dll into your system directory?
yesno
if errorlevel 2 goto userabort
if NOT errorlevel 1 goto insthawkdll
:dllinstret
goto hawkcont

:insthawkdll
if "%windir%"=="" goto windirerr
@echo on
copy /-Y "hawk_bin\mingw1\NL.dll" "%windir%\system"
@echo off
if errorlevel 1 goto copyerr
goto dllinstret

:compilegne
make
goto hawkcont

:recompilegne
make veryclean
make
goto hawkcont

:explaindoxy
echo Doxygen was not found!
echo The documentation for GNE is generated by a program called doxygen, which
echo generates HTML from specially designed comments in the source code.  This
echo documentation also includes a beginner's guide.  You can find doxygen at:
echo.
echo http://www.stack.nl/~dimitri/doxygen/index.html
echo.
echo The documentation can also be viewed online at the GNE website if you do
echo not wish to install doxygen.
echo.
echo (If you have doxygen, but I didn't find it, I was only able to look in
echo  your path for it.  You can either run doxygen manually, insert the
echo  directory doxygen is in into your path, or modify this installer by
echo  placing the path for doxygen at the appropriate point).
goto installfinished

REM ----------------------------------------------------------------------------
REM ----------- ERROR HANDLERS -----------
:mingvarserr
echo The mingw environment variable MINGDIR was not found.
echo This installer requires the MINGDIR variable pointing to the base of your
echo mingw installation for installation to continue.
goto end

:hawknotfound
echo I could not find a complete HawkNL installation.  GNE requires the
echo HawkNL library.  Fortunately the GNE distribution comes with a
echo precompiled version of HawkNL.  If you wish I can launch notepad so you
echo can read more information about the precompiled HawkNL.  Would you like
echo to do so?
yesno
echo.
if errorlevel 2 goto userabort
if errorlevel 1 goto insthawk
@echo on
notepad hawk_bin\HawkNL.txt
@echo off
goto insthawk

:windirerr
echo The windir variable was not defined, that points to the root of your
echo Windows installation (usually C:\Windows).  I need this variable to
echo install any DLLs.
goto end

:copyerr
echo.
echo There was some error when I tried to copy a file.  I am unable to
echo continue with the GNE installation.  Please see the GNE site and e-mail
echo the GNE author if you need help.
echo.
echo (Note: if you got this from CVS ONLY, you won't have the precompiled
echo  HawkNL libs, so that install will fail.  You should run this installer
echo  from the publically released .zip file).
goto end

:othererr
echo.
echo Oops!  Some error I didn't expect occured.  I am unable to
echo continue with the GNE installation.  Please see the GNE site and e-mail
echo the GNE author if you need help.
goto end

:userabort
echo.
echo You decided to abort the installation.
goto end

:end
pause
@echo on
