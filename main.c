
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEST_FILTER 1
#define TEST_FILTER0 0

#define LEN_GRID_ROW   4
#define LEN_GRID_COL   4

extern void dictFileInit ();
extern void dictFileClose();
extern void resetDictOperationPos ();
extern char * findNextWord ();
extern char * matchTargetFromCurrentPos (char *szTarget, int nCurPos);

static char gGridString [LEN_GRID_ROW * LEN_GRID_COL + 1];

void formatHeadShow (void)
{
    printf ("\n\n\n\n");
    printf ("This program is for test the searching and matching function in word list ! \n");
    printf ("Word list target directory : /usr/share/dict/word");
    printf ("\n\n\n\n");
}

void guideInput (void)
{
    printf ("\n");
    printf ("Please input 16 letter and press the Enter key \n");
    printf ("All legal letter will be taken to be built into 4X4 grid \n");
}

void filterInputAndBuildGrid (char * strInput)
{
    int i = 0, j = 0; 
    char * tmp;
    char * tmp1 = gGridString;
    
    if (strInput == 0)
        return ;
    if (strlen(strInput) == 0)
        return;

    tmp = strInput;
    while (*tmp != '\0') {
        *tmp1 ++ = *tmp;
        fputc (*tmp ++, stdout);
        fputc (' ', stdout);
        
        if (++ i >= LEN_GRID_COL) {
            printf ("\n");
            if (++ j >= LEN_GRID_COL) {
                printf ("\n");
                break;
            }
            i = 0;
        }
    }
}

int findAdjacentLetter (char cRow, char cCol, char * strNow, char * mapOccupancy, int nSearchPos)
{
    // check position if legal
    char tmpWord [LEN_GRID_ROW * LEN_GRID_COL + 1];
    char cTmpMapOccu [LEN_GRID_ROW * LEN_GRID_COL];
    int nLen, nTempSearchPos = nSearchPos;
    char tmpChar;
    char * foundWord ;
  
   
#if TEST_FILTER0    
    printf (" row (%d) col (%d) \n", cRow, cCol);
#endif

    if ((cRow < 0) || (cRow >= LEN_GRID_ROW) || (cCol < 0) || (cCol >= LEN_GRID_COL)) {
#if TEST_FILTER0 
        printf (" -1 \n");
#endif
        return -1;
    }

    if (mapOccupancy [cRow * LEN_GRID_COL + cCol] != '\0') {
#if TEST_FILTER0    
        printf (" -2 %c \n", mapOccupancy [cRow * LEN_GRID_COL + cCol]);
#endif
        return -2;
    }

    
    // append new letter
    strcpy (tmpWord, strNow);
    nLen = strlen(tmpWord); 
    tmpChar = gGridString [cRow * LEN_GRID_COL + cCol]; 

#if TEST_FILTER0
    //fputs (strNow, stdout);
    printf ("%s ", strNow);
    //fputs (tmpWord, stdout);
    printf ("%s \n", tmpWord);
    //printf ("\n");
    printf (" len %d c %c \n", nLen, tmpChar);
#endif
    tmpWord [nLen] = tmpChar;
    tmpWord [nLen + 1] = '\0';
#if TEST_FILTER0
    printf (" %s \n", tmpWord);
#endif

    memcpy (cTmpMapOccu, mapOccupancy, LEN_GRID_ROW * LEN_GRID_COL);
    cTmpMapOccu[cRow * LEN_GRID_COL + cCol] = '1';

#if TEST_FILTER0
    //printf ("now \n");
    fputs (tmpWord, stdout);
    printf ("\n");
#endif

    // match word in list

#if 1
    foundWord = matchTargetFromCurrentPos ((tmpWord), 0);
    if (foundWord) {
        printf ("Bingo:-> %s \n", foundWord);
    }
#endif

    findAdjacentLetter (cRow - 1, cCol - 1, tmpWord, cTmpMapOccu, nTempSearchPos);  	// 
    findAdjacentLetter (cRow - 1, cCol + 0, tmpWord, cTmpMapOccu, nTempSearchPos);  	//
    findAdjacentLetter (cRow - 1, cCol + 1, tmpWord, cTmpMapOccu, nTempSearchPos);  	//
    findAdjacentLetter (cRow + 0, cCol + 1, tmpWord, cTmpMapOccu, nTempSearchPos);  	//
    findAdjacentLetter (cRow + 1, cCol + 1, tmpWord, cTmpMapOccu, nTempSearchPos);  	// 
    findAdjacentLetter (cRow + 1, cCol + 0, tmpWord, cTmpMapOccu, nTempSearchPos);  	//
    findAdjacentLetter (cRow + 1, cCol - 1, tmpWord, cTmpMapOccu, nTempSearchPos);  	//
    findAdjacentLetter (cRow + 0, cCol - 1, tmpWord, cTmpMapOccu, nTempSearchPos);  	//
    
    //getchar();
    return 0;
}


int main(int argc, char** argv)
{
    static char value[256];
    char * tmpWord;
    int nFilePos = 0, i, j;

    

    char initWord[LEN_GRID_ROW * LEN_GRID_COL +1];
    char initMap[LEN_GRID_ROW * LEN_GRID_COL];
    memset (initWord, 0, LEN_GRID_ROW * LEN_GRID_COL +1);	// init all pos to '\0'
    memset (initMap, 0, LEN_GRID_ROW * LEN_GRID_COL);	// init all pos to 0 //0  no occupancy
                                                        // 1 = occupancy

    formatHeadShow ();
    guideInput ();

    // check the input valid and format the 4X4 grid
    fgets(value, 256, stdin);

    memset (gGridString, 0, LEN_GRID_ROW * LEN_GRID_COL + 1);

    filterInputAndBuildGrid (value);

    fputs(gGridString, stdout);

    dictFileInit ();
    getchar ();

    initWord [0] = '\0';
    initWord [1] = '\0';
    initMap [0] = '\0';
    resetDictOperationPos ();

    for (i = 0; i < LEN_GRID_ROW; i ++) {
        for (j = 0; j < LEN_GRID_COL; j ++) {
            findAdjacentLetter (i, j, initWord, initMap, nFilePos);
        }
    }
    

#if 0
    do {
        
        tmpWord = findNextWord ();

    } while (tmpWord);

#endif

    dictFileClose();

    getchar ();
    return 0;
}
