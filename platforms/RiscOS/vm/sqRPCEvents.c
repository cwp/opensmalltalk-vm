/**************************************************************************/
/*  A Squeak VM for Acorn RiscOS machines by Tim Rowledge                 */
/*  tim@sumeru.stanford.edu & http://sumeru.stanford.edu/tim              */
/*  Known to work on RiscOS >3.7 for StrongARM RPCs and Iyonix,           */
/*  other machines not yet tested.                                        */
/*                       sqRPCEvents.c                                    */
/* OS interface stuff                                                     */
/**************************************************************************/

/* To recompile this reliably you will need    */           
/* OSLib -  http://ro-oslib.sourceforge.net/   */
/* Castle/AcornC/C++, the Acorn TCPIPLib       */
/* and a little luck                           */
#include "oslib/os.h"
#include "oslib/osbyte.h"
#include "oslib/osfscontrol.h"
#include "oslib/wimp.h"
#include "oslib/wimpspriteop.h"
#include "oslib/colourtrans.h"
#include "sq.h"
#include "sqArguments.h"

#define longAt(i) (*((int *) (i)))

//#define DEBUG

extern wimp_w	sqWindowHandle;
wimp_block	wimpPollBlock;
int		wimpPollWord;
int		windowActive = false;
extern int	pointerBuffer[];
extern os_coord	pointerOffset;
extern os_coord	scalingFactor, scrollOffset, visibleArea;
int displayBitmapIndex;
extern unsigned int * displaySpriteBits;

/*** Variables -- Event Recording ***/

int inputSemaphoreIndex = 0;
#define EVENTQ_SIZE 1024
sqInputEvent eventBuf[EVENTQ_SIZE ]; /* circular buffer */
int eventBufGet = 0;
int eventBufPut = 0;

/* older polling stuff still needs supporting */
#define KEYBUF_SIZE 64
int	keyBuf[KEYBUF_SIZE];	/* circular buffer */
int	keyBufGet = 0;		/* index of next item of keyBuf to read */
int	keyBufPut = 0;		/* index of next item of keyBuf to write */
int	keyBufOverflows = 0;	/* number of characters dropped */

int	buttonState = 0;	/* mouse button and modifier state  */
os_coord	savedMousePosition;	/* mouse position; not modified when window is inactive */
int	mouseButtonDown;	/* keep track of curent mouse button state - for drags outside window */
extern int	getInterruptKeycode(void);
extern int	setInterruptPending(int value);
extern int	setInterruptCheckCounter(int value);

int scanLine, startX, xLen, startY, stopY, pixelsPerWord, pixelsPerWordShift;
void (*reverserFunction)(void);
void (*socketPollFunction)(int delay, int extraFd) = null;


/* Squeak expects to get kbd modifiers as the top 4bits of the 8bit char code, or
	as the 2r1111000 bits of the buttons word. RiscOS doesnt give me the modifiers
	with keypresses, so we have to manually collect them. SQ only seems to care about them
	when doing primMouseButtons anyway.  
				ST bits:  <command><option><control><shift> Notice how Macish this is!
*/ 
#define		ShiftKey		0x00000008
#define		CtlKey			0x00000010
#define		OptionKey		0x00000020
#define		CmdKey			0x00000040

#define		MacDownCursor	31
#define		MacUpCursor		30
#define		MacLeftCursor	28
#define		MacRightCursor	29
#define		MacHomeKey		1
#define		MacEndKey		4
#define		MacDeleteKey	8

unsigned char keymap[256] = {
	/* 0 = shft */0,
	/* 1 = ctl */0,
	/* 2 = alt */0 ,
	/* 3 = L-shft */0 ,
	/* 4 = L-ctl */0 ,
	/* 5 = L-alt */0 ,
	/* 6 = R-shft */0 ,
	/* 7 = R-ctl */0 ,
	/* 8 = R-alt */0 ,
	/* 9 = L-butt */0 ,
	/* 10 = M-butt */0 ,
	/* 11 = R-butt */0 ,
	/* 12 = ? */0 ,
	/* 13 = ? */0 ,
	/* 14 = ? */0 ,
	/* 15 = ? */0 ,
	/* 16 = q */ 'q' ,
	/* 17 = 3/� */ '3',
	/* 18 = 4/$ */ '4',
	/* 19 = 5/% */ '5',
	/* 20 = F4 */ 0 ,
	/* 21 = 8 / * */ '8',
	/* 22 = ? */ 0 ,
	/* 23 = - */ '-',
	/* 24 = 6/^ */ '6',
	/* 25 = L-crsr */ MacLeftCursor,
	/* 26 = ? */0 ,
	/* 27 = K-7/ Home */ 0,
	/* 28 = F11 */ 0,
	/* 29 = F12 */ 0,
	/* 30 = F10 */ 0,
	/* 31 = ScrollLock */ 0,
	/* 32 = PrntScrn */ 0,
	/* 33 = w */ 'w',
	/* 34 = e */ 'e',
	/* 35 = t */ 't',
	/* 36 = 7/& */ '7',
	/* 37 = i */ 'i',
	/* 38 = 9/( */ '9',
	/* 39 = 0/) */ '0',
	/* 40 = - (same as 23) */ '-',
	/* 41 = D-crsr */ MacDownCursor,
	/* 42 = K-8 */ 0,
	/* 43 = K-9 */ 0,
	/* 44 = Pause/Break */ 0,
	/* 45 = `/� (topleft) */ '`',
	/* 46 = ? (seems to be missing) */ 0,
	/* 47 = bkspc */ 0,
	/* 48 = 1/! */ '1',
	/* 49 = 2/" */ '2',
	/* 50 = d */ 'd',
	/* 51 = r */ 'r',
	/* 52 = 6/^ (same as 24)*/ '6',
	/* 53 = u */ 'u',
	/* 54 = o */ 'o',
	/* 55 = p */ 'p',
	/* 56 = [/{ */ '[',
	/* 57 = U-crsr */ MacUpCursor,
	/* 58 = K-+ (?) */ 0,
	/* 59 = K-??? */ 0,
	/* 60 = K-Enter */ 0,
	/* 61 = Insert */ 0,
	/* 62 = Home */ MacHomeKey,
	/* 63 = PgUP */ 0,
	/* 64 = CapsLock ? */ 0,
	/* 65 = a */ 'a',
	/* 66 = x */ 'x',
	/* 67 = f */ 'f',
	/* 68 = y */ 'y',
	/* 69 = j */ 'j',
	/* 70 = k */ 'k',
	/* 71 = 2/" (same 49) */ '2',
	/* 72 = ;/: */ ';',
	/* 73 = Return */ 0,
	/* 74 = K-/ */ 0,
	/* 75 = ?? */ 0,
	/* 76 = K-Del/. */ 0,
	/* 77 = NumLock */ 0,
	/* 78 = PgDOWN */ 0,
	/* 79 = '/@ */ '\'',
	/* 80 = ?? */ 0,
	/* 81 = s */ 's',
	/* 82 = c */ 'c',
	/* 83 = g */ 'g',
	/* 84 = h */ 'h',
	/* 85 = n */ 'n',
	/* 86 = l */ 'l',
	/* 87 = ;/: (same as 72) */ ';',
	/* 88 = ]/} */ ']',
	/* 89 = Delete */ MacDeleteKey,
	/* 90 = K-- */ 0,
	/* 91 = K-* */ 0,
	/* 92 = ?? */ 0,
	/* 93 = =/+ */ '=',
	/* 94 = \/| */ '\\',
	/* 95 = ?? */ 0,
	/* 96 = tab? */ 0x9,
	/* 97 = z */ 'z',
	/* 98 = SPC */ ' ',
	/* 99 = v */ 'v',
	/* 100 = b */ 'b',
	/* 101 = m */ 'm',
	/* 102 = ,/< */ ',',
	/* 103 = ./> */ '.',
	/* 104 = //? */ '/',
	/* 105 = End */ MacEndKey,
	/* 106 = K-ins */ 0,
	/* 107 = K-1/End */ 0,
	/* 108 = K-3/PgDn */ 0,
	/* 109 = ?? */ 0,
	/* 110 = ?? */ 0,
	/* 111 = ?? */ 0,
	/* 112 = esc */0x1B,
	/* 113 = F1 */ 0,
	/* 114 = F2 */ 0,
	/* 115 = F3 */ 0,
	/* 116 = F5 */ 0,
	/* 117 = F6 */ 0,
	/* 118 = F8 */ 0,
	/* 119 = F9 */ 0,
	/* 120 = ~/# */ 0,
	/* 121 = R-crsr */ MacRightCursor,
	/* 122 = K-4 */ 0,
	/* 123 = K-5 */ 0,
	/* 124 = K-2 */ 0,
	/* 125 -255 all null ?? */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 };

 
/*** Functions ***/
	 void ActivateWindow(wimp_block * wblock);
	 void DeactivateWindow( wimp_block * wblock);
	 void DisplayPixmap(void);
extern	 void displayModeChanged(void);
	 void DoNothing(void);
	  int HandleEvents(int);
	 void KeyPressed( wimp_key * wblock);
	 void keyBufAppend(int key);
	 void UserMessage(wimp_message * wblock);
	 void MouseButtons( wimp_pointer * wblock);
	 void PointerEnterWindow(wimp_block *wblock);
	 void PointerLeaveWindow(wimp_block *wblock);
	 void WindowClose(wimp_close * wblock);
	 void WindowOpen( wimp_open * wblock);
extern	 void claimCaret(wimp_pointer * wblock);
extern	 void receivedClaimEntity(wimp_message * wblock);
extern	 void receivedDataRequest(wimp_message * wmessage);
extern	 void receivedDataSave(wimp_message * wblock);
extern	 void receivedDataSaveAck(wimp_message * wblock);
	 void eventBufAppendKey( int key, int buttons, int x, int y);
	 void eventBufAppendMouseDown(int buttons, int x, int y);
	 void eventBufAppendMouseUp(int buttons, int x, int y);
	 void eventBufAppendMouseMove(int x, int y);
extern	 void platReportError( os_error * e);



void setSocketPollFunction(int spf ) {
	socketPollFunction = (void(*)(int, int))spf;
#ifdef DEBUG
{
	extern os_error privateErr;
	privateErr.errnum = (bits)0;
	sprintf(privateErr.errmess, "socketPoll %0x", (int)socketPollFunction);
	platReportError((os_error *)&privateErr);
}
#endif
}

/* Event handling routines */

int HandleEvents(int microSecondsToDelay) {
wimp_event_no wimpPollEvent;
wimp_pointer wblock;
os_error *e;
int kbdstate, pollDelay;

	pollDelay = microSecondsToDelay /* * CLOCKS_PER_SEC / 1000000 */
			>> 14 /* will always give small answer, but good enough */;
	if ( mouseButtonDown | windowActive) {
		/* if the window is active or mouse buttons are supposedly down, */
		/* check for current state */
		e = xwimp_get_pointer_info( &wblock);
		buttonState = ((int)wblock.buttons & 0x7);
		mouseButtonDown = (int)wblock.buttons>0?true:false;
	}
	if (mouseButtonDown | windowActive ) {
		/* the window is active or the mouse buttons are still down, find */
		/* mouse pos & shift state */
		savedMousePosition.x = (wblock.pos.x +
					scrollOffset.x -
					visibleArea.x) >> scalingFactor.x;
		savedMousePosition.y = (visibleArea.y -
					wblock.pos.y -
					scrollOffset.y)>>scalingFactor.y;
		/* if new buttons state ~= prev one, would send event here */
		/* xosbyte_read(osbyte_VAR_KEYBOARD_STATE , &kbdstate); obsolete */
		xosbyte1(osbyte_SCAN_KEYBOARD , 0x80, 0, &kbdstate);
		if (kbdstate >0 ) buttonState = buttonState | ShiftKey;
		xosbyte1(osbyte_SCAN_KEYBOARD , 0x84, 0, &kbdstate);
		if (kbdstate >0 ) buttonState = buttonState | CtlKey;
		xosbyte1(osbyte_SCAN_KEYBOARD , 0x87, 0, &kbdstate);
		if (kbdstate >0 ) buttonState = buttonState | CmdKey;
	}

	if(socketPollFunction) {
		socketPollFunction(microSecondsToDelay, 0);
#ifdef DEBUG
{
	extern os_error privateErr;

	privateErr.errnum = (bits)0;
	sprintf(privateErr.errmess, "socketPoll %0x", (int)socketPollFunction);
	platReportError((os_error *)&privateErr);
}
#endif
	}

	while( true ) {
		xwimp_poll_idle((wimp_MASK_POLLWORD| wimp_MASK_GAIN | wimp_MASK_LOSE | wimp_SAVE_FP) , &wimpPollBlock, (os_t)pollDelay, &wimpPollWord, (wimp_event_no*)&wimpPollEvent);
		switch ( wimpPollEvent ) {
			case wimp_NULL_REASON_CODE:
				/* null means no more interesting events,
				   so return */
				return false ; break;
			case wimp_REDRAW_WINDOW_REQUEST	:
				DisplayPixmap(); break;
			case wimp_OPEN_WINDOW_REQUEST	:
				WindowOpen(&wimpPollBlock.open); break;
			case wimp_CLOSE_WINDOW_REQUEST	:
				WindowClose(&wimpPollBlock.close); break;
			case wimp_POINTER_LEAVING_WINDOW :
				PointerLeaveWindow(&wimpPollBlock); break;
			case wimp_POINTER_ENTERING_WINDOW:
				PointerEnterWindow(&wimpPollBlock); break;
			case wimp_MOUSE_CLICK			:
				MouseButtons(&wimpPollBlock.pointer); break;
			case wimp_USER_DRAG_BOX			:
				DoNothing(); break;
			case wimp_KEY_PRESSED			:
				KeyPressed( &wimpPollBlock.key); break;
			case wimp_MENU_SELECTION		:
				DoNothing(); break;
			case wimp_SCROLL_REQUEST		:
				DoNothing(); break;
			case wimp_USER_MESSAGE			:
				UserMessage(&wimpPollBlock.message); break;
			case wimp_USER_MESSAGE_RECORDED		:
				UserMessage(&wimpPollBlock.message); break;
			case wimp_USER_MESSAGE_ACKNOWLEDGE	:
				UserMessage(&wimpPollBlock.message); break;
		} 
	}
}

void reverseNothing(void) {
/* do nothing, as fast as possible */
}

void reverse_image_1bpps(void) {
unsigned int * srcPtr, *dstPtr;
int j;
	for(j = startY; j < stopY; j += scanLine) {
		srcPtr = (unsigned int *)displayBitmapIndex + j + startX;
		dstPtr = displaySpriteBits + j + startX;
		{ int i, k;
		unsigned int w, nw;
			for (i=xLen; i--; srcPtr++, dstPtr++) {
				w = (unsigned int ) *srcPtr;
				nw = w & 0x1;
				for (k=31; k--;) {
					w = w >> 1;
					nw = (nw << 1) | (w & 0x1);
				}
				*dstPtr = nw;
			}
		}
	}
}

void reverse_image_2bpps(void) {
unsigned int * srcPtr, *dstPtr;
int j;
	for(j = startY; j < stopY; j += scanLine) {
		srcPtr = (unsigned int *)displayBitmapIndex + j + startX;
		dstPtr = displaySpriteBits + j + startX;
		{ int i, k;
		unsigned int w, nw;
			for (i=xLen; i--; srcPtr++, dstPtr++) {
				w = (unsigned int ) *srcPtr;
				nw = w & 0x3;
				for (k=15; k--;) {
					w = w >> 2;
					nw = (nw << 2) | (w & 0x3);
				}
				*dstPtr = nw;
			}
		}
	}
}

void reverse_image_4bpps(void) {
unsigned int * srcPtr, *dstPtr;
int j;
	for(j = startY; j < stopY; j += scanLine) {
		srcPtr = (unsigned int *)displayBitmapIndex + j + startX;
		dstPtr = displaySpriteBits + j + startX;
		{ int i;
		unsigned int w, nw;
			for (i=xLen; i--; srcPtr++, dstPtr++) {
				w = (unsigned int ) *srcPtr;
				nw = w & 0xF;
				w = w >> 4;
				nw = (nw << 4) | (w & 0xF);
				w = w >> 4;
				nw = (nw << 4) | (w & 0xF);
				w = w >> 4;
				nw = (nw << 4) | (w & 0xF);
				w = w >> 4;
				nw = (nw << 4) | (w & 0xF);
				w = w >> 4;
				nw = (nw << 4) | (w & 0xF);
				w = w >> 4;
				nw = (nw << 4) | (w & 0xF);
				w = w >> 4;
				nw = (nw << 4) | (w & 0xF);
				*dstPtr = nw;
			}
		}
	}
}

void reverse_image_bytes(void)  {
unsigned int * srcPtr, *dstPtr;
int j;
	for(j = startY; j < stopY; j += scanLine) {
		srcPtr = (unsigned int *)displayBitmapIndex + j + startX;
		dstPtr = displaySpriteBits + j + startX;
		{ int i;
		unsigned int w, nw;
			for (i=xLen; i--; srcPtr++, dstPtr++) {
				w = *srcPtr;
				nw = w & 0xFF;
				w = w >>8;
				nw = (nw << 8) | (w & 0xFF);
				w = w >>8;
				nw = (nw << 8) | (w & 0xFF);
				w = w >>8;
				nw = (nw << 8) | (w & 0xFF);
				*dstPtr = nw;
			}
		}
	}
}

void reverse_image_words(void) {
unsigned int * srcPtr, *dstPtr;
int j;
	for(j = startY; j < stopY; j += scanLine) {
		srcPtr = (unsigned int *)displayBitmapIndex + j + startX;
		dstPtr = displaySpriteBits + j + startX;
		{ int i;
		unsigned int w, nw;
			for (i=xLen; i--; srcPtr++, dstPtr++) {
				w = (unsigned int ) *srcPtr;
				nw = w & 0x1F;
				w = w >> 5;
				nw = (nw << 5) | (w & 0x1F);
				w = w >> 5;
				nw = (nw << 5) | (w & 0x1F);
				w = w >> 6;
				nw = (nw << 6) | (w & 0x1F);
				w = w >> 5;
				nw = (nw << 5) | (w & 0x1F);
				w = w >> 5;
				nw = (nw << 5) | (w & 0x1F);
				*dstPtr = nw;
			}
		}
	}
}

void reverse_image_longs(void) {
unsigned int * srcPtr, *dstPtr;
int j;
	for(j = startY; j < stopY; j += scanLine) {
		srcPtr = (unsigned int *)displayBitmapIndex + j + startX;
		dstPtr = displaySpriteBits + j + startX;
		{ int i;
		unsigned int w, nw;
			for (i=xLen; i--; srcPtr++, dstPtr++) {
				w = (unsigned int ) *srcPtr;
				nw = w & 0xFF;
				w = w >> 8;
				nw = (nw << 8) | (w & 0xFF);
				w = w >> 8;
				nw = (nw << 8) | (w & 0xFF);
				*dstPtr = nw;
			}
		}
	}
}

void DisplayReverseSetup() {
extern os_coord squeakDisplaySize;
extern int squeakDisplayDepth;
int log2Depth;

	switch (squeakDisplayDepth) {
	case 32:log2Depth=5; reverserFunction = reverse_image_longs; break;
	case 16:log2Depth=4; reverserFunction = reverse_image_words; break;
	case 8: log2Depth=3; reverserFunction = reverse_image_bytes;break;
	case 4: log2Depth=2; reverserFunction = reverse_image_4bpps;break;
	case 2: log2Depth=1; reverserFunction = reverse_image_2bpps;break;
	case 1: log2Depth=0; reverserFunction = reverse_image_1bpps;break;
	default: reverserFunction = reverseNothing; return; 
	}
	/* work out words per scan line */
	pixelsPerWordShift = 5-log2Depth;
	pixelsPerWord= 1 << pixelsPerWordShift;
	scanLine= (squeakDisplaySize.x + pixelsPerWord-1) >> pixelsPerWordShift;
}

int DisplayReverseAreaSetup(int x0, int y0, int x1, int y1) {
int stopX;
	startX = (x0 >> pixelsPerWordShift) ;
	stopX  = (x1 + pixelsPerWord -1) >> pixelsPerWordShift;
	xLen = stopX - startX /* +1 */;
	startY = y0 * scanLine;
	stopY  = y1 * scanLine;
	if(stopX <= startX || stopY <= startY) return false;
	return true;
}

void DisplayPixmap(void) {
/* bitblt the Display bitmap to the screen */
extern osspriteop_area *spriteAreaPtr;
extern osspriteop_header *displaySprite;
extern osspriteop_trans_tab *	pixelTranslationTable;
extern int displayObject(void);
osbool more;
wimp_draw wblock;
os_error * e;
int xA, yA, xB, yB;
	if ( displaySpriteBits == NULL ) {
#ifdef DEBUG
{
		extern os_error privateErr;
		privateErr.errnum = (bits)0;
		sprintf(privateErr.errmess, "DisplayPixmap NULL");
		platReportError((os_error *)&privateErr);
}
#endif
		/* flush the damage rectangles */
		wblock.w = sqWindowHandle;
		more = wimp_redraw_window( &wblock );
		while ( more ) {
			xwimp_get_rectangle (&wblock, &more);
		}
		return;
	}
	wblock.w = sqWindowHandle;
	/* Find latest address of source Bitmap.
	   Sensitive to object format */
	displayBitmapIndex = longAt((displayObject() + 4)+(0 * 4)) + 4; 
	more = wimp_redraw_window( &wblock );
	DisplayReverseSetup();
	while ( more ) {
		xA = (wblock.clip.x0 - visibleArea.x ) >> scalingFactor.x;
		yA = (visibleArea.y - wblock.clip.y1  ) >> scalingFactor.y;
		xB = (wblock.clip.x1 - visibleArea.x ) >> scalingFactor.x;
		yB = (visibleArea.y -  wblock.clip.y0   ) >> scalingFactor.y;
		DisplayReverseAreaSetup(xA, yA, xB, yB);
		reverserFunction();
		if ((e = xosspriteop_put_sprite_scaled (osspriteop_USER_AREA,
			spriteAreaPtr,
			(osspriteop_id)&(displaySprite->name),
			wblock.box.x0, wblock.box.y0,
			os_ACTION_OVERWRITE | osspriteop_GIVEN_WIDE_ENTRIES,
			(os_factors const *)0,pixelTranslationTable)) != NULL) {
			if ( spriteAreaPtr != null) {
				platReportError(e);
				return;
			}
		}
		xwimp_get_rectangle (&wblock, &more);
	}
}

void DisplayPixmapNow(void) {
/* bitblt the Display bitmap to the screen */
extern osspriteop_area *spriteAreaPtr;
extern osspriteop_header *displaySprite;
extern osspriteop_trans_tab *	pixelTranslationTable;
extern os_coord squeakDisplaySize;
//bool more;
int more;
wimp_draw wblock;
os_error * e;
int xA, yA, xB, yB;

	if ( sqWindowHandle == null ) return;
	wblock.w = sqWindowHandle;
	/* set work area to suit these values */
	wblock.box.x0 = -10000;
	wblock.box.y0 = -10000/* squeakDisplaySize.y << scalingFactor.y */;
	wblock.box.x1 = 10000/* squeakDisplaySize.x <<scalingFactor.x */;
	wblock.box.y1 = 10000;

	more = wimp_update_window( &wblock );
	DisplayReverseSetup();
	while ( more ) {
		xA = (wblock.clip.x0 - visibleArea.x ) >> scalingFactor.x;
		yA = (visibleArea.y - wblock.clip.y1  ) >> scalingFactor.y;
		xB = (wblock.clip.x1 - visibleArea.x ) >> scalingFactor.x;
		yB = (visibleArea.y -  wblock.clip.y0   ) >> scalingFactor.y;
		DisplayReverseAreaSetup(xA, yA, xB, yB);
		reverserFunction();
		if ((e = xosspriteop_put_sprite_scaled (osspriteop_USER_AREA,
			spriteAreaPtr,
			(osspriteop_id)&(displaySprite->name),
			wblock.box.x0, wblock.box.y0,
			os_ACTION_OVERWRITE | osspriteop_GIVEN_WIDE_ENTRIES,
			(os_factors const *)0,pixelTranslationTable)) != NULL) {
			if ( spriteAreaPtr != null) {
				platReportError(e);
				return;
			}
		}
		xwimp_get_rectangle (&wblock, &more);
	}
}


void WindowOpen( wimp_open * wblock) {
	scrollOffset.x = wblock->xscroll;
	scrollOffset.y = wblock->yscroll;
	visibleArea.x =  wblock->visible.x0;
	visibleArea.y =  wblock->visible.y1;
	xwimp_open_window(wblock);
}

void WindowClose(wimp_close * wblock) {
	sqWindowHandle = null;
	PointerLeaveWindow((wimp_block *)wblock);
	xwimp_close_window(wblock->w);
	/* temporarily, quit Squeak. One day we'll just close the window */
	/* and allow it to reopen from an iconbar click or somesuch      */
	exit(0);
}

void PointerLeaveWindow(wimp_block *wblock) {
/* the pointer has left my area. swap to normal desktop pointer */
	xwimp_set_pointer_shape(1, (byte const *)-1, 0, 0, 0, 0);
	windowActive = false;
}

void PointerEnterWindow(wimp_block *wblock) {
/* the pointer has moved into my area. swap to my pointer pixmap */
	xwimp_set_pointer_shape(2, (byte const *)pointerBuffer,  16, 16, pointerOffset.x, pointerOffset.y);
	windowActive = true;
}

void MouseButtons( wimp_pointer * wblock) {
/* deal with a mouse button change event */
/* RiscOS only seems to send me mouse downs, and we have to manually claim */
/* input focus with a wimp_BUTTON_CLICK window type */
	if ( (wblock->w == sqWindowHandle)  && (wblock->i == (wimp_i)wimp_ICON_WINDOW)) {
		/* its in my window */
		buttonState = (buttonState & ~0x7) | ((int)wblock->buttons & 0x7);
		/* track the mouse downs separately */
		mouseButtonDown = (int)wblock->buttons>0?true:false;
		// claim caret via message_claimentity() iff we don't already have it
		claimCaret(wblock);
		// do we still use wimp_set_caret_position ?
		xwimp_set_caret_position(sqWindowHandle, (wimp_i)wimp_ICON_WINDOW, 0, 0, (1<<25)|255, 0);
		return;
	}

}

void KeyPressed( wimp_key * wblock) {
/* deal with a keypress. This is complicated by the RiscOS habit of "helpfully"
 * converting keycodes into fully processed key events. We do not even get
 * notification of most alt presses, for example. We also have to convert to
 * Mac numbering in order to satisfy the image code
 */

	int keystate, testkey;

	/* basically keystate will be the event idea of the key pressed */
	keystate = wblock->c;

	if (keystate == getInterruptKeycode() || ( (keystate == wimp_KEY_PRINT)) ) {
		/* The image tends to set the interruptKeycode to suit the Mac
		 * cmd-. nonsense this is decidedly not Acorn compatible, so
		 * check for printscrn/SysRq as well
		 * interrupt is a meta-event; do not report it as a keypress
		 */
		setInterruptPending(true); 
		setInterruptCheckCounter(0);
		return;
	}

	if ( buttonState & 0x70) {
		/* if a metakey is pressed, try looking up the magic number
		 * and dealing with a metakey situation
		 */
		xosbyte1(osbyte_SCAN_KEYBOARD_LIMITED , 0, 0, &testkey);
		/* if a key is scanned ok and it maps, replace the keystate
		 * with the result
		 */
		if ( (testkey != 0xFF) && (testkey = keymap[testkey]) )
			keystate = testkey;
	} else {
		/* no metakey, so check for special key values. */
		switch(keystate) {
			case wimp_KEY_TAB: keystate = 0x09; break;
			case wimp_KEY_LEFT: keystate = MacLeftCursor; break;
			case wimp_KEY_RIGHT: keystate = MacRightCursor; break;
			case wimp_KEY_DOWN: keystate = MacDownCursor; break;
			case wimp_KEY_UP: keystate = MacUpCursor; break;
			case wimp_KEY_HOME: keystate = MacHomeKey; break;
			case wimp_KEY_COPY: keystate = MacEndKey; break;
			case wimp_KEY_DELETE: keystate = MacDeleteKey; break;
			default: break;
		}
	}
	keyBufAppend(keystate | ((buttonState >>3)<<8));
	eventBufAppendKey((keystate & 0xFF), buttonState, savedMousePosition.x, savedMousePosition.y);
}

/* set an asynchronous input semaphore index for events */
int ioSetInputSemaphore(int semaIndex) {
	if( semaIndex < 1)
		return primitiveFail();
	inputSemaphoreIndex = semaIndex;
	return true;
}

void signalInputEvent(void) {
	if(inputSemaphoreIndex > 0)
		signalSemaphoreWithIndex(inputSemaphoreIndex);
}

/* Event buffer functions */

#define iebEmptyP()	(eventBufPut == eventBufGet)
#define iebAdvance(P)	(P= ((P + 1) % EVENTQ_SIZE))

sqInputEvent *eventBufAppendEvent(int  type) {
/* code stolen from ikp's unix code. blame him if it doesn't work.
 * complement me if it does.
 */
	sqInputEvent *evt= &eventBuf[eventBufPut];
	iebAdvance(eventBufPut);
	if (iebEmptyP()) {
		/* overrun: discard oldest event */
		iebAdvance(eventBufGet);
	}
	evt->type= type;
	evt->timeStamp= ioMSecs();
	signalInputEvent();
	return evt;

}

void eventBufAppendKey( int keyValue, int buttons, int x, int y) {
/* add an event record for a keypress */

	eventBufAppendEvent( EventTypeKeyboard);
}

void eventBufAppendMouseDown( int buttons, int x, int y) {
/* add an event record for a mouse press */
	eventBufAppendEvent( EventTypeMouse);
}

void eventBufAppendMouseUp( int buttons, int x, int y) {
/* add an event record for a mouse up */
	eventBufAppendEvent( EventTypeMouse);
}

void eventBufAppendMouseMove( int x, int y) {
/* add an event record for a mouse up */
	eventBufAppendEvent( EventTypeMouse);
}



/* retrieve the next input event from the OS */
int ioGetNextEvent(sqInputEvent *evt) {
	HandleEvents(0);
	primitiveFail();
}

/* key buffer functions to support older images */
void keyBufAppend(int keystate) {
	keyBuf[keyBufPut] = keystate;
	keyBufPut = (keyBufPut + 1) % KEYBUF_SIZE;
	if (keyBufGet == keyBufPut) {
		keyBufGet = (keyBufGet + 1) % KEYBUF_SIZE;
		keyBufOverflows++;
	}
}

int nextKeyPressOrNil(void) {
/*  return the next keypress in the buffer, or -1 if nothing there */
int keystate;
	if (keyBufGet == keyBufPut) {
		return -1;  /* keystroke buffer is empty */
	} else {
		keystate = keyBuf[keyBufGet];
		keyBufGet = (keyBufGet + 1) % KEYBUF_SIZE;
	}
	return keystate;
}

int peekKeyPressOrNil(void) {
/*  return the next keypress in the buffer, or -1 if nothing there */
int keystate;
	if (keyBufGet == keyBufPut) {
		return -1;  /* keystroke buffer is empty */
	} else {
		keystate = keyBuf[keyBufGet];
	}
	return keystate;
}
int ioGetButtonState(void) {
	ioProcessEvents();  /* process all pending events */
	return buttonState;
}

int ioGetKeystroke(void) {
	ioProcessEvents();  /* process all pending events */
	return nextKeyPressOrNil();
}

int ioMousePoint(void) {
/* return the mouse point as 16bits of x | 16bits of y */
	ioProcessEvents();  /* process all pending events */
	return (savedMousePosition.x << 16 | savedMousePosition.y & 0xFFFF);
}

int ioPeekKeystroke(void) {
	ioProcessEvents();  /* process all pending events */
	return peekKeyPressOrNil();
}

/*** I/O Primitives ***/

int ioProcessEvents(void) {
static clock_t nextPollTick = 0;
clock_t currentTick;

//	if( (currentTick = clock()) >= nextPollTick) {
		HandleEvents(0 );
//		nextPollTick = currentTick + 1;
//	}
	return true; 
}

int ioRelinquishProcessorForMicroseconds(int microSeconds) {
/* This operation is platform dependent. On the Mac, it simply calls
 * HandleEvents(), which gives other applications a chance to run.
 * Here, we use microSeconds as the parameter to HandleEvents, so that wimpPollIdle
 * gets a timeout.
 */

	HandleEvents(microSeconds);
	return microSeconds;
}

void UserMessage(wimp_message * wblock) {
/* Deal with user messages */
extern wimp_t Task_Handle;
	if( wblock->sender == Task_Handle) {
		/* it's me - do nothing */
		return;
	}
	switch( wblock->action) {
		case message_QUIT:		ioExit();
								break;
		case message_MODE_CHANGE: displayModeChanged();
								break;
		case message_CLAIM_ENTITY: receivedClaimEntity(wblock);
								break;
		/* these are the two messages we respond to in order
		 * to initiate clipboard transactions
		 * DATA_REQUEST is another app asking for our clipboard
		 * and DATA_SAVE_ACK is part of the dance for giving
		 * it to them. Us asking for some outside clipboard
		 * can be found in sqRPCCLipboard>fetchClipboard()
		 */
		case message_DATA_REQUEST: receivedDataRequest(wblock);
								break; 
		case message_DATA_SAVE_ACK: receivedDataSaveAck(wblock);
								break;
		/* We _might_ sometime respond to DATA_LOAD & DATA_SAVE
		 * here in order to allo dropping of text files via the
		 * DropPlugin
		 */ 
		default: return;
	}
}

void ActivateWindow(wimp_block * wblock) {
/* maybe we will need to do something here */
	PointerEnterWindow(wblock);
}

void DeactivateWindow( wimp_block * wblock) {
/* maybe we will need to do something here */
	PointerLeaveWindow(wblock);
}
 
void DoNothing(void) {
/* do nothing at all, but make sure to do it quickly. Primarily a breakpoint option */
}
