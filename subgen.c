
#include "subgen.h"

bool subgen(unsigned int last, unsigned int targ, char** words,int* words_used){
	// referenced from Hacking: The Art Of Exploitation
	
	unsigned int t[4], l[4];
	bool success;
	unsigned int try, single, carry = 0;
	int len, a, i, j, k, m, z, flag = 0;
	unsigned char mem[70];
	memset(mem,0,sizeof(mem));
	strcpy(mem,CHR);
	len = strlen(mem);
	t[3] = (targ >> 24) & 0xff;
	t[2] = (targ >> 16) & 0xff;
	t[1] = (targ >> 8) & 0xff;
	t[0] = targ & 0xff;
	l[3] = (last >> 24) & 0xff;
	l[2] = (last >> 16) & 0xff;
	l[1] = (last >> 8) & 0xff;
	l[0] = last & 0xff;
	for (a = 1; a < 5; a++){
		if (flag == 4)
			break;
		carry = flag = 0;
		for (z = 0; z < 4; z++){
			for (i = 0; i < len; i++){
				for (j = 0; j < len; j++){
					for (k = 0; k < len; k++){
						for (m = 0; m < len; m++){
							// try with minimum amount of values
							if (a < 2) j = len+1; 
							if (a < 3) k = len+1;
							if (a < 4) m = len+1;
							try = t[z] + carry + mem[i] + mem[j] + mem[k] + mem[m];
							single = try & 0xff;
							if (single == l[z]){
								carry = (try >> 8) & 0xff;
								if (i < len) word[0][z] = mem[i];
								if (j < len) word[1][z] = mem[j];
								if (k < len) word[2][z] = mem[k];
								if (m < len) word[3][z] = mem[m];
								i = j = k = m = len+1; // break
								flag++;
							}
						}
					}
				}
			}
		}
	}
	success = flag == 4;
	if (success){
		while (*((unsigned int *)words[a]) == 0 ){
			a--; 
		}
		*words_used = a;
	}
	return success;
}