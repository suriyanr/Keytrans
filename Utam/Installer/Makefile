XD = ../xd/xd

all: Vutam4.exe

Vutam4.exe: DesktopLink.o PackKeyTransU.h PackKeyTransUDll.h PackTam_Maps.h PackTam_Maps.h PackReadme.h PackVutam2TTF.h PackVutam3TTF.h PackVutam2TTFinTACEBMP.h PackEngTranslitMap.h PackKeyTransUMap.h PackTACEphonMap.h PackTamNet99Map.h PackTamPhonMap.h Installer.o
	$(CROSS_COMPILE)-gcc -s -mwindows Installer.o DesktopLink.o -o ../Vutam4.exe -lole32 -lshell32 -luuid

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

PackTam_Maps.h:
	$(XD) -dPackTam_Maps ../Package/Tam_Maps.txt > PackTam_Maps.h

PackReadme.h:
	$(XD) -dPackReadme ../Package/README.txt > PackReadme.h

PackVutam2TTF.h:
	$(XD) -dPackVutam2TTF ../Package/Vutam2.ttf > PackVutam2TTF.h

PackVutam3TTF.h:
	$(XD) -dPackVutam3TTF ../Package/Vutam3.ttf > PackVutam3TTF.h

PackVutam2TTFinTACEBMP.h:
	$(XD) -dPackVutam2TTFinTACEBMP ../Package/Vutam2TTFinTACE.bmp > PackVutam2TTFinTACEBMP.h

PackEngTranslitMap.h:
	$(XD) -dPackEngTranslitMap ../Package/KeyTransU_EngTranslit.map > PackEngTranslitMap.h

PackKeyTransUMap.h:
	$(XD) -dPackKeyTransUMap ../Package/KeyTransU.map > PackKeyTransUMap.h

PackTACEphonMap.h:
	$(XD) -dPackTACEphonMap ../Package/KeyTransU_TACEphon.map > PackTACEphonMap.h

PackTamNet99Map.h:
	$(XD) -dPackTamNet99Map ../Package/KeyTransU_TamNet99.map > PackTamNet99Map.h

PackTamPhonMap.h:
	$(XD) -dPackTamPhonMap ../Package/KeyTransU_TamPhon.map > PackTamPhonMap.h

clean:
	rm -f Pack*.h
	rm -f DesktopLink.o
	rm -f Installer.o Installer.h
	rm -f ../Vutam4.exe
