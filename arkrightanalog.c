#include <psp2/kernel/modulemgr.h>
#include <taihen.h>
#include <psp2/ctrl.h>
#include <stdio.h>

static tai_hook_ref_t sceCompatWriteSharedCtrlRef;
static SceUID hook;
typedef struct SceCtrlDataPsp {
	unsigned int TimeStamp; // 0x00
	unsigned int Buttons; // 0x04
	unsigned char Lx; // 0x05
	unsigned char Ly; // 0x06
	unsigned char Rx; // 0x07
	unsigned char Ry; // 0x08
	unsigned char Rsrv[4]; // 0x9
} SceCtrlDataPsp;


void _start() __attribute__ ((weak, alias ("module_start")));

static int sceCompatWriteSharedCtrlPatched(SceCtrlDataPsp *pad_data) {
	SceCtrlData pad;
	sceCtrlPeekBufferPositive(0, &pad, 1);
	pad_data->Rx = pad.rx;
	pad_data->Ry = pad.ry;
	return TAI_CONTINUE(int, sceCompatWriteSharedCtrlRef, pad_data);
}

// our own plugin entry
int module_start(SceSize argc, const void *args) {
	
 hook = taiHookFunctionImport(&sceCompatWriteSharedCtrlRef,
                        "ScePspemu",   
                        TAI_ANY_LIBRARY, 
                        0x2306FFED,       
                        sceCompatWriteSharedCtrlPatched);	
  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
	if(hook >= 0){
	taiHookRelease(hook, sceCompatWriteSharedCtrlRef);
	}
	return SCE_KERNEL_STOP_SUCCESS;
}