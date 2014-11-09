all: KeyTrans.c
	gcc -c KeyTrans.c -o KeyTrans.o
	gcc -s -mwindows -o ../Package/KeyTrans2 KeyTrans.o ../Icons/Icon.o -L../Dll/ -lKeyTransHook2

clean:
	rm -f KeyTrans.o ../Package/KeyTrans2.exe
