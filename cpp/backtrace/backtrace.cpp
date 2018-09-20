#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <execinfo.h>

#include "backtrace.h"

void showBacktrace(int level)
{

	int f = 0;

	for (int n=1; n<=level; n++) {
	
	        void * ret;
	        //void * ret = __builtin_return_address(1);
	
		switch (n) {
		case 1:
	        	ret = __builtin_return_address(1);
			break;		
		case 2:
	        	ret = __builtin_return_address(2);
			break;		
		case 3:
	        	ret = __builtin_return_address(3);
			break;		
		case 4:
	        	ret = __builtin_return_address(4);
			break;		
		case 5:
	        	ret = __builtin_return_address(5);
			break;		
		case 6:
	        	ret = __builtin_return_address(6);
			break;		
		case 7:
	        	ret = __builtin_return_address(7);
			break;		
		case 8:
	        	ret = __builtin_return_address(8);
			break;		
		case 9:
	        	ret = __builtin_return_address(9);
			break;		
		case 10:
	        	ret = __builtin_return_address(10);
			break;		
		case 11:
	        	ret = __builtin_return_address(11);
			break;		
		case 12:
	        	ret = __builtin_return_address(12);
			break;		
		case 13:
	        	ret = __builtin_return_address(13);
			break;		
		case 14:
	        	ret = __builtin_return_address(14);
			break;		
		case 15:
	        	ret = __builtin_return_address(15);
			break;		
		case 16:
	        	ret = __builtin_return_address(16);
			break;		
		default:
			return;
		}
	
	        //printf("ret address [%x]\n", ret);
	        void * caller = __builtin_frame_address(0);
	        //printf("call address [%x]\n", caller);

#ifdef __cplusplus
	        Dl_info dlinfo;
	        Dl_info dlinfo2;
	
	        void *ip = ret;
	        if(!dladdr(ip, &dlinfo)) {
	                perror("addr not found\n");
	                return;
	        }
	
	        const char *symname = dlinfo.dli_sname;
	        fprintf(stderr, "% 2d: %p %s+%u (%s)\n", ++f, ip, symname, 0,
	              dlinfo.dli_fname); // (unsigned)(ip - dlinfo.dli_saddr)
	
#endif
	}
	
}

