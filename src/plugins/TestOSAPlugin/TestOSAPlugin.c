/* Automatically generated by
	SmartSyntaxPluginCodeGenerator VMMaker.oscog-rsf.1951 uuid: c714858a-7f63-47bd-a9ec-4bc1350eead2
   from
	TestOSAPlugin VMMaker.oscog-rsf.1951 uuid: c714858a-7f63-47bd-a9ec-4bc1350eead2
 */
static char __buildInfo[] = "TestOSAPlugin VMMaker.oscog-rsf.1951 uuid: c714858a-7f63-47bd-a9ec-4bc1350eead2 " __DATE__ ;



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Carbon/Carbon.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
#include "sqConfig.h"			/* Configuration options */
#include "sqVirtualMachine.h"	/*  The virtual machine proxy definition */
#include "sqPlatformSpecific.h"	/* Platform specific definitions */

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
# undef EXPORT
# define EXPORT(returnType) static returnType
#endif

#include "sqMemoryAccess.h"


/*** Function Prototypes ***/
EXPORT(const char*) getModuleName(void);
EXPORT(sqInt) primAECoerceDesc(void);
EXPORT(sqInt) primAECreateDesc(void);
EXPORT(sqInt) primAEDescToString(void);
EXPORT(sqInt) primAEDisposeDesc(void);
EXPORT(sqInt) primAEGetKeyPtr(void);
EXPORT(sqInt) primGetHandleSize(void);
EXPORT(sqInt) primOpenDefaultConfiguration(void);
EXPORT(sqInt) primOSACompile(void);
EXPORT(sqInt) primOSADisplay(void);
EXPORT(sqInt) primOSADispose(void);
EXPORT(sqInt) primOSADoScript(void);
EXPORT(sqInt) primOSAExecute(void);
EXPORT(sqInt) primOSAGetScriptInfo(void);
EXPORT(sqInt) primOSAGetSource(void);
EXPORT(sqInt) primOSALoad(void);
EXPORT(sqInt) primOSAMakeContext(void);
EXPORT(sqInt) primOSAScriptError(void);
EXPORT(sqInt) primOSAScriptingComponentName(void);
EXPORT(sqInt) primOSAStore(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);
static sqInt sqAssert(sqInt aBool);


/*** Variables ***/

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*failed)(void);
static void * (*firstIndexableField)(sqInt oop);
static sqInt (*isBytes)(sqInt oop);
static sqInt (*isWords)(sqInt oop);
static sqInt (*isWordsOrBytes)(sqInt oop);
static sqInt (*popthenPush)(sqInt nItems, sqInt oop);
static sqInt (*positive32BitIntegerFor)(unsigned int integerValue);
static sqInt (*sizeOfSTArrayFromCPrimitive)(void *cPtr);
static sqInt (*stackIntegerValue)(sqInt offset);
static sqInt (*stackValue)(sqInt offset);
static sqInt (*success)(sqInt aBoolean);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt failed(void);
extern void * firstIndexableField(sqInt oop);
extern sqInt isBytes(sqInt oop);
extern sqInt isWords(sqInt oop);
extern sqInt isWordsOrBytes(sqInt oop);
extern sqInt popthenPush(sqInt nItems, sqInt oop);
extern sqInt positive32BitIntegerFor(unsigned int integerValue);
extern sqInt sizeOfSTArrayFromCPrimitive(void *cPtr);
extern sqInt stackIntegerValue(sqInt offset);
extern sqInt stackValue(sqInt offset);
extern sqInt success(sqInt aBoolean);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"TestOSAPlugin VMMaker.oscog-rsf.1951 (i)"
#else
	"TestOSAPlugin VMMaker.oscog-rsf.1951 (e)"
#endif
;



/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

	/* InterpreterPlugin>>#getModuleName */
EXPORT(const char*)
getModuleName(void)
{
	return moduleName;
}

	/* TestOSAPlugin>>#primAECoerceDesc:to: */
EXPORT(sqInt)
primAECoerceDesc(void)
{
	AEDesc *rcvr;
	AEDesc *result;
	DescType *typeCode;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(1)));
	typeCode = ((DescType *) (firstIndexableField(stackValue(1))));
	success(isWordsOrBytes(stackValue(0)));
	result = ((AEDesc *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(2)));
	rcvr = ((AEDesc *) (firstIndexableField(stackValue(2))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((AECoerceDesc(rcvr,*typeCode,result)));
	if (failed()) {
		return null;
	}
	popthenPush(3, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primAECreateDesc:from: */
EXPORT(sqInt)
primAECreateDesc(void)
{
	char *aString;
	AEDesc *rcvr;
	sqInt size;
	DescType *typeCode;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(1)));
	typeCode = ((DescType *) (firstIndexableField(stackValue(1))));
	success(isBytes(stackValue(0)));
	aString = ((char *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(2)));
	rcvr = ((AEDesc *) (firstIndexableField(stackValue(2))));
	if (failed()) {
		return null;
	}
	size = sizeOfSTArrayFromCPrimitive(aString);
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((AECreateDesc(*typeCode, aString, size, rcvr)));
	if (failed()) {
		return null;
	}
	popthenPush(3, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primAEDescToString: */
EXPORT(sqInt)
primAEDescToString(void)
{
	char *aString;
	AEDesc *rcvr;
	sqInt size;
	sqInt _return_value;

	success(isBytes(stackValue(0)));
	aString = ((char *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(1)));
	rcvr = ((AEDesc *) (firstIndexableField(stackValue(1))));
	if (failed()) {
		return null;
	}
	size = sizeOfSTArrayFromCPrimitive(aString);
	BlockMove(*(rcvr->dataHandle), aString, size);
	if (failed()) {
		return null;
	}
	_return_value = ((sqInt)((aString) - BaseHeaderSize));
	if (failed()) {
		return null;
	}
	popthenPush(2, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primAEDisposeDesc */
EXPORT(sqInt)
primAEDisposeDesc(void)
{
	AEDesc *rcvr;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(0)));
	rcvr = ((AEDesc *) (firstIndexableField(stackValue(0))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((AEDisposeDesc(rcvr)));
	if (failed()) {
		return null;
	}
	popthenPush(1, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primAEGetKeyPtr:type:actual:to: */
EXPORT(sqInt)
primAEGetKeyPtr(void)
{
	char *bytes;
	DescType *ignoreDesc;
	Size  ignoreSize;
	DescType *key;
	AEDesc *rcvr;
	sqInt size;
	DescType *type;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(3)));
	key = ((DescType *) (firstIndexableField(stackValue(3))));
	success(isWordsOrBytes(stackValue(2)));
	type = ((DescType *) (firstIndexableField(stackValue(2))));
	success(isWordsOrBytes(stackValue(1)));
	ignoreDesc = ((DescType *) (firstIndexableField(stackValue(1))));
	success(isBytes(stackValue(0)));
	bytes = ((char *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(4)));
	rcvr = ((AEDesc *) (firstIndexableField(stackValue(4))));
	if (failed()) {
		return null;
	}
	size = (ignoreSize = sizeOfSTArrayFromCPrimitive(bytes));
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((AEGetKeyPtr(rcvr, *key, *type, ignoreDesc, bytes, size, &ignoreSize)));
	if (failed()) {
		return null;
	}
	popthenPush(5, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primGetHandleSize: */
EXPORT(sqInt)
primGetHandleSize(void)
{
	sqInt anIndex;
	unsigned *rcvr;
	sqInt _return_value;

	anIndex = stackIntegerValue(0);
	success(isWords(stackValue(1)));
	rcvr = ((unsigned *) (firstIndexableField(stackValue(1))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((GetHandleSize((Handle) *(rcvr+anIndex))));
	if (failed()) {
		return null;
	}
	popthenPush(2, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOpenDefaultConfiguration:subtype: */
EXPORT(sqInt)
primOpenDefaultConfiguration(void)
{
	ComponentInstance *component;
	DescType *subtype;
	DescType *type;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(1)));
	type = ((DescType *) (firstIndexableField(stackValue(1))));
	success(isWordsOrBytes(stackValue(0)));
	subtype = ((DescType *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(2)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(2))));
	if (failed()) {
		return null;
	}
	*component = OpenDefaultComponent(*type,*subtype);
	if (failed()) {
		return null;
	}
	_return_value = ((sqInt)((component) - BaseHeaderSize));
	if (failed()) {
		return null;
	}
	popthenPush(3, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSACompile:mode:to: */
EXPORT(sqInt)
primOSACompile(void)
{
	ComponentInstance *component;
	sqInt mode;
	OSAID *object;
	AEDesc *source;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(2)));
	source = ((AEDesc *) (firstIndexableField(stackValue(2))));
	mode = stackIntegerValue(1);
	success(isWordsOrBytes(stackValue(0)));
	object = ((OSAID *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(3)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(3))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSACompile(*component,source,mode,object)));
	if (failed()) {
		return null;
	}
	popthenPush(4, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSADisplay:as:mode:to: */
EXPORT(sqInt)
primOSADisplay(void)
{
	ComponentInstance *component;
	sqInt mode;
	AEDesc *result;
	OSAID *source;
	DescType *type;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(3)));
	source = ((OSAID *) (firstIndexableField(stackValue(3))));
	success(isWordsOrBytes(stackValue(2)));
	type = ((DescType *) (firstIndexableField(stackValue(2))));
	mode = stackIntegerValue(1);
	success(isWordsOrBytes(stackValue(0)));
	result = ((AEDesc *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(4)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(4))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSADisplay(*component,*source,*type,mode,result)));
	if (failed()) {
		return null;
	}
	popthenPush(5, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSADispose: */
EXPORT(sqInt)
primOSADispose(void)
{
	OSAID *anOSAID;
	ComponentInstance *component;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(0)));
	anOSAID = ((OSAID *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(1)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(1))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSADispose(*component,*anOSAID)));
	if (failed()) {
		return null;
	}
	popthenPush(2, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSADoScript:in:mode:resultType:to: */
EXPORT(sqInt)
primOSADoScript(void)
{
	ComponentInstance *component;
	OSAID *context;
	sqInt giLocker;
	sqInt mode;
	AEDesc *result;
	sqInt resultsOfCall;
	AEDesc *source;
	DescType *type;
	sqInt _return_value;

	giLocker = 0;
	success(isWordsOrBytes(stackValue(4)));
	source = ((AEDesc *) (firstIndexableField(stackValue(4))));
	success(isWordsOrBytes(stackValue(3)));
	context = ((OSAID *) (firstIndexableField(stackValue(3))));
	mode = stackIntegerValue(2);
	success(isWordsOrBytes(stackValue(1)));
	type = ((DescType *) (firstIndexableField(stackValue(1))));
	success(isWordsOrBytes(stackValue(0)));
	result = ((AEDesc *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(5)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(5))));
	if (failed()) {
		return null;
	}
	
        giLocker = interpreterProxy->ioLoadFunctionFrom("getUIToLock", "");
        if (giLocker != 0) {
            long *foo;
            foo = malloc(sizeof(long)*9);
            foo[0] = 6;
            foo[1] = OSADoScript;
            foo[2] = *component;
            foo[3] = source;
            foo[4] = *context;
            foo[5] = *type;
            foo[6] = mode;
            foo[7] = result;
            foo[8] = 0;
            ((int (*) (void *)) giLocker)(foo);
            resultsOfCall = interpreterProxy->positive32BitIntegerFor(foo[8]);
            free(foo);}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor(resultsOfCall);
	if (failed()) {
		return null;
	}
	popthenPush(6, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSAExecute:in:mode:to: */
EXPORT(sqInt)
primOSAExecute(void)
{
	ComponentInstance *component;
	OSAID *context;
	sqInt giLocker;
	sqInt mode;
	OSAID *result;
	sqInt resultsOfCall;
	OSAID *script;
	sqInt _return_value;

	giLocker = 0;
	success(isWordsOrBytes(stackValue(3)));
	script = ((OSAID *) (firstIndexableField(stackValue(3))));
	success(isWordsOrBytes(stackValue(2)));
	context = ((OSAID *) (firstIndexableField(stackValue(2))));
	mode = stackIntegerValue(1);
	success(isWordsOrBytes(stackValue(0)));
	result = ((OSAID *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(4)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(4))));
	if (failed()) {
		return null;
	}
	
         giLocker = interpreterProxy->ioLoadFunctionFrom("getUIToLock", "");
         if (giLocker != 0) {
            long *foo;
            foo = malloc(sizeof(long)*8);
            foo[0] = 5;
            foo[1] = OSAExecute;
            foo[2] = *component;
            foo[3] = *script;
            foo[4] = *context;
            foo[5] = mode;
            foo[6] = result;
            foo[7] = 0;
            ((int (*) (void *)) giLocker)(foo);
            resultsOfCall = interpreterProxy->positive32BitIntegerFor(foo[7]);
            free(foo); }

	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor(resultsOfCall);
	if (failed()) {
		return null;
	}
	popthenPush(5, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSAGetScriptInfo:type:to: */
EXPORT(sqInt)
primOSAGetScriptInfo(void)
{
	DescType *aDescType;
	OSAID *aScriptID;
	ComponentInstance *component;
	int *resultData;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(2)));
	aScriptID = ((OSAID *) (firstIndexableField(stackValue(2))));
	success(isWordsOrBytes(stackValue(1)));
	aDescType = ((DescType *) (firstIndexableField(stackValue(1))));
	success(isWords(stackValue(0)));
	resultData = ((int *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(3)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(3))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSAGetScriptInfo(*component,*aScriptID,*aDescType, (long *)resultData)));
	if (failed()) {
		return null;
	}
	popthenPush(4, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSAGetSource:type:to: */
EXPORT(sqInt)
primOSAGetSource(void)
{
	DescType *aDescType;
	OSAID *aScriptID;
	ComponentInstance *component;
	AEDesc *resultData;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(2)));
	aScriptID = ((OSAID *) (firstIndexableField(stackValue(2))));
	success(isWordsOrBytes(stackValue(1)));
	aDescType = ((DescType *) (firstIndexableField(stackValue(1))));
	success(isWordsOrBytes(stackValue(0)));
	resultData = ((AEDesc *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(3)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(3))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSAGetSource(*component,*aScriptID,*aDescType, resultData)));
	if (failed()) {
		return null;
	}
	popthenPush(4, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSALoad:mode:to: */
EXPORT(sqInt)
primOSALoad(void)
{
	ComponentInstance *component;
	sqInt mode;
	OSAID *result;
	AEDesc *source;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(2)));
	source = ((AEDesc *) (firstIndexableField(stackValue(2))));
	mode = stackIntegerValue(1);
	success(isWordsOrBytes(stackValue(0)));
	result = ((OSAID *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(3)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(3))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSALoad(*component,source,mode,result)));
	if (failed()) {
		return null;
	}
	popthenPush(4, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSAMakeContext:parent:to: */
EXPORT(sqInt)
primOSAMakeContext(void)
{
	ComponentInstance *component;
	AEDesc *name;
	OSAID *parent;
	OSAID *result;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(2)));
	name = ((AEDesc *) (firstIndexableField(stackValue(2))));
	success(isWordsOrBytes(stackValue(1)));
	parent = ((OSAID *) (firstIndexableField(stackValue(1))));
	success(isWordsOrBytes(stackValue(0)));
	result = ((OSAID *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(3)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(3))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSAMakeContext(*component,name,*parent,result)));
	if (failed()) {
		return null;
	}
	popthenPush(4, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSAScriptError:type:to: */
EXPORT(sqInt)
primOSAScriptError(void)
{
	ComponentInstance *component;
	AEDesc *result;
	DescType *selector;
	DescType *type;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(2)));
	selector = ((DescType *) (firstIndexableField(stackValue(2))));
	success(isWordsOrBytes(stackValue(1)));
	type = ((DescType *) (firstIndexableField(stackValue(1))));
	success(isWordsOrBytes(stackValue(0)));
	result = ((AEDesc *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(3)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(3))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSAScriptError(*component,*selector,*type,result)));
	if (failed()) {
		return null;
	}
	popthenPush(4, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSAScriptingComponentNameTo: */
EXPORT(sqInt)
primOSAScriptingComponentName(void)
{
	AEDesc *anAEDesc;
	ComponentInstance *component;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(0)));
	anAEDesc = ((AEDesc *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(1)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(1))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSAScriptingComponentName(*component,anAEDesc)));
	if (failed()) {
		return null;
	}
	popthenPush(2, _return_value);
	return null;
}

	/* TestOSAPlugin>>#primOSAStore:resultType:mode:to: */
EXPORT(sqInt)
primOSAStore(void)
{
	ComponentInstance *component;
	sqInt mode;
	AEDesc *result;
	OSAID *source;
	DescType *type;
	sqInt _return_value;

	success(isWordsOrBytes(stackValue(3)));
	source = ((OSAID *) (firstIndexableField(stackValue(3))));
	success(isWordsOrBytes(stackValue(2)));
	type = ((DescType *) (firstIndexableField(stackValue(2))));
	mode = stackIntegerValue(1);
	success(isWordsOrBytes(stackValue(0)));
	result = ((AEDesc *) (firstIndexableField(stackValue(0))));
	success(isWordsOrBytes(stackValue(4)));
	component = ((ComponentInstance *) (firstIndexableField(stackValue(4))));
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((OSAStore(*component,*source,*type,mode,result)));
	if (failed()) {
		return null;
	}
	popthenPush(5, _return_value);
	return null;
}


/*	Note: This is coded so that it can be run in Squeak. */

	/* InterpreterPlugin>>#setInterpreter: */
EXPORT(sqInt)
setInterpreter(struct VirtualMachine*anInterpreter)
{
	sqInt ok;

	interpreterProxy = anInterpreter;
	ok = ((interpreterProxy->majorVersion()) == (VM_PROXY_MAJOR))
	 && ((interpreterProxy->minorVersion()) >= (VM_PROXY_MINOR));
	if (ok) {

#if !defined(SQUEAK_BUILTIN_PLUGIN)
		failed = interpreterProxy->failed;
		firstIndexableField = interpreterProxy->firstIndexableField;
		isBytes = interpreterProxy->isBytes;
		isWords = interpreterProxy->isWords;
		isWordsOrBytes = interpreterProxy->isWordsOrBytes;
		popthenPush = interpreterProxy->popthenPush;
		positive32BitIntegerFor = interpreterProxy->positive32BitIntegerFor;
		sizeOfSTArrayFromCPrimitive = interpreterProxy->sizeOfSTArrayFromCPrimitive;
		stackIntegerValue = interpreterProxy->stackIntegerValue;
		stackValue = interpreterProxy->stackValue;
		success = interpreterProxy->success;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}

	/* SmartSyntaxInterpreterPlugin>>#sqAssert: */
static sqInt
sqAssert(sqInt aBool)
{
	/* missing DebugCode */;
	return aBool;
}


#ifdef SQUEAK_BUILTIN_PLUGIN

static char _m[] = "TestOSAPlugin";
void* TestOSAPlugin_exports[][3] = {
	{(void*)_m, "getModuleName", (void*)getModuleName},
	{(void*)_m, "primAECoerceDesc\000\377", (void*)primAECoerceDesc},
	{(void*)_m, "primAECreateDesc\000\377", (void*)primAECreateDesc},
	{(void*)_m, "primAEDescToString\000\377", (void*)primAEDescToString},
	{(void*)_m, "primAEDisposeDesc\000\377", (void*)primAEDisposeDesc},
	{(void*)_m, "primAEGetKeyPtr\000\377", (void*)primAEGetKeyPtr},
	{(void*)_m, "primGetHandleSize\000\000", (void*)primGetHandleSize},
	{(void*)_m, "primOpenDefaultConfiguration\000\377", (void*)primOpenDefaultConfiguration},
	{(void*)_m, "primOSACompile\000\000", (void*)primOSACompile},
	{(void*)_m, "primOSADisplay\000\000", (void*)primOSADisplay},
	{(void*)_m, "primOSADispose\000\377", (void*)primOSADispose},
	{(void*)_m, "primOSADoScript\000\000", (void*)primOSADoScript},
	{(void*)_m, "primOSAExecute\000\000", (void*)primOSAExecute},
	{(void*)_m, "primOSAGetScriptInfo\000\377", (void*)primOSAGetScriptInfo},
	{(void*)_m, "primOSAGetSource\000\377", (void*)primOSAGetSource},
	{(void*)_m, "primOSALoad\000\000", (void*)primOSALoad},
	{(void*)_m, "primOSAMakeContext\000\377", (void*)primOSAMakeContext},
	{(void*)_m, "primOSAScriptError\000\377", (void*)primOSAScriptError},
	{(void*)_m, "primOSAScriptingComponentName\000\377", (void*)primOSAScriptingComponentName},
	{(void*)_m, "primOSAStore\000\000", (void*)primOSAStore},
	{(void*)_m, "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};

#else /* ifdef SQ_BUILTIN_PLUGIN */

signed char primGetHandleSizeAccessorDepth = 0;
signed char primOSACompileAccessorDepth = 0;
signed char primOSADisplayAccessorDepth = 0;
signed char primOSADoScriptAccessorDepth = 0;
signed char primOSAExecuteAccessorDepth = 0;
signed char primOSALoadAccessorDepth = 0;
signed char primOSAStoreAccessorDepth = 0;

#endif /* ifdef SQ_BUILTIN_PLUGIN */
