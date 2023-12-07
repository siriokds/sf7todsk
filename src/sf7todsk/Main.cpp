#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/stat.h>   // stat
#include <stdbool.h>    // bool type

#include <stdint.h>

#define SF7_SECTOR_SIZE				256
#define SF7_SECTOR_PER_TRACKS		16
#define SF7_NUM_OF_TRACKS			40
#define SF7_NUM_OF_SIDES			1

#define SF7_TRACK_LEN	(SF7_SECTOR_SIZE * SF7_SECTOR_PER_TRACKS * SF7_NUM_OF_SIDES)		// 4096
#define SF7_DISKSIZE	(SF7_TRACK_LEN * SF7_NUM_OF_TRACKS)									//163840

#include "EDSK_Utils.h"



void CONIO_WriteChar(char c) { printf("%c", c); }
void CONIO_WriteString(const char* s) {	printf("%s", s);}
void CONIO_WriteLine(const char* s) {	printf("%s\n",s);}
void CONIO_WriteByte(uint8_t v) {	printf("%02X", v); }
void CONIO_WriteShort(uint16_t v) { printf("%04X", v); }
void CONIO_WriteDword(uint32_t v) { printf("%08X", v); }

bool file_exists(char* filename) {
	struct stat st;
	return (stat(filename, &st) == 0);
}

int file_size(char* filename) {
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

int usage()
{
	CONIO_WriteLine("Usage : <filename.sf7> [dsk_filename]\n");

	return 0x01;
}

int main(int argc, char* argv[])
{
	CONIO_WriteLine("SF7TODSK v1.0: Sega SF-7000 .sf7 disk file to .dsk (Extended DSK) converter. SiRioKD 2023. Published under GNU General Public License (GPL)");

	char srcFilename[255];
	char dstFilename[255];
	size_t srcFilenameLength;

	if (argc < 2)
		return usage();

	srcFilenameLength = strlen(argv[1]);
	if (srcFilenameLength < 5)
	{
		printf("invalid sf7 filename.\n\n");
		return 0x02;
	}

	strcpy(srcFilename, argv[1]);

	char* ext = &srcFilename[srcFilenameLength - 4];
	if (_stricmp(ext, ".sf7") != 0)
	{
		printf("invalid sf7 filename extension.\n\n");
		return 0x03;
	}

	if (!file_exists(argv[1]))
	{
		printf("File %s not found\n\n", argv[1]);
		return 0x04;
	}

	if (file_size(argv[1]) != SF7_DISKSIZE)
	{
		printf("invalid sf7 file size (must be %d bytes).\n\n", SF7_DISKSIZE);
		return 0x05;
	}


	if (argc > 2)
	{
		strcpy(dstFilename, argv[2]);
	}
	else
	{
		strcpy(dstFilename, argv[1]);
		dstFilename[srcFilenameLength - 3] = 'd';
		dstFilename[srcFilenameLength - 2] = 's';
		dstFilename[srcFilenameLength - 1] = 'k';
	}
	
	if (file_exists(dstFilename))
	{
		if (remove(dstFilename) != 0)
		{
			printf("Cannot overwrite destination filename: '%s'.", dstFilename);
			return 0x10;
		}
	}

	FILE* fp;

	uint8_t sf7Data[SF7_DISKSIZE];

	for (int i = 0; i < SF7_DISKSIZE; i++) sf7Data[i] = 0xFF;


	if ((fp = fopen(srcFilename, "rb")) != 0)
	{
		fread(fp, 1, SF7_DISKSIZE, fp);
		fclose(fp);
	}

	if ((fp = fopen(dstFilename, "wb")) != 0)
	{
		WriteDiskHeader(fp, (char*)"SiRioKD");

		int sf7DataOffset = 0;

		for (int i = 0; i < SF7_NUM_OF_TRACKS; i++)
		{
			WriteTrackHeader(fp, (uint8_t)i);
			fwrite(sf7Data + sf7DataOffset, 1, SF7_TRACK_LEN, fp);
			sf7DataOffset += SF7_TRACK_LEN;
		}

		fclose(fp);
	}

	int dstFileSize = file_size(dstFilename);
	printf("Successfully created disk '%s', size: %d bytes.\n\n", dstFilename, dstFileSize);



	return 0;
}