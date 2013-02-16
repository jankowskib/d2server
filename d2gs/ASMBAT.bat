@ECHO OFF

IF NOT EXIST D:\Masm32\Bin\Ml.Exe GOTO _NoMasm32

IF EXIST 1.obj DEL 1.obj
IF EXIST 1.asm D:\MASM32\Bin\Ml.exe /ID:\MASM32\Include /c /Zf /Zd /Zi /coff 1.asm
IF EXIST Resource.rc GOTO _ExistRC
IF EXIST 1.obj D:\MASM32\Bin\Link.exe /BASE:0x68020000 /SUBSYSTEM:Console /LIBPATH:D:\MASM32\Lib 1.obj 
GOTO _Done

:_ExistRC
IF NOT EXIST Resource.res D:\MASM32\Bin\Rc.exe /iD:\MASM32\Include Resource.rc
IF EXIST 1.obj D:\MASM32\Bin\Link.exe /SUBSYSTEM:Windows /LIBPATH:D:\MASM32\Lib 1.obj Resource.res 

:_Done:
IF EXIST 1.obj DEL 1.obj
GOTO :_End

:_NoMasm32
ECHO Could not find "D:\Masm32\Bin\Ml.Exe".

:_End

PAUSE