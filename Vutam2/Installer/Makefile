all: Vutam2.exe

Vutam2.exe: DesktopLink.o PackKeyTrans.o PackKeyTransHookDll.o PackVutamTTF.o PackReadme.o Installer.h Installer.o
	gcc -s -mwindows Installer.o DesktopLink.o PackKeyTrans.o PackKeyTransHookDll.o PackVutamTTF.o PackReadme.o -o ../Vutam2.exe -lole32 -lshell32 -luuid

Installer.o: Installer.c Installer.h
	gcc -O -c Installer.c -o Installer.o

Installer.h:
	echo -n "#define LENGTH_KEYTRANS " > Installer.h
	wc -c ../Package/KeyTrans2.exe | cut -b 1-8 >> Installer.h
	echo -n "#define LENGTH_KEYTRANSHOOKDLL " >> Installer.h
	wc -c ../Package/KeyTransHook2.dll | cut -b 1-8 >> Installer.h
	echo -n "#define LENGTH_VUTAMTTF " >> Installer.h
	wc -c ../Package/Vutam2.ttf | cut -b 1-8 >> Installer.h
	echo -n "#define LENGTH_README " >> Installer.h
	wc -c ../Package/README.txt | cut -b 1-8 >> Installer.h

DesktopLink.o:
	g++ -c DesktopLink.cpp -o DesktopLink.o

PackKeyTrans.o:
	reswrap -i -o PackKeyTrans.h ../Package/KeyTrans2.exe
	reswrap -o PackKeyTrans.c ../Package/KeyTrans2.exe
	gcc -c PackKeyTrans.c -o PackKeyTrans.o

PackKeyTransHookDll.o:
	reswrap -i -o PackKeyTransHookDll.h ../Package/KeyTransHook2.dll
	reswrap -o PackKeyTransHookDll.c ../Package/KeyTransHook2.dll
	gcc -c PackKeyTransHookDll.c -o PackKeyTransHookDll.o

PackVutamTTF.o:
	reswrap -i -o PackVutamTTF.h ../Package/Vutam2.ttf
	reswrap -o PackVutamTTF.c ../Package/Vutam2.ttf
	gcc -c PackVutamTTF.c -o PackVutamTTF.o

PackReadme.o:
	reswrap -i -o PackReadme.h ../Package/README.txt
	reswrap -o PackReadme.c ../Package/README.txt
	gcc -c PackReadme.c -o PackReadme.o

clean:
	rm -f PackReadme.h PackReadme.c PackReadme.o
	rm -f PackVutamTTF.h PackVutamTTF.c PackVutamTTF.o
	rm -f PackKeyTransHookDll.h PackKeyTransHookDll.c PackKeyTransHookDll.o
	rm -f PackKeyTrans.h PackKeyTrans.c PackKeyTrans.o
	rm -f DesktopLink.o
	rm -f Installer.o Installer.h
	rm -f ../Vutam2.exe

