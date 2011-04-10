
goluahps.dll: dlldata.obj goluah_p.obj goluah_i.obj
	link /dll /out:goluahps.dll /def:goluahps.def /entry:DllMain dlldata.obj goluah_p.obj goluah_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0500 /DREGISTER_PROXY_DLL \
		$<
# _WIN32_WINNT=0x0500 is for Win2000, change it to 0x0400 for NT4 or Win95 with DCOM

clean:
	@del goluahps.dll
	@del goluahps.lib
	@del goluahps.exp
	@del dlldata.obj
	@del goluah_p.obj
	@del goluah_i.obj
