/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: GetSyscallHandler.c 961 2005-04-09 20:00:11Z tyranid $
# Routines for accessing the EE's on-chip serial port.
*/

#include <kernel.h>

#ifdef F_GetSyscallHandler
static u32* g_pSyscallTable = NULL;

/** Initialise the syscall table address */
static void InitSyscallTable(void)
{
    u32 oldintr, oldop;
	u32 *pAddr;

    oldintr = DIntr();
    oldop = ee_opmode_set(0);
	pAddr = (u32 *) 0x800002f0;
	g_pSyscallTable = (u32 *) ((pAddr[0] << 16) | (pAddr[2] & 0xFFFF));
    ee_opmode_set(oldop);
    if(oldintr) { EIntr(); }
}

/** Get the address of an EE syscall handler function.
    @param syscall_no - The syscall number.
	@return - The address of the syscall handler function (or NULL)
*/
void *GetSyscallHandler(int syscall_no)
{
    u32 oldintr, oldop;
	u32 addr = 0;

	if(g_pSyscallTable == NULL)
	{
		InitSyscallTable();
	}

	if(g_pSyscallTable != NULL)
	{
        oldintr = DIntr();
	    oldop = ee_opmode_set(0);
		addr = g_pSyscallTable[syscall_no];
	    ee_opmode_set(oldop);
        if(oldintr) { EIntr(); }
	}

	return (void *) addr;
}
#endif

#ifdef F_GetExceptionHandler

extern void *GetSyscallHandler(int syscall_no);

/** Get the address of an EE exception handler function.
    @param exception_no - The exception cause number.
	@return - The address of the exception handler function (or NULL)
*/
void *GetExceptionHandler(int ex_cause_no)
{
    u32 oldintr, oldop;
	u32 addr;

    if((ex_cause_no >= 1) && (ex_cause_no < 4))
    {
        // TLB-related exceptions(1-3)

        // get address of the syscall "SetVTLBRefillHandler"
        addr = GetSyscallHandler(13);
    }
    else if((ex_cause_no >= 4) && (ex_cause_no < 16))
    {
        // "common" exceptions(4-15)

        // get address of the syscall "SetVCommonHandler"
        addr = GetSyscallHandler(14);

        ex_cause_no -= 4; // adjust the cause number
    }
    else
    {
        // everything else
        return(NULL);
    }

    // suspend interrupts and enter "kernel" operating mode.
    oldintr = DIntr();
	oldop = ee_opmode_set(0);

    // harvest the address of the corresponding exception handler table.
    lo16 = ((vu32 *) addr)[0x20 / 4];
    hi16 = ((vu32 *) addr)[0x14 / 4];
    
    addr = ((u32 *) ((u32) (hi16 << 16) | lo16))[ex_cause_no];

    // return to the old operating mode and resume interrupts.
	ee_opmode_set(oldop);
    if(oldintr) { EIntr(); }

	return (void *) addr;
}
#endif

#ifdef F_GetInterruptHandler

extern void *GetSyscallHandler(int syscall_no);

/** Get the address of an EE interrupt handler function.
    @param intr_cause_no - The interrupt number.
	@return - The address of the interrupt handler function (or NULL)
*/
void *GetInterruptHandler(int intr_cause_no)
{
    u32 oldintr, oldop;
	u32 addr;

    // make sure intr_cause_no is between 0 and 7
    if((irq < 0) || (irq > 7))
    {
        return(NULL);
    }

    // get address of the syscall "SetVInterruptHandler"
    addr = GetSyscallHandler(15);

    // suspend interrupts and enter "kernel" operating mode.
    oldintr = DIntr();
	oldop = ee_opmode_set(0);

    // harvest the address of the corresponding exception handler table.
    lo16 = ((vu32 *) addr)[0x10 / 4];
    hi16 = ((vu32 *) addr)[0x1C / 4];

    addr = ((u32 *) ((u32) (hi16 << 16) | lo16))[intr_cause_no];

    // return to the old operating mode and resume interrupts.
	ee_opmode_set(oldop);
    if(oldintr) { EIntr(); }

	return (void *) addr;
}
#endif

