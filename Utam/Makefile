CROSS_COMPILE=i586-mingw32msvc
export CROSS_COMPILE

all:
	cd Dll; make all
	cd Icons; make all
	cd Main; make all
	cd Font; make all
	cd Sample; make all

clean:
	cd Dll; make clean
	cd Icons; make clean
	cd Main; make clean
	cd Font; make clean
	cd Installer; make clean
	rm -f KeyTransU.zip
	rm -f Vutam3.zip
	rm -f Package/README.txt
	rm -f Package/*.map
	rm -f Package/KeyTransU.ini
	rm -f Package/logger.txt

release: clean all
	cp README.txt Package/
	zip -r KeyTransU.zip Package/KeyTransU.exe Package/Utam.ttf Package/KeyTransU.dll Package/README.txt Package/KeyTransU.map Package/KeyTransU_EngPhon.map Package/KeyTransU_TamGram.map Package/KeyTransU_TamPhon.map Package/KeyTransU_Vutam.map
	cd Installer; make all
	zip Vutam3.zip Vutam3.exe INSTALL.txt
