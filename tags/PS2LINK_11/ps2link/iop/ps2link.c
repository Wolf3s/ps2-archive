/*********************************************************************
 * Copyright (C) 2003 Tord Lindstrom (pukko@home.se)
 * This file is subject to the terms and conditions of the PS2Link License.
 * See the file LICENSE in the main directory of this distribution for more
 * details.
 */

#include <stdlib.h>
#include <loadcore.h>
#include <intrman.h>
#include <tamtypes.h>
#include <sifrpc.h>
#include <cdvd.h>


// Entry points
extern int fsysMount(void);
extern int cmdHandlerInit(void);
extern int ttyMount(void);
extern int naplinkRpcInit(void);

////////////////////////////////////////////////////////////////////////
// main
//   start threads & init rpc & filesys
int
_start( int argc, char **argv)
{

    FlushDcache();
    CpuEnableIntr(0);

    CdInit(1);
    CdStop();

    SifInitRpc(0);

    if ((argc < 2) || (strncmp(argv[1], "-notty", 6))) {
        ttyMount();
        // Oh well.. There's a bug in either smapif or lwip's etharp
        // that thrashes udp msgs which are queued while waiting for arp
        // request
        // alas, this msg will probably not be displayed
        printf("tty mounted\n");
    }

    fsysMount();
    cmdHandlerInit();
    naplinkRpcInit();
    return 0;
}
