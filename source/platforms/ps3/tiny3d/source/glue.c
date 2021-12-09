#include "rsxtiny.h"

void rsxtiny_FlushBuffer(tiny_gcmContextData *context) {
    u32 offset = 0;
	gcmControlRegister volatile *control = gcmGetControlRegister(context);
	__asm __volatile__("sync"); // Sync, to make sure the command was written;
	gcmAddressToOffset((void *) (u64) context->current, &offset);
	control->put = offset;
}


/* Maps the memory at ioAddress into the RSX's memory space so the userspace thread
 * and the RSX can comunicate.
 * This shared memory must be 1mb aligned, and at least 1mb long.
 * Also Initilizes a RSX command buffer of cmdSize inside the shared memory. 
 * 
 * Returns a context structure.
 */
tiny_gcmContextData *rsxtiny_Init(const u32 cmdSize, const u32 ioSize, const void* ioAddress) {
	#ifdef OLD_TINY3D
    u32 contextPointer;
	s32 ret = gcmInitBody(&contextPointer, cmdSize, ioSize, ioAddress);
	if (ret == 0) {
		// Double cast fixes warning about diffrent pointer sizes.
		tiny_gcmContextData *context = (tiny_gcmContextData *) (u64) contextPointer;
		return context;
	}
    #else
    gcmContextData * contextPointer;
	s32 ret = gcmInitBody(&contextPointer, cmdSize, ioSize, ioAddress);
	if (ret == 0) {
		// Double cast fixes warning about diffrent pointer sizes.
		tiny_gcmContextData *context = (void *) contextPointer;
		return context;
	}
    #endif
	return NULL;
}

int rsxtiny_AddressToOffset(void* ptr, u32 *offset) {
    #ifdef OLD_TINY3D
	// Double cast for warnings.
	return gcmAddressToOffset((u32) (u64) ptr, offset);
    #else
    return gcmAddressToOffset(ptr, offset);
    #endif
}
