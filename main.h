#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum RET{
	RET_OK =1,
	RET_ERR =2,
	RET_MEM_MALLOC = 3,	
	RET_PARA_ERR = 4,
};

typedef struct TBL_KEY{
	unsigned int len;
	void* pKey;
}TBL_KEY;

typedef struct TBL_DATA{
	unsigned int len;
	void* pData;
}TBL_DATA;	

typedef unsigned int (*TblIdCalIndxFunc)(void* key,unsigned int len);

typedef struct TblPrority
{
	unsigned int uiTblId;
	unsigned int uiTblStoreMode;	
	unsigned int TblSize;	
	unsigned int uiKeySize;
	unsigned int uiDataSize;
	TblIdCalIndxFunc pFunc;
}TblPrority;

typedef struct TBL_ELEMENT
{
	TBL_KEY* pEleKey;
	TBL_DATA* pEleData;
	struct TBL_ELEMENT* pElemNext;	
}TBL_ELEMENT;

typedef struct TBL_ELEMENT_HEAD
{
	unsigned int uiElemCnt;
	unsigned int uiKeyLen;
	unsigned int uiDataLen; 
	TBL_ELEMENT* pEleHead;
}TBL_ELEMENT_HEAD;

enum TblId
{
	/*don't set tblid smaller than TBL_ID_MIN*/
	TBL_ID_MIN = 0,
	
	TBL_ID_AGE = 1,
	TBL_ID_HEIGHT = 2,
	
	TBL_ID_MAX = 10,
	/*don't set tblId bigger than TBL_ID_MAX*/	
};

enum TblStorMode
{
	/*don't set tblid smaller than TBL_STR_MODE_MIN*/
	TBL_STR_MODE_MIN = 0,
	
	TBL_STR_MODE_ARRAY = 1,
	TBL_STR_MODE_HASH  = 2,
	TBL_STR_MODE_AVL = 3,
	TBL_STR_MODE_RBT = 4,
	
	TBL_STR_MODE_MAX = 10,
	/*don't set tblId bigger than TBL_ID_MAX*/	
};

typedef void*(*Tbl_Init)();
typedef int(*Tbl_Destroy)();
typedef int(*Tbl_Add)(int, void*, void*, void*, void*);
typedef int(*Tbl_Del)(int, void*, void*, void*);
typedef int(*Tbl_Query)(int, void*, void*, void*, void*);

typedef struct TblStorModeFunc
{
	int uiStrMode;
	Tbl_Init pInitFunc;
	Tbl_Destroy pDestroyFunc;
	Tbl_Add pAddFunc;
	Tbl_Del pDelFunc;
	Tbl_Query pQueryFunc;
}TblStorModeFunc;

void* ArrayCreate(unsigned int TblSize,unsigned int KeySize,unsigned int DataSize);
int ArrayAdd(unsigned int TblId, void* pTblmKey,void* pTblmData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);
int ArrayQuery(unsigned int TblId, void* pTblmKey,void* pTblmData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);
int ArrayDelete(unsigned int TblId, void* pTblmKey,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);

void* HashCreate(unsigned int TblSize,unsigned int KeySize,unsigned int DataSize);
int HashAdd(unsigned int TblId, void* pTblmKey,void* pTblmData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);
int HashQuery(unsigned int TblId, void* pTblmKey,void* pTblmData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);
int HashDelete(unsigned int TblId, void* pTblmKey,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);

/*string hash seed:magic number*/
#define M 249997 
#define M1 1000001 
#define M2 0xF0000000 
unsigned int uiIntCalHashIndex(void* key, int uiLen);
// RS Hash Function 
unsigned int RSHash(char*str);
// JS Hash Function 
unsigned int JSHash(char*str);
// P. J. Weinberger Hash Function 
unsigned int PJWHash(char*str);
// ELF Hash Function 
unsigned int ELFHash(char*str);
// BKDR Hash Function 
unsigned int BKDRHash(char*str);
// SDBM Hash Function 
unsigned int SDBMHash(char*str);
// DJB Hash Function 
unsigned int DJBHash(char*str);
// AP Hash Function 
unsigned int APHash(char*str);   

unsigned int TBLM_SetKeyAndData(TBL_KEY* KEY,void* pKey,int uiKeyLen,TBL_DATA* DATA,void* pData,int uiDataLen);
unsigned int TBLM_Init(TblPrority** pTblmPro,TBL_ELEMENT_HEAD** pTblmData);
unsigned int TBLM_Destroy(TblPrority** pTblmPro,TBL_ELEMENT_HEAD** pTblmData);
unsigned int TBLM_Add(unsigned int TblId, TBL_KEY* pTblKey,TBL_DATA* pTblData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);
unsigned int TBLM_Delete(unsigned int TblId, TBL_KEY* pTblKey,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);
unsigned int TBLM_Query(unsigned int TblId, TBL_KEY* pTblKey,TBL_DATA* pTblData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead);


TblPrority Table[] = {
	/*TblId           storeMode              TblSize     keySize   dataSize           IndexCalFUnc*/
	{TBL_ID_AGE,      TBL_STR_MODE_ARRAY, 	  10,        20,		4,                 BKDRHash,},
	{TBL_ID_HEIGHT,   TBL_STR_MODE_ARRAY,     10, 	     20,		4,                 BKDRHash,},
};
unsigned int g_TblSize  = sizeof(Table)/sizeof(Table[0]);

TblStorModeFunc g_auStrModeFuncTbl[] = {
	/*MODE                 InitFunc       DestroyFunc   AddFunc          DelFunc          QueryFunc*/  
	{TBL_STR_MODE_MIN,     NULL,          NULL,        NULL,             NULL,            NULL         },
	{TBL_STR_MODE_ARRAY,   ArrayCreate,  NULL,         ArrayAdd,        ArrayDelete,    ArrayQuery  },
	{TBL_STR_MODE_HASH,    NULL,          NULL,        NULL,         NULL,      NULL   },
	{TBL_STR_MODE_AVL,     NULL,          NULL,        NULL,             NULL,            NULL         },
	{TBL_STR_MODE_RBT,     NULL,          NULL,        NULL,             NULL,            NULL         },
};
unsigned int g_ModeSize = sizeof(g_auStrModeFuncTbl)/sizeof(g_auStrModeFuncTbl[0]);
