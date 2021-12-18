
#include "utils.h"
#include "common.h"

#define OFFSET_BUFFER 500 
char shellcode[]; 
unsigned int shell_size;
// change this for your use case 

int main(int argc, int argv){
	
	char* shellcode_padded;
	unsigned int shell_padded_size;
	if (shell_size % 4 != 0){
		shell_padded_size = shell_size + 4 - shell_size % 4;
		shellcode_padded = calloc(sizeof(char),shell_padded_size + 1);
		if (shellcode_padded == NULL){
			printf("malloc failed\n");
			return 1;
		}
		memset(shellcode_padded, CLD, shell_padded_size - shell_size);
		strcat(shellcode_padded,shellcode);
	}
	else {
		shellcode_padded = (char*)shellcode;
		shell_padded_size = shell_size;
	}
	
	char* shell_end = (char*)(shellcode_padded + shell_padded_size);
	unsigned int encoder_size = calc_encoder_size(
		shell_end, shell_padded_size);
	
	char* encoder_shell = malloc(encoder_size);
	if (encoder_shell == NULL){
		printf("malloc failed\n");
		return 1;
	}
	char* encoder_shell_start = encoder_shell;
	encoder_shell = add_esp_ascii(
		encoder_shell,OFFSET_BUFFER + encoder_size + shell_padded_size);
	
	encoder_shell = xor_eax_ascii(encoder_shell);
	unsigned int start = 0, end;
	shell_end -= 4;
	
	for (int i = 0; i < shell_size; i+=4){	
	    end = *((unsigned int*)shell_end);
		encoder_shell = mov_eax_ascii(encoder_shell,start,end);
		*encoder_shell = PUSH_EAX;
		encoder_shell++;
		start = end;
		shell_end -= 4;
	}
	while (encoder_shell - encoder_shell_start < encoder_size){
		// do encoder padding 
		// there isn't really a printable opcode that's semantically like NOP
		// so I'm using INC_EAX here, if you've found something - fork this
		*encoder_shell = INC_EAX;
		encoder_shell++;
	}
	
	printf("\n%s\n",encoder_shell_start);
	free(encoder_shell_start);
	return 0;
}
