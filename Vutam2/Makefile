all:
	cd Dll; make all
	cd Icons; make all
	cd Main; make all
	cd Font; make all

clean:
	cd Dll; make clean
	cd Icons; make clean
	cd Main; make clean
	cd Font; make clean
	cd Installer; make clean
	rm -f KeyTrans2.zip
	rm -f Vutam2.zip
	rm -f Package/README.txt

release: clean all
	cp README.txt Package/
	zip -r KeyTrans2.zip Package/KeyTrans2.exe Package/Vutam2.ttf Package/KeyTransHook2.dll Package/README.txt
	cd Installer; make all
	zip Vutam2.zip Vutam2.exe
