#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int parity(unsigned long ino) {
	int noofones = 0;
	unsigned long mask = 0x00000001ul; // 1st bit
	while(mask != 0) {
		if(mask & ino) {
			noofones++;
		}
		mask = mask << 1;
	}
	return (noofones & 1);
}

int main(int argc, char **argv) {

	unsigned long long mask = 0x1ffff;
	unsigned long long code = 0x1de3c3901L;
	// this is even
	unsigned char high_parity = (code >> 32) & 1;
	// this is odd
	unsigned char low_parity = (code) & 1;
	unsigned long high = code >> 16; // top 17 bits;
	unsigned long low = code & mask;
	printf("High paritY: %d, Low parity: %d\n", high_parity, low_parity);
	printf("High: %d (%lu), low: %d (%lu)\n", parity(high), high, parity(low), low);


	return 0;
}

