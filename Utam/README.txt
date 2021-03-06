:

VUTAM4 (KeyTransU Bundle)
==========================

Facilities:
-----------

1. This is a keyboard driver with four baskets in a keyboard.
These are: a) Normal, b) Shift, c) Alt and d) Ctrl+Alt.

2. Language and layout are changeable to suit the knowledgable end user, who can edit / create the map files. Vide PS.

3. This has all types of ligature features.

4. This facilitates BMP Unicode character typing(TAU), and TACE in PUA.

5. It is suitable for Windows2000, WindowsXP, WindowsVista , Win7 and Win8 etc....


Limitations:
------------

Unless the concerned language is enabled in the System, (this requires the Install CD) via control panel --> RegionalLanguages etc, (apllies to 2K & XP), this driver works in WordPad, Outlook, OutlookExpress, FrontPage, Internet Explorer, MS's Word and in other MS's Office suite. If enabled, together with the above, it works in NotePad text files too.

Those of you not able to do the above enabling, can still use it,  after opening the .txt file with WordPad and saving in RTF.

For TACE, enabling of Tamil is not necessary.

It is ready for:
----------------

This driver comes ready for use with Tamil.

Presently, Tamil has three methods (styles) of keyboard input:

1). English Phonetic (Transliteration), fit for those who can only speak Tamil !

2). MS's Tamil Phonetic, (Fit for heavy users, who have little time to know about key maps !! ). In this style, ALL vow.marks to be typed to the RIGHT of concerned consonant. TACE follows this style too.

and,  3). Tamil Grammatic (TamilNet99). (Meant for knowledgeable users, who have enough spare time ?)

All these three methods (styles) avoid facing a serious handicap created by  Unicode. (More on this at:  http://v.ramasami.tripod.com)

The (new) 4th, VUTAM Type_As_You_Write method, however, faces this problem and hence facilitates one to type Tamil, the way Tamils write Tamil on paper. (Type As You Write. ) This is the default method in this SW. This is suitable for those who know to read and write Tamil.

Necessary map files for all four methods, are also part of this package. End user can choose the method most suitable for him. The default is the VUTAM method.

A free Unicode Tamil font file, VUTAM3.ttf, is also available for immediate use of the package, for TAU & TACE encodings.. VUTAM2.TTF gives old Tamil letters in TACE map.

VUTAM Type As You Write Layout:
---------------------------------------

The 'a' vowel marked consonants, the 'u' vowel marked consonants and the 'U' vowel marked consonants are accomodated in the first three baskets. The 1st row of fourth has been mapped.

Other Layouts:
-------------

First two baskets accomodate Tamil and the rest, English.

Installation:
-----------
  First unzip the dloaded zip, to extract its files; Copy these in a location of your choice (say my Docs.), in a folder named VUTAM4. Make a shortcut of keytransU.exe in desktop.
  The list of files in the zip archive are as follows:
  Package/KeyTransU.exe
  Package/KeyTransU.dll
  Package/README.txt
  Package/Tam_Maps.txt
  Package/Vutam2TTFinTACE.bmp
  Package/Vutam2.ttf
  Package/Vutam3.ttf
  Package/KeyTransU_EngTranslit.map
  Package/KeyTransU_TACEphon.map
  Package/KeyTransU_TamPhon.map
  Package/KeyTransU.map
  Package/KeyTransU_TamNet99.map


To install VUTAM3.ttf & VUTAM2.TTF:
----------------------------------------------

In Vista right click the VUTAM3 font file in Documents/KeyTransU folder and choose Install. In all other Windows versions, you will have to manually copy the VUTAM3 font thru' Control Panel -> Fonts.

Now you are all set.

Enable / Disable:
-----------------

Keyboard method: (Toggle)

Pressing Ctrl+Space Bar will disable the translation.
          RePressing Ctrl+Space Bar will enable the translation.

Mouse method:

Click on the Keyboard icon on the tray bar. Choose Disable to disable the translation. Choose Enable to enable the translation.

USE:
----

First create/open the file in which you want to type Tamil. "file" here means OE, IE etc too. Then click & execute Vutam4 DeskTop shortcut. A small keyboard icon appears in the system tray (right hand bottom of screen). Save the blank file in Unicode/RTF as appropriate. Choose the font, style and start typing.


You may enable/ disable/ change style of typing/ exit etc by right clicking the tray icon and left clicking the required choice.

To uninstall:
------------

Delete the shortcut from Desktop and the folder KeyTransU from the My Documents folder.

Logic used in mappings :
-----------------------

1) All short vowels/vowel marks in normal (1st) basket. Vow. marks in punc marks. Longs in 2nd (Shift) basket.
 2) All vowels follow Eng. vowels a,e,i,o,u. a = அ; e = இ ;  i = ஐ; o = ஒ & u = உ 
 3) letters y = எ and I = ஔ
 4) க,ச,ட,த,ப are in k,c,d,t,p. , ர,ல,வ are in r,l,v. ம,ன  are in m, n. ஜ,ஸ,ஷ,ஹ,க்ஷ, are in J, s, S, h and H. ய =  w due to similarity in shape ! f that looks like a grazing goat with its fore legs on a tree trunk = ள,  that looks like a grazing cattle with all its fours on ground !! x and z accomodate ற & ழ due to convention.  ங & ஞ are in g and j since ng and nj give similar sounds.  ண the biggest, is in b; what a coincidence !! 
5) The left overs are: ந, ஃ, ௐ, ஶ, ஸ்ரீ, ௸, ௳, ௴, ௵  etc. These are in q, Q, Z, X, R, T, D, F, G resply. 
6) Tamil numerals are in third basket Alt. from 1 ~ 0.
7) The main idea is "Easy remember" and NOT frequent use, as in English TypeWriters.

Our thanks are to:
---------------------------
(1) High_logic for their fcp4, used for creating VUTAM3.ttf & VUTAM2.ttf

Warranty:
------------

This is a freeware without any implicit or explicit warranty of anykind. You use it at your own risk.

Source:
---------
Available at https://github.com/suriyanr/Keytrans/tree/master/Utam

Feedback:
-------------

v.ramasami@gmail.com

7th December 2014.

PS: 
(1) A sample layout file is available.
(2) .map files are ANSI text files; can be opened by wordpad, NotePad++ and MS EDIT, an old DOS SW, available still in the web. 
(3) The later two give auto line nos., which eases location of erroneous syntax, for correction.
(4) When a .map file is modified or new ones created, they have to be tested by choosing them (one at a time) while SW is running. Files with no errors will be accepted; but files with syntax errors will be rejected with a message.
(5) Logical errors can't be tested by SW.
(6) All map files have some examples in the beginning of the file, as an introduction.
(7) Unless one knows what he is doing, avoid modifying .map files !

<EOF>

 
