#define SYSTEM_WORK	0x060FFC00
#define SYSTEM_SIZE	( 0x06100000 - 0x060FFC00 )

extern unsigned int _BSSBegin, _BSSEnd;

void main( void )
{
	unsigned char *pDestination;
	unsigned int Index;

	/* Zero the .bss */
	for( pDestination = ( unsigned char * )&_BSSBegin;
		pDestination < ( unsigned char * )&_BSSEnd; ++pDestination )
	{
		*pDestination = 0;
	}

	/* This is for clearing the SGL system variables */
	for( pDestination = ( unsigned char * )SYSTEM_WORK, Index = 0;
		Index < SYSTEM_SIZE; ++Index )
	{
		*pDestination = 0;
	}

	while( 1 )
	{
		/* Just keep going */
	}
}


