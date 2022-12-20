


/*********************************************
 *															* 															
 *	Generic includes		 						   * 
 *															* 															
 *********************************************/

#include "machine.h"	// contains PSX, PC, N64

#ifdef PSX
#include "defs_gen.h"
#include "lib_psx.h"	// set to libn64.h , libpc.h or libpsx.h
#include "defs_psx.h"
#include "..\..\global\striker.h"	 // share structure for all formats
#include	"structs.h"
#include	"main.h"
#include	"general.h"
#include "sound.h"
#include "text.h"
#endif

#ifdef PC
#include "john.h"
#endif

