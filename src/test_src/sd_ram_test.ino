

#include <SPI.h>
#include <SpiRAM.h>
#include "SdFat.h"
#include "FatLib\FatFile.h"
// SD chip select pin
const uint8_t sd_chipSelect = 4;
const uint8_t sram_chipSelect = 10;
// file system object
SdFat sd;

// create a serial stream
ArduinoOutStream cout(Serial);

String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

#define nullptr NULL
#define byte unsigned char
#define SD_READ_LINE_MAX_LENGHT  50

SpiRAM spiRam(0, sram_chipSelect);

inline unsigned char read_spi_ram( SpiRAM* _sram, unsigned int _addr) {
	return (unsigned char)_sram->read_byte((int)_addr);
}

inline void write_spi_ram(SpiRAM* _sram, unsigned int _addr, unsigned char _value) {
	_sram->write_byte(_addr, (char)_value);
}

//FRAME PARSE INFO
struct SD_FRAME_HEADER {
	unsigned char animation_id;
	unsigned char frame_curr;
	unsigned char  frame_max;
	unsigned char frame_data_w;
	unsigned char frame_data_h;
	void print_header() {
		Serial.print("FRAME_HEADER FOR ANIMATION:"); Serial.print(animation_id); Serial.print(" CURR:"); Serial.print(frame_curr); Serial.print(" FROM : "); Serial.print(frame_max); Serial.print(" W : "); Serial.print(frame_data_w); Serial.print(" H : "); Serial.println(frame_data_h);
	}
};


void write_sd_animation_to_sram(const char* _path) {

	char _buffer[SD_READ_LINE_MAX_LENGHT];
	//CLEAR BUFFER
	for (int i = 0; i < SD_READ_LINE_MAX_LENGHT; i++) {
		_buffer[i] = '\0';
	}

	ifstream sdin(_path);
	SD_FRAME_HEADER tmp_header;
	bool frame_started = false;
	int animation_id_counter = 0;
	unsigned int next_frame_offset = 0; //yeah startoffset ggf add further header offset
	unsigned int next_data_offset = sizeof(SD_FRAME_HEADER);
	int row_counter = 0;
	while (sdin.getline(_buffer, SD_READ_LINE_MAX_LENGHT, '\n') || sdin.gcount()) {

		//GET CURRENT LINE
		String tmp_line = "";
		for (int i = 0; i < sdin.gcount(); i++) {
			tmp_line += String(_buffer[i]);
		}
		//CLEAR BUFFER
		for (int i = 0; i < SD_READ_LINE_MAX_LENGHT; i++) {
			_buffer[i] = '\0';
		}
		Serial.println(tmp_line);
		//CHECK FOR LINE TYPE
		if (tmp_line.indexOf("_") > 0) {

			tmp_header.frame_curr = getValue(tmp_line, '_', 1).toInt();
			tmp_header.frame_max = getValue(tmp_line, '_', 2).toInt();
			tmp_header.frame_data_w = getValue(tmp_line, '_', 3).toInt();
			tmp_header.frame_data_h = getValue(tmp_line, '_', 4).toInt();
			tmp_header.animation_id = animation_id_counter;
			tmp_header.print_header();

			frame_started = true;
			animation_id_counter++;

			//WRITE HEADER TO SRAM FERTIG
			//edit write struct to sram

			write_spi_ram(&spiRam, next_frame_offset+0, tmp_header.animation_id);
			write_spi_ram(&spiRam, next_frame_offset+1, tmp_header.frame_curr);
			write_spi_ram(&spiRam, next_frame_offset+2, tmp_header.frame_max);
			write_spi_ram(&spiRam, next_frame_offset+3, tmp_header.frame_data_w);
			write_spi_ram(&spiRam, next_frame_offset+4, tmp_header.frame_data_h);



		}
		else if (tmp_line.indexOf(",") > 0) {

				//BLA BLA BLA WRITE TO RAM BLA BLA BLA
			//immer mit index of das n#chte , finden und di parsen
			int calc_ram_index = row_counter*tmp_header.frame_data_h + i;
			write_spi_ram(&spiRam, next_data_offset + calc_ram_index, read_value);

			row_counter++;
		}
		else if (tmp_line == "" || sdin.eof()) //seems to be an "" line in the cs project i have checked this twice
		{
			frame_started = false;
			next_frame_offset += sizeof(tmp_header) + (tmp_header.frame_data_w* tmp_header.frame_data_h * sizeof(byte));
			next_data_offset = next_frame_offset + sizeof(SD_FRAME_HEADER);
			Serial.print("FRAME FINISH next frame offset is "); Serial.print(next_frame_offset); Serial.print(" NEXT DATA OFFSET IS:"); Serial.println(next_data_offset);
		}

	}
	sdin.close();
}


void setup(void) {
	Serial.begin(115200);

	// Wait for USB Serial
	while (!Serial) {
		SysCall::yield();
	}


	//INIT SD
	if (!sd.begin(sd_chipSelect, SPI_FULL_SPEED)) {
		sd.initErrorHalt();
	}

	write_sd_animation_to_sram("animation.txt");

}
//------------------------------------------------------------------------------
void loop(void) {}
