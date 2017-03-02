
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// static definition
static char *gBuffer;
static int gBuflen;

#define BUF_LEN_TARGET_WORD 0x80

static char * szDictPathName = "/usr/share/dict/words";
static FILE * fpDictHandle = 0;
static char gbufWord [BUF_LEN_TARGET_WORD];

void dictFileInit ()
{
    fpDictHandle = fopen(szDictPathName, "r");  
    if (fpDictHandle == 0) {
        
        printf ("\nFatal error: can't find the target file (%s), So EXIT now ! \n", szDictPathName);
        exit (-1);
    } else {
        printf ("\n Open find the target file (%s) successfully \n", szDictPathName);
    }
}

void dictFileClose()
{
    if (fpDictHandle != 0) {
        printf ("\n Close the target file (%s) \n", szDictPathName);
        fclose (fpDictHandle);
    }
}


char * findNextWord ()
{
    char * firstChar;
    if (fpDictHandle != 0) {
        firstChar = fgets(gbufWord, BUF_LEN_TARGET_WORD, fpDictHandle);
        if (!firstChar)
            //fputs(gbufWord, stdout);
        //else 
            return (char*)0;
    }

    return gbufWord;
}

char * matchTargetFromCurrentPos (char *szTarget, int nCurPos)
{
    char * firstChar;
    int nLen ;

    if (fpDictHandle == 0) 
        return 0;

    fseek (fpDictHandle, nCurPos, SEEK_SET);

    //printf ("target %s \n", szTarget);

    do {
        if ((firstChar = findNextWord ()) == (char*)0)
            break;
        
        nLen = strlen(firstChar);
        *(firstChar + nLen - 1) = '\0'; // remove "\n" need to be optimise

        //printf ("Word %s \n", firstChar);

        if (strcmp (firstChar, szTarget) == 0) {
            //printf ("Bingo:-> %s \n", firstChar);
            return firstChar;
        }

    } while (firstChar);

    return 0;
}

int getCurrentOperationPos ()
{
    return ftell (fpDictHandle);
}

void resetDictOperationPos ()
{
    fseek (fpDictHandle, 0, SEEK_SET);
}


