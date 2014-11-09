all: KeyTransHook.dll

KeyTransHook.dll: KeyTransHook.c KeyTransHook.h
	gcc -s -shared KeyTransHook.c -o KeyTransHook.dll
	dlltool --dllname KeyTransHook.dll --def KeyTransHook.def --output-lib libKeyTransHook.a
	cp KeyTransHook.dll ../Package/

clean:
	rm -f KeyTransHook.dll libKeyTransHook.a ../Package/KeyTransHook.dll
