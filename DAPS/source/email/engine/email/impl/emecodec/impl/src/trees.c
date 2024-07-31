/*=================================================================================
====
	MODULE NAME : zip-trees.c
	MODULE NAME : zip

    GENERAL DESCRIPTION
        output deflated data using Huffman coding
        
    Copyright (C) 1995-2004 Mark Adler
    For conditions of distribution and use, see copyright notice in common.h
===================================================================================
====
Revision History
Modification Tracking
	Date		Author			Number		Description of changes
   ---------- -------------- --------- --------------------------------------
   2006-10-29   duhuanbiao       1               porting

===================================================================================
==*/


/*******************************************************************************
* Include File Section
*******************************************************************************/

#include "zip.h"


/*******************************************************************************
* Macro Define Section
*******************************************************************************/


/* Bit length codes must not exceed MAX_BL_BITS bits */
#define MAX_BL_BITS 7

/* end of block literal code */
#define END_BLOCK 256

/* repeat previous bit length 3-6 times (2 bits of repeat count) */
#define REP_3_6      16

/* repeat a zero length 3-10 times  (3 bits of repeat count) */
#define REPZ_3_10    17

/* repeat a zero length 11-138 times  (7 bits of repeat count) */
#define REPZ_11_138  18

/* Index within the aiHeap array of least frequent node in the Huffman tree */
#define SMALLEST 1

/* Number of bits used within bi_buf. */
#define BUF_SIZE (8 * 2 * sizeof(WE_INT8))

/* Send a code of the given tree. c and tree must not have side effects */
#define SET_CODE(s, c, tree) SET_BITS(s, tree[c].Code, tree[c].Len)

/* Output a short LSB first on the stream.
* IN assertion: there is enough room in pendingBuf.
*/
#define SET_DOUBLE(s, w) { \
    SET_BUF_BYTE(s, (WE_UINT8)((w) & 0xff)); \
    SET_BUF_BYTE(s, (WE_UINT8)((WE_UINT16)(w) >> 8)); \
}

/* Send a value on a given number of bits.
 * IN assertion: length <= 16 and value fits in length bits.
 */
#define SET_BITS(s, value, length) \
{ \
    WE_INT32 iLen = (WE_INT32)length; \
    if (s->iOutBufAvail > ((WE_INT32)BUF_SIZE - iLen)) \
    { \
    WE_UINT32 uiVal = (WE_UINT32)value;\
    s->uOutBufOffset |= (WE_UINT16)(uiVal << (WE_UINT32)s->iOutBufAvail); \
    SET_DOUBLE(s, s->uOutBufOffset); \
    s->uOutBufOffset = (WE_UINT16)(uiVal >> (BUF_SIZE - (WE_UINT32)s->iOutBufAvail)); \
    s->iOutBufAvail += (iLen - (WE_INT32)BUF_SIZE); \
    } \
    else \
    { \
    s->uOutBufOffset |= (WE_UINT16)(((WE_UINT32)value) << (WE_UINT32)s->iOutBufAvail); \
    s->iOutBufAvail += iLen; \
    } \
}
/* Remove the smallest element from the aiHeap and recreate the aiHeap with
 * one less element. Updates aiHeap and iHeapLen.
 */
#define PQREMOVE(s, tree, top) \
{\
    top = s->aiHeap[SMALLEST]; \
    s->aiHeap[SMALLEST] = s->aiHeap[s->iHeapLen--]; \
    Tree_HeapStore(s, tree, SMALLEST); \
}

/* Compares to subtrees, using the tree depth as tie breaker when
 * the subtrees have equal frequency. This minimizes the worst case length.
 */
#define ISSMALL(tree, n, m, depth) \
    (tree[n].Freq < tree[m].Freq || \
(tree[n].Freq == tree[m].Freq && depth[n] <= depth[m]))

/* limit its bound */
#define TREE_BOUND(v,m,n) ((v) = (v) < (m) ? (m) : ((v) > (n) ? (n) : (v)))

/*******************************************************************************
 * Prototype Declare Section
*******************************************************************************/
/* extra bits for each length code */
static const WE_INT32 s_iaOther_LBits[LENGTH_CODES] 
   = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

/* extra bits for each distance code */
static const WE_INT32 s_iaOther_DBits[D_CODES] 
   = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

/* extra bits for each bit length code */
static const WE_INT32 s_iaOther_BLBits[BL_CODES]
   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7};

/* The lengths of the bit length codes are sent in order 
 * of decreasing probability, to avoid transmitting the 
 * lengths for unused bit length codes.*/
static const WE_UINT8 s_iaBL_List[BL_CODES]
   = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};


static const WE_INT32 s_iaDist[D_CODES] = {
    0,     1,     2,     3,     4,     6,     8,    12,    16,    24,
   32,    48,    64,    96,   128,   192,   256,   384,   512,   768,
 1024,  1536,  2048,  3072,  4096,  6144,  8192, 12288, 16384, 24576
};

static const St_Tree_Data s_staLiteralTee[CODES_LENGTH + 2] = {
{{ 12},{  8}}, {{140},{  8}}, {{ 76},{  8}}, {{204},{  8}}, {{ 44},{  8}},
{{172},{  8}}, {{108},{  8}}, {{236},{  8}}, {{ 28},{  8}}, {{156},{  8}},
{{ 92},{  8}}, {{220},{  8}}, {{ 60},{  8}}, {{188},{  8}}, {{124},{  8}},
{{252},{  8}}, {{  2},{  8}}, {{130},{  8}}, {{ 66},{  8}}, {{194},{  8}},
{{ 34},{  8}}, {{162},{  8}}, {{ 98},{  8}}, {{226},{  8}}, {{ 18},{  8}},
{{146},{  8}}, {{ 82},{  8}}, {{210},{  8}}, {{ 50},{  8}}, {{178},{  8}},
{{114},{  8}}, {{242},{  8}}, {{ 10},{  8}}, {{138},{  8}}, {{ 74},{  8}},
{{202},{  8}}, {{ 42},{  8}}, {{170},{  8}}, {{106},{  8}}, {{234},{  8}},
{{ 26},{  8}}, {{154},{  8}}, {{ 90},{  8}}, {{218},{  8}}, {{ 58},{  8}},
{{186},{  8}}, {{122},{  8}}, {{250},{  8}}, {{  6},{  8}}, {{134},{  8}},
{{ 70},{  8}}, {{198},{  8}}, {{ 38},{  8}}, {{166},{  8}}, {{102},{  8}},
{{230},{  8}}, {{ 22},{  8}}, {{150},{  8}}, {{ 86},{  8}}, {{214},{  8}},
{{ 54},{  8}}, {{182},{  8}}, {{118},{  8}}, {{246},{  8}}, {{ 14},{  8}},
{{142},{  8}}, {{ 78},{  8}}, {{206},{  8}}, {{ 46},{  8}}, {{174},{  8}},
{{110},{  8}}, {{238},{  8}}, {{ 30},{  8}}, {{158},{  8}}, {{ 94},{  8}},
{{222},{  8}}, {{ 62},{  8}}, {{190},{  8}}, {{126},{  8}}, {{254},{  8}},
{{  1},{  8}}, {{129},{  8}}, {{ 65},{  8}}, {{193},{  8}}, {{ 33},{  8}},
{{161},{  8}}, {{ 97},{  8}}, {{225},{  8}}, {{ 17},{  8}}, {{145},{  8}},
{{ 81},{  8}}, {{209},{  8}}, {{ 49},{  8}}, {{177},{  8}}, {{113},{  8}},
{{241},{  8}}, {{  9},{  8}}, {{137},{  8}}, {{ 73},{  8}}, {{201},{  8}},
{{ 41},{  8}}, {{169},{  8}}, {{105},{  8}}, {{233},{  8}}, {{ 25},{  8}},
{{153},{  8}}, {{ 89},{  8}}, {{217},{  8}}, {{ 57},{  8}}, {{185},{  8}},
{{121},{  8}}, {{249},{  8}}, {{  5},{  8}}, {{133},{  8}}, {{ 69},{  8}},
{{197},{  8}}, {{ 37},{  8}}, {{165},{  8}}, {{101},{  8}}, {{229},{  8}},
{{ 21},{  8}}, {{149},{  8}}, {{ 85},{  8}}, {{213},{  8}}, {{ 53},{  8}},
{{181},{  8}}, {{117},{  8}}, {{245},{  8}}, {{ 13},{  8}}, {{141},{  8}},
{{ 77},{  8}}, {{205},{  8}}, {{ 45},{  8}}, {{173},{  8}}, {{109},{  8}},
{{237},{  8}}, {{ 29},{  8}}, {{157},{  8}}, {{ 93},{  8}}, {{221},{  8}},
{{ 61},{  8}}, {{189},{  8}}, {{125},{  8}}, {{253},{  8}}, {{ 19},{  9}},
{{275},{  9}}, {{147},{  9}}, {{403},{  9}}, {{ 83},{  9}}, {{339},{  9}},
{{211},{  9}}, {{467},{  9}}, {{ 51},{  9}}, {{307},{  9}}, {{179},{  9}},
{{435},{  9}}, {{115},{  9}}, {{371},{  9}}, {{243},{  9}}, {{499},{  9}},
{{ 11},{  9}}, {{267},{  9}}, {{139},{  9}}, {{395},{  9}}, {{ 75},{  9}},
{{331},{  9}}, {{203},{  9}}, {{459},{  9}}, {{ 43},{  9}}, {{299},{  9}},
{{171},{  9}}, {{427},{  9}}, {{107},{  9}}, {{363},{  9}}, {{235},{  9}},
{{491},{  9}}, {{ 27},{  9}}, {{283},{  9}}, {{155},{  9}}, {{411},{  9}},
{{ 91},{  9}}, {{347},{  9}}, {{219},{  9}}, {{475},{  9}}, {{ 59},{  9}},
{{315},{  9}}, {{187},{  9}}, {{443},{  9}}, {{123},{  9}}, {{379},{  9}},
{{251},{  9}}, {{507},{  9}}, {{  7},{  9}}, {{263},{  9}}, {{135},{  9}},
{{391},{  9}}, {{ 71},{  9}}, {{327},{  9}}, {{199},{  9}}, {{455},{  9}},
{{ 39},{  9}}, {{295},{  9}}, {{167},{  9}}, {{423},{  9}}, {{103},{  9}},
{{359},{  9}}, {{231},{  9}}, {{487},{  9}}, {{ 23},{  9}}, {{279},{  9}},
{{151},{  9}}, {{407},{  9}}, {{ 87},{  9}}, {{343},{  9}}, {{215},{  9}},
{{471},{  9}}, {{ 55},{  9}}, {{311},{  9}}, {{183},{  9}}, {{439},{  9}},
{{119},{  9}}, {{375},{  9}}, {{247},{  9}}, {{503},{  9}}, {{ 15},{  9}},
{{271},{  9}}, {{143},{  9}}, {{399},{  9}}, {{ 79},{  9}}, {{335},{  9}},
{{207},{  9}}, {{463},{  9}}, {{ 47},{  9}}, {{303},{  9}}, {{175},{  9}},
{{431},{  9}}, {{111},{  9}}, {{367},{  9}}, {{239},{  9}}, {{495},{  9}},
{{ 31},{  9}}, {{287},{  9}}, {{159},{  9}}, {{415},{  9}}, {{ 95},{  9}},
{{351},{  9}}, {{223},{  9}}, {{479},{  9}}, {{ 63},{  9}}, {{319},{  9}},
{{191},{  9}}, {{447},{  9}}, {{127},{  9}}, {{383},{  9}}, {{255},{  9}},
{{511},{  9}}, {{  0},{  7}}, {{ 64},{  7}}, {{ 32},{  7}}, {{ 96},{  7}},
{{ 16},{  7}}, {{ 80},{  7}}, {{ 48},{  7}}, {{112},{  7}}, {{  8},{  7}},
{{ 72},{  7}}, {{ 40},{  7}}, {{104},{  7}}, {{ 24},{  7}}, {{ 88},{  7}},
{{ 56},{  7}}, {{120},{  7}}, {{  4},{  7}}, {{ 68},{  7}}, {{ 36},{  7}},
{{100},{  7}}, {{ 20},{  7}}, {{ 84},{  7}}, {{ 52},{  7}}, {{116},{  7}},
{{  3},{  8}}, {{131},{  8}}, {{ 67},{  8}}, {{195},{  8}}, {{ 35},{  8}},
{{163},{  8}}, {{ 99},{  8}}, {{227},{  8}}
};


static const WE_INT32 s_iaLength[LENGTH_CODES] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 20, 24, 28, 32, 40, 48, 56,
64, 80, 96, 112, 128, 160, 192, 224, 0
};

const St_Tree_Data s_stDisanceTee[D_CODES] = {
{{ 0},{ 5}}, {{16},{ 5}}, {{ 8},{ 5}}, {{24},{ 5}}, {{ 4},{ 5}},
{{20},{ 5}}, {{12},{ 5}}, {{28},{ 5}}, {{ 2},{ 5}}, {{18},{ 5}},
{{10},{ 5}}, {{26},{ 5}}, {{ 6},{ 5}}, {{22},{ 5}}, {{14},{ 5}},
{{30},{ 5}}, {{ 1},{ 5}}, {{17},{ 5}}, {{ 9},{ 5}}, {{25},{ 5}},
{{ 5},{ 5}}, {{21},{ 5}}, {{13},{ 5}}, {{29},{ 5}}, {{ 3},{ 5}},
{{19},{ 5}}, {{11},{ 5}}, {{27},{ 5}}, {{ 7},{ 5}}, {{23},{ 5}}
};

const WE_UINT8 iaDistCode[512] = {
    0,  1,  2,  3,  4,  4,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  8,
    8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,  9, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  0,  0, 16, 17,
    18, 18, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22,
    23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29
};

const WE_UINT8 iaLength[256]= {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 12, 12,
    13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16,
    17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19,
    19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28
};

/*******************************************************************************
* Type Define Section
*******************************************************************************/
typedef struct s_Tree 
{
    WE_INT32     iExtraBase;          /* base index for extra_bits */
    WE_INT32     iMaxElems;               /* max number of elements in the tree */
    WE_INT32     iMaxLength;          /* max bit length for the codes */
}s_Tree;

typedef enum _E_TREE_METHOD
{
    E_TREE_BL,
    E_TREE_L,
    E_TREE_D
        
} E_TREE_METHOD;



/************************************************************************/
/* Function declare */
/************************************************************************/

static WE_VOID Mass_Init(P_St_ZIP_State pstState);
static WE_VOID Mass_Zipped(P_St_ZIP_State pstState, P_St_Tree_Data pstLiteralTree,P_St_Tree_Data pstDistanceTree);
static WE_VOID Mass_Copy(P_St_ZIP_State pstState, WE_INT8 *piInputBuf, WE_UINT32 iLen,WE_INT32 IHead);
static WE_VOID Tree_HeapStore(P_St_ZIP_State pstState,P_St_Tree_Data pstTreeData, WE_INT32 iNodeNum);
static WE_VOID Tree_CodeLenBulid(P_St_ZIP_State pstState, P_St_Tree_Define pstTreeDefine, E_TREE_METHOD eMethod);
static WE_VOID Tree_CodesGen(P_St_Tree_Data pstTreeData, WE_INT32 iMaxCode, WE_UINT16 *puBlCount);
static WE_VOID Hfm_Bulid(P_St_ZIP_State pstState, P_St_Tree_Define pstTreeDefine, E_TREE_METHOD eMethod);
static WE_VOID Hfm_Scan(P_St_ZIP_State pstState, P_St_Tree_Data pstTreeData, WE_INT32 iMaxCode);
static WE_VOID Hfm_Sended(P_St_ZIP_State pstState, P_St_Tree_Data pstTreeData, WE_INT32 iMaxCode);
static WE_VOID AllHfm_Sended(P_St_ZIP_State pstState, WE_INT32 iLCodes, WE_INT32 iDCodes,WE_INT32 iBLCodes);
static WE_VOID State_Setting(P_St_ZIP_State pstState);
static WE_VOID State_Ending(P_St_ZIP_State pstState);
static WE_VOID State_flush(P_St_ZIP_State pstState);
static WE_INT32  BlHfm_Build(P_St_ZIP_State pstState);
static WE_UINT32 Code_Reverse(WE_UINT32 iOrig, WE_INT32 iLen);


/************************************************************************/
/* Function prototype */
/************************************************************************/
/*=================================================================================
===
	FUNCTION: HFM_Init

	CREATE DATE: 2006-11-1
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Initialize the  data structures for a new raw stream.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:
		zip_reset

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_VOID HFM_Init(
              P_St_ZIP_State pstState
              )
{
    WE_VOID *pvMemTree = NULL;
	s_Tree astTreeTemp[3] = {
		{LITERALS+1, CODES_LENGTH, MAX_BITS},
		{0,          D_CODES,	   MAX_BITS},
        {0,			 BL_CODES,	   MAX_BL_BITS} };

    if (pstState == NULL)
    {
        return;
    }

	pvMemTree = TOALLOC(3, sizeof(s_Tree));

    if (NULL == pvMemTree)
    {
        return;
    }
    //DBGPRINTF("%d,   %d", 3 * sizeof(s_Tree), sizeof(astTreeTemp));
	pvMemTree = MEMCPY(pvMemTree, astTreeTemp, sizeof(astTreeTemp));


    TOFREE(pstState->stLDest.pstStatTree);

    pstState->stLDest.pstDynTree = pstState->astDynLTree;
    pstState->stLDest.pstStatTree = (s_Tree*)pvMemTree;

    pstState->stDDest.pstDynTree = pstState->astDynDTree;
    pstState->stDDest.pstStatTree = (s_Tree*)((WE_INT8*)pvMemTree + sizeof(s_Tree));

    pstState->stBLDest.pstDynTree = pstState->astBLTree;
    pstState->stBLDest.pstStatTree = (s_Tree*)((WE_INT8*)pvMemTree + 2 * sizeof(s_Tree));

    pstState->uOutBufOffset = 0;
    pstState->iOutBufAvail = 0;
    pstState->iLastEobLens = 8; /* enough lookahead for inflate */

    /* Initialize the first block of the first file: */
    Mass_Init(pstState);
}
/*=================================================================================
===
	FUNCTION: Mass_Init

	CREATE DATE: 2006-11-1
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Initialize a new block.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:
		zip_reset

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_VOID Mass_Init(
                      P_St_ZIP_State pstState
                      )
{
    WE_INT32 iNum = 0; /* literates over tree elements */
    
    if (pstState == NULL)
    {
        return;
    }
    /* Initialize the trees. */
    for (iNum = 0; iNum < CODES_LENGTH; iNum++) 
    {
        pstState->astDynLTree[iNum].Freq = 0;
    }
    for (iNum = 0; iNum < D_CODES; iNum++) 
    {
        pstState->astDynDTree[iNum].Freq = 0;
    }
    for (iNum = 0; iNum < BL_CODES; iNum++) 
    {
        pstState->astBLTree[iNum].Freq = 0;
    }
    
    pstState->astDynLTree[END_BLOCK].Freq = 1;
    pstState->ulOptBlockLens = pstState->ulStaBlockLens = 0L;
    pstState->uiMatchSize = 0;
    pstState->uiLLastIndex = 0;
}

/*=================================================================================
===
	FUNCTION: Tree_HeapStore

	CREATE DATE: 2006-11-1
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Restore the aiHeap property by moving down the tree starting at node iNodeNum,
	exchanging a node with the smallest of its two sons if necessary, stopping when 
	the aiHeap property is re-established (each father smaller than its two sons).
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:
		zip_reset

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_VOID Tree_HeapStore(
                           P_St_ZIP_State pstState,
                           P_St_Tree_Data pstTreeData, 
                           WE_INT32 iNodeNum
                           )
{
    WE_INT32 iValue = 0;
    WE_INT32 iHeapNum = (WE_INT32)(((WE_UINT32)iNodeNum) << 1);  /* left son of iNodeNum */
    
    if (pstState == NULL ||
        pstTreeData == NULL)
    {
        return;
    }
    TREE_BOUND(iNodeNum, 0, HEAP_MAXSIZE - 1);
    iValue = pstState->aiHeap[iNodeNum];
    
    while (iHeapNum <= pstState->iHeapLen) 
    {
        TREE_BOUND(iHeapNum, 0, HEAP_MAXSIZE - 2);
        /* Set iHeapNum to the smallest of the two sons: */
        if (iHeapNum < pstState->iHeapLen &&
            ISSMALL(pstTreeData, pstState->aiHeap[iHeapNum+1], pstState->aiHeap[iHeapNum], pstState->aucSubTreeDepth)) 
        {
            iHeapNum++;
        }
	
        /* Exit if iValue is smaller than both sons */
        if (ISSMALL(pstTreeData, iValue, pstState->aiHeap[iHeapNum], pstState->aucSubTreeDepth)) 
        {
            break;
        }
        
        /* Exchange iValue with the smallest son */
        TREE_BOUND(iNodeNum, 0, HEAP_MAXSIZE - 1);
        pstState->aiHeap[iNodeNum] = pstState->aiHeap[iHeapNum]; 
        iNodeNum = iHeapNum;
        
        /* And continue down the tree, setting iHeapNum to the left son of iNodeNum */
        iHeapNum = (WE_INT32)((WE_UINT32)iHeapNum << 1);
    }

    TREE_BOUND(iNodeNum, 0, HEAP_MAXSIZE - 1);
	pstState->aiHeap[iNodeNum] = iValue;

}


/*=================================================================================
===
	FUNCTION: Tree_CodeLenBulid

	CREATE DATE: 2006-11-1
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Compute the optimal bit lengths for a tree and update the total bit length
	for the current block.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:
		Hfm_Bulid

	IMPORTANT NOTES:
		IN assertion: the fields freq and dad are set, aiHeap[iHeapMax] and
	above are the tree nodes sorted by increasing frequency.
		OUT assertions: the field len is set to the optimal bit length, the
	array puBlCount contains the frequencies for each bit length.
	The length opt_len is updated; ulStaBlockLens is also updated if stree is
	not null.

===================================================================================
==*/
static WE_VOID Tree_CodeLenBulid(
                              P_St_ZIP_State pstState,
                              P_St_Tree_Define pstTreeDefine, 
                              E_TREE_METHOD eMethod
                              )
{
    P_St_Tree_Data pstTreeData = NULL;
    WE_INT32 iMaxCode = 0;
    WE_INT32 iBase = 0;
    WE_INT32 iMaxLength = 0;
    WE_INT32 iHeapIndex = 0;  /* heap index */
    WE_INT32 iNum1 = 0;       /* iterate over the tree elements */
    WE_INT32 iNum2 = 0;       /* iterate over the tree elements */
    WE_INT32 iBitLens = 0;    /* bit length */
    WE_INT32 iExBits = 0;     /* extra bits */
    WE_UINT16 uiFreq = 0U;    /* frequency */
    WE_INT32 iOverFlow = 0;   /* number of elements with bit length too large */
    
    if (pstState == NULL ||
        pstTreeDefine == NULL)
    {
        return;
    }
    
    pstTreeData = pstTreeDefine->pstDynTree;
    iMaxCode = pstTreeDefine->iMaxCode;
    iBase = pstTreeDefine->pstStatTree->iExtraBase;
    iMaxLength = pstTreeDefine->pstStatTree->iMaxLength;
    
    for (iBitLens = 0; iBitLens <= MAX_BITS; iBitLens++)
    {
        pstState->auBLCount[iBitLens] = 0;
    }
    
    /* In a first pass, compute the optimal bit lengths (which may
    * overflow in the case of the bit length tree).
    */
    pstTreeData[pstState->aiHeap[pstState->iHeapMax]].Len = 0; /* root of the heap */
    
    for (iHeapIndex = pstState->iHeapMax+1; iHeapIndex < HEAP_MAXSIZE; iHeapIndex++) 
    {
        iNum1 = pstState->aiHeap[iHeapIndex];
        iBitLens = pstTreeData[pstTreeData[iNum1].Dad].Len + 1;
        if (iBitLens > iMaxLength) 
        {
            iBitLens = iMaxLength;
            iOverFlow++;
        }
        pstTreeData[iNum1].Len = (WE_UINT16)iBitLens;
        /* We overwrite tree[iNum1].Dad which is no longer needed */
        
        if (iNum1 > iMaxCode) 
        {
            continue; /* not a leaf node */
        }
        
        pstState->auBLCount[iBitLens]++;
        iExBits = 0;
        uiFreq = pstTreeData[iNum1].Freq;        
        switch(eMethod) 
        {		
        case E_TREE_L:
            if (iNum1 >= iBase) 
            {
                iExBits = s_iaOther_LBits[iNum1-iBase];
            }
            pstState->ulStaBlockLens += (WE_ULONG)(uiFreq * ((WE_INT32)s_staLiteralTee[iNum1].Len + iExBits));
            break;
        case E_TREE_D:
            if (iNum1 >= iBase) 
            {
                iExBits = s_iaOther_DBits[iNum1-iBase];
            }
            pstState->ulStaBlockLens += (WE_ULONG)(uiFreq * ((WE_INT32)s_stDisanceTee[iNum1].Len + iExBits));
            break;
        case E_TREE_BL:
            if (iNum1 >= iBase) 
            {
                iExBits = s_iaOther_BLBits[iNum1-iBase];
            }
            break;
        default:
            return;
        }
        pstState->ulOptBlockLens += (WE_ULONG)(uiFreq * (iBitLens + iExBits));
    }
    if (iOverFlow == 0) 
    {
        return;
    }
    
    /* Find the first bit length which could increase: */
    do {
        iBitLens = iMaxLength-1;
        while (pstState->auBLCount[iBitLens] == 0)
        {
            iBitLens--;
        }
        pstState->auBLCount[iBitLens]--;      /* move one leaf down the tree */
        pstState->auBLCount[iBitLens+1] += 2; /* move one overflow item as its brother */
        pstState->auBLCount[iMaxLength]--;
        /* The brother of the overflow item also moves one step up,
        * but this does not affect auBLCount[iMaxLength]
        */
        iOverFlow -= 2;
    } while (iOverFlow > 0);
    
    /* Now recompute all bit lengths, scanning in increasing frequency.
    * h is still equal to HEAP_SIZE. (It is simpler to reconstruct all
    * lengths instead of fixing only the wrong ones. This idea is taken
    * from 'ar' written by Haruhiko Okumura.)
    */
    for (iBitLens = iMaxLength; iBitLens != 0; iBitLens--) 
    {
        iNum1 = pstState->auBLCount[iBitLens];
        while (iNum1 != 0) 
        {
            iNum2 = pstState->aiHeap[--iHeapIndex];
            if (iNum2 > iMaxCode) 
            {
                continue;
            }
            if ((WE_UINT32) pstTreeData[iNum2].Len != (WE_UINT32) iBitLens) 
            {
                pstState->ulOptBlockLens += (WE_ULONG)((iBitLens - (WE_INT32)pstTreeData[iNum2].Len)
                    * (WE_INT32)pstTreeData[iNum2].Freq);
                pstTreeData[iNum2].Len = (WE_UINT16)iBitLens;
            }
            iNum1--;
        }
    }
}

 /*=================================================================================
===
	FUNCTION: Tree_CodesGen 

	CREATE DATE: 2006-11-1
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Generate the codes for a given tree and bit counts (which need not be optimal)
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:
		Hfm_Bulid

	IMPORTANT NOTES:
		IN assertion: the array auBLCount contains the bit length statistics for
	the given tree and the field len is set for all tree elements.
		OUT assertion: the field code is set for all tree elements of non
	zero code length.

===================================================================================
==*/
static WE_VOID Tree_CodesGen (
                           P_St_Tree_Data pstTreeData,/* the tree to decorate */
                           WE_INT32 iMaxCode,         /* largest code with non zero frequency */
                           WE_UINT16 *puBlCount       /* number of codes at each bit length */
                           ) 
          
{
    WE_UINT16 auNextCode[MAX_BITS+1] = {0};   /* next code value for each bit length */
    WE_UINT16 uCodeValue = 0;                 /* running code value */
    WE_INT32 iBitsIndex = 0;                  /* bit index */
    WE_INT32 iCodeIndex = 0;                  /* code index */

    if (pstTreeData == NULL ||
        puBlCount == NULL)
    {
        return;
    }
    /* The distribution counts are first used to generate the code values
     * without bit reversal.*/
    for (iBitsIndex = 1; iBitsIndex <= MAX_BITS; iBitsIndex++)
	{
        uCodeValue = (WE_UINT16)((uCodeValue + puBlCount[iBitsIndex - 1]) << 1);
        auNextCode[iBitsIndex] = uCodeValue;
    }

    /* Check that the bit counts in puBlCount are consistent. The last code
     * must be all ones.
     */
    for (iCodeIndex = 0;  iCodeIndex <= iMaxCode; iCodeIndex++) 
	{
        WE_INT32 len = pstTreeData[iCodeIndex].Len;
        if (len == 0) 
		{
			continue;
		}
        /* Now reverse the bits */
        pstTreeData[iCodeIndex].Code = (WE_UINT16)Code_Reverse(auNextCode[len]++, len);
    }
}

 /*=================================================================================
===
	FUNCTION: Hfm_Bulid 

	CREATE DATE: 2006-11-1
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Construct one Huffman tree and assigns the code bit strings and lengths.
	Update the total bit length for the current block.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:
		BlHfm_Build, Hfm_Flush_Mass

	IMPORTANT NOTES:
		IN assertion: the field freq is set for all tree elements.
		OUT assertions: the fields len and code are set to the optimal bit length
	and corresponding code. The length opt_len is updated; ulStaBlockLens is also 
	updated if stree is not null. The field max_code is set.

===================================================================================
==*/
static WE_VOID Hfm_Bulid(
                      P_St_ZIP_State pstState,
                      P_St_Tree_Define pstTreeDefine,
                      E_TREE_METHOD eMethod /* the tree descriptor */ 
                      ) 
{
    P_St_Tree_Data pstTreeData;

    WE_INT32 iMaxElems = 0;
    WE_INT32 iNum1 = 0;     /* iterate over heap elements */
    WE_INT32 iNum2= 0;      /* iterate over heap elements */
    WE_INT32 iMaxCode = -1; /* largest code with non zero frequency */
    WE_INT32 iNodeNew = 0;  /* new node being created */

    if (pstTreeDefine == NULL ||
        pstState ==NULL)
    {
        return;
    }

    /* Construct the initial heap, with least frequent element in
     * heap[SMALLEST]. The sons of heap[iNum1] are heap[2 * iNum1] and heap[2*iNum1+1].
     * aiHeap[0] is not used.
     */
    pstState->iHeapLen = 0;
	pstState->iHeapMax = HEAP_MAXSIZE;

    pstTreeData = pstTreeDefine->pstDynTree;
    iMaxElems = pstTreeDefine->pstStatTree->iMaxElems;

    for (iNum1 = 0; iNum1 < iMaxElems; iNum1++)
    {
        if (pstTreeData[iNum1].Freq != 0) 
        {
            pstState->aiHeap[++(pstState->iHeapLen)] = iMaxCode = iNum1;
            pstState->aucSubTreeDepth[iNum1] = 0;
        } 
        else 
        {
            pstTreeData[iNum1].Len = 0;
        }
    }

    /* The pkzip format requires that at least one distance code exists,
     * and that at least one bit should be sent even if there is only one
     * possible code. So to avoid special checks later on we force at least
     * two codes of non zero frequency.
     */
    while (pstState->iHeapLen < 2) 
	{
        iNodeNew = pstState->aiHeap[++(pstState->iHeapLen)] = (iMaxCode < 2 ? ++iMaxCode : 0);
        TREE_BOUND(iNodeNew, 0, HEAP_MAXSIZE - 1);
        pstTreeData[iNodeNew].Freq = 1;
        pstState->aucSubTreeDepth[iNodeNew] = 0;
        pstState->ulOptBlockLens--;
		switch(eMethod) 
		{		
		case E_TREE_L:
            TREE_BOUND(iNodeNew, 0, CODES_LENGTH + 2 - 1);
			pstState->ulStaBlockLens -= s_staLiteralTee[iNodeNew].Len;
			break;
		case E_TREE_D:
            TREE_BOUND(iNodeNew, 0, D_CODES - 1);
			pstState->ulStaBlockLens -= s_stDisanceTee[iNodeNew].Len;
			break;
		case E_TREE_BL:
		default:
			return;
		}
    }
    pstTreeDefine->iMaxCode = iMaxCode;

    /* The elements aiHeap[iHeapLen/2+1 .. iHeapLen] are leaves of the tree,
     * establish sub-heaps of increasing lengths:
     */
    for (iNum1 = pstState->iHeapLen/2; iNum1 >= 1; iNum1--) 
	{
		Tree_HeapStore(pstState, pstTreeData, iNum1);
	}

    /* Construct the Huffman tree by repeatedly combining the least two
     * frequent nodes.
     */
    iNodeNew = iMaxElems;              /* next internal node of the tree */
    do 
	{
        PQREMOVE(pstState, pstTreeData, iNum1);  /* iNum1 = node of least frequency */
        iNum2 = pstState->aiHeap[SMALLEST]; /* iNum2 = node of next least frequency */

        pstState->aiHeap[--(pstState->iHeapMax)] = iNum1; /* keep the nodes sorted by frequency */
        pstState->aiHeap[--(pstState->iHeapMax)] = iNum2;

        /* Create a new node father of iNum1 and iNum2 */
        pstTreeData[iNodeNew].Freq = pstTreeData[iNum1].Freq + pstTreeData[iNum2].Freq;
        pstState->aucSubTreeDepth[iNodeNew] = (WE_UINT8)((pstState->aucSubTreeDepth[iNum1] >= pstState->aucSubTreeDepth[iNum2] ?
                                pstState->aucSubTreeDepth[iNum1] : pstState->aucSubTreeDepth[iNum2]) + 1);
        pstTreeData[iNum1].Dad = pstTreeData[iNum2].Dad = (WE_UINT16)iNodeNew;
        /* and insert the new iNodeNew in the aiHeap */
        pstState->aiHeap[SMALLEST] = iNodeNew++;
        Tree_HeapStore(pstState, pstTreeData, SMALLEST);

    } while (pstState->iHeapLen >= 2 && (WE_UINT32)pstState->iHeapLen < HEAP_MAXSIZE);

    pstState->aiHeap[--(pstState->iHeapMax)] = pstState->aiHeap[SMALLEST];

    /* At this point, the fields freq and dad are set. We can now
     * generate the bit lengths.
     */
    Tree_CodeLenBulid(pstState, (P_St_Tree_Define )pstTreeDefine, eMethod);

    /* The field len is now set, we can generate the bit codes */
    Tree_CodesGen ((P_St_Tree_Data)pstTreeData, iMaxCode, pstState->auBLCount);
}

 /*=================================================================================
===
	FUNCTION: Hfm_Scan 

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Scan a literal or distance tree to determine the frequencies of the codes
	in the bit length tree.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
static WE_VOID Hfm_Scan (
                      P_St_ZIP_State pstState, 
                      P_St_Tree_Data pstTreeData,/* the tree to be scanned */ 
                      WE_INT32 iMaxCode /* and its largest code of non zero frequency */
                      )
   
{
    WE_INT32 iCycle = 0;          /* iterates over all tree elements */
    WE_INT32 iLastLen = -1;       /* last emitted length */
    WE_INT32 iCurLen = 0;         /* length of current code */
    WE_INT32 iNextLen = 0;        /* length of next code */
    WE_INT32 iCurrRep = 0;        /* repeat count of the current code */
    WE_INT32 iMaxRep = 7;         /* max repeat count */
    WE_INT32 iMinRep = 4;         /* min repeat count */
    
    if (pstState == NULL ||
        pstTreeData == NULL)
    {
        return;
    }
    iNextLen = pstTreeData[0].Len;
    if (iNextLen == 0) 
    {
        iMaxRep = 138;
        iMinRep = 3;
    }
    pstTreeData[iMaxCode+1].Len = (WE_UINT16)0xffff; /* guard */

    for (iCycle = 0; iCycle <= iMaxCode; iCycle++) 
    {
        iCurLen = iNextLen; iNextLen = pstTreeData[iCycle+1].Len;
        if (++iCurrRep < iMaxRep && iCurLen == iNextLen) 
        {
            continue;
        } 
        else if (iCurrRep < iMinRep) 
        {
            pstState->astBLTree[iCurLen].Freq += (WE_UINT16)iCurrRep;
        } 
        else if (iCurLen != 0) 
        {
            if (iCurLen != iLastLen) 
            {
                pstState->astBLTree[iCurLen].Freq++;
            }
            pstState->astBLTree[REP_3_6].Freq++;
        } 
        else if (iCurrRep <= 10) 
        {
            pstState->astBLTree[REPZ_3_10].Freq++;
        } 
        else 
        {
            pstState->astBLTree[REPZ_11_138].Freq++;
        }

        iCurrRep = 0; iLastLen = iCurLen;
        
        if (iNextLen == 0)
        {
            iMaxRep = 138, iMinRep = 3;
        } 
        else if (iCurLen == iNextLen) 
        {
            iMaxRep = 6, iMinRep = 3;
        } 
        else 
        {
            iMaxRep = 7, iMinRep = 4;
        }
    }
}

 /*=================================================================================
===
	FUNCTION: Hfm_Sended 

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Send a literal or distance tree in compressed form, using the codes in
	astBLTr.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
static WE_VOID Hfm_Sended (
                        P_St_ZIP_State pstState, 
                        P_St_Tree_Data pstTreeData,/* the tree to be scanned */
                        WE_INT32 iMaxCode /* and its largest code of non zero frequency */
                        )    
{
    WE_INT32 iCycle = 0;                /* iterates over all tree elements */
    WE_INT32 iLastLen = -1;             /* last emitted length */
    WE_INT32 iCurLen = 0;               /* length of current code */
    WE_INT32 iNextLen = 0;              /* length of next code */
    WE_INT32 iCurrRep = 0;              /* repeat count of the current code */
    WE_INT32 iMaxRep = 7;               /* max repeat count */
    WE_INT32 iMinRep = 4;               /* min repeat count */
    
    if (pstState == NULL ||
        pstTreeData == NULL)
    {
        return;
    }
    
    iNextLen = (WE_INT32)pstTreeData[0].Len;
    /* tree[max_code+1].Len = -1; */  /* guard already set */
    if (iNextLen == 0) 
    {
        iMaxRep = 138;
        iMinRep = 3;
    }
    for (iCycle = 0; iCycle <= iMaxCode; iCycle++) 
    {
        iCurLen = iNextLen; 
        iNextLen = (WE_INT32)pstTreeData[iCycle+1].Len;

        if (++iCurrRep < iMaxRep && iCurLen == iNextLen)
        {
            continue;
        } 
        else if (iCurrRep < iMinRep) 
        {
            do 
            { 
                SET_CODE(pstState, iCurLen, pstState->astBLTree);
            
            } while (--iCurrRep != 0);
            
        }
        else if (iCurLen != 0) 
        {
            if (iCurLen != iLastLen)
            {
                SET_CODE(pstState, iCurLen, pstState->astBLTree); 
                iCurrRep--;
            }
            SET_CODE(pstState, REP_3_6, pstState->astBLTree);
            SET_BITS(pstState, (iCurrRep - 3), 2);
            
        }
        else if (iCurrRep <= 10) 
        {
            SET_CODE(pstState, REPZ_3_10, pstState->astBLTree); 
            SET_BITS(pstState, (iCurrRep - 3), 3);
            
        } 
        else 
        {
            SET_CODE(pstState, REPZ_11_138, pstState->astBLTree); 
            SET_BITS(pstState, (iCurrRep - 11), 7);
        }
        
        iCurrRep = 0;
        iLastLen = iCurLen;
        
        if (iNextLen == 0) 
        {
            iMaxRep = 138;
            iMinRep = 3;
        } 
        else if (iCurLen == iNextLen) 
        {
            iMaxRep = 6;
            iMinRep = 3;
        } 
        else 
        {
            iMaxRep = 7;
            iMinRep = 4;
        }
    }
}

 /*=================================================================================
===
	FUNCTION: Hfm_Sended 

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Construct the Huffman tree for the bit lengths and return the index in
	s_iaBL_List of the last bit length code to send.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
static WE_INT32 BlHfm_Build(
                            P_St_ZIP_State pstState
                            )
{
    WE_INT32 iLastBlIndex = 0;  /* index of last bit length code of non zero freq */
    
    if (pstState == NULL)
    {
        return ZIP_PARA_ERR;
    }
    /* Determine the bit length frequencies for literal and distance trees */
    Hfm_Scan(pstState, (P_St_Tree_Data)pstState->astDynLTree, pstState->stLDest.iMaxCode);
    Hfm_Scan(pstState, (P_St_Tree_Data)pstState->astDynDTree, pstState->stDDest.iMaxCode);
    
    /* Build the bit length tree: */
    Hfm_Bulid(pstState, (P_St_Tree_Define )(&(pstState->stBLDest)), E_TREE_BL);
    /* opt_len now includes the length of the tree representations, except
    * the lengths of the bit lengths codes and the 5+5+4 bits for the counts.
    */
    
    /* Determine the number of bit length codes to send. The pkzip format
    * requires that at least 4 bit length codes be sent. (appnote.txt says
    * 3 but the actual value used is 4.)
    */
    for (iLastBlIndex = BL_CODES-1; iLastBlIndex >= 3; iLastBlIndex--) 
    {
        if (pstState->astBLTree[s_iaBL_List[iLastBlIndex]].Len != 0) 
        {
            break;
        }
    }
    /* Update opt_len to include the bit length tree and counts */
    pstState->ulOptBlockLens += (WE_ULONG)(3 * (iLastBlIndex + 1) + 5 + 5 + 4);
    
    return iLastBlIndex;
}

 /*=================================================================================
===
	FUNCTION: AllHfm_Sended 

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Send the header for a block using dynamic Huffman trees: the counts, the
	lengths of the bit length codes, the literal tree and the distance tree.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:
		IN assertion: iLCodes >= 257, iDCodes >= 1, iBLCodes >= 4.

===================================================================================
==*/
static WE_VOID AllHfm_Sended(
                          P_St_ZIP_State pstState, 
                          WE_INT32 iLCodes,
                          WE_INT32 iDCodes,
                          WE_INT32 iBLCodes
                          )
{
    WE_INT32 iRank = 0;                       /* index in s_iaBL_List */
    
    if (pstState == NULL)
    {
        return;
    }
    
    SET_BITS(pstState, (iLCodes - 257), 5); /* not +255 as stated in appnote.txt */
    SET_BITS(pstState, (iDCodes - 1),   5);
    SET_BITS(pstState, (iBLCodes - 4),  4); /* not -3 as stated in appnote.txt */
    for (iRank = 0; iRank < iBLCodes; iRank++)
    {
        SET_BITS(pstState, pstState->astBLTree[s_iaBL_List[iRank]].Len, 3);
    }
    
    Hfm_Sended(pstState, (P_St_Tree_Data)pstState->astDynLTree, iLCodes-1); /* literal tree */
    Hfm_Sended(pstState, (P_St_Tree_Data)pstState->astDynDTree, iDCodes-1); /* distance tree */
}

 /*=================================================================================
===
	FUNCTION: Mass_SavedSend 

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Send a stored block
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
WE_VOID Mass_SavedSend(
                    P_St_ZIP_State pstState, 
                    WE_INT8 *pucBuf, 
                    WE_ULONG ulStoreLens, 
                    WE_INT32 iEof
                    )
{
    if (pstState == NULL ||
        pucBuf == NULL)
    {
        return;
    }
    SET_BITS(pstState, ((STORED_BLOCK<<1) + (WE_UINT32)iEof), 3);  /* send block type */
    
    Mass_Copy(pstState, pucBuf, (WE_UINT32)ulStoreLens, 1); /* with header */
}

 /*=================================================================================
===
	FUNCTION: Unzip_Align 

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Send one empty static block to give enough lookahead for zip_stream.
	This takes 10 bits, of which 7 may remain in the bit buffer.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
WE_VOID Unzip_Align(
                 P_St_ZIP_State pstState
                 )
{
    if (pstState == NULL)
    {
        return;
    }
    SET_BITS(pstState, (STATIC_TREES << 1), 3);
    SET_CODE(pstState, END_BLOCK, s_staLiteralTee);
    
    State_flush(pstState);
    /* Of the 10 bits for the empty block, we have already sent
    * (10 - bi_valid) bits. The lookahead for the last real code (before
    * the EOB of the previous block) was thus at least one plus the length
    * of the EOB plus what we have just sent of the empty static block.
    */
    if ((1 + pstState->iLastEobLens + 10 - pstState->iOutBufAvail) < 9) 
    {
        SET_BITS(pstState, (STATIC_TREES << 1), 3);
        SET_CODE(pstState, END_BLOCK, s_staLiteralTee);
        
        State_flush(pstState);
    }
    pstState->iLastEobLens = 7;
}
 /*=================================================================================
===
	FUNCTION: Hfm_Flush_Mass 

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Determine the best encoding for the current block: dynamic trees, static
	trees or store, and output the encoded block to the zip file.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
WE_VOID Hfm_Flush_Mass(
                    P_St_ZIP_State pstState, 
                    WE_INT8 *pucBuf, 
                    WE_ULONG ulStoreLens, 
                    WE_INT32 iEof
                    )
{
    WE_ULONG ulOptLen = 0UL;
    WE_ULONG ulStaticLen = 0UL;     /* ulStaBlockLens in bytes */
    WE_INT32 iLastBlIndex = 0;      /* index of last bit length code of non zero freq */
    
    if (pstState == NULL ||
        pucBuf == NULL)
    {
        return;
    }
    /* Build the Huffman trees unless a stored block is forced */
    if (pstState->iLevel > 0) 
    {
        
        /* Check if the file is binary or text */
        if (ulStoreLens > 0 && pstState->pstStream->iDataType == ZIP_UNKNOWN)
        {
            State_Setting(pstState);
        }
        /* Construct the literal and distance trees */
        Hfm_Bulid(pstState, (P_St_Tree_Define )(&(pstState->stLDest)), E_TREE_L);
        
        Hfm_Bulid(pstState, (P_St_Tree_Define )(&(pstState->stDDest)), E_TREE_D);
        /* At this point, opt_len and ulStaBlockLens are the total bit lengths of
        * the compressed block data, excluding the tree representations.
        */
        
        /* Build the bit length tree for the above two trees, and get the index
        * in s_iaBL_List of the last bit length code to send.
        */
        iLastBlIndex = BlHfm_Build(pstState);
        
        /* Determine the best encoding. Compute the block lengths in bytes. */
        ulOptLen = (pstState->ulOptBlockLens+3+7)>>3;
        ulStaticLen = (pstState->ulStaBlockLens+3+7)>>3;
        
        
        if (ulStaticLen <= ulOptLen)
        {
            ulOptLen = ulStaticLen;
        }
        
    } 
    else 
    {
        ulOptLen = ulStaticLen = ulStoreLens + 5; /* force a stored block */
    }
    
    if ((ulStoreLens + 4) <= ulOptLen && pucBuf != (WE_INT8*)0)
    {
        
        Mass_SavedSend(pstState, pucBuf, ulStoreLens, iEof);
    } 
    else if (pstState->iStrategy == ZIP_FIXED || ulStaticLen == ulOptLen) 
    {
        
        SET_BITS(pstState, ((STATIC_TREES<<1) + (WE_UINT32)iEof), 3);
        Mass_Zipped(pstState, (P_St_Tree_Data)s_staLiteralTee, (P_St_Tree_Data)s_stDisanceTee);
        
    } 
    else 
    {
        SET_BITS(pstState, ((DYN_TREES<<1) + (WE_UINT32)iEof), 3);
        AllHfm_Sended(pstState, pstState->stLDest.iMaxCode + 1, pstState->stDDest.iMaxCode + 1,
            iLastBlIndex+1);
        Mass_Zipped(pstState, (P_St_Tree_Data)pstState->astDynLTree, (P_St_Tree_Data)pstState->astDynDTree);
    }
    /* The above check is made mod 2^32, for files larger than 512 MB
    * and uLong implemented on 32 bits.
    */
    Mass_Init(pstState);
    
    if (iEof) 
    {
        State_Ending(pstState);
    }
}

 /*=================================================================================
===
	FUNCTION: Hfm_Save

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Save the match info and tally the frequency counts. Return true if
	the current block must be flushed.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
WE_INT32 Hfm_Save (
                   P_St_ZIP_State pstState, 
                   WE_UINT32 uiDistance, 
                   WE_UINT32 uiLength
                   )
{
    if (pstState == NULL)
    {
        return ZIP_PARA_ERR;
    }
    pstState->puDBuf[pstState->uiLLastIndex] = (WE_UINT16)uiDistance;
    pstState->pucLBuf[pstState->uiLLastIndex++] = (WE_UINT8)uiLength;
    if (uiDistance == 0) 
    {
        /* uiLength is the unmatched char */
        pstState->astDynLTree[uiLength].Freq++;
    } 
    else 
    {
        pstState->uiMatchSize++;
        
        /* Here, uiLength is the match length - MIN_MATCH */
        uiDistance--;             /* uiDistance = match distance - 1 */
        pstState->astDynLTree[iaLength[uiLength] + LITERALS + 1].Freq++;
        pstState->astDynDTree[DIST_CODE(uiDistance)].Freq++;
    }
    
    
    return (pstState->uiLLastIndex == pstState->uiLBufSize-1);
    /* We avoid equality with uiLBufSize because of wraparound at 64K
    * on 16 bit machines and because stored blocks are restricted to
    * 64K-1 bytes.
    */
}

 /*=================================================================================
===
	FUNCTION: Mass_Zipped

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Send the block data compressed using the given Huffman trees.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
static WE_VOID Mass_Zipped(
                        P_St_ZIP_State pstState, 
                        P_St_Tree_Data pstLiteralTree, 
                        P_St_Tree_Data pstDistanceTree
                        )
{
    WE_UINT32 uiDistance = 0U;      /* distance of matched string */
    WE_INT32 iLength = 0;         /* match length or unmatched char (if uiDistance == 0) */
    WE_UINT32 uiLitBuffIndex = 0U;    /* running index in l_buf */
    WE_UINT32 code = 0U;      /* the code to send */
    WE_INT32 extra = 0;      /* number of extra bits to send */
    
    if (pstState == NULL ||
        pstLiteralTree == NULL ||
        pstDistanceTree == NULL)
    {
        return;
    }
    if (pstState->uiLLastIndex != 0) 
    {
        do 
        {
            uiDistance = (WE_UINT32)pstState->puDBuf[uiLitBuffIndex];
            iLength = (WE_INT32)pstState->pucLBuf[uiLitBuffIndex++];
            if (uiDistance == 0) 
            {
                SET_CODE(pstState, iLength, pstLiteralTree); /* send a literal WE_UINT8 */
            } 
            else 
            {
                /* Here, iLength is the match length - MIN_MATCH */
                code = iaLength[iLength];
                SET_CODE(pstState, code+LITERALS+1, pstLiteralTree); /* send the length code */
                extra = s_iaOther_LBits[code];
                if (extra != 0) 
                {
                    iLength -= s_iaLength[code];
                    SET_BITS(pstState, iLength, extra);       /* send the extra length bits */
                }
                uiDistance--; /* uiDistance is now the match distance - 1 */
                code = DIST_CODE(uiDistance);
                
                SET_CODE(pstState, code, pstDistanceTree);       /* send the distance code */
                extra = s_iaOther_DBits[code];
                if (extra != 0) 
                {
                    uiDistance -= (WE_UINT32)s_iaDist[code];
                    SET_BITS(pstState, uiDistance, extra);   /* send the extra distance bits */
                }
            } /* literal or match pair ? */
            
            /* Check that the overlay between pending_buf and puDBuf+pucLBuf is ok: */
            
        } while (uiLitBuffIndex < pstState->uiLLastIndex);
    }
    
    SET_CODE(pstState, END_BLOCK, pstLiteralTree);
    pstState->iLastEobLens = pstLiteralTree[END_BLOCK].Len;
}

 /*=================================================================================
===
	FUNCTION: State_Setting

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Set the data type to BINARY or TEXT, using a crude approximation:
	set it to ZIP_TEXT if all symbols are either printable characters (33 to 255)
	or white spaces (9 to 13, or 32); or set it to ZIP_BINARY otherwise.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:
		 IN assertion: the fields Freq of astDynLTree are set.

===================================================================================
==*/
static WE_VOID State_Setting(
                          P_St_ZIP_State pstState
                          )
{
    WE_INT32 iCycle = 0;
    if (pstState == NULL)
    {
        return;
    }
    
    for (iCycle = 0; iCycle < 9; iCycle++)
    {
        if (pstState->astDynLTree[iCycle].Freq != 0)
        {
            break;
        }
    }
    if (iCycle == 9)
    {
        for (iCycle = 14; iCycle < 32; iCycle++)
        {
            if (pstState->astDynLTree[iCycle].Freq != 0)
            {
                break;
            }
        }
    }
    pstState->pstStream->iDataType = (iCycle == 32) ? ZIP_TEXT : ZIP_BINARY;
}

 /*=================================================================================
===
	FUNCTION: Code_Reverse

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Reverse the first len bits of a code, using straightforward code .
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:
		 IN assertion: 1 <= len <= 15.

===================================================================================
==*/
static WE_UINT32 Code_Reverse(
                              WE_UINT32 iOrig,/* the value to invert */
                              WE_INT32 iLen   /* its bit length */      
                              )
{
    WE_UINT32 uiRes = 0;
    do 
    {
        uiRes |= iOrig & 1;
        iOrig >>= 1, uiRes <<= 1;
        
    } while (--iLen > 0);
    
    return uiRes >> 1;
}
 /*=================================================================================
===
	FUNCTION: State_flush

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Flush the bit buffer, keeping at most 7 bits in it.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
static WE_VOID State_flush(
                            P_St_ZIP_State pstState
                        )
{
    if (pstState ==NULL)
    {
        return;
    }
    if (pstState->iOutBufAvail == 16)
    {
        SET_DOUBLE(pstState, pstState->uOutBufOffset);
        pstState->uOutBufOffset = 0;
        pstState->iOutBufAvail = 0;
    } 
    else if (pstState->iOutBufAvail >= 8) 
    {
        SET_BUF_BYTE(pstState, (WE_UINT8)pstState->uOutBufOffset);
        pstState->uOutBufOffset >>= 8;
        pstState->iOutBufAvail -= 8;
    }
}

 /*=================================================================================
===
	FUNCTION: State_Ending

	CREATE DATE: 2006-11-2
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Flush the bit buffer and align the output on a byte boundary
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		

	CALL BY:

	IMPORTANT NOTES:

===================================================================================
==*/
static WE_VOID State_Ending(
                            P_St_ZIP_State pstState
                         )
{
    if (pstState ==NULL)
    {
        return;
    }
    if (pstState->iOutBufAvail > 8) 
    {
        SET_DOUBLE(pstState, pstState->uOutBufOffset);
    } 
    else if (pstState->iOutBufAvail > 0) 
    {
        SET_BUF_BYTE(pstState, (WE_UINT8)pstState->uOutBufOffset);
    }
    pstState->uOutBufOffset = 0;
    pstState->iOutBufAvail = 0;
    
}

/*=================================================================================
===
FUNCTION: Mass_Copy

  CREATE DATE: 2006-11-2
  AUTHOR: duhuanbiao
  
    DESCRIPTION:
    Copy a stored block, storing first the length and its
    one's complement if requested.
    
      ARGUMENTS PASSED:
      
        RETURN VALUE:
        
          
            CALL BY:
            
              IMPORTANT NOTES:
              
                ===================================================================================
==*/
static WE_VOID Mass_Copy(
                      P_St_ZIP_State pstState,
                      WE_INT8 *piInputBuf    /* the input data */, 
                      WE_UINT32 iLen         /* its length */,
                      WE_INT32 IHead         /* true if block header must be written */
                      )
{
    if (pstState == NULL ||
        piInputBuf == NULL)
    {
        return;
    }
    State_Ending(pstState);        /* align on byte boundary */
    pstState->iLastEobLens = 8;    /* enough lookahead for inflate */
    
    if (IHead) 
    {
        SET_DOUBLE(pstState, (WE_UINT16)iLen);
        SET_DOUBLE(pstState, (WE_UINT16)~iLen);
        
    }
    
    while (iLen--)
    {
        SET_BUF_BYTE(pstState, *piInputBuf++);
    }
}