/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * pspnand_driver.h - Definitions and interfaces to the NAND (flash) driver.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@0xd6.org>
 *
 * $Id: pspnand_driver.h 1211 2005-10-24 06:36:00Z mrbrown $
 */

#ifndef PSPNAND_DRIVER_H
#define PSPNAND_DRIVER_H

#include <pspkerneltypes.h>

#ifdef __cplusplus
extern "C" {
#endif


int sceNandSetWriteProtect(int protectFlag);

int sceNandLock(int writeFlag);

void sceNandUnlock(void);

int sceNandReadStatus(void);

int sceNandReset(int flag);

int sceNandReadId(void *buf, SceSize size);

int sceNandReadPages(u32 ppn, void *buf, void *buf2, u32 count);

int sceNandReadExtraOnly(u32 ppn, void *buf, u32 count);

int sceNandReadAccess(u32 ppn, void *buf, void *buf2, u32 count, u32 unk);

int sceNandWriteAccess(u32 ppn, u8 *user, u8 *spare, u32 len, u32 unk);

int sceNandWriteBlockWithVerify(u32 ppn, u8 *user, u8 *spare);

int sceNandEraseBlockWithRetry(u32 ppn);

/*
// sceNandWritePages
// sceNandEraseBlock
// sceNandCalcEcc
// sceNandVerifyEcc
// sceNandCollectEcc
*/

int sceNandGetPageSize(void);

int sceNandGetPagesPerBlock(void);

int sceNandGetTotalBlocks(void);

/*
// sceNandWriteBlock
*/

int sceNandReadBlockWithRetry(u32 ppn, void *buf, void *buf2);

int sceNandInit(void);
/*
// sceNandVerifyBlockWithRetry
*/

int sceNandIsBadBlock(u32 ppn);

/*
// sceNandEraseAllBlock
// sceNandTestBlock
*/

#ifdef __cplusplus
}
#endif

#endif /* PSPNAND_DRIVER_H */
