all: Vutam3.exe

Vutam3.exe: DesktopLink.o PackKeyTransU.o PackKeyTransUDll.o PackUtamTTF.o PackReadme.o PackTamGramMap.o PackVutamMap.o PackEngPhonMap.o PackTamPhonMap.o Installer.h Installer.o
	gcc -s -mwindows Installer.o DesktopLink.o PackKeyTransU.o PackKeyTransUDll.o PackUtamTTF.o PackReadme.o PackTamGramMap.o PackVutamMap.o PackEngPhonMap.o PackTamPhonMap.o -o ../Vutam3.exe -lole32 -lshell32 -luuid

Installer.o: Installer.c Installer.h
	gcc -O -c Installer.c -o Installer.o

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
	wc -c ../Package/KeyTransU_Vutam.map | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_ENGPHONMAP " >> Installer.h
	wc -c ../Package/KeyTransU_EngPhon.map | awk '{print $$1}' >> Installer.h
	echo -n "#define LENGTH_TAMPHONMAP " >> Installer.h
	wc -c ../Package/KeyTransU_TamPhon.map | awk '{print $$1}' >> Installer.h

DesktopLink.o:
	g++ -c DesktopLink.cpp -o DesktopLink.o

PackKeyTransU.o:
	reswrap -i -o PackKeyTransU.h ../Package/KeyTransU.exe
	reswrap -o PackKeyTransU.c ../Package/KeyTransU.exe
	gcc -c PackKeyTransU.c -o PackKeyTransU.o

PackKeyTransUDll.o:
	cp ../Package/KeyTransU.dll ../Package/KeyTransUdll
	reswrap -i -o PackKeyTransUDll.h ../Package/KeyTransUdll
	reswrap -o PackKeyTransUDll.c ../Package/KeyTransUdll
	gcc -c PackKeyTransUDll.c -o PackKeyTransUDll.o
	rm -f ../Package/KeyTransUdll

PackUtamTTF.o:
	reswrap -i -o PackUtamTTF.h ../Package/Utam.ttf
	reswrap -o PackUtamTTF.c ../Package/Utam.ttf
	gcc -c PackUtamTTF.c -o PackUtamTTF.o

PackReadme.o:
	reswrap -i -o PackReadme.h ../Package/README.txt
	reswrap -o PackReadme.c ../Package/README.txt
	gcc -c PackReadme.c -o PackReadme.o

PackTamGramMap.o:
	reswrap -i -o PackTamGramMap.h ../Package/KeyTransU_TamGram.map
	reswrap -o PackTamGramMap.c ../Package/KeyTransU_TamGram.map
	gcc -c PackTamGramMap.c -o PackTamGramMap.o

PackVutamMap.o:
	reswrap -i -o PackVutamMap.h ../Package/KeyTransU_Vutam.map
	reswrap -o PackVutamMap.c ../Package/KeyTransU_Vutam.map
	gcc -c PackVutamMap.c -o PackVutamMap.o

PackTamPhonMap.o:
	reswrap -i -o PackTamPhonMap.h ../Package/KeyTransU_TamPhon.map
	reswrap -o PackTamPhonMap.c ../Package/KeyTransU_TamPhon.map
	gcc -c PackTamPhonMap.c -o PackTamPhonMap.o

PackEngPhonMap.o:
	reswrap -i -o PackEngPhonMap.h ../Package/KeyTransU_EngPhon.map
	reswrap -o PackEngPhonMap.c ../Package/KeyTransU_EngPhon.map
	gcc -c PackEngPhonMap.c -o PackEngPhonMap.o

clean:
	rm -f PackReadme.h PackReadme.c PackReadme.o
	rm -f PackUtamTTF.h PackUtamTTF.c PackUtamTTF.o
	rm -f PackKeyTransUDll.h PackKeyTransUDll.c PackKeyTransUDll.o
	rm -f PackKeyTransU.h PackKeyTransU.c PackKeyTransU.o
	rm -f PackTamGramMap.h PackTamGramMap.c PackTamGramMap.o
	rm -f PackVutamMap.h PackVutamMap.c PackVutamMap.o
	rm -f PackTamPhonMap.h PackTamPhonMap.c PackTamPhonMap.o
	rm -f PackEngPhonMap.h PackEngPhonMap.c PackEngPhonMap.o
	rm -f DesktopLink.o
	rm -f Installer.o Installer.h
	rm -f ../Vutam3.exe
