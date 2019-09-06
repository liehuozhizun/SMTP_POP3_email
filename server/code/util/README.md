# Basic Layer: Utility
## Module Description
* Files in this folder provide *Global Macros* and *Helper function* which should be used in many situations.

### emailError.h    // provide Global Macros
```sh
/* IO Error */
OPEN_FAILED     101
REMOVE_FAILED   102
SEEK_FAILED     103
READ_FAILED     104
WRITE_FAILED    105

/* Heap Error */
MALLOC_FAILED   201
```

### systemLog.h     // write current system status to log
```sh
RC CheckLogDir();  // check whether log directory
RC CreateLogDir(); // create log directory
RC Logger();       // ostream log to log file.
```

### time.h          // get current time, data.
```sh
string GetCurrentTime(); // get current Y-M-D-T
string GetCurrentData(); // get current Y-M-D
```

## Author(s)
**Hang Yuan** (hyuan211@gmail.com)
**Yujia Li** (liyj070707@gmail.com)

## Tester(s)


## Major Progress Update  
* Start coding               - 8/10/19  
* Add systemLog.h to Utility - 8/11/19
* Add time.h to Utility      - 8/11/19
