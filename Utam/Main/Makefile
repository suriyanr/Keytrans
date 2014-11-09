all: KeyTransU.c
	gcc -O2 -c KeyTransU.c -o KeyTransU.o
	gcc -s -mwindows -o ../Package/KeyTransU KeyTransU.o ../Icons/Icon.o -L../Dll/ -lKeyTransU

clean:
	rm -f KeyTransU.o ../Package/KeyTransU.exe
