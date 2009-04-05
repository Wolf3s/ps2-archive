/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# 
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/

#include <libgs.h>




//PLEASE WAIT. &  your ps2sdk source will still compile with no problem



short GsInit(void)
{
		/*Reset dma(gif channel only)*/
	__asm__("
	li	$2,0x1000A000
	nop
	sw	$0,0x80($2)
	sw	$0,0($2)
	sw	$0,0x30($2)
	sw	$0,0x10($2)
	sw	$0,0x50($2)
	sw	$0,0x40($2)
	li	$2,0xFF1F		#0xFF1F
	sw	$2,0x1000E010
	lw	$2,0x1000E010
	li	$3,0xFF1F		#0xFF1F
	and	$2,$3
	sw	$2,0x1000E010
sync.p
	sw	$0,0x1000E000
	sw	$0,0x1000E020
	sw	$0,0x1000E030
	sw	$0,0x1000E050
	sw	$0,0x1000E040
	li	$3,1
	lw	$2,0x1000E000
	ori	$3,$2,1
	nop
	sw	$3,0x1000E000
	nop
	");
	
	//reset the GS
	GS_SET_CSR_flush(1);
	GS_SET_CSR_reset(1);
		

	//wait for GS to finish reset
	__asm__(" 
		sync.p 
		nop");


	

	//default to ntsc to prevent crash
	__asm__("
		
		li	$3,0x02				# call SetGsCrt	
		li	$4,0x00				# non-interlace	
		li	$5,0x02				# ntsc
		li	$6,0x01				# frame mode
		syscall
		nop
		
		");



	return 0;
}






short GsSetCRTCMode(int interlace,  int videomode,  int fieldmode)
{
	__asm__("
		
		li	$3,0x02				# call SetGsCrt					
		syscall
		nop
		
			");

	return 0;
}



short GsSetCRTCSettings(unsigned long settings, unsigned char alpha_value)
{
	
	*((volatile unsigned long *)(gs_p_pmode)) =  (settings|((unsigned long)(001) << 2)|((unsigned long)(alpha_value) 	<< 8));

	return 0;
}




short GsSetVideoMode(int interlace,  int videomode,  int fieldmode)
{

	GsSetCRTCMode(interlace, videomode, fieldmode);
	GsSetCRTCSettings(CRTC_SETTINGS_DEFAULT, 255);
	return 0;
}






short GsSetDefaultDrawEnv(sGS_DRAWENV *drawenv, unsigned short x, unsigned short y, unsigned short w, unsigned short h)
{
	drawenv->offset_x	= x;
	drawenv->offset_y	= y;
	drawenv->clip.x		= 0;
	drawenv->clip.y		= 0;
	drawenv->clip.w		= w-1;
	drawenv->clip.h		= h-1;
	drawenv->draw_mask	= 0;
	drawenv->auto_clear	= 1;

	drawenv->bg_color.r			= 0x00;
	drawenv->bg_color.g			= 0x00;
	drawenv->bg_color.b			= 0x00;
	drawenv->bg_color.a			= 0x00;
	drawenv->bg_color.q			= 1.0f;
	
	return 1;
}








short GsSetDefaultDrawEnvAddress(sGS_DRAWENV *drawenv, unsigned short vram_page, unsigned char	vram_width, unsigned char pix_mode)
{
	drawenv->vram_page	= vram_page;
	drawenv->vram_width = vram_width;
	drawenv->pix_mode	= pix_mode;
	drawenv->vram_x		= 0;
	drawenv->vram_y		= 0;

	return 1;
}







short GsSetDefaultDisplayEnv(GS_DISPENV *dispenv, unsigned short x, unsigned short y, unsigned short w, unsigned short h)
{
	dispenv->screen.x	= x;
	dispenv->screen.y	= y;
	dispenv->screen.w	= w;
	dispenv->screen.h	= h;

	dispenv->magnify_h	= 4;
	dispenv->magnify_v	= 0;

	return 1;
}







short GsSetDefaultDisplayEnvAddress(GS_DISPENV *dispenv, unsigned short vram_page, unsigned char	vram_width, unsigned char pix_mode)
{
	dispenv->vram_page	= vram_page;
	dispenv->vram_width = vram_width;
	dispenv->pix_mode	= pix_mode;
	dispenv->vram_x		= 0;
	dispenv->vram_y		= 0;

	return 1;
}





short GsSetDefaultZBufferEnv(sGS_ZENV *zenv, unsigned short vram_addr, unsigned char pix_mode, unsigned char update_mask)
{
	zenv->vram_page   = vram_addr;
	zenv->pix_mode    = pix_mode;
	zenv->update_mask = update_mask;
	
	return 1;
}