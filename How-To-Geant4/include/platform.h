#if __unix__ || __APPLE__                              // for checking if the code shall be compiled on an UNIX system
#include <unistd.h>                       //To use getpid() to get the process ID to use as random seed on UNIX systems
#include <sys/types.h>                    //To use getpid() to get the process ID to use as random seed on UNIX systems
#define _getpid() getpid()
#endif
