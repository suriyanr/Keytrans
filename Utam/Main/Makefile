all: KeyTransU.c
	$(CROSS_COMPILE)-gcc -O2 -c KeyTransU.c -o KeyTransU.o
	$(CROSS_COMPILE)-gcc -s -mwindows -o ../Package/KeyTransU.exe KeyTransU.o ../Icons/Icon.o -L../Dll/ -lKeyTransU

clean:
	rm -f KeyTransU.o ../Package/KeyTransU.exe
