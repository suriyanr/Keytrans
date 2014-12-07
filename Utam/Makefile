# Ubuntu
#CROSS_COMPILE=i586-mingw32msvc
# OL 7.0
CROSS_COMPILE=i686-w64-mingw32

export CROSS_COMPILE

all:
	cd Dll; make all
	cd Icons; make all
	cd Main; make all
	cd Font; make all
	cd Sample; make all
	cd xd; make all

clean:
	cd Dll; make clean
	cd Icons; make clean
	cd Main; make clean
	cd Font; make clean
	cd Installer; make clean
	cd xd; make clean
	rm -f KeyTransU.zip
	rm -f Vutam4.zip
	rm -f Vutam4exe.zip
	rm -f Package/README.txt
	rm -f Package/*.map
	rm -f Package/KeyTransU.ini
	rm -f Package/logger.txt
	rm -f Package/Tam_Maps.txt
	rm -f Package/Vutam2TTFinTACE.bmp

release: clean all
	cp README.txt Package/
	zip -r Vutam4.zip Package/KeyTransU.exe Package/KeyTransU.dll Package/README.txt Package/Tam_Maps.txt Package/Vutam2TTFinTACE.bmp Package/Vutam2.ttf Package/Vutam3.ttf Package/KeyTransU_EngTranslit.map Package/KeyTransU_TACEphon.map Package/KeyTransU_TamPhon.map Package/KeyTransU.map Package/KeyTransU_TamNet99.map
	cd Installer; make all
	zip Vutam4exe.zip Vutam4.exe INSTALL.txt
