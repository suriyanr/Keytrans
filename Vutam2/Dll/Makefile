all: KeyTransHook2.dll

KeyTransHook2.dll: KeyTransHook2.c KeyTransHook2.h
	gcc -s -shared KeyTransHook2.c -o KeyTransHook2.dll
	dlltool --dllname KeyTransHook2.dll --def KeyTransHook2.def --output-lib libKeyTransHook2.a
	cp KeyTransHook2.dll ../Package/

clean:
	rm -f KeyTransHook2.dll libKeyTransHook2.a ../Package/KeyTransHook2.dll
