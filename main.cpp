#include <stdio.h>
#include "BmpDecoder.h"
#include "BmpEncoder.h"

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Usage: main <src.bmp> <dst.bmp>\n");
		return 0;
	}

	unsigned char *bitmap = OpenImage(argv[1]);
	int size;
	bitmap = Encoder(bitmap, rows , cols, size);	
	Write(argv[2], bitmap, size);
	return 0;
}