Introduction:
=============
   KeyTrans2 comprises of two components - A Font (VUTAM2), and a Keyboard
Translator. The Keyboard Translator sits between the Application and the
Keyboard driver. This helps it to translate Key Strokes that an application
receives. In its current avatar, it works with a custom made font called
"VUTAM2".

Software Requirements:
======================
   Any windows OS. Any richtext editor. (Being 8/16 bits, in Win98, a text 
editor that works in ALL windows, like ledlineit, available free at
http://www.sophists.com/Led/LedLineIt/ to be used, to avoid seeing ????.)
   Files created in Vutam2 will be more than 50 % smaller, compared to files 
created in any other Tamil font, due to extensive ligatures.
   Any simple editor/word processor meant for English will be sufficient for
files created with Vutam2 for such purposes; software specific to Tamil will
not be required.


The VUTAM2 Font (Vutam2.ttf):
=============================
   Vutam2 is a Tamil TTF font, developed by Sri V. Ramasami. It was developed
to improve on rendering, which seems to be lacking in the other commonly
available Tamil fonts. The Layout is also very easy to remember. The "Vutam2"
font needs to be installed to view files written with the "Vutam2" font.


The Keyboard Translator (KeyTrans2.exe):
=======================================
   KeyTrans2 comes into play when text needs to be entered using the "Vutam2"
   font mentioned above.
   The Characters fall in three "Buckets".
   Bucket 1, is what is typed in Normal Mode.
   Bucket 2, is what is typed in Shift (CAPS) Mode.
   Bucket 3, is what is typed in Ctrl+Alt Mode.

   KeyTrans2 comes into play when mapping the characters which exist in 
   Bucket 3. They also help in effectuating the 242 ligatures.

   Bucket 1:
      All Short vowels and all 23 dotted and undotted consonants are
      accomodated.

   Bucket 2:
      All Long vowels and 23 consonants with their vowel marker "vu" have
      been accomodated.

   Bucket 3:
      All 23 consonants with their vowel marker "vU" have been accomodated.

   Note: Tamil vowels correspond to their English counterparts.
         Consonants are 18 + 5 = 23. Some extras may be found in Bucket 3.
         Punctiation Marks "lost" in Bucket 1 and Bucket 2, are "found" in
         Bucket 3.


INSTALLATION:
=============
   Installing KeyTrans2:
   --------------------
      Unzip the Downloaded Vutam2.zip. Run Vutam2.exe.

   Installing the Font: 
   --------------------
      Open the Drive:\Windows\Fonts folder; Choose "Install New Font". 
      In "File", browse to the Drive/Folder where "Vutam2.ttf" resides,
      Select it, and Press OK. It is installed as "VUTAM2" in the Fonts folder.


UNINSTALL:
==========
   UnInstalling KeyTrans2:
   ----------------------
      Exit out of the program (which is running in the System Tray).
      Delete the Drive:\Program Files\KeyTrans2 directory.

   UnInstalling the Font:
   ----------------------
      Open the Drive:\Windows\Fonts folder.
      Delete the VUTAM2 File seen there.

THANKS:
=======

1. HighLogic for their font creator programme, FCP.
   (www.high-logic.com)

2. Jonathon Fowler
   jonof@edgenetwork.org
   http://jonof.edgenetwork.org/?p=misc#keyfunc

   The "Hook and DLL" information was obtained from KeyFunc.

3. Abraxas23
   http://www.codeproject.com/shell/StealthDialog.asp

   The "TrayBar Icon howto" was mostly borrowed from there.

4. www.sarovar.org
   For kindly hosting this open source project.


LICENSE AND WARRANTY:
=====================

This program is licensed under the terms of the GNU General Public License

The program carries no warranty of any kind. You use this completely at your
own risk. I deny all responsibility for this program.


USAGE AND CONFIGURATION:
========================

The program is configured using a file named "KeyTrans2.ini" which is to be
found in the same directory as the executable.
To use the program with VUTAM2, no configuration file is necessary. Just run
the KeyTrans2.exe file and you are all set.
You can create the default configuration that it uses, by "KeyTrans2 /m"

Initialization file:
[Mappings]
...
[Ligatures]
...

The Mappings are of the form:
Virtual Key Name=combination of numbers, which will generate the character
                 using the Alt - numeric keypad method.

The Ligatures are of the form:
Rule<1..100>=Bucket<1|2|3> VirtualKeyName Bucket<1|2|3> VirtualKeyName Bucket<1|2|3> VirtualKeyName <combination of numbers, same as for Mappings>

Currently a max of 100 rules are supported.
Bucket1 => the Key pressed normal.
Bucket2 => the Key pressed with shift.
Bucket3 => the Key pressed with Ctrl-Alt

Each Rule takes a minimum of 2 keys, maximum of 3.

Example Ligatures section for VUTAM.
[Ligatures]
Rule1=BUCKET1 VK_D BUCKET1 VK_BRA_CLOSE 08734
Rule2=BUCKET1 VK_D BUCKET2 VK_BRA_CLOSE 241


SOURCE CODE:
============

This program was written to compile with MinGW, a port of GCC to Windows.
You can find out about it at http://www.mingw.org.


CONTACT:
========

V. Ramasami,  <vgr_ramasami@sancharnet.in>
Aruppukottai,
S.India.
22nd February, 2006.
