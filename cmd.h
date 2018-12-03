#ifndef _CMD_H_
#define _CMD_H_

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef char s8;
typedef short s16;
typedef int s32;

typedef int(*CMD_ACTION)(const u8 *cmdString);

typedef struct {
    unsigned char cmd[50];
    CMD_ACTION action;
} CMD_MAP;

int cmd_inputHandle(u8 *cmd);

#endif //! _CMD_H_
