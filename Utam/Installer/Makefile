XD = ../xd/xd

all: Vutam3.exe

Vutam3.exe: DesktopLink.o PackKeyTransU.h PackKeyTransUDll.h PackUtamTTF.h PackReadme.h PackTamGramMap.h PackVuTamMap.h PackEngPhonMap.h PackTamPhonMap.h Installer.o
	$(CROSS_COMPILE)-gcc -s -mwindows Installer.o DesktopLink.o -o ../Vutam3.exe -lole32 -lshell32 -luuid

Installer.o: Installer.c
	$(CROSS_COMPILE)-gcc -O -c Installer.c -o Installer.o

DesktopLink.o:
	$(CROSS_COMPILE)-g++ -c DesktopLink.cpp -o DesktopLink.o

PackKeyTransU.h:
	$(XD) -dPackKeyTransU ../Package/KeyTransU.exe > PackKeyTransU.h

PackKeyTransUDll.h:
	cp ../Package/KeyTransU.dll ../Package/KeyTransUdll
	$(XD) -dPackKeyTransUDll ../Package/KeyTransUdll > PackKeyTransUDll.h
	rm -f ../Package/KeyTransUdll

PackUtamTTF.h:
	$(XD) -dPackUtamTTF ../Package/Utam.ttf > PackUtamTTF.h

PackReadme.h:
	$(XD) -dPackReadme ../Package/README.txt > PackReadme.h

PackTamGramMap.h:
	$(XD) -dPackTamGramMap ../Package/KeyTransU_TamGram.map > PackTamGramMap.h

PackVuTamMap.h:
	$(XD) -dPackVuTamMap ../Package/KeyTransU_VuTam.map > PackVuTamMap.h

PackTamPhonMap.h:
	$(XD) -dPackTamPhonMap ../Package/KeyTransU_TamPhon.map > PackTamPhonMap.h

PackEngPhonMap.h:
	$(XD) -dPackEngPhonMap ../Package/KeyTransU_EngPhon.map > PackEngPhonMap.h

clean:
	rm -f PackReadme.h
	rm -f PackUtamTTF.h
	rm -f PackKeyTransUDll.h
	rm -f PackKeyTransU.h
	rm -f PackTamGramMap.h
	rm -f PackVuTamMap.h
	rm -f PackTamPhonMap.h
	rm -f PackEngPhonMap.h
	rm -f DesktopLink.o
	rm -f Installer.o Installer.h
	rm -f ../Vutam3.exe
