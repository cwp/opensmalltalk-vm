/* Automatically generated by
	VMPluginCodeGenerator VMMaker.oscog-eem.68 uuid: 1fd12e1a-c0b3-4d41-91bb-26f029240bfe
   from
	VMProfileMacSupportPlugin VMMaker.oscog-eem.68 uuid: 1fd12e1a-c0b3-4d41-91bb-26f029240bfe
 */
static char __buildInfo[] = "VMProfileMacSupportPlugin VMMaker.oscog-eem.68 uuid: 1fd12e1a-c0b3-4d41-91bb-26f029240bfe " __DATE__ ;



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/getsect.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
/*  The virtual machine proxy definition */
#include "sqVirtualMachine.h"
/* Configuration options */
#include "sqConfig.h"
/* Platform specific definitions */
#include "sqPlatformSpecific.h"

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
#undef EXPORT
// was #undef EXPORT(returnType) but screws NorCroft cc
#define EXPORT(returnType) static returnType
#endif

#include "sqMemoryAccess.h"


/*** Constants ***/
#define PrimErrBadArgument 3


/*** Function Prototypes ***/
static VirtualMachine * getInterpreter(void);
EXPORT(const char*) getModuleName(void);
static sqInt halt(void);
static sqInt msg(char *s);
EXPORT(sqInt) primitiveDLSym(void);
EXPORT(sqInt) primitiveExecutableModulesAndOffsets(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);


/*** Variables ***/

#ifdef SQUEAK_BUILTIN_PLUGIN
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"VMProfileMacSupportPlugin VMMaker.oscog-eem.68 (i)"
#else
	"VMProfileMacSupportPlugin VMMaker.oscog-eem.68 (e)"
#endif
;



/*	Note: This is coded so that plugins can be run from Squeak. */

static VirtualMachine *
getInterpreter(void)
{
	return interpreterProxy;
}


/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*)
getModuleName(void)
{
	return moduleName;
}

static sqInt
halt(void)
{
	;
	return 0;
}

static sqInt
msg(char *s)
{
	fprintf(stderr, "\n%s: %s", moduleName, s);
	return 0;
}


/*	Answer the address of the argument in the current process or nil if none. */

EXPORT(sqInt)
primitiveDLSym(void)
{
    void *addr;
    sqInt i;
    char *name;
    sqInt nameObj;
    char *namePtr;
    sqInt sz;

	nameObj = interpreterProxy->stackValue(0);
	if (!(interpreterProxy->isBytes(nameObj))) {
		return interpreterProxy->primitiveFailFor(PrimErrBadArgument);
	}
	sz = interpreterProxy->byteSizeOf(nameObj);
	name = malloc(sz + 1);
	namePtr = interpreterProxy->firstIndexableField(nameObj);
	for (i = 0; i <= (sz - 1); i += 1) {
		name[i] = (namePtr[i]);
	}
	name[sz] = 0;
	addr = dlsym(RTLD_SELF,name);
	free(name);
	return interpreterProxy->methodReturnValue((addr == 0
		? interpreterProxy->nilObject()
		: interpreterProxy->positive32BitIntegerFor(addr)));
}


/*	Answer an Array of quads for executable modules (the VM executable
	and loaded libraries). Each quad is the module's name, its vm address
	relocation in memory, the (unrelocated) start address, and the size. */

EXPORT(sqInt)
primitiveExecutableModulesAndOffsets(void)
{
    const struct mach_header *h;
    sqInt i;
    const char *name;
    char *nameObjData;
    sqInt nimages;
    sqInt present;
    sqInt resultObj;
    const struct section *s;
    unsigned long size;
    sqInt slide;
    unsigned long start;
    sqInt valueObj;

	present = _dyld_present();
	if (!(present)) {
		return interpreterProxy->primitiveFail();
	}
	nimages = _dyld_image_count();
	resultObj = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classArray(), nimages * 4);
	if (resultObj == 0) {
		return interpreterProxy->primitiveFail();
	}
	interpreterProxy->pushRemappableOop(resultObj);
	for (i = 0; i <= (nimages - 1); i += 1) {

		/* impossible start & size */

		start = size = -1;
		name = _dyld_get_image_name(i);
		slide = _dyld_get_image_vmaddr_slide(i);
		h = _dyld_get_image_header(i);
		if (h != null) {
			s = getsectbynamefromheader(h,SEG_TEXT,SECT_TEXT);
			if (s != null) {
				start = s->addr;
				size = s->size;
			}
		}
		valueObj = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), strlen(name));
		if (interpreterProxy->failed()) {
			interpreterProxy->popRemappableOop();
			return interpreterProxy->primitiveFail();
		}
		interpreterProxy->storePointerofObjectwithValue(i * 4, interpreterProxy->topRemappableOop(), valueObj);
		nameObjData = interpreterProxy->arrayValueOf(valueObj);
		memcpy(nameObjData, name, strlen(name));
		valueObj = interpreterProxy->signed32BitIntegerFor(slide);
		if (interpreterProxy->failed()) {
			interpreterProxy->popRemappableOop();
			return interpreterProxy->primitiveFail();
		}
		interpreterProxy->storePointerofObjectwithValue((i * 4) + 1, interpreterProxy->topRemappableOop(), valueObj);
		valueObj = interpreterProxy->positive32BitIntegerFor(start);
		if (interpreterProxy->failed()) {
			interpreterProxy->popRemappableOop();
			return interpreterProxy->primitiveFail();
		}
		interpreterProxy->storePointerofObjectwithValue((i * 4) + 2, interpreterProxy->topRemappableOop(), valueObj);
		valueObj = interpreterProxy->positive32BitIntegerFor(size);
		if (interpreterProxy->failed()) {
			interpreterProxy->popRemappableOop();
			return interpreterProxy->primitiveFail();
		}
		interpreterProxy->storePointerofObjectwithValue((i * 4) + 3, interpreterProxy->topRemappableOop(), valueObj);
	}
	resultObj = interpreterProxy->popRemappableOop();
	return interpreterProxy->popthenPush(1, resultObj);
}


/*	Note: This is coded so that is can be run from Squeak. */

EXPORT(sqInt)
setInterpreter(struct VirtualMachine*anInterpreter)
{
    sqInt ok;

	interpreterProxy = anInterpreter;
	ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
	if (ok == 0) {
		return 0;
	}
	ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
	return ok;
}


#ifdef SQUEAK_BUILTIN_PLUGIN

void* VMProfileMacSupportPlugin_exports[][3] = {
	{"VMProfileMacSupportPlugin", "getModuleName", (void*)getModuleName},
	{"VMProfileMacSupportPlugin", "primitiveDLSym", (void*)primitiveDLSym},
	{"VMProfileMacSupportPlugin", "primitiveExecutableModulesAndOffsets", (void*)primitiveExecutableModulesAndOffsets},
	{"VMProfileMacSupportPlugin", "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};

#endif /* ifdef SQ_BUILTIN_PLUGIN */
