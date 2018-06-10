
JDArchiveSvrps.dll: dlldata.obj JDArchiveSvr_p.obj JDArchiveSvr_i.obj
	link /dll /out:JDArchiveSvrps.dll /def:JDArchiveSvrps.def /entry:DllMain dlldata.obj JDArchiveSvr_p.obj JDArchiveSvr_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del JDArchiveSvrps.dll
	@del JDArchiveSvrps.lib
	@del JDArchiveSvrps.exp
	@del dlldata.obj
	@del JDArchiveSvr_p.obj
	@del JDArchiveSvr_i.obj
