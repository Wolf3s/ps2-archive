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

 #include <dma.h>
 #include <dma_registers.h>
 #include <tamtypes.h>
 #include <kernel.h>
 #include <malloc.h>
 #include <string.h>

 u32 dma_chcr[10] = { 0x10008000, 0x10009000, 0x1000A000, 0x1000B000, 0x1000B400, 0x1000C000, 0x1000C400, 0x1000C800, 0x1000D000, 0x1000D400 };
 u32 dma_madr[10] = { 0x10008010, 0x10009010, 0x1000A010, 0x1000B010, 0x1000B410, 0x1000C010, 0x1000C410, 0x1000C810, 0x1000D010, 0x1000D410 };
 u32 dma_size[10] = { 0x10008020, 0x10009020, 0x1000A020, 0x1000B020, 0x1000B420, 0x1000C020, 0x1000C420, 0x1000C820, 0x1000D020, 0x1000D420 };
 u32 dma_tadr[10] = { 0x10008030, 0x10009030, 0x1000A030, 0x1000B030, 0x1000B430, 0x1000C030, 0x1000C430, 0x1000C830, 0x1000D030, 0x1000D430 };

 static int dma_handler_id[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

 int dma_initialized = -1;

 ///////////////////
 // DMA FUNCTIONS //
 ///////////////////

 int dma_initialize(void) {

  // Reset the dma controller.
  ResetEE(0x01);

  // Save the initialization status.
  dma_initialized = 1;

  // End function.
  return 0;

 }

 int dma_shutdown(void) {

  // Save the shutdown status.
  dma_initialized = -1;

  // End function.
  return 0;

 }

 ///////////////////////////
 // DMA CHANNEL FUNCTIONS //
 ///////////////////////////

 int dma_channel_initialize(int channel, void *handler) {

  // Check to see if we're initialized.
  if (dma_initialized < 0) { if (dma_initialize() < 0) { return -1; } }

  // Shut down the channel before making changes.
  if (dma_channel_shutdown(channel) < 0) { return -1; }

  // If a handler is provided...
  if (handler != NULL) {

   // Add the handler, storing the handler id.
   dma_handler_id[channel] = AddDmacHandler(channel, handler, 0);

   // Enable the channel interrupt.
   EnableDmac(channel);

  }

  // End function.
  return 0;

 }

 int dma_channel_initialize_i(int channel, void *handler) {

  // Check to see if we're initialized.
  if (dma_initialized < 0) { if (dma_initialize() < 0) { return -1; } }

  // Shut down the channel before making changes.
  if (dma_channel_shutdown_i(channel) < 0) { return -1; }

  // If a handler is provided...
  if (handler != NULL) {

   // Add the handler, storing the handler id.
   dma_handler_id[channel] = AddDmacHandler(channel, handler, 0);

   // Enable the channel interrupt.
   iEnableDmac(channel);

  }

  // End function.
  return 0;

 }

 int dma_channel_wait(int channel, int timeout) {

  // While the channel is not ready...
  while (*(volatile u32 *)dma_chcr[channel] & 0x00000100) {

   // Delay for a little while.
   asm __volatile__( "nop;nop;nop;nop;nop;nop;nop;nop" );

   // Decrement the timeout counter, exiting if it expires.
   if (timeout > 0) { if (timeout-- == 0) { return -1; } }

  }

  // End function.
  return 0;

 }

 int dma_channel_send(int channel, void *data, int data_size) {

  // Wait for the channel to become ready.
  if (dma_channel_wait(channel, 100000) < 0) { return -1; }

  // Flush the data cache.
  SyncDCache(data, data + data_size);

  // Set the size of the data, in quadwords.
  *(volatile u32 *)dma_size[channel] = DMA_SET_QWC(((data_size + 15) >> 4));

  // Set the address of the data.
  *(volatile u32 *)dma_madr[channel] = DMA_SET_MADR((u32)data, 0);

  // Start the transfer.
  *(volatile u32 *)dma_chcr[channel] = DMA_SET_CHCR(1, 0, 0, 0, 1, 1, 0);

  // End function.
  return 0;

 }

 int dma_channel_send_i(int channel, void *data, int data_size) {

  // Wait for the channel to become ready.
  if (dma_channel_wait(channel, 100000) < 0) { return -1; }

  // Flush the data cache.
  iSyncDCache(data, data + data_size);

  // Set the size of the data, in quadwords.
  *(volatile u32 *)dma_size[channel] = DMA_SET_QWC(((data_size + 15) >> 4));

  // Set the address of the data.
  *(volatile u32 *)dma_madr[channel] = DMA_SET_MADR((u32)data, 0);

  // Start the transfer.
  *(volatile u32 *)dma_chcr[channel] = DMA_SET_CHCR(1, 0, 0, 0, 1, 1, 0);

  // End function.
  return 0;

 }

 int dma_channel_send_chain(int channel, void *data) {

  // Wait for the channel to become ready.
  if (dma_channel_wait(channel, 100000) < 0) { return -1; }

  // Flush the data cache.
  FlushCache(0);

  // Set the size of the data, in quadwords.
  *(volatile u32 *)dma_size[channel] = DMA_SET_QWC(0);

  // Set the address of the data.
  *(volatile u32 *)dma_madr[channel] = DMA_SET_MADR(0, 0);

  // Set the tag address of the data.
  *(volatile u32 *)dma_tadr[channel] = DMA_SET_TADR((u32)data, 0);

  // Start the transfer.
  *(volatile u32 *)dma_chcr[channel] = DMA_SET_CHCR(1, 1, 0, 0, 1, 1, 0);

  // End function.
  return 0;

 }

 int dma_channel_send_chain_i(int channel, void *data) {

  // Wait for the channel to become ready.
  if (dma_channel_wait(channel, 100000) < 0) { return -1; }

  // Flush the data cache.
  iFlushCache(0);

  // Set the size of the data, in quadwords.
  *(volatile u32 *)dma_size[channel] = DMA_SET_QWC(0);

  // Set the address of the data.
  *(volatile u32 *)dma_madr[channel] = DMA_SET_MADR(0, 0);

  // Set the tag address of the data.
  *(volatile u32 *)dma_tadr[channel] = DMA_SET_TADR((u32)data, 0);

  // Start the transfer.
  *(volatile u32 *)dma_chcr[channel] = DMA_SET_CHCR(1, 1, 0, 0, 1, 1, 0);

  // End function.
  return 0;

 }

 int dma_channel_receive(int channel, void *data, int data_size) {

  // Wait for the channel to become ready.
  if (dma_channel_wait(channel, 100000) < 0) { return -1; }

  // Set the size of the data, in quadwords.
  *(volatile u32 *)dma_size[channel] = DMA_SET_QWC(((data_size + 15) >> 4));

  // Set the address of the data.
  *(volatile u32 *)dma_madr[channel] = DMA_SET_MADR((u32)data & 0x0FFFFFFF, 0);

  // Start the transfer.
  *(volatile u32 *)dma_chcr[channel] = DMA_SET_CHCR(0, 0, 0, 0, 0, 1, 0);

  // End function.
  return 0;

 }

 int dma_channel_shutdown(int channel) {

  // If a handler was provided...
  if (dma_handler_id[channel] != 0) {

   // Disable the channel.
   DisableDmac(channel);

   // Remove the handler.
   RemoveDmacHandler(channel, dma_handler_id[channel]);

   // Clear the handler id.
   dma_handler_id[channel] = 0;

  }

  // End function.
  return 0;

 }

 int dma_channel_shutdown_i(int channel) {

  // If a handler was provided...
  if (dma_handler_id[channel] != 0) {

   // Disable the channel.
   iDisableDmac(channel);

   // Remove the handler.
   RemoveDmacHandler(channel, dma_handler_id[channel]);

   // Clear the handler id.
   dma_handler_id[channel] = 0;

  }

  // End function.
  return 0;

 }

 //////////////////////////
 // DMA PACKET FUNCTIONS //
 //////////////////////////

 int dma_packet_allocate(DMA_PACKET *packet, int data_size) {

  // Check for an already allocated packet.
  if (packet != NULL) { dma_packet_free(packet); }

  // Allocate the data buffer.
  packet->data = memalign(data_size, 128);

  // Save the data size.
  packet->data_size = data_size;

  // Clear before use.
  dma_packet_clear(packet);

  // End function.
  return 0;

 }

 int dma_packet_clear(DMA_PACKET *packet) {

  // Clear the data.
  memset(packet->data, 0x00, packet->data_size);

  // Reset the counter.
  packet->count = 0;

  // End function.
  return 0;

 }

 int dma_packet_append(DMA_PACKET *packet, u64 data) {

  // Append the data.
  packet->data[packet->count++] = data;

  // End function.
  return 0;

 }

 int dma_packet_send(DMA_PACKET *packet, int channel) {

  // Pad to the nearest quadword.
  if (packet->count & 0x01) { dma_packet_append(packet, 0); }

  // Send to the specified channel.
  dma_channel_send(channel, packet->data, packet->count << 3);

  // End function.
  return 0;

 }

 int dma_packet_send_i(DMA_PACKET *packet, int channel) {

  // Pad to the nearest quadword.
  if (packet->count & 0x01) { dma_packet_append(packet, 0); }

  // Send to the specified channel.
  dma_channel_send_i(channel, packet->data, packet->count << 3);

  // End function.
  return 0;

 }

 int dma_packet_send_chain(DMA_PACKET *packet, int channel) {

  // Pad to the nearest quadword.
  if (packet->count & 0x01) { dma_packet_append(packet, 0); }

  // Send to the specified channel.
  dma_channel_send_chain(channel, packet->data);

  // End function.
  return 0;

 }

 int dma_packet_send_chain_i(DMA_PACKET *packet, int channel) {

  // Pad to the nearest quadword.
  if (packet->count & 0x01) { dma_packet_append(packet, 0); }

  // Send to the specified channel.
  dma_channel_send_chain_i(channel, packet->data);

  // End function.
  return 0;

 }

 int dma_packet_free(DMA_PACKET *packet) {

  // Free the data buffer.
  free(packet->data);

  // End function.
  return 0;

 }
