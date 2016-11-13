# TODO

## DOCU
* git pfade anpassen


## ARD SRC

* header für animations states adden im ram bei index 0 -> per anzahle der *.anim files erstellen
* animation states states in extern eeprom sichern
* color_table.csv auch von sd lesen und im ram speicerhn

(immer nur die aktuelle animation im ram speicerhn sonst neu laden)
->vll ein age hinzufügen und die älteste kicken

-> für dem ram eine usage table adden die angibt wie viele byte unangefasst sind
-> free methode hinzufügen

->read delay to layer header
-> add rainmow mode
-> add clock mode

-> extra debug func print to serial adden ink line and func name
## FRAME_BUILDER GUI

* max 255 layer
* limit sram adden
* matrix wh auf 255 limiten


* add color button adden
-> colo picker farbe wählen und dann im array nach der nächsten verfügbaren farbe suchen wenn abstand zur gross zum array hinzufügen
-> color tbale zum ram dazuzählen
->add delay to layer header
->wenn beendet schreibe neue color table to basic color table
-> bmp import ->expand color table -> singe bmp eg 16x16 or multiframe bmp 16x512
