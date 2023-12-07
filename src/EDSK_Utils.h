#pragma once


void WriteDiskHeader(FILE* fp, char* creatorName)
{
	const uint8_t headCR[] = { 0x0D, 0x0A };
	const char head1[] = { "EXTENDED CPC DSK File" };
	const char head2[] = { "Disk-Info" };

	fprintf(fp, head1);
	fwrite(headCR, 1, 2, fp);
	fprintf(fp, head2);
	fwrite(headCR, 1, 2, fp);

	char _creatorName[14];
	
	char* d = _creatorName;
	char* s = creatorName;
	int len = 13;
	while (*s && len-- > 0)		*d++ = *s++;
	while (len-- > 0)			*d++ = 0;

	fwrite(_creatorName, 1, 14, fp);

	fputc((uint8_t)SF7_NUM_OF_TRACKS, fp);
	fputc((uint8_t)SF7_NUM_OF_SIDES, fp);
	fputc((uint8_t)0x00, fp);
	fputc((uint8_t)0x00, fp);

	for (int i = 0; i < SF7_NUM_OF_TRACKS; i++)
		fputc((uint8_t)(SF7_TRACK_LEN >> 8) + 1, fp);

	for (int i = 0; i < 164; i++)								// Filler to 256
		fputc((uint8_t)0x00, fp);
}



void WriteTrackHeader(FILE* fp, uint8_t tracknum)
{
	const uint8_t headCR[] = { 0x0D, 0x0A };
	const char head1[] = { "Track-Info" };

	fprintf(fp, head1);
	fwrite(headCR, 1, 2, fp);
	fputc((uint8_t)0x00, fp);

	fputc((uint8_t)0x00, fp);
	fputc((uint8_t)0x00, fp);
	fputc((uint8_t)0x00, fp);


	fputc((uint8_t)tracknum, fp);		// Track
	fputc((uint8_t)0x00, fp);			// Side

	fputc((uint8_t)0x00, fp);
	fputc((uint8_t)0x00, fp);

	fputc((uint8_t)0x01, fp);
	fputc((uint8_t)0x10, fp);
	fputc((uint8_t)0x2A, fp);
	fputc((uint8_t)0xE5, fp);

	for (int i = 0; i < SF7_SECTOR_PER_TRACKS; i++)
	{
		fputc((uint8_t)tracknum, fp);					// Track
		fputc((uint8_t)0x00, fp);						// Side
		fputc((uint8_t)(i + 1), fp);					// Sector ID
		fputc((uint8_t)(SF7_SECTOR_SIZE >> 8), fp);		// Sector Size

		fputc((uint8_t)0x00, fp);						// FDC status register 1 (equivalent to NEC765 ST1 status register)
		fputc((uint8_t)0x00, fp);						// FDC status register 2 (equivalent to NEC765 ST2 status register)

		// Actual data length in bytes
		fputc((uint8_t)(SF7_SECTOR_SIZE & 255), fp);	// Low byte
		fputc((uint8_t)(SF7_SECTOR_SIZE >> 8), fp);		// High byte
	}	

	for (int i = 0; i < 104; i++)						// Filler to 256
		fputc((uint8_t)0x00, fp);
}
