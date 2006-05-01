
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LzmaUtilsCommon.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define BLOCK_SIZE 8192

FILE *inputFile;
FILE *outputFile;

/* This function is called by LzmaDecode() to read more compressed data. */
static int LzmaReadCompressedStream (__attribute__ ((unused)) void *object,
                                     const unsigned char **buffer, SizeT *size)
{
    static unsigned char inputBuffer[BLOCK_SIZE];
    *size = fread (inputBuffer, sizeof (inputBuffer[0]),
                   BLOCK_SIZE, inputFile);
    *buffer = inputBuffer;
    if (*size == 0)
        return (LZMA_RESULT_DATA_ERROR);
    return (LZMA_RESULT_OK);
}

int main(int argc, char **argv)
{

    outputFile = fopen("output.file", "wb");
    inputFile = fopen("foo.lz", "rb");

    puts("unlzma");
    int i; /* Counter for 'for' loop */
    unsigned char headerBytes[LZMA_HEADER_SIZE]; /* Header of LZMA file */
    CLzmaDecoderState state; /* Decoder data */
    UInt64 uncompressedSize = 0; /* Nice to start counting from zero :-) */
    SizeT decodedChunkSize = 1; /* Initialize to non-zero */
    int isStreamed = 1; /* Assume streamed LZMA file */
    unsigned char outputBuffer[BLOCK_SIZE];
    ILzmaInCallback Callback; /* Includes only a pointer to...*/
    Callback.Read = LzmaReadCompressedStream;  /* ...a function. */

    /* Show help if --help is the first command line argument. */
    if (argc > 1 && strcmp ("--help", argv[1]) == 0)
    {
        printf ("\nLZMAdec - a small LZMA decoder\n\n"
                "Reads inputFile and uncompresses to stdout.\n"
                "Returns 0 on success and 1 on error.\n\n"
                LZMA_SDK_COPYRIGHT "\n"
                "\nThis is free software "
                "licensed under the GNU LGPL.\n\n");
        return (0);
    }

    /* Read the LZMA header */
    if (fread (headerBytes, sizeof (headerBytes[0]),
               LZMA_HEADER_SIZE, inputFile) != LZMA_HEADER_SIZE)
    {
        puts("Error reading header");
        return (1); /* Input too short */
    }
    /* Decode properties (lc, lp, pb) */
    if (LzmaDecodeProperties (&state.Properties,
                              headerBytes, LZMA_PROPERTIES_SIZE) != LZMA_RESULT_OK)
    {
        puts("Invalid properties data");
        return (1); /* Input too short */
    } /* Invalid properties data */

    /* Calculate the decompressed size */
    for (i = 0; i < 8; i++)
    {
        uncompressedSize += (UInt64)(headerBytes[
                                         i + LZMA_PROPERTIES_SIZE]) << (i * 8);
        /* Check if it is not streamed */
        if (headerBytes[i + LZMA_PROPERTIES_SIZE] != 0xFF)
        {
            isStreamed = 0;
            puts("(not streamed)");
        }
    }

    /* There's no idea to try decoding empty content */
    if (uncompressedSize == 0)
    {
        puts("Empty compressed file");
        return (0);
    } /* Empty compressed file */

    /* Allocate memory */
    state.Probs = (CProb *)malloc(LzmaGetNumProbs(&state.Properties)
                                  * sizeof(CProb));
    state.Dictionary = (unsigned char *)
                       malloc(state.Properties.DictionarySize);
    if (state.Probs == 0 || state.Dictionary == 0)
        return (1); /* Unable to allocate memory */

    puts("init...");
    /* Initialize the decoder */
    LzmaDecoderInit (&state);

    /* Decoder loop terminates when
    - the amount of the decoded data matches the
      uncompressed size (non-streamed file)
    - call to the decoder returns no more data (streamed)
    - an error occurs */
    puts("decompress...");



    while (uncompressedSize != 0
            && decodedChunkSize != 0)
    {

        /* Call the decoding function from the LZMA SDK
        to do the real work for us. ;-) */
        if (LzmaDecode (&state, &Callback, outputBuffer,
                        MIN (BLOCK_SIZE,
                             uncompressedSize),
                        &decodedChunkSize)
                != 0)
            break; /* Decoding error */

        /* Write out the uncompressed data */
        if (fwrite (outputBuffer,
                    sizeof (outputBuffer[0]),
                    decodedChunkSize, outputFile)
                != decodedChunkSize)
            break; /* Write error */

        /* In case of a non-streamed LZMA file
        uncompressedSize is the amount of uncompressed
        data that has not been decoded yet. */
        if (!isStreamed)
            uncompressedSize -=
                decodedChunkSize;
    }

    return (0);
}
