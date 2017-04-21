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

#ifndef __X11SYSTEM__X11_H__
#define __X11SYSTEM__X11_H__

#include "x11image.h"
#include "xwindow.h"

typedef struct {
     //CoreLayerRegionConfig  config;
	 int config;
} SetModeData;

typedef struct {
     //DFBRegion              region;
	int			region;
//     CoreSurfaceBufferLock *lock;
	int *lock;
} UpdateScreenData;


typedef struct {
     UpdateScreenData     update;
     SetModeData          setmode;

     unsigned int         vpsmem_length;
 

//     CoreSurface         *primary;

     XWindow             *xw;
} DFBX11Shared;

struct __DFB_X11 {
     Bool                 use_shm;
     int                  xshm_major;
     int                  xshm_minor;
                         
     Display             *display;
     Screen              *screenptr;
     int                  screennum;
	 DFBX11Shared         *shared;                     
     Visual              *visuals[30];
};

typedef enum {
     X11_CREATE_WINDOW,
     X11_UPDATE_SCREEN,
     X11_SET_PALETTE,
     X11_IMAGE_INIT,
     X11_IMAGE_DESTROY,
     X11_DESTROY_WINDOW,
} DFBX11Call;

extern DFBX11Shared gShared;
extern DFBX11       *gpX11;


int dfb_x11_create_window_handler ( DFBX11 *x11 );
int dfb_x11_destroy_window_handler( DFBX11 *x11 );

int dfb_x11_update_screen_handler ( DFBX11 *x11, UpdateScreenData *data );
int dfb_x11_set_palette_handler   ( DFBX11 *x11 );

int dfb_x11_image_init_handler    ( DFBX11 *x11, x11Image *image );
int dfb_x11_image_destroy_handler ( DFBX11 *x11, x11Image *image );


#endif //__X11SYSTEM__X11_H__

