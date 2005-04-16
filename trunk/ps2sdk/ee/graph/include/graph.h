/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (c) 2005 Dan Peori <peori@oopo.net>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/

#ifndef __GRAPH_H__
#define __GRAPH_H__

 #include <tamtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

 #define GRAPH_MODE_NTSC	0x00	// NTSC		(640x448i)
 #define GRAPH_MODE_NTSC_WIDE	0x01	// NTSC_WIDE	(768x448i)
 #define GRAPH_MODE_PAL		0x02	// PAL		(640x512i)
 #define GRAPH_MODE_PAL_WIDE	0x03	// PAL_WIDE	(896x512i)
 #define GRAPH_MODE_HDTV	0x04	// HDTV		(640x480p)
 #define GRAPH_MODE_HDTV_WIDE	0x05	// HDTV_WIDE	(832x480p)
 #define GRAPH_MODE_VGA		0x06	// VGA		(640x480p)
 #define GRAPH_MODE_VGA_WIDE	0x07	// VGA_WIDE	(832x480p)

 #define GRAPH_PSM_32		0x00	// 32 bits per pixel.
 #define GRAPH_PSM_24		0x01	// 24 bits per pixel.
 #define GRAPH_PSM_16		0x02	// 16 bits per pixel.
 #define GRAPH_PSM_8		0x13	// 8 bits per pixel, palettized.
 #define GRAPH_PSM_4		0x14	// 4 bits per pixel, palettized.
 #define GRAPH_PSM_8H		0x1B	// 8 bits per pixel, palettized.
 #define GRAPH_PSM_4HH		0x24	// 4 bits per pixel, palettized.
 #define GRAPH_PSM_4HL		0x2C	// 4 bits per pixel, palettized.

 typedef struct { int width, height, mode, interlace, size; u64 display; } GRAPH_MODE;

 /////////////////////
 // GRAPH FUNCTIONS //
 /////////////////////

 int graph_initialize(void);

 int graph_shutdown(void);

 //////////////////////////
 // GRAPH MODE FUNCTIONS //
 //////////////////////////

 int graph_mode_set(int mode, int psm, int zpsm);

 int graph_mode_get(GRAPH_MODE *mode);

 /////////////////////////
 // GRAPH SET FUNCTIONS //
 /////////////////////////

 int graph_set_displaybuffer(int address);

 int graph_set_drawbuffer(int address);

 int graph_set_zbuffer(int address);

 int graph_set_clearbuffer(int red, int green, int blue);

 //////////////////////////
 // GRAPH VRAM FUNCTIONS //
 //////////////////////////

 int graph_vram_read(int address, int width, int height, int psm, void *data, int data_size);

 int graph_vram_write(int address, int width, int height, int psm, void *data, int data_size);

 //////////////////////////
 // GRAPH WAIT FUNCTIONS //
 //////////////////////////

 int graph_wait_hsync(void);

 int graph_wait_vsync(void);

#ifdef __cplusplus
}
#endif

#endif
