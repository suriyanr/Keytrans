all: Vutam3.exe

Vutam3.exe: DesktopLink.o PackKeyTransU.o PackKeyTransUDll.o PackUtamTTF.o PackReadme.o PackTamGramMap.o PackVuTamMap.o PackEngPhonMap.o PackTamPhonMap.o Installer.h Installer.o
	$(CROSS_COMPILE)-gcc -s -mwindows Installer.o DesktopLink.o PackKeyTransU.o PackKeyTransUDll.o PackUtamTTF.o PackReadme.o PackTamGramMap.o PackVuTamMap.o PackEngPhonMap.o PackTamPhonMap.o -o ../Vutam3.exe -lole32 -lshell32 -luuid

Installer.o: Installer.c Installer.h
	$(CROSS_COMPILE)-gcc -O -c Installer.c -o Installer.o

Installer.h:
	echo -n "#define LENGTH_KEYTRANSU " > Installer.h
	wc -c ../Package/KeyTransU.exe | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_KEYTRANSUDLL " >> Installer.h
	wc -c ../Package/KeyTransU.dll | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_UTAMTTF " >> Installer.h
	wc -c ../Package/Utam.ttf | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_README " >> Installer.h
	wc -c ../Package/README.txt | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_TAMGRAMMAP " >> Installer.h
	wc -c ../Package/KeyTransU_TamGram.map | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_VUTAMMAP " >> Installer.h
	wc -c ../Package/KeyTransU_VuTam.map | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_ENGPHONMAP " >> Installer.h
	wc -c ../Package/KeyTransU_EngPhon.map | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_TAMPHONMAP " >> Installer.h
	wc -c ../Package/KeyTransU_TamPhon.map | awk '{print $$1}' >> Installer.h

DesktopLink.o:
	$(CROSS_COMPILE)-g++ -c DesktopLink.cpp -o DesktopLink.o

PackKeyTransU.o:
	reswrap -i -o PackKeyTransU.h ../Package/KeyTransU.exe
	reswrap -o PackKeyTransU.c ../Package/KeyTransU.exe
	$(CROSS_COMPILE)-gcc -c PackKeyTransU.c -o PackKeyTransU.o

PackKeyTransUDll.o:
	cp ../Package/KeyTransU.dll ../Package/KeyTransUdll
	reswrap -i -o PackKeyTransUDll.h ../Package/KeyTransUdll
	reswrap -o PackKeyTransUDll.c ../Package/KeyTransUdll
	$(CROSS_COMPILE)-gcc -c PackKeyTransUDll.c -o PackKeyTransUDll.o
	rm -f ../Package/KeyTransUdll

PackUtamTTF.o:
	reswrap -i -o PackUtamTTF.h ../Package/Utam.ttf
	reswrap -o PackUtamTTF.c ../Package/Utam.ttf
	$(CROSS_COMPILE)-gcc -c PackUtamTTF.c -o PackUtamTTF.o

PackReadme.o:
	reswrap -i -o PackReadme.h ../Package/README.txt
	reswrap -o PackReadme.c ../Package/README.txt
	$(CROSS_COMPILE)-gcc -c PackReadme.c -o PackReadme.o

PackTamGramMap.o:
	reswrap -i -o PackTamGramMap.h ../Package/KeyTransU_TamGram.map
	reswrap -o PackTamGramMap.c ../Package/KeyTransU_TamGram.map
	$(CROSS_COMPILE)-gcc -c PackTamGramMap.c -o PackTamGramMap.o

PackVuTamMap.o:
	reswrap -i -o PackVuTamMap.h ../Package/KeyTransU_VuTam.map
	reswrap -o PackVuTamMap.c ../Package/KeyTransU_VuTam.map
	$(CROSS_COMPILE)-gcc -c PackVuTamMap.c -o PackVuTamMap.o

PackTamPhonMap.o:
	reswrap -i -o PackTamPhonMap.h ../Package/KeyTransU_TamPhon.map
	reswrap -o PackTamPhonMap.c ../Package/KeyTransU_TamPhon.map
	$(CROSS_COMPILE)-gcc -c PackTamPhonMap.c -o PackTamPhonMap.o

PackEngPhonMap.o:
	reswrap -i -o PackEngPhonMap.h ../Package/KeyTransU_EngPhon.map
	reswrap -o PackEngPhonMap.c ../Package/KeyTransU_EngPhon.map
	$(CROSS_COMPILE)-gcc -c PackEngPhonMap.c -o PackEngPhonMap.o

clean:
	rm -f PackReadme.h PackReadme.c PackReadme.o
	rm -f PackUtamTTF.h PackUtamTTF.c PackUtamTTF.o
	rm -f PackKeyTransUDll.h PackKeyTransUDll.c PackKeyTransUDll.o
	rm -f PackKeyTransU.h PackKeyTransU.c PackKeyTransU.o
	rm -f PackTamGramMap.h PackTamGramMap.c PackTamGramMap.o
	rm -f PackVuTamMap.h PackVuTamMap.c PackVuTamMap.o
	rm -f PackTamPhonMap.h PackTamPhonMap.c PackTamPhonMap.o
	rm -f PackEngPhonMap.h PackEngPhonMap.c PackEngPhonMap.o
	rm -f DesktopLink.o
	rm -f Installer.o Installer.h
	rm -f ../Vutam3.exe
