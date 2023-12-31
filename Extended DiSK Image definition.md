<h1>Extended DiSK Image definition</h1>

<p>The extended DSK image is a file designed to describe copy-protected floppy disk software.
It's definition was defined by Marco Vieth, Ulrich Doewich and Kevin Thacker.
</p><p>This format has been widely adopted and is one of the major file formats
used (the other major format is the standard disk image with the "MV - CPC" identifier). 
</p><p>The extended disk image format should be used for copy-protected disc software,
or in place of a standard disk image if the resulting image is smaller than in the standard
disk image form.

</p>
<h2>Extended DiSK Format (Rev.5)</h2>

<p>The disc image has the following format:

<table border="1">
<tbody><tr>
<td>DISK INFORMATION BLOCK (256 bytes)</td>
</tr>
<tr>
<td>TRACK INFORMATION BLOCK * number of tracks * number of sides</td>
</tr>
</tbody></table>
</p><p>The track blocks are stored in increasing order 0..number of tracks, with alternating
sides interleaved if the disc image describes a double sided disk.
e.g. if the disk image represents a double sided disk, the order of tracks is:
track 0 side 0, track 0 side 1, track 1 side 0, track 1 side 1.... track (number of tracks-1) side 0,
track (number of tracks-1) side 1
</p><p>The tracks are <b>always</b> ordered in this way regardless of the disc-format described by the disc
image.
</p><p>The location of the track information block is found by using the track size table.
</p><h2>DISK INFORMATION BLOCK</h2>

<p>The "DISK INFORMATION BLOCK" is always located at offset 0 in the disk image file, and has
the following structure:

</p><p><table border="1">
<thead>
<tr>
<th>offset</th>
<th>description</th>
<td>bytes
</td></tr>
</thead>
<tbody>
<tr>
<td>00 - 21</td>
<td>"EXTENDED CPC DSK File\r\nDisk-Info\r\n"</td>
<td>34</td>
</tr>
<tr>
<td>22 - 2f</td>
<td>name of creator (utility/emulator)</td>
<td>14</td>
</tr>
<tr>
<td>30</td>
<td>number of tracks</td>
<td>1</td>
</tr>
<tr>
<td>31</td>
<td>number of sides</td>
<td>1</td>
</tr>
<tr>
<td>32 - 33</td>
<td>unused</td>
<td>2</td>
</tr>
<tr>
<td>34 - xx</td>
<td>track size table</td>
<td>number of tracks*number of sides</td>
</tr>
</tbody>
</table>

</p><p>NOTES:
</p><ul>
<li>An extended DSK image is identified by the "EXTENDED" tag. The 
track size at offset 32h and 33h, used by the STANDARD disk image
is ignored for extended format DSK images.
</li><li>If track data exists, then it starts at offset 100h.
</li><li>The "EXTENDED" tag is present to prevent existing emulators
which support the standard DSK image from interpreting the data
wrong and possibly crashing.
</li><li>"\r" is the C programming language equivalent of ASCII character 13.
</li><li>"\n" is the C programming language equivalent of ASCII character 10.
</li></ul>


<h2>TRACK OFFSET TABLE</h2>

<table border="1">
<thead>
<tr>
<th>offset</th>
<th>description</th>
<th>bytes</th>
</tr>
</thead>
<tbody>
<tr>
<td>01</td>
<td>high byte of track 0 length (equivalent to track length/256)</td>
<td>1</td>
</tr>
<tr>
<td>...</td>
<td>...</td>
<td>...</td>
</tr>
</tbody>
</table>

<p>NOTES:
</p><ul>
<li>Depending on the information in the disk information block, the table contains
<ul>
<li>track lengths for a single sided floppy disc
</li><li>track lengths for a double sided floppy disc 
</li></ul>
</li><li>track lengths are stored in the same order as the tracks in the image
e.g. In the case of a double sided disk: Track 0 side 0, Track 0 side 1, Track
	1 side 0 etc...
</li><li>A size of "0" indicates an unformatted track. In this case there is no data,
and no track information block for this track in the image file!
</li><li>Actual length  of track data = (high byte of track length) * 256
</li><li>Track length includes the size of the TRACK INFORMATION BLOCK (256 bytes)
</li><li>The location of a Track Information Block for a chosen track is found by summing
	the sizes of all tracks up to the chosen track plus the size of the Disc
	Information Block (&amp;100 bytes). The first track is at offset &amp;100
	in the disc image.
</li></ul>

<h2>TRACK INFORMATION BLOCK</h2>
<table border="1">
<thead>
<tr>
<th>offset</th>
<th>description</th>
<th>bytes</th>
</tr>
</thead>
<tbody>
<tr>
<td>00 - 0c</td>
<td>"Track-Info\r\n"</td>
<td>13</td>
</tr>
<tr>
<td>0d - 0f</td>
<td>unused</td>
<td>3</td>
</tr>
<tr>
<td>10</td>
<td>track number</td>
<td>1</td>
</tr>
<tr>
<td>11</td>
<td>side number</td>
<td>1</td>
</tr>
<tr>
<td>12 - 13</td>
<td>unused</td>
<td>2</td>
</tr>
<tr>
<td>14</td>
<td>sector size</td>
<td>1</td>
</tr>
<tr>
<td>15</td>
<td>number of sectors</td>
<td>1</td>
</tr>
<tr>
<td>16</td>
<td>GAP#3 length</td>
<td>1</td>
</tr>
<tr>
<td>17</td>
<td>filler byte</td>
<td>1</td>
</tr>
<tr>
<td>18 - xx</td>
<td>Sector Information List</td>
<td>xx</td>
</tr>
</tbody>
</table>


<p>NOTES:
</p><ul>
<li>Identical to the original DSK format definition.
</li></ul>

<h2>SECTOR INFORMATION LIST</h2>

<table border="1">
<thead>
<tr>
<th>offset</th>
<th>description</th>
<th>bytes</th>
</tr>
</thead>
<tbody>
<tr>
<td>00</td>
<td>track (equivalent to C parameter in NEC765 commands)</td>
<td>1</td>
</tr>
<tr>
<td>01</td>
<td>side (equivalent to H parameter in NEC765 commands)</td>
<td>1</td>
</tr>
<tr>
<td>02</td>
<td>sector ID (equivalent to R parameter in NEC765 commands)</td>
<td>1</td>
</tr>
<tr>
<td>03</td>
<td>sector size (equivalent to N parameter in NEC765 commands)</td>
<td>1</td>
</tr>
<tr>
<td>04</td>
<td>FDC status register 1 (equivalent to NEC765 ST1 status register)</td>
<td>1</td>
</tr>
<tr>
<td>05</td>
<td>FDC status register 2 (equivalent to NEC765 ST2 status register)</td>
<td>1</td>
</tr>
<tr>
<td>06 - 07</td>
<td>actual data length in bytes</td>
<td>2</td>
</tr>
</tbody>
</table>

<p>NOTES:  
</p><ul>
<li>Identical to the original definition except for the addition of the
        sector data length. This value is in bytes and stored in little
        endian notation. (low byte followed by high byte)
</li><li>The location of each sectors data is found by adding the size
	of the previous sectors, plus the size of the 256 byte header.
</li><li>For 8k Sectors (N="6"), only 1800h bytes is stored. Please see extensions below!!!
</li></ul>

<h2>Extensions to the above specification</h2>

<ol>
<li>This extension was proposed by John Elliott.

<p>Extension to TRACK INFORMATION BLOCK:
</p><p><table border="1">
<thead>
<tr>
<th>offset</th>
<th>description</th>
<th>bytes</th>
</tr>
</thead>
<tbody>
<tr>
<td>12</td>
<td>Data rate. (See note 1 and note 3)</td>
<td>1</td>
</tr>
<tr>
<td>13</td>
<td>Recording mode. (See note 2 and note 3)</td>
<td>1</td>
</tr>
</tbody>
</table>

</p><p>Notes:
</p><ol>
<li>Data rate defines the rate at which data was written to the track.
This value applies to the entire track.
<p><table border="1">
<thead>
<tr>
<th>Date rate</th>
<th>description</th>
</tr>
</thead>
<tbody>
<tr>
<td>0</td>
<td>Unknown.</td>
</tr>
<tr>
<td>1</td>
<td>Single or double density</td>
</tr>
<tr>
<td>2</td>
<td>High Density</td>
</tr>
<tr>
<td>3</td>
<td>Extended density</td>
</tr>
</tbody>
</table>

</p><p>Existing files should have zeroes in these bytes; hence the use of 0 for unknown.

</p></li><li>Recording mode is used to define how the data was written. It defines the encoding used to write the data to the disc
and the structure of the data on the disc including the layout of the sectors. This value applies to the entire track.

<p><table border="1">
<thead>
<tr>
<th>Date rate</th>
<th>description</th>
</tr>
</thead>
<tbody>
<tr>
<td>0</td>
<td>Unknown.</td>
</tr>
<tr>
<td>1</td>
<td>FM</td>
</tr>
<tr>
<td>2</td>
<td>MFM</td>
</tr>
</tbody>
</table>

</p><p>Existing files should have zeroes in these bytes; hence the use of 0 for unknown.

</p></li><li>How to determine the actual rate.
<p>The NEC765 floppy disc controller is supplied with a single clock. When reading from and writing to a disc using the NEC765 you can choose FM or MFM
recording modes. Use of these modes and the clock into the NEC765 define the final rate at which the data is written to the disc.
</p><p>When FM recording mode is used, data is read from or written to at a rate which is double that of when MFM is used. The time for each bit will be twice the time for MFM.
</p><p>Examples:
</p><p><table border="1">
<thead>
<tr>
<th>NEC765 Clock</th>
<th>FM/MFM</th>
<th>Actual rate</th>
</tr>
</thead>
<tbody>
<tr>
<td>4Mhz</td>
<td>FM</td>
<td>4us per bit</td>
</tr>
<tr>
<td>4Mhz</td>
<td>MFM</td>
<td>2us per bit</td>
</tr>
</tbody>
</table>

</p></li></ol>

</li><li>This extension was proposed by Simon Owen.

<ol>
<li>It has been found that many protections using 8K Sectors (N="6") do store more 
than &amp;1800 bytes of useable data. It was thought that &amp;1800 was the maximum useable limit, but 
this has proved wrong. So you should support 8K of data to ensure this data is read correctly.
The size of the sector will be reported in the SECTOR INFORMATION LIST as described above.
<p>For sector size N="7" the full 16K will be stored. It is assumed that sector sizes are defined
as 3 bits only, so that a sector size of N="8" is equivalent to N="0".

</p></li><li>Storing Multiple Versions of Weak/Random Sectors.
<p>Some copy protections have what is described as 'weak/random' data. Each time the sector is read one or more bytes will change, the value may be random between consecutive reads of the same sector.
</p><p>To support these formats the following extension has been proposed.
</p><p>Where a sector has weak/random data, there are multiple copies stored. The actual sector size field in the SECTOR INFORMATION LIST describes the size of all the copies.
To determine if a sector has multiple copies then compare the actual sector size field to the size defined by the N parameter. For multiple copies the actual sector size field will have a value which is a multiple of the size defined by the N parameter.
The emulator should then choose which copy of the sector it should return on each read.





</p></li></ol></li></ol>