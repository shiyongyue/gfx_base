#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XShm.h>

#include <asm/msr.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//#define FONTNAME "-kochi-gothic-medium-r-normal--30-*-*-*-*-*-*-*"
#define FONTNAME "/home/stormyberg/my/ArialMT.ttf"
static Display *display;
static Window mainwin;
static XFontStruct *fontinfo;
static XImage *backbuf, *backbuf2;
static Pixmap backbuf3;
static XShmSegmentInfo shm_img;
static XShmSegmentInfo shm_pix;

static int scr_depth;

#define SCREEN DefaultScreen(display)
#define VISUAL DefaultVisual(display,SCREEN)

static int button_press( XButtonEvent *ev )
{
	int click=0;
	static unsigned int flipCount = 0;
	int before;
	int after;
	GC gc = DefaultGC( display, SCREEN );
	flipCount+=1;
	int color = flipCount%3;
	printf("click = %d color =%d\n",click,color);

	switch (click) {

	case 0:
		//rdtscll( before );
		printf("backbuf->bytes_per_line = %d backbuf->height=%d\n",backbuf->bytes_per_line,backbuf->height);
	//	memset( backbuf->data, 0xff, backbuf->bytes_per_line*backbuf->height );
		//rdtscll( after );
	unsigned char *pData = backbuf->data;{
		for(int i = 0; i < SCREEN_HEIGHT; i++ ){ // per line
			unsigned char	*line = pData + i * SCREEN_WIDTH * 4;
			for(int j = 0; j < SCREEN_WIDTH; j++){
				unsigned char *row = line + j * 4;
				if(color == 0){
				row[0] = 0xff;   // blue
				row[1] = 0x00;   //green is max
				row[2] = 0x00;  // red
				row[3] = 0xff;  //alph channel unsed for 24bit depth
//				printf("set buffer blue\n");
				}else if(1==color){
				row[0] = 0x00;   // blue
				row[1] = 0xff;   //green is max
				row[2] = 0x00;  // red
				row[3] = 0xff;  //alph channel unsed for 24bit depth
//				printf("set buffer green\n");
				}else{
				row[0] = 0x00;   // blue
				row[1] = 0x00;   //green is max
				row[2] = 0xff;  // red
				row[3] = 0xff;  //alph channel unsed for 24bit depth
//				printf("set buffer red\n");
				
				}

			}
		}	
	}
		
		printf("memset:  %d\n", after-before);
//		break;

//	case 1:		
		//rdtscll( before );
		XShmPutImage( display, ev->window, gc, backbuf, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, False );
		XSync( display, False );
		//rdtscll( after );
		printf("xshmput: %d\n", after-before);
		break;

#if 0
	case 2:
		//rdtscll( before );
		XPutImage( display, ev->window, gc, backbuf2, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		XSync( display,False  );
		//rdtscll( after );

		printf("put:     %d\n", after-before);
		break;
		
	case 3:
		//rdtscll( before );
		
		XCopyArea( display, backbuf3, ev->window, gc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0 );
		XSync( display,False  );
		//rdtscll( after );

		printf("shmpix:  %d\n", after-before);
		break;
#endif
	default:
		click=0;
	}

	click++;

	return 0;
}

static int key_press( XKeyEvent *ev )
{
	if ( XKeycodeToKeysym( display, ev->keycode, 0 ) == XK_Escape )
		return 1;
	return 0;	
}

static void expose( XExposeEvent *ev )
{
	GC gc = DefaultGC( display, SCREEN );
	XGCValues val;

	XPutImage( display, ev->window, gc, backbuf, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	//val.foreground = WhitePixel( display, SCREEN );
	//val.font = fontinfo->fid;

	//XChangeGC( display, gc, GCForeground|GCFont, &val );

	//XDrawString( display, ev->window, gc, 30, 30, "abcd", 4 );

	XFlush( display );
}

static XImage *shmimage()
{
	XImage *ret;
	
	ret = XShmCreateImage( display, VISUAL, scr_depth, ZPixmap, NULL,
			       &shm_img, SCREEN_WIDTH, SCREEN_HEIGHT );

	shm_img.shmid = shmget( IPC_PRIVATE, ret->bytes_per_line*ret->height, IPC_CREAT|0600 );
	shm_img.readOnly = 0;
	shm_img.shmaddr = shmat( shm_img.shmid, 0, 0 );
	ret->data = shm_img.shmaddr;

#if 0
	unsigned char *pData = ret->data;{
		for(int i = 0; i < SCREEN_HEIGHT; i++ ){ // per line
			char	*line = pData + i * SCREEN_WIDTH * 4;
			for(int j = 0; j < SCREEN_WIDTH; j++){
				char *row = line + j * 4;
				row[0] = 0x00;   // blue
				row[1] = 0x80;   //green is max
				row[2] = 0x80;  // red
				row[3] = 0xff;  //alph channel unsed for 24bit depth
			}
		}	
	}
#endif


	XShmAttach( display, &shm_img );
	XSync( display, False );

	shmctl( shm_img.shmid, IPC_RMID, 0 );

	return ret;
}

static Pixmap shmpixmap()
{
	XImage *img;
	Pixmap ret;
	GC gc;
	XGCValues val;
	int format = XShmPixmapFormat(display);
	const int depth=DefaultDepth(display,SCREEN);

	/* bytes_per_line を取得 */
	img = XShmCreateImage( display, VISUAL, depth, format, NULL,
			       &shm_pix, SCREEN_WIDTH, SCREEN_HEIGHT );
	shm_pix.shmid = shmget( IPC_PRIVATE, img->bytes_per_line*img->height, IPC_CREAT|0600 );
	shm_pix.readOnly = 0;
	shm_pix.shmaddr = shmat( shm_pix.shmid, 0, 0 );
	XDestroyImage( img );

	XShmAttach( display, &shm_pix );
	XSync( display, False );

	ret = XShmCreatePixmap( display, mainwin, shm_pix.shmaddr, &shm_pix, SCREEN_WIDTH, SCREEN_HEIGHT, depth );

	/* わかるように色を付けておこう… */
	memset( shm_pix.shmaddr, 0x3f, SCREEN_WIDTH*SCREEN_HEIGHT*depth/8 );

	shmctl( shm_pix.shmid, IPC_RMID, 0 );

	val.foreground = WhitePixel(display,SCREEN);
	
	gc = XCreateGC( display, ret, GCForeground, &val );

	/* GCも動く証拠 */
	XDrawLine( display, ret, gc, 0, 0, 100, 100 );
	XFreeGC( display, gc );
	printf("%d\n",shm_pix.shmaddr[3]);

	return ret;
}	

static void load()
{
	backbuf2 = XCreateImage( display, DefaultVisual(display,SCREEN), scr_depth,
				ZPixmap, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 
				32, 0 );
	backbuf2->data = malloc( backbuf2->bytes_per_line * backbuf2->height );
	
	backbuf = shmimage();
	backbuf3 = shmpixmap();
}

static void destroy()
{
	//XFreeFontInfo( NULL, fontinfo, 1 );
	XDestroyImage( backbuf );
	XFreePixmap( display, backbuf3 );
}

int main( int argc, char **argv )
{
	setlocale( LC_ALL, "" );

	display = XOpenDisplay( NULL );
	//fontinfo = XLoadQueryFont( display, FONTNAME );
	printf("shiyongyue fontinfo = %p\n",fontinfo);
	scr_depth = DefaultDepth(display,SCREEN);

	mainwin = XCreateSimpleWindow( display, RootWindow(display,SCREEN),
				       0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, WhitePixel(display, SCREEN) );

	load();

	XSelectInput( display, mainwin, KeyPressMask | ButtonPressMask | ExposureMask );

	XRaiseWindow( display, mainwin );
	XMapWindow( display, mainwin );

	XFlush( display );


	while ( 1 ) {
		int ret = 0;
		XEvent ev;
		XNextEvent( display, &ev );

		switch ( ev.type ) {
		case ButtonPress:
			printf("ButtonPress\n");
			ret = button_press( (XButtonEvent*)&ev );
			break;

		case KeyPress:
			printf("KeyPress\n");
			ret = key_press( (XKeyEvent*)&ev );
			break;

		case Expose:
			printf("Expose\n");
			expose( (XExposeEvent*)&ev );
		}

		if ( ret )
			break;
	}

	destroy();
	XCloseDisplay( display );

	return 0;
}
