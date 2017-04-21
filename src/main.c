#include <stdio.h>
#include <stdlib.h>
#include "primary.h"
#include "xwindow.h"
#include "x11.h"
#include "dlog.h"
extern int system_initialize(  void **data );
extern int call_handler(/* int           caller,*/
              int           call_arg,
              void         *call_ptr,
              void         *ctx,
              int          *ret_val );
extern DFBX11       *gpX11;
int main(int argc, char *argv[])
{
	int ret;
	char *p;
	TraceLog("enter");
	ret = 	system_initialize(&p);
	DebugLog("system_initialize ret = %d",ret);
//	char c = getchar();
	sleep(6);	
		 call_handler(X11_DESTROY_WINDOW, NULL,gpX11,&ret );
	return 0;
}
