/*
   (c) Copyright 2001-2008  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrjälä <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primary.h"
#include "xwindow.h"
#include "x11.h"
#include "x11_surface_pool.h"
#include "x11_surface_pool_bridge.h"

#ifdef USE_GLX
#include "glx_surface_pool.h"
#endif

#include "vpsmem_surface_pool.h"
#include "dlog.h"

DFBX11       *gpX11;


/*
static VideoMode modes[] = {
     {  320,  200 },
     {  320,  240 },
     {  512,  384 },
     {  640,  480 },
     {  768,  576 },

     { 1024,  576 },     // 16:9
     { 1024,  600 },     // Where does that mode come from? :-)
     { 1024,  768 },     // 4:3

     { 1280,  720 },     // 16:9
     { 1280,  960 },     // 4:3
     { 1280, 1024 },     // 5:4

     { 1440,  810 },     // 16:9
     { 1440, 1080 },     // 4:3

     { 1600,  900 },     // 16:9, obviously :)
     { 1600, 1200 },     // 4:3

     { 1920, 1080 },     // 16:9
     { 1920, 1200 },     // 16:10

     { 0, 0 }
};
*/
/**********************************************************************************************************************

static FusionCallHandlerResult call_handler( int           caller,
                                             int           call_arg,
                                             void         *call_ptr,
                                             void         *ctx,
                                             unsigned int  serial,
                                             int          *ret_val );

**********************************************************************************************************************/
/* FusionCallHandlerResult*/
int call_handler(/* int           caller,*/
              int           call_arg,
              void         *call_ptr,
              void         *ctx,
              int          *ret_val );

static int
InitLocal( DFBX11 *x11, DFBX11Shared *shared )
{
     int i, n;

     XInitThreads();
	//printf("share =%p\n",shared);
//     x11->shared = shared;
  //   x11->core   = core;

     x11->display = XOpenDisplay(getenv("DISPLAY"));
     if (!x11->display) {
          printf("X11: Error in XOpenDisplay for '%s'\n", getenv("DISPLAY"));
          return -1;
     }
	DebugLog("x11->display = %p",x11->display);
#if 0
typedef struct {
    XExtData *ext_data;             /* hook for extension to hang data */
    struct _XDisplay *display;      /* back pointer to display structure */
    Window root;                    /* root window ID */
    int width, height;              /* width and height of screen */
    int mwidth, mheight;            /* width and height of in millimeters */
    int ndepths;                    /* number of depths possible */
    Depth *depths;                  /* list of allowable depths on the screen */
    int root_depth;                 /* bits per pixel */
    Visual *root_visual;            /* root visual */
    GC default_gc;                  /* GC for the root root visual */
    Colormap cmap;                  /* default colormap */
    unsigned long white_pixel;
    unsigned long black_pixel;      /* white and black pixel values */
    int max_maps, min_maps;         /* max and min colormaps */
    int backing_store;              /* Never, WhenMapped, Always */
    Bool save_unders;
    long root_input_mask;           /* initial root input mask */
} Screen;

#endif
     x11->screenptr = DefaultScreenOfDisplay(x11->display);
     x11->screennum = DefaultScreen(x11->display);
	DebugLog("x11->screenptr = %p, x11->screennum= %d",x11->screenptr,x11->screennum);
	DebugLog("x11->screenptr->ndepths = %d",x11->screenptr->ndepths);
	DebugLog("screen info:");
	DebugLog("display = %p",x11->screenptr->display);
	DebugLog("Window root = %d",x11->screenptr->root);
	DebugLog("width = %d",x11->screenptr->width);
	DebugLog("height = %d",x11->screenptr->height);
	DebugLog("ndepths = %p",x11->screenptr->ndepths);
	DebugLog("white_pixel = %x",x11->screenptr->white_pixel);
	DebugLog("black_pixel = %x",x11->screenptr->black_pixel);
     for (i=0; i< x11->screenptr->ndepths; i++) {
#if 0
Depth

Depth defines a valid depth and list of associated visuals. A list of these structures is contained in the Screen structure, which is itself a member of the Display structure. Not used directly in any Xlib function. This structure should not be accessed directly, but instead through XGetVisualInfo and XMatchVisualInfo.

typedef struct {
    int depth;        /* this depth (Z) of the depth */
    int nvisuals;     /* number of Visual types at this depth */
    Visual *visuals;  /* list of visuals possible at this depth */
} Depth;

Visual

Visual describes a way of using color resources on a particular screen. A pointer to a visual structure is an argument to XCreateColormap, XCreateImage, and XCreateWindow. The valid visual structures for a screen can be determined with XGetVisualInfo or XMatchVisualInfo, or with the Defaultvisual (screen) macro. The visual used to create a window is returned by XGetwindowAttributes.

typedef struct {
    XExtData *ext_data;          /* hook for extension to hang data */
    VisualID visualid;           /* visual ID of this visual */
    int class;                   /* class of screen (Pseudocolor, etc.) */
    unsigned long red_mask;      /* TrueColor, DirectColor only */
    unsigned long green_mask;    /* TrueColor, DirectColor only */
    unsigned long blue_mask;     /* TrueColor, DirectColor only */
    int bits_per_rgb;            /* log base 2 of distinct color values */
    int map_entries;             /* number of colormap entries */
} Visual;




#endif

          const Depth *depth = &x11->screenptr->depths[i];
		  DebugLog("depth[%d]->nvisuals = %d",i,depth->nvisuals);

          for (n=0; n< depth->nvisuals; n++) {
               Visual *visual = &depth->visuals[n];

               printf( "InitLocal:X11/Visual: [%2d] ID 0x%02lx, depth %d, red 0x%06lx, green 0x%06lx, blue 0x%06lx, %d bits/rgb, %d entries\n",
                        n, visual->visualid, depth->depth,
                        visual->red_mask, visual->green_mask, visual->blue_mask,
                        visual->bits_per_rgb, visual->map_entries );

               switch (depth->depth) {
                    case 32:
                         if (visual->red_mask   == 0xff0000 &&
                             visual->green_mask == 0x00ff00 &&
                             visual->blue_mask  == 0x0000ff &&
                             !x11->visuals[0])
                              x11->visuals[0] = visual;
							 DebugLog("save depth 32 to x11->visuals[0] = %p",x11->visuals[0]);
                         break;
                    case 24:
                         if (visual->red_mask   == 0xff0000 &&
                             visual->green_mask == 0x00ff00 &&
                             visual->blue_mask  == 0x0000ff &&
                             !x11->visuals[1]){
								  x11->visuals[1] = visual;
								  DebugLog("save depth 24 to x11->visuals[1] = %p",x11->visuals[1]);
						 }
                         break;

/*
                    case 16:
                         if (visual->red_mask   == 0xf800 &&
                             visual->green_mask == 0x07e0 &&
                             visual->blue_mask  == 0x001f &&
                             !x11->visuals[DFB_PIXELFORMAT_INDEX(DSPF_RGB16)])
                              x11->visuals[DFB_PIXELFORMAT_INDEX(DSPF_RGB16)] = visual;
                         break;

                    case 15:
                         if (visual->red_mask   == 0x7c00 &&
                             visual->green_mask == 0x03e0 &&
                             visual->blue_mask  == 0x001f &&
                             !x11->visuals[DFB_PIXELFORMAT_INDEX(DSPF_RGB555)])
                              x11->visuals[DFB_PIXELFORMAT_INDEX(DSPF_RGB555)] = visual;
                         break;*/
               }
          }
     }

     if (XShmQueryExtension( x11->display ))
	{
		DebugLog("x11->use_shm = %d",x11->use_shm);
          XShmQueryVersion( x11->display, &x11->xshm_major, &x11->xshm_minor, &x11->use_shm );
		DebugLog("x11->use_shm = %d",x11->use_shm);
		//DebugLog("force to set not use_shm");
		//x11->use_shm = 0;
	}

     //x11->screen = dfb_screens_register( NULL, x11, &x11PrimaryScreenFuncs );

     //dfb_layers_register( x11->screen, x11, &x11PrimaryLayerFuncs );

     return 0;
}

/**********************************************************************************************************************/
#if 0
static void
system_get_info( /*CoreSystemInfo *info*/ )
{
	/*
     info->type = CORE_X11;   
     info->caps = CSCAPS_ACCELERATION;

     snprintf( info->name, DFB_CORE_SYSTEM_INFO_NAME_LENGTH, "X11" );
	 */
}
#endif
extern DFBX11Shared gShared;

int system_initialize(  void **data )
{
     int     ret;
     DFBX11       *x11;
     DFBX11Shared *shared;
     x11 = malloc( sizeof(DFBX11) );
     if (!x11)
          return -1;
/*
     shared = SHCALLOC( dfb_core_shmpool( core ), 1, sizeof(DFBX11Shared) );
     if (!shared) {
          D_FREE( x11 );
          return D_OOSHM();
     }
*/
	 TraceLog("enter");

     /*
      * Local init (master and slave)
      */
     ret = InitLocal( x11, shared );
	TraceLog("InitLocal ret = %d",ret);
     if (ret) {
          //SHFREE( dfb_core_shmpool( core ), shared );
		 ErrLog("InitLocal return err");
          free( x11 );
          return ret;
     }

	TraceLog("in");
     /*
      * Shared init (master only)
      */
//     shared->data_shmpool = dfb_core_shmpool_data( core );

 //    shared->screen_size.w = x11->screenptr->width;
   //  shared->screen_size.h = x11->screenptr->height;

//     fusion_skirmish_init( &shared->lock, "X11 System", dfb_core_world(core) );

  //   fusion_call_init( &shared->call, call_handler, x11, dfb_core_world(core) );


     /*
      * Must be set before initializing the pools!
      */
     *data = x11;
	 gpX11 = x11;
	 gpX11->shared = &gShared;
call_handler(/* int           caller,*/
              X11_CREATE_WINDOW,
              NULL,
              gpX11,
 /*             unsigned int  serial, */
              &ret );
 call_handler( X11_UPDATE_SCREEN, NULL,gpX11,&ret );

     /*
      * Master init
      */
    // dfb_surface_pool_initialize( core, &x11SurfacePoolFuncs, &shared->x11image_pool );
/*
#ifdef USE_GLX
     dfb_surface_pool_initialize( core, &glxSurfacePoolFuncs, &shared->glx_pool );
#endif

     if (dfb_config->video_length) {
          shared->vpsmem_length = dfb_config->video_length;

          dfb_surface_pool_initialize( core, &vpsmemSurfacePoolFuncs, &shared->vpsmem_pool );
     }

#ifdef USE_GLX
     dfb_surface_pool_bridge_initialize( core, &x11SurfacePoolBridgeFuncs, x11, &shared->x11_pool_bridge );
#endif

     fusion_arena_add_shared_field( dfb_core_arena( core ), "x11", shared );
*/
     return 0;
}
#if 0
static int
system_join(  void **data )
{
     int     ret;
     void         *ptr;
     DFBX11       *x11;
     DFBX11Shared *shared;

     x11 = malloc( sizeof(DFBX11) );
     if (!x11)
          return -1;

//     fusion_arena_get_shared_field( dfb_core_arena( core ), "x11", &ptr );
     shared = ptr;


     /*
      * Local init (master and slave)
      */
     ret = InitLocal( x11, shared );
     if (ret) {
          free( x11 );
          return ret;
     }


     /*
      * Must be set before joining the pools!
      */
     *data = x11;


     /*
      * Slave init
      */
  /*   if (shared->x11image_pool)
          dfb_surface_pool_join( core, shared->x11image_pool, &x11SurfacePoolFuncs );

#ifdef USE_GLX
     if (shared->glx_pool)
          dfb_surface_pool_join( core, shared->glx_pool, &glxSurfacePoolFuncs );
#endif

     if (shared->vpsmem_pool)
          dfb_surface_pool_join( core, shared->vpsmem_pool, &vpsmemSurfacePoolFuncs );

#ifdef USE_GLX
     if (shared->x11_pool_bridge)
          dfb_surface_pool_bridge_join( core, shared->x11_pool_bridge, &x11SurfacePoolBridgeFuncs, x11 );
#endif
*/
     return 0;
}

#endif
#if 0
static int
system_shutdown( int emergency )
{
   //  DFBX11       *x11    = dfb_system_data();
     //DFBX11Shared *shared = x11->shared;
	printf("emergency = %d\n",emergency);
     /*
      * Master deinit
      */
  /*   if (shared->x11_pool_bridge)
          dfb_surface_pool_bridge_destroy( shared->x11_pool_bridge );

     if (shared->vpsmem_pool)
          dfb_surface_pool_destroy( shared->vpsmem_pool );

     if (shared->glx_pool)
          dfb_surface_pool_destroy( shared->glx_pool );

     if (shared->x11image_pool)
          dfb_surface_pool_destroy( shared->x11image_pool );

*/
     /*
      * Shared deinit (master only)
      */
  /*
	 fusion_call_destroy( &shared->call );

     fusion_skirmish_prevail( &shared->lock );

     if (shared->xw)
         dfb_x11_close_window( x11, shared->xw );

     fusion_skirmish_destroy( &shared->lock );


     SHFREE( dfb_core_shmpool( x11->core ), shared );
*/

     /*
      * Local deinit (master and slave)
      */
   //  if (x11->display)
 //        XCloseDisplay( x11->display );

//     free( x11 );

     return 0;
}
#endif
#if 0
static int
system_leave( bool emergency )
{
     DFBX11       *x11    = dfb_system_data();
     DFBX11Shared *shared = x11->shared;

     /*
      * Slave deinit
      */
     if (shared->x11_pool_bridge)
          dfb_surface_pool_bridge_leave( shared->x11_pool_bridge );

     if (shared->vpsmem_pool)
          dfb_surface_pool_leave( shared->vpsmem_pool );

     if (shared->glx_pool)
          dfb_surface_pool_leave( shared->glx_pool );

     if (shared->x11image_pool)
          dfb_surface_pool_leave( shared->x11image_pool );


     /*
      * Local deinit (master and slave)
      */
     if (x11->display)
         XCloseDisplay( x11->display );

     D_FREE( x11 );

     return 0;
}
#endif
/*
static int
system_suspend()
{
     return DFB_UNIMPLEMENTED;
}

static int
system_resume()
{
     return DFB_UNIMPLEMENTED;
}

static volatile void *
system_map_mmio( unsigned int    offset,
                 int             length )
{
     return NULL;
}

static void
system_unmap_mmio( volatile void  *addr,
                   int             length )
{
}

static int
system_get_accelerator()
{
     return dfb_config->accelerator;
}

static VideoMode *
system_get_modes()
{
     return modes;
}

static VideoMode *
system_get_current_mode()
{
     return &modes[0];   
}

static int
system_thread_init()
{
     return 0;
}

static bool
system_input_filter( CoreInputDevice *device,
                     DFBInputEvent   *event )
{
     return false;
}

static unsigned long
system_video_memory_physical( unsigned int offset )
{
     return 0;
}

static void *
system_video_memory_virtual( unsigned int offset )
{
     return NULL;
}

static unsigned int
system_videoram_length()
{
     return 0;
}

static unsigned long
system_aux_memory_physical( unsigned int offset )
{
     return 0;
}

static void *
system_aux_memory_virtual( unsigned int offset )
{
     return NULL;
}

static unsigned int
system_auxram_length()
{
     return 0;
}

static void
system_get_busid( int *ret_bus, int *ret_dev, int *ret_func )
{
}

static void
system_get_deviceid( unsigned int *ret_vendor_id,
                     unsigned int *ret_device_id )
{
}
*/
/* FusionCallHandlerResult*/
int call_handler(/* int           caller,*/
              int           call_arg,
              void         *call_ptr,
              void         *ctx,
 /*             unsigned int  serial, */
              int          *ret_val )
{
     DFBX11 *x11 = ctx;

     switch (call_arg) {
          case X11_CREATE_WINDOW:
               *ret_val = dfb_x11_create_window_handler( x11 );
               break;

          case X11_DESTROY_WINDOW:
               *ret_val = dfb_x11_destroy_window_handler( x11 );
               break;

          case X11_UPDATE_SCREEN:
               *ret_val = dfb_x11_update_screen_handler( x11, call_ptr );
               break;

          case X11_SET_PALETTE:
               *ret_val = dfb_x11_set_palette_handler( x11 );
               break;

          case X11_IMAGE_INIT:
               *ret_val = dfb_x11_image_init_handler( x11, call_ptr );
               break;

          case X11_IMAGE_DESTROY:
               *ret_val = dfb_x11_image_destroy_handler( x11, call_ptr );
               break;

          default:
               printf( "unknown call" );
               *ret_val = -1;
               break;
     }

     return 0;
}

