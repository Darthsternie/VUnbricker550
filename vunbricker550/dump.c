#include <pspsdk.h>
#include <pspkernel.h>
#include <psploadexec.h>
#include <pspiofilemgr.h>
#include <psppower.h>
#include <psprtc.h>
#include <psputils.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <vlf.h>
#include "./h/main.h"

int OnBackToMainMenu(int enter)
{
	if (!enter)
	{
		vlfGuiCancelCentralMenu();
		vlfGuiCancelBottomDialog();
		MainMenu(2);		
	}

	return VLF_EV_RET_NOTHING;
}

void dump_filesystem(const char* src, const char* dst){
	sceIoMkdir(dst, 0777);
}

int OnDumpMenuSelect(int sel){
	switch (sel)
	{
		case 0:
		//	SwapFlash(0);
			dump_filesystem("flach0:/", "ms0:/dump/flash0");
		break;

		case 1:
		//	SwapFlash(1);
			dump_filesystem("flach1:/", "ms0:/dump/flash1");
		break;

		case 2:
		//	SwapFlash(1);
			dump_filesystem("flach2:/", "ms0:/dump/flash2");
		break;

		case 3:
		//	SwapFlash(1);
			dump_filesystem("flach3:/", "ms0:/dump/flash3");
		break;
	}

	return VLF_EV_RET_NOTHING;
}

void DumpMenu(int sel){
	char *items[] =	{
		"Dump Flash0",
		"Dump Flash1",
		"Dump Flash2",
		"Dump Flash3",
	};

	vlfGuiCentralMenu(4, items, sel, OnDumpMenuSelect, 0, 0);
	vlfGuiBottomDialog(VLF_DI_CANCEL, VLF_DI_ENTER, 1, 0, VLF_DEFAULT, OnBackToMainMenu);
}
