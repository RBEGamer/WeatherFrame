# WeatherFrame
A esp2866 powered weaher display.
It displays the current weather (rain, sun, snow,..) on the 8x8 pixel matrix with an anmimation.
The animation can be created by an excel sheet.
The weather data comes from openweathermap which required an valid api key (but its free :) )
* Openweathermap support for weatherdata
* WS2812 LED Matrix Display
* 8x8 Pixels 
* Framesize 25x25x4 cm
* Can be build only with 3D Printed parts! Please see /3d_parts/
For further build instructions,please see the pictures at /images/


# PARTS (FOR 8x8 FRAME)
 * 64 WS2812 LEDs
 * 64 3D Printed single_frame.stl Parts
 * Nodemcu V3 or an ohter ESP2866 Board
 * SD Card Reader (for storing the animation files)
 * 23K256 SPI SRAM IC
 * Levelshifter 3.3V -> 5V
 * Some Woodparts for the Frame
 * small sd card eg 1Gb for the animation files
 
# TOOLS
 * 3d printer
 * cutter
 * Wires
 * Plasicglue and/or hotglue
 * Solder stuff
 
# BUILD INSTRUCTION
 * Print all needed 3D Parts at the /3d_parts/ directory 
 * 64x single_frame.stl but this depent on your matrix size
 * glue all singe_frame pieces together (see at /images/)
 * glue the leds int the holes of your glued matrix
 * wire your leds up (DO -> DI, GND->GND, V+ -> V+)
 * connect the DI of your first LED with pin 5 of the ESP (see the WS2812_PIN define)
 * connect the SDCard reader with ESP SPI Bus and the CS pin to pin 5 (see the SD_CARD_CS_PIN define)
 * connect the SRAM Chip to the SPI Bus and the CS pin to pin 3 (see the SRAM_CS_PIN define)
 * glue the ESP,SDCARD,SRAM IC to the back of your matri
 * upload the firmware

# SOFTWARE SETUP
 * create a free account at http://openweathermap.org and request an api-key
 * download the sketch and open it in the actual Arduino IDE
 * paste your api-key at #define OWM_API_KEY _your_api_key_
 * set yout city-id (you can find it, if you choose an city at the owm homepage) at #define OWM_CITY_ID _your_city_id
 * setup your wifi konfig at ESP_WLAN_SSID "_your_ssid_" and ESP_WLAN_KEY "_your_key_"
 * upload the sketch
 
# SD CARD SETUP
 * download the sample SDCard image from this rep located at /owm_animations.img
 * burn the image to your SD Card use WinDiskImager(WIN), DiskUtility(MAC), Terminal(Unix)
 * put the card to the slot of your WeatherFrame
 
 
# (OPTIONAL) SOFTWARE SETUP
 * if necessary edit the pin-config (WS2812_PIN, SD_CARD_CS_PIN, SRAM_CS_PIN)
 * if necessary edit the matrix settings (VISIBLE_MATRIX_WITH,VISIBLE_MATRIX_HEIGHT,MATRIX_ORIGIN_LEFT_UP,MATRIX_MODE_ROW)
 * if your leds has no row/collum order you can use a lookuptable for the right id, see the comments at USE_LED_LOOKUP
 

# (OPTIONAL) ANIMATION SETUP
 * you need for every weather state (see the owm api documentation) an _weather_state_.anim ascii file in the root dir of the sd card
 * to create or edit our own animations directly in the .anim file or you use the animation creator tool or the frame_generator excel sheet
 * The colors of the animation are stored in a color lookup table. In the defualt code there are 64 Colors inspired from the NES System :)
   http://www.thealmightyguru.com/Games/Hacking/Wiki/index.php?title=File:Palette_NTSC.png
 * You can use these color codes to setup a frame for the frames stored in the *.anim file
 * The *.animfile stores the frames row by row, e.g. : _color_code_,_color_code_,...
 * The frames are seperated by a blank line.
 * With the 23K256 whitch has 32768Bytes
 * if you have no more space in your ram then you can define COLOR_TABLE_PROGMEM to move the color tbale to your flash
