This is the d2gs 1.11b source package release.


1.Directories:
  config\		some config file samples
  d2gs\			source code of d2gs
  d2warden\		source code of d2warden
  patch_d2server\	source code of patch_d2server, which will be used to patch the d2gs. It only copy the 1.exe code section into the d2server.dll.


2.Setup working space
  The d2gs 1.11b use x86 asm code, you must install a MASM32 to compile it. I suggest you download and install the MASM32 package from http://www.masm32.com/. And this MASM32 package should install to C:\MASM32. Then copy the ASMBAT.bat into your C:\MASM32
  The d2warden use a MSVC 2003 compiler, so, install it.
  Also, I suggest you install the UltraEdit. Then configure UltraEdit use the MASM32. This can be done via setup the tool in its "Advance" menu:
  //////////////////////////////////////////////////////////////////////////////////////////////////// 
commandline£ºC:\MASM32\ASMBAT.bat %N 
working directory£º%P 
name£ºAssemble(&B) 
... 
//////////////////////////////////////////////////////////////////////////////////////////////////// 

3. Compiling d2gs
  a.Go into d2gs directory, open the 1.asm with UltraEdit;
  b.From the UltraEdit Advanced menu, select Assemble
    This will generate a 1.exe in the d2gs directory, ok, it compiled.
  c.Run the patch_d2server.exe
    This will copy the 1.exe code section into the d2server.dll.
  d. OK, you got a compiled d2server.dll, copy this new d2server.dll into your diablo directory.


  BTW: the UberQuest.asm in d2gs directory is a UberQuest implementation of the UberQuest, most code from PlugY, thanks...... :)

4. Compiling d2warden
  a. Go into d2warden directory, open the d2warden project with MSVC 2003.
  b. Compile, you got the d2warden.dll.
  c. Copy the d2warden.dll into your diablo directory.

