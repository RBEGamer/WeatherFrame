# WeatherFrame
## WIP !!!

A esp2866 powered weaher display.
It displays the current weather (rain, sun, snow,..) on the 8x8 pixel matrix with an anmimation.
The animation can be created by an excel sheet or pc tool (see `src/FRAME_BUILDER_src/`).
The weather data comes from openweathermap which required an valid api key (but its free :) )
* Openweathermap support for weatherdata
* WS2812 LED Matrix Display
* 8x8 Pixels 
* Framesize 25x25x4 cm
* Can be build only with 3D Printed parts! Please see `/3d_parts/`
For further build instructions,please see the pictures at `/images/


# PARTS (FOR 8x8 FRAME)
 * 64x WS2812 LEDs
 * 64x 3D Printed single_frame.stl Parts
 * Nodemcu V3 or an ohter ESP2866 Board
 * 23K256 SRAM IC
 * 24LC32 I2C EEprom
 * Levelshifter 3.3V -> 5V 4 channel
 * SD Card breakooutboard
 * Some Wooden parts for the Frame
 * small sd card eg 1Gb
 * 250x250x3mm 75% tranparency acryl plate (FRONT_PLATE)
 * 250x250x3mm black acryl plate (BACK_PLATE)
 * jumperwires
 * 3x 6mm push buttons
 
# TOOLS
 * 3d printer
 * cutter
 * Wires
 * Plasicglue and/or hotglue
 * Solder stuff
 * SD CARD Reader
 * Drill 6mm

# NEEDED SOFTWARE
 * latest Arduino IDE : https://www.arduino.cc
 * SPI-SRAM Libary : http://playground.arduino.cc/Main/SpiRAM
 * Adafruit-NeoPixel Libary : https://github.com/adafruit/Adafruit_NeoPixel
 
### VS-PROJECT
 * in the `src/arduino_src` is a Visual-Studio project attached too because i am using VisualMicro for Arduino developing :
 http://www.visualmicro.com
 

# BUILD INSTRUCTION
 * Print all needed 3D Parts at the `/3d_parts/` directory 
 * 64x ´single_frame.stl´ but this depent on your matrix size
 * glue all singe_frame pieces together (see `/images/`)
 * glue the leds int the holes of your glued matrix
 * wire your leds up (`DO -> DI`, `GND->GND`, `V+ -> V+`)
 * connect the DI of your first LED with pin 5 of the ESP (see the `WS2812_PIN define`)
 * connect the SDCard reader with ESP SPI Bus and the CS pin to pin 5 (see the `SD_CARD_CS_PIN define`)
 * connect the SRAM Chip to the SPI Bus and the CS pin to pin 3 (see the ´SRAM_CS_PIN define´)
 * glue the ESP, SDCARD, SRAM IC to the back of your matrix
 * upload the firmware

# SOFTWARE SETUP
 * create a free account at http://openweathermap.org and request an api-key
 * download the sketch (`/src/arduino_src/frame_framework/frame_framework.ino`) and open it in the actual Arduino IDE
 * paste your api-key at `#define OWM_API_KEY _your_api_key_`
 * set yout city-id (you can find it, if you choose an city at the owm homepage) at `#define OWM_CITY_ID _your_city_id`
 * setup your wifi konfig at `ESP_WLAN_SSID "_your_ssid_"` and `ESP_WLAN_KEY "_your_key_"`
 * upload the sketch
 
# SD CARD SETUP
 * download the sample SDCard image from this rep located at `/owm_animations.img`
 * burn the image to your SD Card use WinDiskImager(WIN), DiskUtility(MAC), Terminal(Unix)
 * put the card to the slot of your WeatherFrame
 
 
# (OPTIONAL) SOFTWARE SETUP
 * if necessary edit the pin-config (`WS2812_PIN`, `SD_CARD_CS_PIN`, `SRAM_CS_PIN`)
 * if necessary edit the matrix settings (`VISIBLE_MATRIX_WITH`, `VISIBLE_MATRIX_HEIGHT`, `MATRIX_ORIGIN_LEFT_UP`, `MATRIX_MODE_ROW`)
 * if your leds has no row/collum order you can use a lookuptable for the right id, see the comments at `USE_LED_LOOKUP`
 

 
# ANIMATION SETUP | ADD/EDIT ANIMATIONS
* Download and install the custom animation tool PixelFrameAnimator https://github.com/RBEGamer/PixelFrameAnimator
* Set the size at the settings tab to the size of your weather frame
* Draw your Frames or import BMP Files
* Export the File and save it to your sd card


### INFO ABOUT ANIMATION,RAM AND PIXELS [OLD VERSION]
 * each animation can have up to 256 frame with a matrix size of `256x256 pixels
 * the maximum space for this setup is `256(frames)*256(W)*256(H)*1(COLOR)+ 256(FRAMES)*5(HEADER) = 17.564KByte (animation data) + 256(colors)*3(rgb) = 18.332KByte (total)`  but that wont fit into your SRAM
 * for a typical animation of 5 frames @ 8x8 pixel + Header + 8(unique not shared colors) will require 328(animation) + 32(color)Byte of SRAM so the 32K256 IC can hold up to 99 animations

# TODO
For the TODO list see `TODOList.md`
