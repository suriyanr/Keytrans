all: KeyTransU.dll

KeyTransU.dll: KeyTrans2HookU.c KeyTrans2HookU.h
	$(CROSS_COMPILE)-gcc -s -O2 -shared KeyTrans2HookU.c -o KeyTransU.dll
	$(CROSS_COMPILE)-dlltool --dllname KeyTransU.dll --def KeyTrans2HookU.def --output-lib libKeyTransU.a
	cp KeyTransU.dll ../Package/

clean:
	rm -f KeyTransU.dll libKeyTransU.a ../Package/KeyTransU.dll
