#include "hls_stream.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<string.h>
#include <stdio.h>
#include <ap_int.h>

using namespace std;

#define DICTORY_LEN    8
#define DICTORY_INVALID_VALUE    0
#define INDICE_ARRAY_LEN    4
#define QUERY_ARRAY_LEN     4

#define INDICE1_INCLUDE_INDICE2  1
#define INDICE2_INCLUDE_INDICE1  2

#define INDICE_ARRAY_LEN    8
#define QUERY_ARRAY_LEN     8
#define DATA_ARRAY_LEN      1   /*QUERY NUMBER*/
#define DATA_ROW_LEN        17    /*QUERY_ARRAY_LEN+INDICE_ARRAY_LEN+1*/
#define BUFFER_A_B_E_LEN    17  /*DATA_ARRAY_LEN*DATA_ROW_LEN*/
#define BUFFER_C_D_LEN      34  /* 2*DATA_ARRAY_LEN*DATA_ROW_LEN */
#define BUFFER_E_LEN    	68  /*4*DATA_ARRAY_LEN*DATA_ROW_LEN*/
#define Width    12


#define PE_INVALID_QUERY_INDEX QUERY_ARRAY_LEN
typedef ap_int<Width>		self_int;
/* 数据结构 */
typedef int PE_ARRAY_INDICE_S[INDICE_ARRAY_LEN];
typedef int PE_ARRAY_QUERY_S[QUERY_ARRAY_LEN];

typedef struct tagPE_DATA_UNIT{
	int     ulIndxValue;
	int               iIndiceSize;
	int               iQuerySize;
	PE_ARRAY_INDICE_S stIndiceUnit;
	PE_ARRAY_QUERY_S  stQueryUnit;
}PE_DATA_UNIT_S;

typedef struct tagPE_DATA{
	int iDataUnitSize;
    PE_DATA_UNIT_S astDataUnit[DATA_ARRAY_LEN];
}PE_DATA_S;

void PE_Calcu_Layer_DataSetEmpty(PE_DATA_UNIT_S  *pstInData,PE_DATA_UNIT_S  *pstOutData);
void PE_DATA_UNIT_Memcopy(PE_DATA_UNIT_S *pstInData, PE_DATA_UNIT_S *pstOutData);
int PE_Query_Memcopy(PE_ARRAY_QUERY_S arrayQueryIn,int indxIn,PE_ARRAY_QUERY_S arrayQueryOut,int indxOut);
int PE_Indice_Memcopy(PE_ARRAY_INDICE_S arrayIndiceIn,int indxIn,PE_ARRAY_INDICE_S arrayIndiceOut,int indxOut);
void PE_DATA_UNIT_MemZero(PE_DATA_UNIT_S *pstInData);
void PE_Query_Zero(PE_ARRAY_QUERY_S arrayQuery);
void PE_Indice_Zero(PE_ARRAY_INDICE_S arrayIndice);
void PE_ComputeUnit_2
(
	PE_DATA_UNIT_S *pstInUnit,
	PE_DATA_UNIT_S *pstDataSet,
	PE_DATA_UNIT_S *pstOutUnit,
	int *flag_reduce,
	int leng_Aquery
);
void PE_ComputeUnit_1(int *A_ddr, int *B_ddr, int *C, int flag_A, int flag_B);
int PE_Find(int indxEle, int *PE_Array, int PE_ArrayLen);
void PE_Find_flag(int indxEle, int *PE_Array, int *flag);
void PE_Sort_Descend(int *PE_Array, int PE_ArrayLen);
bool PE_Array_Equal(int *Array1,int *Array2,int ArrayLen);
bool PE_IsSubIndice(PE_ARRAY_INDICE_S stIndice1,int indiceCnt1,PE_ARRAY_INDICE_S stIndice2,int indiceCnt2);
bool PE_IndiceQueryMerged_Equal(PE_DATA_UNIT_S *pstInUnit1,PE_DATA_UNIT_S *pstInUnit2);
bool PE_DataUnit_Equal(PE_DATA_UNIT_S *pstInUnit1,PE_DATA_UNIT_S *pstInUnit2);
bool PE_IsSubSet_DataUnit(PE_DATA_UNIT_S *pstInUnit,int iDataUnitSize,PE_DATA_UNIT_S *astSetUnit,int iSetDataUnitSize);
void PE_MergeUnit
(
	PE_DATA_UNIT_S *astInDataUnit1,
	int iDataUnitSize1,
	PE_DATA_UNIT_S *astInDataUnit2,
	int iDataUnitSize2,
	PE_DATA_UNIT_S *astOutDataUnit,
    int *piOutDataUnitSize
);
void PE_ComputeUnit_merge(int *C_ddr, int *D_ddr, int *E_ddr, int flag_A, int flag_B);
