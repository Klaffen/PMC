#ifndef DAT220_PROJECT_PACKETTYPES_H
#define DAT220_PROJECT_PACKETTYPES_H

// Host → All players: authoritative results
#define FUNCTION_MOVE        1001
#define FUNCTION_SHOOT       1002
#define FUNCTION_HURT        1003
#define FUNCTION_WEAPONSWAP  1004
#define FUNCTION_ADD_UNIT    1005
#define FUNCTION_END_OF_TURN 1010
#define FUNCTION_YOUR_TURN   1011
#define FUNCTION_END_OF_GAME 1020

// Client → Host: action requests (host validates, executes, then broadcasts results)
#define REQUEST_MOVE         2001
#define REQUEST_SHOOT        2002
#define REQUEST_WEAPONSWAP   2004

#endif
