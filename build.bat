@REM Sentinel build script
@ECHO OFF
SetLocal EnableDelayedExpansion

SET CC=gcc
SET EXT=c
SET PROJECT=Sentinel
SET SRC=src
SET OBJ=obj
SET BINDIR=bin

IF "%1"=="CLEAN" (
    ECHO Cleaning dirs
    RMDIR /Q /S %BINDIR%
    RMDIR /Q /S %OBJ%
    EXIT /B 0
)

IF NOT EXIST "%OBJ%" MKDIR "%OBJ%"
IF NOT EXIST "%BINDIR%" MKDIR "%BINDIR%"

SET SRCS=
FOR /R %SRC% %%f IN (*.%EXT%) DO (
    SET SRCS=!SRCS! %%f
)

IF "%2"=="RELEASE" (
    SET RELEASE=for release
    SET CFLAGS=-O2
) ELSE (
    SET CFLAGS=-g -Wall
)
SET IFLAGS=-Iinclude
SET LFLAGS=-lopengl32 -lgdi32

ECHO Building %PROJECT% %RELEASE%

FOR %%f IN (%SRCS%) DO (
    IF "%1"=="" (
        ECHO Compiling %%~nf
        %CC% %CFLAGS% -c %IFLAGS% "%%f" -o "%OBJ%/%%~nf.o"
    )
    IF "%1"=="ALL" (
        ECHO Compiling %%~nf
        %CC% %CFLAGS% -c %IFLAGS% "%%f" -o "%OBJ%/%%~nf.o"
    )
    IF "%1"=="NOLIB" (
        ECHO Compiling %%~nf
        %CC% %CFLAGS% -c %IFLAGS% "%%f" -o "%OBJ%/%%~nf.o"
    )
    IF "%1"=="%%~nf" (
        ECHO Compiling %%~nf
        %CC% %CFLAGS% -c %IFLAGS% "%%f" -o "%OBJ%/%%~nf.o"
    )
    SET "OBJS=!OBJS! %OBJ%/%%~nf.o"
)
ECHO Linking objects
%CC% %CFLAGS% %IFLAGS% %OBJS% %LFLAGS% -o %BINDIR%/%PROJECT%.exe