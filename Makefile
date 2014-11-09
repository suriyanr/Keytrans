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
	rm -f KeyTrans.zip
	rm -f Vutam.zip
	rm -f Package/README.txt

release: clean all
	cp README.txt Package/
	zip -r KeyTrans.zip Package/KeyTrans.exe Package/Vutam.ttf Package/KeyTransHook.dll Package/README.txt
	cd Installer; make all
	zip Vutam.zip Vutam.exe
