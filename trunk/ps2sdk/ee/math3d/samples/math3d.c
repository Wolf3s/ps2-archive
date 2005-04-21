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

 #include <tamtypes.h>

 #include <dma.h>
 #include <stdio.h>
 #include <graph.h>
 #include <malloc.h>
 #include <math3d.h>
 #include <packet.h>

 int lines[12][2] = {
  { 0, 1 }, { 1, 3 }, { 3, 2 },
  { 2, 0 }, { 4, 5 }, { 5, 7 },
  { 7, 6 }, { 6, 4 }, { 0, 4 },
  { 2, 6 }, { 1, 5 }, { 3, 7 }
 };

 int triangles[12][3] = {
  { 0, 1, 2 }, { 1, 2, 3 }, { 4, 5, 6 },
  { 5, 6, 7 }, { 1, 5, 3 }, { 5, 3, 7 },
  { 0, 4, 2 }, { 4, 2, 6 }, { 4, 5, 0 },
  { 5, 0, 1 }, { 6, 7, 2 }, { 7, 2, 3 }
 };

 VECTOR points[8] __attribute__ ((aligned (128))) = {
  { -10.00f, -10.00f, -10.00f, 1.00f },
  {  10.00f, -10.00f, -10.00f, 1.00f },
  { -10.00f,  10.00f, -10.00f, 1.00f },
  {  10.00f,  10.00f, -10.00f, 1.00f },
  { -10.00f, -10.00f,  10.00f, 1.00f },
  {  10.00f, -10.00f,  10.00f, 1.00f },
  { -10.00f,  10.00f,  10.00f, 1.00f },
  {  10.00f,  10.00f,  10.00f, 1.00f }
 };

 u64 colours[5] = {
  GIF_SET_RGBAQ(0x80, 0x00, 0x00, 0x80, 0x3F800000),
  GIF_SET_RGBAQ(0x00, 0x80, 0x00, 0x80, 0x3F800000),
  GIF_SET_RGBAQ(0x00, 0x00, 0x80, 0x80, 0x3F800000),
  GIF_SET_RGBAQ(0x00, 0x00, 0x80, 0x80, 0x3F800000),	// Gradient top.
  GIF_SET_RGBAQ(0x00, 0x00, 0x00, 0x80, 0x3F800000)	// Gradient bottom.
 };

 VECTOR object_position = { 0.00f, 0.00f, 0.00f, 0.00f };
 VECTOR object_rotation = { 0.00f, 0.00f, 0.00f, 0.00f };

 VECTOR camera_position = { 0.00f, 0.00f, -50.00f, 0.00f };
 VECTOR camera_rotation = { 0.00f, 0.00f,   0.00f, 0.00f };

 int main() { int loop0;

  PACKET packet;

  VECTORI pointsi[8]  __attribute__ ((aligned (128)));

  MATRIX local_world;
  MATRIX world_view;
  MATRIX view_screen;
  MATRIX local_screen;

  // Initialize the dma library.
  dma_initialize();

  // Initialize the graphics library.
  graph_initialize();

  // Allocate space for the packet.
  packet_allocate(&packet, 1024);

  // Set the graphics mode.
  graph_set_mode(GRAPH_MODE_NTSC, GRAPH_PSM_32, GRAPH_PSM_32);		// NTSC (640x448i@60Hz)
  // graph_mode_set(GRAPH_MODE_PAL,  GRAPH_PSM_32, GRAPH_PSM_32);	// PAL  (640x512i@50Hz)
  // graph_mode_set(GRAPH_MODE_HDTV, GRAPH_PSM_32, GRAPH_PSM_32);	// HDTV (720x420p@60Hz)
  // graph_mode_set(GRAPH_MODE_VGA,  GRAPH_PSM_32, GRAPH_PSM_32);	// VGA  (640x480p@60Hz)

  // Set the display buffer address.
  graph_set_displaybuffer(0);

  // Set the draw buffer address.
  graph_set_drawbuffer(0);

  // Set the zbuffer address.
  graph_set_zbuffer(graph_get_width() * graph_get_height() * (graph_get_bpp() >> 3));

  // Create the view_screen matrix.
  create_view_screen(view_screen, graph_get_width(), graph_get_height(), 16777215, 4, 3, 512.00f, 1.00f, 65536.00f);		// Standard (4:3)
  // create_view_screen(view_screen, graph_get_width(), graph_get_height(), 16777215, 16, 9, 512.00f, 1.00f, 65536.00f);	// Widescreen (16:9)

  // The main loop...
  for (;;) {

   // Spin the cube a bit.
   object_rotation[1] += 0.010f; if (object_rotation[1] > 3.14f) { object_rotation[1] -= 6.28f; }
   object_rotation[2] += 0.013f; if (object_rotation[2] > 3.14f) { object_rotation[2] -= 6.28f; }

   // Create the local_world matrix. (object space)
   create_local_world(local_world, object_position, object_rotation);

   // Create the world_view matrix. (camera space)
   create_world_view(world_view, camera_position, camera_rotation);

   // Create the local_screen matrix.
   create_local_screen(local_screen, local_world, world_view, view_screen);

   // Apply the local_screen matrix to the points.
   point_calculate(pointsi, local_screen, points, 8);

   // Wait for the vsync period.
   graph_wait_vsync();

   // Clear the screen.
   graph_set_clearbuffer(80, 80, 80);

   // Reset the packet.
   packet_reset(&packet);

   // Draw the gradient.
   packet_append_64(&packet, GIF_SET_TAG(10, 1, 0, 0, GIF_TAG_PACKED, 1));
   packet_append_64(&packet, 0x0E);
   packet_append_64(&packet, GIF_SET_PRIM(3, 1, 0, 0, 0, 1, 0, 0, 0));
   packet_append_64(&packet, GIF_REG_PRIM);
   packet_append_64(&packet, colours[3]);
   packet_append_64(&packet, GIF_REG_RGBAQ);
   packet_append_64(&packet, GIF_SET_XYZ((2048 - (graph_get_width() / 2)) << 4, (2048 - (graph_get_height() / 2)) << 4, 0));
   packet_append_64(&packet, GIF_REG_XYZ2);
   packet_append_64(&packet, GIF_SET_XYZ((2048 + (graph_get_width() / 2)) << 4, (2048 - (graph_get_height() / 2)) << 4, 0));
   packet_append_64(&packet, GIF_REG_XYZ2);
   packet_append_64(&packet, colours[4]);
   packet_append_64(&packet, GIF_REG_RGBAQ);
   packet_append_64(&packet, GIF_SET_XYZ((2048 + (graph_get_width() / 2)) << 4, (2048 + (graph_get_height() / 2)) << 4, 0));
   packet_append_64(&packet, GIF_REG_XYZ2);
   packet_append_64(&packet, GIF_SET_XYZ((2048 + (graph_get_width() / 2)) << 4, (2048 + (graph_get_height() / 2)) << 4, 0));
   packet_append_64(&packet, GIF_REG_XYZ2);
   packet_append_64(&packet, GIF_SET_XYZ((2048 - (graph_get_width() / 2)) << 4, (2048 + (graph_get_height() / 2)) << 4, 0));
   packet_append_64(&packet, GIF_REG_XYZ2);
   packet_append_64(&packet, colours[3]);
   packet_append_64(&packet, GIF_REG_RGBAQ);
   packet_append_64(&packet, GIF_SET_XYZ((2048 - (graph_get_width() / 2)) << 4, (2048 - (graph_get_height() / 2)) << 4, 0));
   packet_append_64(&packet, GIF_REG_XYZ2);

   // Draw the lines.
   for (loop0=0;loop0<12;loop0++) {
    packet_append_64(&packet, GIF_SET_TAG(4, 1, 1, GIF_SET_PRIM(1, 1, 0, 0, 0, 1, 0, 0, 0), GIF_TAG_PACKED, 1));
    packet_append_64(&packet, 0x0E);
    packet_append_64(&packet, GIF_SET_RGBAQ(0x00, 0x00, 0x00, 0x80, 0x3F800000));
    packet_append_64(&packet, GIF_REG_RGBAQ);
    packet_append_64(&packet, GIF_SET_XYZ(pointsi[lines[loop0][0]].x, pointsi[lines[loop0][0]].y, pointsi[lines[loop0][0]].z));
    packet_append_64(&packet, GIF_REG_XYZ2);
    packet_append_64(&packet, GIF_SET_RGBAQ(0x00, 0x00, 0x00, 0x80, 0x3F800000));
    packet_append_64(&packet, GIF_REG_RGBAQ);
    packet_append_64(&packet, GIF_SET_XYZ(pointsi[lines[loop0][1]].x, pointsi[lines[loop0][1]].y, pointsi[lines[loop0][1]].z));
    packet_append_64(&packet, GIF_REG_XYZ2);
   }

   // Draw the triangles.
   for (loop0=0;loop0<12;loop0++) {
    packet_append_64(&packet, GIF_SET_TAG(6, 1, 1, GIF_SET_PRIM(3, 1, 0, 0, 0, 0, 0, 0, 0), GIF_TAG_PACKED, 1));
    packet_append_64(&packet, 0x0E);
    packet_append_64(&packet, colours[loop0 >> 2]);
    packet_append_64(&packet, GIF_REG_RGBAQ);
    packet_append_64(&packet, GIF_SET_XYZ(pointsi[triangles[loop0][0]].x, pointsi[triangles[loop0][0]].y, pointsi[triangles[loop0][0]].z));
    packet_append_64(&packet, GIF_REG_XYZ2);
    packet_append_64(&packet, colours[loop0 >> 2]);
    packet_append_64(&packet, GIF_REG_RGBAQ);
    packet_append_64(&packet, GIF_SET_XYZ(pointsi[triangles[loop0][1]].x, pointsi[triangles[loop0][1]].y, pointsi[triangles[loop0][1]].z));
    packet_append_64(&packet, GIF_REG_XYZ2);
    packet_append_64(&packet, colours[loop0 >> 2]);
    packet_append_64(&packet, GIF_REG_RGBAQ);
    packet_append_64(&packet, GIF_SET_XYZ(pointsi[triangles[loop0][2]].x, pointsi[triangles[loop0][2]].y, pointsi[triangles[loop0][2]].z));
    packet_append_64(&packet, GIF_REG_XYZ2);
   }

   // Send off the packet.
   packet_send(&packet, DMA_CHANNEL_GIF, DMA_FLAG_NORMAL);

  }

  // Shut down the graphics library.
  graph_shutdown();

  // Shut down the dma library.
  dma_shutdown();

  // End program.
  return 0;

 }
