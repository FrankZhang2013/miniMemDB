#include "main.h"

void* ArrayCreate(unsigned int TblSize,unsigned int KeySize,unsigned int DataSize)
{
	void* pData = NULL;
	if (TblSize<0 || KeySize<0 || DataSize<0)
	{
		return NULL;
	}
	pData = (void*)malloc(TblSize*sizeof(TBL_ELEMENT));
	memset(pData,0,TblSize*sizeof(TBL_ELEMENT));
	return pData;
}

int ArrayDestroy()
{
	return RET_OK;
}
int ArrayAdd(unsigned int TblId, void* pTblmKey,void* pTblmData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	TBL_KEY* pTblKey = (TBL_KEY*)pTblmKey;
	TBL_DATA* pTblData = (TBL_DATA*)pTblmData;
	TblPrority* pTblPriNode = pTblPri + TblId;
	unsigned int uiKeyLen = pTblPriNode->uiKeySize;
	unsigned int uiDataLen = pTblPriNode->uiDataSize;
	unsigned int uiTblSize = pTblPriNode->TblSize;
	unsigned int uiTblIndex = *(int *)(pTblKey->pKey);
	TBL_ELEMENT_HEAD* pElemHead = NULL;
	TBL_ELEMENT* pElemNode =  NULL;
	TBL_KEY* pKeyTmp = NULL;
	TBL_DATA* pDataTmp = NULL;
	void* pTmp = NULL;
	if (pTblPriNode->pFunc != NULL)
	{
		uiTblIndex = pTblPriNode->pFunc(pTblKey,uiKeyLen) % uiTblSize;
	}

	pElemHead = pTblDataHead + uiTblIndex;
	if(pElemHead == NULL)
	{
		printf("node create error,tblId=%d, index = %d!\n",TblId,uiTblIndex);
		return RET_ERR;
	}

	if (pElemNode == NULL)
	{
		pElemNode = (TBL_ELEMENT*)malloc(sizeof(* pElemNode ));
		if (pElemNode == NULL)
		{
			return RET_MEM_MALLOC;	
		}
	}
	pKeyTmp = (TBL_KEY*)malloc(sizeof(TBL_KEY));
	pKeyTmp->len = pTblKey->len;
	pTmp = (void*)malloc(pTblKey->len);
	memcpy(pTmp,pTblKey->pKey,pTblKey->len);	
	pElemNode->pEleKey = pKeyTmp;
	pElemNode->pEleKey->pKey = pTmp;
	
	pDataTmp = (TBL_DATA*)malloc(sizeof(TBL_DATA));
	pDataTmp->len = uiDataLen;
	pTmp = (void*)malloc(uiDataLen);
	memcpy(pTmp,pTblData->pData,uiDataLen);
	pElemNode->pEleData = pDataTmp;
	pElemNode->pEleData->pData = pTmp;

	pElemNode->pElemNext = NULL;
	pElemHead->uiElemCnt = 1;
	pElemHead->pEleHead = pElemNode;	
	return RET_OK;
}

int ArrayQuery(unsigned int TblId, void* pTblmKey,void* pTblmData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	TBL_KEY* pTblKey = (TBL_KEY*)pTblmKey;
	TBL_DATA* pTblData = (TBL_DATA*)pTblmData;
	TblPrority* pTblPriNode = pTblPri + TblId;
	unsigned int uiKeyLen = pTblPriNode->uiKeySize;
	unsigned int uiDataLen = pTblPriNode->uiDataSize;
	unsigned int uiTblSize = pTblPriNode->TblSize;
	unsigned int uiTblIndex = *(int *)(pTblKey->pKey);
	TBL_ELEMENT* pElemNode = NULL;
	TBL_ELEMENT_HEAD* pElemHead = NULL;
	if (pTblPriNode->pFunc != NULL)
	{
		uiTblIndex = pTblPriNode->pFunc(pTblKey,uiKeyLen) % uiTblSize;
	}	

	pElemHead = pTblDataHead + uiTblIndex;
	if(pElemHead == NULL)
	{
		printf("node create error,tblId=%d, index = %d!\n",TblId,uiTblIndex);
		return RET_ERR;
	}
	if (pElemHead->uiElemCnt <= 0)
	{
		return RET_ERR;	
	}
	pElemNode = pElemHead->pEleHead;
	if (pElemNode == NULL)
	{
		return RET_ERR;	
	}
	/*
	if (pElemNode->pEleKey->len != uiKeyLen)
	{
		return RET_ERR;
	}
	*/
	if (memcpy(pElemNode->pEleKey->pKey,pTblKey->pKey,pTblKey->len) != 0)
	{
		return RET_ERR;
	}
	pTblData->len = uiDataLen;
	memcpy(pTblData->pData,pElemNode->pEleData->pData,uiDataLen); 
	return RET_OK;	
}

int ArrayDelete(unsigned int TblId, void* pTblmKey,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	TBL_KEY* pTblKey = (TBL_KEY*)pTblmKey;
	TblPrority* pTblPriNode = pTblPri + TblId; 
	unsigned int uiKeyLen = pTblPriNode->uiKeySize;
	unsigned int uiDataLen = pTblPriNode->uiDataSize;
	unsigned int uiTblSize = pTblPriNode->TblSize;
	unsigned int uiTblIndex = *(int *)((TBL_KEY*)pTblKey->pKey);
	TBL_KEY* pKeyTemp = NULL;
	TBL_DATA* pDataTemp = NULL;
	TBL_ELEMENT_HEAD* pElemHead = NULL;
	TBL_ELEMENT* pElemNode = NULL;
	if (pTblPriNode->pFunc != NULL)
	{
		uiTblIndex = pTblPriNode->pFunc(pTblKey,uiKeyLen) % uiTblSize;
	}	
	pElemHead = pTblDataHead + uiTblIndex;

	if(pElemHead == NULL)
	{
		printf("node create error,tblId=%d, index = %d!\n",TblId,uiTblIndex);
		return RET_ERR;
	}
	pElemNode = pElemHead->pEleHead;
	if (pElemNode == NULL)
	{
		return RET_OK;	
	}
	pKeyTemp = pElemNode->pEleKey;
	pDataTemp = pElemNode->pEleData;
	if (pKeyTemp==NULL || pDataTemp==NULL)
	{
		return RET_ERR;
	}
	if(pKeyTemp->len != pTblKey->len)
	{
		return RET_ERR;
	}
	if(memcmp(pKeyTemp->pKey,pTblKey->pKey,pTblKey->len) == 0)
	{
		free(pDataTemp->pData);
		free(pDataTemp);
		pElemNode->pEleData = NULL;
		
		free(pKeyTemp->pKey);
		free(pKeyTemp);
		pElemNode->pEleKey = NULL;

		pElemHead->uiElemCnt--;
		return RET_OK;		
	}
	return RET_ERR;
}

void* HashCreate(unsigned int TblSize,unsigned int KeySize,unsigned int DataSize)
{
	void* pData = NULL;
	if (TblSize<0 || KeySize<0 || DataSize<0)
	{
		return NULL;
	}
	pData = (void*)malloc(TblSize*sizeof(TBL_ELEMENT));
	memset(pData,0,TblSize*sizeof(TBL_ELEMENT));	
	return pData;
}
int HashAdd(unsigned int TblId, void* pTblmKey,void* pTblmData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	TBL_KEY* pTblKey = (TBL_KEY*)pTblmKey;
	TBL_DATA* pTblData = (TBL_DATA*)pTblmData;
	TblPrority* pTblPriNode = pTblPri + TblId;
	TBL_ELEMENT_HEAD* pTblDataListHead = NULL;
	unsigned int KeyLen = 0;
	unsigned int DataLen = 0;
	unsigned int hashIndex = 0;
	TBL_ELEMENT* pHashDataPriv = NULL;
	TBL_ELEMENT* pHashDataHead = NULL;
	TBL_ELEMENT* pHashDataNext = NULL;
	TBL_KEY* pKeyTmp = NULL;
	TBL_DATA* pDataTmp = NULL;
	if (pTblPriNode == NULL)
	{
		return RET_ERR;
	}

	if (pTblPriNode->pFunc != NULL)
	{
		hashIndex = pTblPriNode->pFunc(pTblmKey,KeyLen);
	}
	else
	{
		hashIndex = *(int*)pTblmKey;
	}
	if (hashIndex > pTblPriNode->TblSize)
	{
		return RET_ERR;
	}
	pTblDataListHead = pTblDataHead + hashIndex;
	if (pTblDataListHead == NULL || pTblDataListHead->uiKeyLen != KeyLen)
	{
		return RET_ERR;
	}
	KeyLen = pTblDataListHead->uiKeyLen;
	DataLen = pTblDataListHead->uiDataLen;
	pHashDataHead = pTblDataListHead->pEleHead;
	if (pTblDataListHead->uiElemCnt == 0)
	{
		if (pHashDataHead == NULL)
		{
			pHashDataHead = (TBL_ELEMENT*)malloc(sizeof(TBL_ELEMENT));
		}
		pKeyTmp =  (TBL_KEY*)malloc(sizeof(TBL_KEY));
		pKeyTmp->len = KeyLen;
		pKeyTmp->pKey = (void*)malloc(KeyLen);
		memcpy(pKeyTmp->pKey,pTblmKey,KeyLen);
		pHashDataHead->pEleKey = pKeyTmp;

		pDataTmp = (TBL_DATA*)malloc(sizeof(TBL_DATA));
		pDataTmp->len = DataLen;
		pDataTmp->pData = (void*)malloc(DataLen);
		memcpy(pDataTmp->pData,pTblmData,DataLen);

		pHashDataHead->pEleData = pDataTmp;
		pHashDataHead->pElemNext = NULL;
		pTblDataListHead->uiElemCnt++;
		return RET_OK;
	}
	while(pHashDataHead != NULL)
	{
		if (memcpy(pHashDataHead->pEleKey,pTblmKey,KeyLen) == 0)
		{
			return RET_OK;
		}
		pHashDataPriv = pHashDataHead;
		pHashDataNext = pHashDataHead->pElemNext;
		pHashDataHead = pHashDataNext;
	}
	if (pHashDataHead == NULL)
	{
		pHashDataHead = (TBL_ELEMENT*)malloc(sizeof(TBL_ELEMENT));
	}
	pKeyTmp =  (TBL_KEY*)malloc(sizeof(TBL_KEY));
	pKeyTmp->len = KeyLen;
	pKeyTmp->pKey = (void*)malloc(KeyLen);
	memcpy(pKeyTmp->pKey,pTblmKey,KeyLen);
	pHashDataHead->pEleKey = pKeyTmp;

	pDataTmp = (TBL_DATA*)malloc(sizeof(TBL_DATA));
	pDataTmp->len = DataLen;
	pDataTmp->pData = (void*)malloc(DataLen);
	memcpy(pDataTmp->pData,pTblmData,DataLen);

	pHashDataHead->pEleData = pDataTmp;
	pHashDataHead->pElemNext = NULL;
	pTblDataListHead->uiElemCnt++;
	pHashDataPriv->pElemNext = pHashDataHead;
	pHashDataHead->pElemNext = NULL;
	return RET_OK;
}

int HashQuery(unsigned int TblId, void* pTblmKey,void* pTblmData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	TBL_KEY* pTblKey = (TBL_KEY*)pTblmKey;
	TBL_DATA* pTblData = (TBL_DATA*)pTblmData;
	TblPrority* pTblPriNode = pTblPri + TblId;
	TBL_ELEMENT_HEAD* pTblHashData = NULL;
	unsigned int HashIndex = 0;
	unsigned int KeyLen = 0;
	unsigned int DataLen = 0;
	TBL_ELEMENT* pTblHashHead = NULL;
	TBL_ELEMENT* pTblHashNext = NULL;
	if (pTblPriNode == NULL)
	{
		return RET_ERR;
	}
	if (pTblPriNode->pFunc != NULL)
	{
		HashIndex = pTblPriNode->pFunc(pTblmKey,KeyLen);
	}
	else
	{
		HashIndex = *(int*)pTblmKey;
	}
	if (HashIndex > pTblPriNode->TblSize)
	{
		return RET_ERR;
	}
	pTblHashData = pTblDataHead + HashIndex;
	if (pTblHashData == NULL)
	{
		return RET_ERR;
	}
	KeyLen = pTblHashData->uiKeyLen;
	DataLen = pTblHashData->uiDataLen;
	if (pTblHashData->uiElemCnt <= 0 || pTblHashData->uiKeyLen != pTblKey->len )
	{
		return RET_ERR;
	}
	pTblHashHead= pTblHashData->pEleHead;
	while(pTblHashHead != NULL)
	{
		if (memcpy(pTblKey->pKey,pTblHashHead->pEleKey,KeyLen) == 0)
		{
			pTblData->len = DataLen;
			memcpy(pTblData->pData,pTblHashHead->pEleData,DataLen);
			return RET_OK;
		}
		pTblHashNext = pTblHashHead->pElemNext;
		pTblHashHead = pTblHashNext;
	}
	return RET_ERR;
}

int HashDelete(unsigned int TblId, void* pTblmKey,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	TBL_KEY* pTblKey = (TBL_KEY*)pTblmKey;
	TBL_DATA* pTblData = NULL;
	TblPrority* pTblPriNode = pTblPri + TblId;
	TBL_ELEMENT_HEAD* pTblHashData = NULL;
	unsigned int HashIndex = 0;
	unsigned int KeyLen = 0;
	unsigned int DataLen = 0;
	TBL_ELEMENT* pTblHashHead = NULL;
	TBL_ELEMENT* pTblHashNext = NULL;
	if (pTblPriNode == NULL)
	{
		return RET_ERR;
	}
	if (pTblPriNode->pFunc != NULL)
	{
		HashIndex = pTblPriNode->pFunc(pTblmKey,KeyLen);
	}
	else
	{
		HashIndex = *(int*)pTblmKey;
	}
	if (HashIndex > pTblPriNode->TblSize)
	{
		return RET_ERR;
	}
	pTblHashData = pTblDataHead + HashIndex;
	if (pTblHashData == NULL)
	{
		return RET_ERR;
	}
	KeyLen = pTblHashData->uiKeyLen;
	DataLen = pTblHashData->uiDataLen;
	if (pTblHashData->uiElemCnt <= 0 || pTblHashData->uiKeyLen != pTblKey->len )
	{
		return RET_ERR;
	}
	pTblHashHead= pTblHashData->pEleHead;
	while(pTblHashHead != NULL)
	{
		if (memcpy(pTblKey->pKey,pTblHashHead->pEleKey,KeyLen) == 0)
		{
			pTblData->len = DataLen;
			memcpy(pTblData->pData,pTblHashHead->pEleData,DataLen);
			return RET_OK;
		}
		pTblHashNext = pTblHashHead->pElemNext;
		pTblHashHead = pTblHashNext;
	}	
	return RET_ERR;
}
/*get tblId info, not circle get,*/
unsigned int TBLM_Init(TblPrority** pTblmPro,TBL_ELEMENT_HEAD** pTblmData)
{
	unsigned int ret = RET_OK;
	unsigned int uiTblindex = 0;
	TblPrority* pTblPriNode = NULL;
	TBL_ELEMENT_HEAD* pElemHead = NULL;
	TBL_ELEMENT* pEleNode = NULL;
	Tbl_Init pInitFunc = NULL;
	TblPrority* pTblPro = NULL;
	TBL_ELEMENT_HEAD* pTblData = NULL;
	if(pTblPro != NULL  || pTblData != NULL)
	{
		ret = RET_ERR;
		return ret;
	}
	/*propertity initial*/
	pTblPro = (TblPrority*)malloc(sizeof(*pTblPro) * TBL_ID_MAX);
	if (pTblPro == NULL)
	{
		ret = RET_MEM_MALLOC;
		return ret;		
	}
	memset(pTblPro,0, sizeof(*pTblPro) * TBL_ID_MAX);
	for(uiTblindex = 0; uiTblindex < g_TblSize; ++uiTblindex)
	{
		printf("%d: tblId=%d\n",uiTblindex, Table[uiTblindex].uiTblId);
		if ((Table[uiTblindex].uiTblId > TBL_ID_MIN) && (Table[uiTblindex].uiTblId < TBL_ID_MAX))
		{
			pTblPriNode = pTblPro+Table[uiTblindex].uiTblId;
			pTblPriNode->TblSize = Table[uiTblindex].TblSize;
			pTblPriNode->pFunc = Table[uiTblindex].pFunc;
			pTblPriNode->uiDataSize = Table[uiTblindex].uiDataSize;
			pTblPriNode->uiKeySize = Table[uiTblindex].uiKeySize;
			pTblPriNode->uiTblId = Table[uiTblindex].uiTblId;
			pTblPriNode->uiTblStoreMode = Table[uiTblindex].uiTblStoreMode;
		}
	}
	*pTblmPro = pTblPro;
	/*data initial*/
	pTblData = (TBL_ELEMENT_HEAD*) malloc(sizeof(*pTblData) * TBL_ID_MAX);
	if (pTblData == NULL)
	{
		ret = RET_MEM_MALLOC;
		return ret;			
	}
	memset(pTblData,0,sizeof(*pTblData) * TBL_ID_MAX);
	
	printf("--------------------------------------------------\n");
	for (uiTblindex = 0; uiTblindex < g_TblSize; ++uiTblindex)
	{
		printf("%d:tbld=%d\n",uiTblindex, Table[uiTblindex].uiTblId);
		if ((Table[uiTblindex].uiTblId > TBL_ID_MIN) && (Table[uiTblindex].uiTblId < TBL_ID_MAX))
		{
			pElemHead = pTblData + Table[uiTblindex].uiTblId;
			pElemHead->pEleHead = NULL;
			pElemHead->uiKeyLen = Table[uiTblindex].uiKeySize;
			pElemHead->uiDataLen = Table[uiTblindex].uiDataSize;
			pElemHead->uiElemCnt = 0;
			pInitFunc = g_auStrModeFuncTbl[Table[uiTblindex].uiTblStoreMode].pInitFunc;
			if (pInitFunc != NULL)
			{
				pElemHead->pEleHead = (TBL_ELEMENT*)pInitFunc(Table[uiTblindex].TblSize,Table[uiTblindex].uiKeySize,Table[uiTblindex].uiDataSize);
			}
		}
	}
	*pTblmData = pTblData;
	return RET_OK;
}

unsigned int TBLM_Destroy(TblPrority** pTblmPro,TBL_ELEMENT_HEAD** pTblmData)
{
	unsigned int ret = RET_OK;
	TblPrority* pTblPro = *pTblmPro;
	TBL_ELEMENT_HEAD* pTblData = *pTblmData;
	unsigned int TblIndex;
	TblPrority* pTblPriNode = NULL;
	TBL_ELEMENT_HEAD* pElemHead = NULL;
	TBL_ELEMENT* pElem = NULL;
	if (pTblmPro==NULL || pTblmData==NULL)
	{
		return RET_OK;
	}

	for(TblIndex = 0;TblIndex<g_TblSize;++g_TblSize)
	{
		pTblPriNode = pTblPro + Table[TblIndex].uiTblId;
		if (pTblPriNode != NULL)
		{
			free(pTblPriNode);
			pTblPriNode = NULL;
		}
	}
	free(pTblPro);
	pTblPro = NULL;
	* pTblmPro = NULL;

	for(TblIndex = 0;TblIndex<g_TblSize;++g_TblSize)
	{
		pElemHead = pTblData + Table[TblIndex].uiTblId;
		if (pElemHead != NULL)
		{
			pElem = pElemHead->pEleHead;
			if (pElem != NULL)
			{
				if(pElem->pEleKey != NULL)
				{
					free(pElem->pEleKey);
				}

				if(pElem->pEleData != NULL)
				{
					free(pElem->pEleData);
				}
				free(pElem);
				pElem = NULL;
			}
			free(pElemHead);
			pElemHead = NULL;
		}
	}
	free(pTblData);
	pTblData = NULL;
	* pTblmData = NULL;
	return RET_OK;
}

unsigned int TBLM_Add(unsigned int TblId, TBL_KEY* pTblKey,TBL_DATA* pTblData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	TblPrority* pTblPriNode = NULL;
	unsigned int ret = RET_OK;	
	Tbl_Add pAdd = NULL;
	if ((TblId < TBL_ID_MIN) || (TblId > TBL_ID_MAX) || (pTblKey == NULL) || (pTblData == NULL))
	{
		return RET_PARA_ERR;
	}
	pTblPriNode = pTblPri + TblId;
	
	if ((pTblPriNode->uiTblStoreMode <= TBL_STR_MODE_MIN) && (pTblPriNode->uiTblStoreMode >= TBL_STR_MODE_MAX))
	{
		return RET_PARA_ERR;
	}
	
	pAdd = g_auStrModeFuncTbl[pTblPriNode->uiTblStoreMode].pAddFunc;
	if (pAdd == NULL)
	{
		return RET_PARA_ERR;
	}
	ret = pAdd(TblId,pTblKey,pTblData,pTblPri,pTblDataHead);
	if(ret != RET_OK)
	{
		printf("add error,TblId=%d,\n",TblId);
		return RET_ERR;
	}
	return ret;
}

unsigned int TBLM_Delete(unsigned int TblId, TBL_KEY* pTblKey,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	unsigned int ret = RET_OK;
	TblPrority* pTblPriNode = 	NULL;
	Tbl_Del pDelFun = NULL;
	if ((TblId < TBL_ID_MIN) || (TblId > TBL_ID_MAX) || (pTblKey == NULL) )
	{
		return RET_PARA_ERR;
	}
	pTblPriNode = 	pTblPri + TblId;
	if (pTblPriNode == NULL)
	{
		printf("tbl priority init error, tblId = %d\n",TblId);
		return RET_ERR;
	}

	if (pTblPriNode->uiTblStoreMode <= TBL_STR_MODE_MIN || pTblPriNode->uiTblStoreMode >= TBL_STR_MODE_MAX)
	{
		printf("tblMode error,tblId=%d, mode=%d\n",TblId,pTblPriNode->uiTblStoreMode);
		return RET_PARA_ERR;
	}
	pDelFun = g_auStrModeFuncTbl[pTblPriNode->uiTblStoreMode].pDelFunc;
	if (pDelFun == NULL)
	{
		printf("del func error is null,mode=%d \n",pTblPriNode->uiTblStoreMode);
		return RET_PARA_ERR;
	}
	ret = pDelFun(TblId,pTblKey, pTblPri, pTblDataHead);
	if (ret != RET_OK)
	{
		printf("del error,tblId=%d\n",TblId);
		return RET_ERR;
	}
	return RET_OK;
}

unsigned int TBLM_Query(unsigned int TblId, TBL_KEY* pTblKey,TBL_DATA* pTblData,TblPrority* pTblPri,TBL_ELEMENT_HEAD* pTblDataHead)
{
	int ret = RET_OK;
	TblPrority* pTblPriNode = NULL;
	Tbl_Query pDelQury = NULL;
	if ((TblId < TBL_ID_MIN) || (TblId > TBL_ID_MAX) || (pTblKey == NULL) || (pTblData == NULL))
	{
		return RET_PARA_ERR;
	}
	pTblPriNode = 	pTblPri + TblId;
	if (pTblPriNode == NULL)
	{
		printf("tbl priority init error, tblId = %d\n",TblId);
		return RET_ERR;
	}

	if (pTblPriNode->uiTblStoreMode <= TBL_STR_MODE_MIN || pTblPriNode->uiTblStoreMode >= TBL_STR_MODE_MAX)
	{
		printf("tblMode error,tblId=%d, mode=%d\n",TblId,pTblPriNode->uiTblStoreMode);
		return RET_PARA_ERR;
	}
	pDelQury = g_auStrModeFuncTbl[pTblPriNode->uiTblStoreMode].pQueryFunc;
	if (pDelQury == NULL)
	{
		printf("query func error is null,mode=%d \n",pTblPriNode->uiTblStoreMode);
		return RET_PARA_ERR;
	}
	ret = pDelQury(TblId,pTblKey, pTblData,pTblPri, pTblDataHead);
	if (ret != RET_OK)
	{
		printf("query error,tblId=%d\n",TblId);
		return RET_ERR;
	}
	return RET_OK;	
}

unsigned int TBLM_SetKeyAndData(TBL_KEY* KEY,void* pKey,int uiKeyLen,TBL_DATA* DATA,void* pData,int uiDataLen)
{
	if ((KEY == NULL) || (pKey == NULL) || (DATA==NULL) || (pData==NULL))
	{
		return RET_PARA_ERR;
	}
	KEY->len = uiKeyLen;
	KEY->pKey = (void*)malloc(uiKeyLen);
	if(KEY->pKey == NULL)
	{
		return RET_MEM_MALLOC;
	}
	memset(KEY->pKey,0,uiKeyLen);
	memcpy(KEY->pKey,pKey,uiKeyLen);

	DATA->len = uiDataLen;
	DATA->pData = (void*)malloc(uiDataLen);
	if (DATA->pData == NULL)
	{
		return RET_MEM_MALLOC;
	}
	memset(DATA->pData,0,uiDataLen );
	memcpy(DATA->pData,pData,uiDataLen);
	return RET_OK;
}


/*input data: integer array*/
unsigned int uiIntCalHashIndex(void* key, int uiLen)
{
    int *data = (int*)key;
    int uiHashIndex = 0;
    int num = uiLen / 4;
    int i;
    if ((key != NULL) && (uiLen > 0) )
    {
        for (i = 0; i < num; ++i)
        {
            uiHashIndex += *(data + i);
        }
    }
    return uiHashIndex;
}


// RS Hash Function 
unsigned int RSHash(char*str) 
{ 
    unsigned int b=378551 ; 
    unsigned int a=63689 ; 
    unsigned int hash=0 ; 
    while(*str) 
    { 
        hash=hash*a+(*str++); 
        a*=b ; 
    } 
    return(hash % M); 
} 
 
// JS Hash Function 
unsigned int JSHash(char*str) 
{ 
    unsigned int hash=1315423911 ; 
    while(*str) 
    { 
        hash^=((hash<<5)+(*str++)+(hash>>2)); 
    } 
    return(hash % M); 
} 
 
// P. J. Weinberger Hash Function 
unsigned int PJWHash(char*str) 
{ 
    unsigned int BitsInUnignedInt=(unsigned int)(sizeof(unsigned int)*8); 
    unsigned int ThreeQuarters=(unsigned int)((BitsInUnignedInt*3)/4); 
    unsigned int OneEighth=(unsigned int)(BitsInUnignedInt/8); 
    unsigned int HighBits=(unsigned int)(0xFFFFFFFF)<<(BitsInUnignedInt-OneEighth); 
    unsigned int hash=0 ; 
    unsigned int test=0 ; 
    while(*str) 
    { 
        hash=(hash<<OneEighth)+(*str++); 
        if((test=hash&HighBits)!=0) 
        { 
            hash=((hash^(test>>ThreeQuarters))&(~HighBits)); 
        } 
    } 
    return(hash % M); 
} 
 
// ELF Hash Function 
unsigned int ELFHash(char*str) 
{ 
    unsigned int hash=0 ; 
    unsigned int x=0 ; 
    while(*str) 
    { 
        hash=(hash<<4)+(*str++); 
        if((x=hash&0xF0000000L)!=0) 
        { 
            hash^=(x>>24); 
            hash&=~x ; 
        } 
    } 
    return(hash % M); 
} 
 
// BKDR Hash Function 
unsigned int BKDRHash(char*str) 
{ 
    unsigned int seed=131 ;// 31 131 1313 13131 131313 etc.. 
    unsigned int hash=0 ; 
    while(*str) 
    { 
        hash=hash*seed+(*str++); 
    } 
    return(hash % M); 
} 
 
// SDBM Hash Function 
unsigned int SDBMHash(char*str) 
{ 
    unsigned int hash=0 ; 
    while(*str) 
    { 
        hash=(*str++)+(hash<<6)+(hash<<16)-hash ; 
    } 
    return(hash % M); 
} 
 
// DJB Hash Function 
unsigned int DJBHash(char*str) 
{ 
    unsigned int hash=5381 ; 
    while(*str) 
    { 
        hash+=(hash<<5)+(*str++); 
    } 
    return(hash % M); 
} 
 
// AP Hash Function 
unsigned int APHash(char*str) 
{ 
    unsigned int hash=0 ; 
    int i ; 
    for(i=0;*str;i++) 
    { 
        if((i&1)==0) 
        { 
            hash^=((hash<<7)^(*str++)^(hash>>3)); 
        } 
        else 
        { 
            hash^=(~((hash<<11)^(*str++)^(hash>>5))); 
        } 
    } 
    return(hash % M); 
}


int main()
{
	TblPrority* g_TblProInfp = NULL;
	TBL_ELEMENT_HEAD* g_ElementHead = NULL;
	//printf("tbl size= %d\n",g_TblSize);
	int ret = RET_OK;
	TBL_KEY userA_Key = {0};
	TBL_DATA userA_Data = {0};
	char* name ;
	int name_len = 0;	
	int age = 0;
	int age_len=sizeof(age);
	
	ret = TBLM_Init(&g_TblProInfp,&g_ElementHead);
	if (ret != RET_OK)
	{
		TBLM_Destroy(&g_TblProInfp,&g_ElementHead);
		return ret;
	}

	/*add.del.query function: example:寰€TBL_ID_AGE琛ㄤ腑娣诲姞璁板綍:name--age*/
/*Add function*/
	/*绗竴涓汉*/	
	name = "xiaoming";
	name_len = strlen(name);
	age = 20;
	ret = TBLM_SetKeyAndData(&userA_Key,name,name_len,&userA_Data,&age,age_len);
	if (ret != RET_OK)
	{
		printf("set Key data error\n");
		return RET_ERR;
	}
	ret = TBLM_Add(TBL_ID_AGE,&userA_Key,&userA_Data,g_TblProInfp,g_ElementHead);
	if (ret != RET_OK)
	{
		return RET_ERR;
	}
	printf("add ok, key=0x%08x,data=0x%08x\n",userA_Key.pKey,userA_Data.pData);
    /*绗簩涓汉*/
	name = "xiaohong";
	name_len = strlen(name);
	age = 21;
	ret = TBLM_SetKeyAndData(&userA_Key,name,name_len,&userA_Data,&age,age_len);
	if (ret != RET_OK)
	{
		printf("set Key data error\n");
		return RET_ERR;
	}
	ret = TBLM_Add(TBL_ID_AGE,&userA_Key,&userA_Data,g_TblProInfp,g_ElementHead);
	if (ret != RET_OK)
	{
		return RET_ERR;
	}
	printf("add ok, key=%08x,data=%08x\n",userA_Key.pKey,userA_Data.pData);
	/*绗笁涓汉*/
	name= "xiaogang";
	name_len = strlen(name);
	age = 20;
	ret = TBLM_SetKeyAndData(&userA_Key,name,name_len,&userA_Data,&age,age_len);
	if (ret != RET_OK)
	{
		printf("set Key data error\n");
		return RET_ERR;
	}
	ret = TBLM_Add(TBL_ID_AGE,&userA_Key,&userA_Data,g_TblProInfp,g_ElementHead);
	if (ret != RET_OK)
	{
		printf("add error, tblid=%d\n",TBL_ID_AGE);
		return RET_ERR;
	}
	printf("add ok, key=%08x,data=%08x\n",userA_Key.pKey,userA_Data.pData);
	
	printf("11111111111111111111111111\r\n");
/*Delete function*/
	name = "xiaogang";
	name_len = strlen(name);
	ret = TBLM_SetKeyAndData(&userA_Key,name,name_len,&userA_Data,&age,age_len);
	if (ret != RET_OK)
	{
		printf("set Key data error\n");
		return RET_ERR;
	}
	TBLM_Delete(TBL_ID_AGE,&userA_Key,g_TblProInfp,g_ElementHead);
	if (ret != RET_OK)
	{
		printf("del error, tblid=%d\n",TBL_ID_AGE);
		return RET_ERR;
	}
	printf("delete ok, key=%08x,data=%08x\n",userA_Key.pKey,userA_Data.pData);
	printf("2222222222222222222\r\n");
/*Query funtion*/
	name = "xiaogang";
	name_len = strlen(name);
	age = 0;
	ret = TBLM_SetKeyAndData(&userA_Key,name,name_len,&userA_Data,&age,age_len);
	if (ret != RET_OK)
	{
		printf("set Key data error\n");
		return RET_ERR;
	}
	ret = TBLM_Query(TBL_ID_AGE,&userA_Key,&userA_Data,g_TblProInfp,g_ElementHead);
	if (ret != RET_OK)
	{
		printf("query error, tblid=%d\n",TBL_ID_AGE);
		return RET_ERR;
	}
	printf("query ok, key=%08x,data=%08x\n",userA_Key.pKey,userA_Data.pData);
	printf("33333333333333333333333\r\n");
/*Delete function*/
	name = "xiaogang";
	name_len = strlen(name);
	ret = TBLM_SetKeyAndData(&userA_Key,name,name_len,&userA_Data,&age,age_len);
	if (ret != RET_OK)
	{
		printf("set Key data error\n");
		return RET_ERR;
	}
	TBLM_Delete(TBL_ID_AGE,&userA_Key,g_TblProInfp,g_ElementHead);
	if (ret != RET_OK)
	{
		printf("del error, tblid=%d\n",TBL_ID_AGE);
		return RET_ERR;
	}
	printf("delete ok, key=%08x,data=%08x\n",userA_Key.pKey,userA_Data.pData);

}

