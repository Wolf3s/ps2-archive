#ifndef __SETJMP_H__
#define __SETJMP_H__

/* Very simple setjmp support. Have to be tested though. Someone? */

#include <tamtypes.h>

#if defined(_EE)
/* we need some room after the gpr, to store some fp registers. */
#define _JBLEN 14
#define _JBTYPE u128
#else
/* seems the IOP's sysclib does have one more value than the newlib's version... */
#define _JBLEN 12
#define _JBTYPE u32
#endif

typedef _JBTYPE jmp_buf[_JBLEN];

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#endif
