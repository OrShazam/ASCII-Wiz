

#define CHR "%_1234567890abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ-"

#define CLD 0xFC 
// a more stealthy NOP

#define AND_EAX '%'
#define SUB_EAX '-'
#define PUSH_EAX 'P'
#define POP_EAX 'X'
#define INC_EAX '@'

#define PUSH_ESP 'T'
#define POP_ESP '\'

#define AND_COMBO_1 0x454e4f4a

#define AND_COMBO_2 0x3a313035

// 0x454e4f4a & 0x3a313035 = 0
// -> (eax & 0x454e4f4a) & (eax & 0x3a313035) = 0 