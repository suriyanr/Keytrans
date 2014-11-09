all: KeyTrans.c
	gcc -c KeyTrans.c -o KeyTrans.o
	gcc -s -mwindows -o ../Package/KeyTrans KeyTrans.o ../Icons/Icon.o -L../Dll/ -lKeyTransHook

clean:
	rm -f KeyTrans.o ../Package/KeyTrans.exe
