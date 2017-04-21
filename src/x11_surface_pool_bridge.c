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

//#define DIRECT_ENABLE_DEBUG
#if 0
#include <config.h>

#include <direct/debug.h>
#include <direct/hash.h>
#include <direct/mem.h>

#include <fusion/shmalloc.h>

#include <core/surface_pool_bridge.h>
#endif
#include "x11.h"
#include "x11image.h"
#include "glx_surface_pool.h"
#include "x11_surface_pool.h"


/**********************************************************************************************************************/

typedef struct {
} x11PoolBridgeData;

typedef struct {
     DFBX11         *x11;
     Display        *display;
} x11PoolBridgeLocalData;

typedef struct {
} x11PoolTransferData;

/**********************************************************************************************************************/

static int
x11PoolBridgeDataSize()
{
#if 0
     return sizeof(x11PoolBridgeData);
#endif
}

static int
x11PoolBridgeLocalDataSize()
{
#if 0
     return sizeof(x11PoolBridgeLocalData);
#endif
}

static int
x11PoolTransferDataSize()
{
#if 0
     return sizeof(x11PoolTransferData);
#endif
}

static int
x11InitPoolBridge( /*CoreDFB  */void                        *core,
             /*      CoreSurfacePoolBridge */void           *bridge,
                   void                             *bridge_data,
                   void                             *bridge_local,
                   void                             *context,
              /*     CoreSurfacePoolBridgeDescription */void*ret_desc )
{
#if 0
     x11PoolBridgeLocalData *local = bridge_local;
     DFBX11                 *x11   = context;

     D_DEBUG_AT( X11_Bridge, "%s()\n", __FUNCTION__ );

     D_MAGIC_ASSERT( bridge, CoreSurfacePoolBridge );
     D_ASSERT( bridge_local != NULL );
     D_ASSERT( context != NULL );
     D_ASSERT( ret_desc != NULL );

     local->x11     = x11;
     local->display = x11->display;

     ret_desc->caps = CSPBCAPS_NONE;

     snprintf( ret_desc->name, DFB_SURFACE_POOL_BRIDGE_DESC_NAME_LENGTH, "X11 Pool Bridge" );

#endif
     return 0;
}

static int
x11JoinPoolBridge( /*CoreDFB                        */void  *core,
                   /*CoreSurfacePoolBridge          */void  *bridge,
                   void                             *bridge_data,
                   void                             *bridge_local,
                   void                             *context )
{
#if 0
     x11PoolBridgeLocalData *local = bridge_local;
     DFBX11                 *x11   = context;

     D_DEBUG_AT( X11_Bridge, "%s()\n", __FUNCTION__ );

     D_MAGIC_ASSERT( bridge, CoreSurfacePoolBridge );
     D_ASSERT( bridge_local != NULL );
     D_ASSERT( context != NULL );

     local->x11     = x11;
     local->display = x11->display;

#endif
     return 0;
}

static int
x11DestroyPoolBridge( /*CoreSurfacePoolBridge */void*bridge,
                      void                  *bridge_data,
                      void                  *bridge_local )
{
#if 0
     D_DEBUG_AT( X11_Bridge, "%s()\n", __FUNCTION__ );

     D_MAGIC_ASSERT( bridge, CoreSurfacePoolBridge );

#endif
     return 0;
}

static int
x11LeavePoolBridge( /*CoreSurfacePoolBridge */void*bridge,
                    void                  *bridge_data,
                    void                  *bridge_local )
{
#if 0
     D_DEBUG_AT( X11_Bridge, "%s()\n", __FUNCTION__ );

     D_MAGIC_ASSERT( bridge, CoreSurfacePoolBridge );

#endif
     return 0;
}

static int
x11CheckTransfer( /*CoreSurfacePoolBridge */void*bridge,
                  void                  *bridge_data,
                  void                  *bridge_local,
                  /*CoreSurfaceBuffer    */void *buffer,
                  /*CoreSurfaceAllocation */void*from,
                  /*CoreSurfaceAllocation */void*to )
{
#if 0
     x11PoolBridgeLocalData *local  = bridge_local;
     DFBX11                 *x11    = local->x11;
     DFBX11Shared           *shared = x11->shared;

     D_DEBUG_AT( X11_Bridge, "%s()\n", __FUNCTION__ );

     if (from->pool == shared->x11image_pool && to->pool == shared->glx_pool) {
          x11AllocationData *alloc = from->data;

          if (alloc->real) {
               /* ARGB does not work (BadMatch) */
               if (buffer->format == DSPF_RGB32)
                    return 0;
          }
     }
     else if (from->pool == shared->glx_pool && to->pool == shared->x11image_pool) {
          x11AllocationData *alloc = to->data;

          if (alloc->real) {
               /* ARGB does not work (BadMatch) */
               if (buffer->format == DSPF_RGB32)
                    return 0;
          }
     }

#endif
     return -1;// DFB_UNSUPPORTED;
}

static int
x11StartTransfer( /*CoreSurfacePoolBridge */void  *bridge,
                  void                    *bridge_data,
                  void                    *bridge_local,
                  /*CoreSurfacePoolTransfer */void*transfer,
                  void                    *transfer_data )
{
#if 0
     int               ret;
     int                     i;
     CoreSurfaceBufferLock   lock;
     LocalPixmap            *pixmap;
     x11PoolBridgeLocalData *local  = bridge_local;
     DFBX11                 *x11    = local->x11;
     DFBX11Shared           *shared = x11->shared;
     CoreSurfaceAllocation  *from   = transfer->from;
     CoreSurfaceAllocation  *to     = transfer->to;

     D_DEBUG_AT( X11_Bridge, "%s()\n", __FUNCTION__ );

     if (from->pool == shared->x11image_pool && to->pool == shared->glx_pool) {
          x11AllocationData *alloc = from->data;

          D_ASSERT( alloc->real );

          dfb_surface_buffer_lock_init( &lock, CSAID_ANY, CSAF_WRITE );

          ret = dfb_surface_pool_lock( to->pool, to, &lock );
          if (ret) {
               dfb_surface_buffer_lock_deinit( &lock );
               return ret;
          }

          pixmap = lock.handle;
          D_MAGIC_ASSERT( pixmap, LocalPixmap );


          XLockDisplay( local->display );

          for (i=0; i<transfer->num_rects; i++) {
               const DFBRectangle *rect = &transfer->rects[i];

               D_DEBUG_AT( X11_Bridge, "  -> XCopyArea( %4d,%4d-%4dx%4d )\n", rect->x, rect->y, rect->w, rect->h );

               XCopyArea( local->display, alloc->image.pixmap, pixmap->pixmap, pixmap->gc,
                          rect->x, rect->y, rect->w, rect->h, rect->x, rect->y );
          }

          XFlush( local->display );

          XUnlockDisplay( local->display );


          dfb_surface_pool_unlock( to->pool, to, &lock );

          dfb_surface_buffer_lock_deinit( &lock );

          return 0;
     }

     if (from->pool == shared->glx_pool && to->pool == shared->x11image_pool) {
          x11AllocationData *alloc = to->data;

          D_ASSERT( alloc->real );

          dfb_surface_buffer_lock_init( &lock, CSAID_ANY, CSAF_READ );

          ret = dfb_surface_pool_lock( from->pool, from, &lock );
          if (ret) {
               dfb_surface_buffer_lock_deinit( &lock );
               return ret;
          }

          pixmap = lock.handle;
          D_MAGIC_ASSERT( pixmap, LocalPixmap );


          XLockDisplay( local->display );

          glFinish();

          for (i=0; i<transfer->num_rects; i++) {
               const DFBRectangle *rect = &transfer->rects[i];

               D_DEBUG_AT( X11_Bridge, "  -> XCopyArea( %4d,%4d-%4dx%4d )\n", rect->x, rect->y, rect->w, rect->h );

               XCopyArea( local->display, pixmap->pixmap, alloc->image.pixmap, alloc->image.gc,
                          rect->x, rect->y, rect->w, rect->h, rect->x, rect->y );
          }

          XFlush( local->display );

          XUnlockDisplay( local->display );


          dfb_surface_pool_unlock( from->pool, from, &lock );

          dfb_surface_buffer_lock_deinit( &lock );

          return 0;
     }

#endif
     return -1;
}

static int
x11FinishTransfer( /*CoreSurfacePoolBridge */void  *bridge,
                   void                    *bridge_data,
                   void                    *bridge_local,
                   /*CoreSurfacePoolTransfer */void*transfer,
                   void                    *transfer_data )
{
#if 0
     x11PoolBridgeLocalData *local = bridge_local;

     D_DEBUG_AT( X11_Bridge, "%s()\n", __FUNCTION__ );

     XLockDisplay( local->display );

     XSync( local->display, False );

     XUnlockDisplay( local->display );

#endif
     return 0;
}

#if 0

const SurfacePoolBridgeFuncs x11SurfacePoolBridgeFuncs = {
     PoolBridgeDataSize:      x11PoolBridgeDataSize,
     PoolBridgeLocalDataSize: x11PoolBridgeLocalDataSize,
     PoolTransferDataSize:    x11PoolTransferDataSize,

     InitPoolBridge:          x11InitPoolBridge,
     JoinPoolBridge:          x11JoinPoolBridge,
     DestroyPoolBridge:       x11DestroyPoolBridge,
     LeavePoolBridge:         x11LeavePoolBridge,

     CheckTransfer:           x11CheckTransfer,

     StartTransfer:           x11StartTransfer,
     FinishTransfer:          x11FinishTransfer
};

#endif
