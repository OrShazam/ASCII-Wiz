

#include "utils.h"


char* mov_eax_ascii(char* buffer, unsigned int start, unsigned int end){
	char word[4][4]; int count;
	bool success;
	success = subgen(start, end, (char**)word, &count);
	if (!success){
		printf("Error in generating mov with sub\n");
		exit(-1);
	}
	for (int i = 0; i < count; i++){
		*buffer = SUB_EAX;
		buffer++;
		*((unsigned int*)buffer) = *((unsigned int*)word[i]);
		buffer += 4;
	}
	return buffer;
}

char* sub_esp_ascii(char* buffer, unsigned int offset){
	*buffer = PUSH_ESP;
	buffer++;
	*buffer = POP_EAX;
	buffer++;
	buffer = mov_eax_ascii(buffer, 0, offset);
	*buffer = PUSH_EAX;
	buffer++;
	*buffer = POP_ESP;
	buffer++;
	return buffer;
}

char* xor_eax_ascii(char* buffer){
	*buffer = AND_EAX;
	buffer++;
	*((unsigned int*)buffer) = AND_COMBO_1
	buffer += 4;
	*buffer = AND_EAX;
	buffer++;
	*((unsigned int*)buffer) = AND_COMBO_1
	buffer += 4;
	return buffer;
}

unsigned int calc_encoder_size(char* shell_end, unsigned int shell_size){
	// maximally we would need 21 bytes (4 subs + push eax) 
	// for each 4 instructions
	// + 8 bytes for sub esp + 10 bytes for xor eax
	// assuming shellcode is already padded
	unsigned int max_encoder_size = (shell_size) * (21 / 4) + 8 + 10;
	unsigned int start=0; unsigned int end;
	char* buffer = malloc(max_encoder_size);
	char* buffer_start = buffer;
	buffer += 8; // sub esp
	buffer += 10; // xor eax 
	shell_end -= 4;
	for (int i = 0; i < shell_size; i+=4){	
	    end = *((unsigned int*)shell_end);
		buffer = mov_eax_ascii(buffer,start,end);
		buffer++; // push eax
		start = end;
		shell_end -= 4;
	}
	max_encoder_size = buffer - buffer_start;
	free(buffer_start);
	if (max_encoder_size % 4 != 0){
		// encoder size should be aligned to avoid bugs
		max_encoder_size += 4 - max_encoder_size % 4;
	}
	return max_encoder_size;
}