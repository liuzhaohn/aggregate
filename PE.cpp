
/* 2021-11-21 完成如下函数：
 * PE_PrintDataUnit、PE_DATA_UNIT_Memcopy、PE_Find
HLS综合代码 */
#include "hls_stream.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<string.h>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <ap_int.h>

using namespace std;

#define INDICE_ARRAY_LEN    8
#define QUERY_ARRAY_LEN     8
#define DATA_ARRAY_LEN      1   /*QUERY NUMBER*/
#define DATA_ROW_LEN        17    /*QUERY_ARRAY_LEN+INDICE_ARRAY_LEN+1*/
#define BUFFER_A_B_E_LEN    17  /*DATA_ARRAY_LEN*DATA_ROW_LEN*/
#define BUFFER_C_D_LEN      34  /* 2*DATA_ARRAY_LEN*DATA_ROW_LEN */
#define BUFFER_E_LEN    	68  /*4*DATA_ARRAY_LEN*DATA_ROW_LEN*/
#define Width    12

#define PE_INVALID_QUERY_INDEX QUERY_ARRAY_LEN

typedef ap_int<Width>		int_self;
/* 数据结构 */
typedef int PE_ARRAY_INDICE_S[INDICE_ARRAY_LEN];
typedef int PE_ARRAY_QUERY_S[QUERY_ARRAY_LEN];

typedef struct tagPE_DATA_UNIT{
    int               ulIndxValue;
    int               iIndiceSize;
    int               iQuerySize;
	PE_ARRAY_INDICE_S stIndiceUnit;
	PE_ARRAY_QUERY_S  stQueryUnit;
}PE_DATA_UNIT_S;

//PE功能函数

void PE_Indice_Zero(PE_ARRAY_INDICE_S arrayIndice)
{
	myloop:
    for (int iIndx = 0; iIndx < INDICE_ARRAY_LEN; iIndx++)
    {
        arrayIndice[iIndx] = 0;
    }

    return;
}

void PE_Query_Zero(PE_ARRAY_QUERY_S arrayQuery)
{
    for (int iIndx = 0; iIndx < QUERY_ARRAY_LEN; iIndx++)
    {
        arrayQuery[iIndx] = 0;
    }
    return;
}

void PE_DATA_UNIT_MemZero(PE_DATA_UNIT_S *pstInData)
{
    //清零
    pstInData->ulIndxValue = 0;
    pstInData->iIndiceSize = 0;
    pstInData->iQuerySize = 0;

    PE_Indice_Zero(pstInData->stIndiceUnit);
    PE_Query_Zero(pstInData->stQueryUnit);
}

int PE_Indice_Memcopy
(
    PE_ARRAY_INDICE_S arrayIndiceIn,
    int indxIn,
    PE_ARRAY_INDICE_S arrayIndiceOut,
    int indxOut
)
{
	myloop:
    for (int iIndx = 0; iIndx < indxIn; iIndx++)
    {
        if (0 != arrayIndiceIn[iIndx])
        {
            arrayIndiceOut[indxOut++] = arrayIndiceIn[iIndx];
        }
    }

    return indxOut;
}

int PE_Query_Memcopy
(
    PE_ARRAY_QUERY_S arrayQueryIn,
    int indxIn,
    PE_ARRAY_QUERY_S arrayQueryOut,
    int indxOut
)
{
    for (int iIndx = 0; iIndx < indxIn; iIndx++)
    {
        if (0 != arrayQueryIn[iIndx])
        {
            arrayQueryOut[indxOut++] = arrayQueryIn[iIndx];
        }
    }

    return indxOut;
}

void PE_DATA_UNIT_Memcopy(PE_DATA_UNIT_S *pstInData, PE_DATA_UNIT_S *pstOutData)
{
    //复制之前先对结果区清零
    PE_DATA_UNIT_MemZero(pstOutData);

    //复制data unit中的indice
    pstOutData->iIndiceSize = PE_Indice_Memcopy(pstInData->stIndiceUnit,  pstInData->iIndiceSize,
                                                pstOutData->stIndiceUnit, pstOutData->iIndiceSize);

    //复制data unit中的indice value
    pstOutData->ulIndxValue = pstInData->ulIndxValue;

    //复制data unit中的query
    pstOutData->iQuerySize = PE_Query_Memcopy(pstInData->stQueryUnit,  pstInData->iQuerySize,
                                              pstOutData->stQueryUnit, pstOutData->iQuerySize);
}

int PE_Find(int indxEle, int *PE_Array, int PE_ArrayLen)
{
    int QIndx = 0;
    for (; QIndx < PE_ArrayLen; QIndx++)
    {
        if (indxEle == PE_Array[QIndx])
        {
            //若找到，遍历一个indice元素
            break;
        }
    }

    //返回在query中的索引值，未查询到索引值为：PE_INVALID_QUERY_INDEX
    if (PE_ArrayLen == QIndx)
    {
        QIndx = PE_INVALID_QUERY_INDEX;
    }
    return QIndx;
}

void PE_Find_flag(int indxEle, int *PE_Array, int *flag, int leng_Aquery)
{
    int QIndx = 0;
    int flag_temp=0;
    myloop:
    for (; QIndx < leng_Aquery; QIndx++)
    {
        if (indxEle == PE_Array[QIndx])
        {
            //若找到，遍历一个indice元素
            flag_temp = 1;
            break;
        }
    }
    *flag = flag_temp;
    return;
}

/*PE*/
void PE_Calcu_Layer_DataSetEmpty
(
	PE_DATA_UNIT_S  *pstInData,
	PE_DATA_UNIT_S  *pstOutData
)
{
	PE_DATA_UNIT_Memcopy(pstInData, pstOutData);

	return;
}

void PE_ComputeUnit_2
(
	PE_DATA_UNIT_S *pstInUnit,
	PE_DATA_UNIT_S *pstDataSet,
	PE_DATA_UNIT_S *pstOutUnit,
	int *flag_reduce,
	int leng_Aquery
)
{
    //判断DataSet中的所有indice元素是否是InUnit中queries的子集
    bool bFindFlag;
    for (int indx = 0; indx < pstDataSet->iIndiceSize; indx++)
    {
        int flag=0;
        PE_Find_flag(pstDataSet->stIndiceUnit[indx], pstInUnit->stQueryUnit, &flag, leng_Aquery);
        if (flag == 1)
        {
            //查询到
            bFindFlag = true;
            *flag_reduce = 1;
        }
        else
        {
            bFindFlag = false;
            *flag_reduce = 0;
            break;
        }
    }
    if (true == bFindFlag)
    {
        PE_DATA_UNIT_S stDataUnitTmp;
        PE_DATA_UNIT_MemZero(&stDataUnitTmp);

        PE_DATA_UNIT_Memcopy(pstInUnit, &stDataUnitTmp);

        int QueIndx=0;
        for (int indx = 0; indx < pstDataSet->iIndiceSize; indx++)
        {
            //在in data query中删除data set indice的元素
            QueIndx = PE_Find(pstDataSet->stIndiceUnit[indx], pstInUnit->stQueryUnit, QUERY_ARRAY_LEN);
            if (PE_INVALID_QUERY_INDEX != QueIndx)
            {
                //在in data indice中添加data set indice的元素
                stDataUnitTmp.stIndiceUnit[stDataUnitTmp.iIndiceSize++]
                        =  pstDataSet->stIndiceUnit[indx];
                stDataUnitTmp.stQueryUnit[QueIndx] = 0;
                QueIndx = PE_INVALID_QUERY_INDEX;
            }
        }
        stDataUnitTmp.ulIndxValue +=  pstDataSet->ulIndxValue;
        PE_DATA_UNIT_Memcopy(&stDataUnitTmp, pstOutUnit);
    }
    else
    {
        PE_DATA_UNIT_Memcopy(pstInUnit, pstOutUnit);
    }
	return;
}

void PE_ComputeUnit_1(int *A_ddr, int *B_ddr, int *C_ddr, int flag_A, int flag_B)
{
#pragma HLS INTERFACE ap_fifo depth=17  	port=A_ddr //depth = DATA_ARRAY_LEN*DATA_ROW_LEN
#pragma HLS INTERFACE ap_fifo depth=17 		port=B_ddr //depth = DATA_ARRAY_LEN*DATA_ROW_LEN
#pragma HLS INTERFACE ap_fifo depth=34 		port=C_ddr //depth = 2*DATA_ARRAY_LEN*DATA_ROW_LEN

	int A[DATA_ARRAY_LEN][DATA_ROW_LEN]={0};
	int B[DATA_ARRAY_LEN][DATA_ROW_LEN]={0};
	int C[DATA_ARRAY_LEN][DATA_ARRAY_LEN][DATA_ROW_LEN]={0};
	int leng_Aindices=0;
	int leng_Aquery=0;
    int leng_Bindices=0;
	int leng_Bquery=0;
	int A_size=flag_A;
	int B_size=flag_B;
	PE_DATA_UNIT_S pstOutUnit;
	for (int l=0; l<A_size; ++l)
	{
#pragma HLS PIPELINE
		for (int m = 0; m < DATA_ROW_LEN; ++m)
		{
			A[l][m] = *(A_ddr++);
		}
	}
	for (int l=0; l<B_size; ++l)
	{
#pragma HLS PIPELINE
		for (int m = 0; m < DATA_ROW_LEN; ++m)
		{
			B[l][m] = *(B_ddr++);
		}
	}
	//将A_ddr赋值给数组A，并传递给pstInUnit
    for (int L=0; L<A_size; ++L)
	{
#pragma HLS UNROLL
		leng_Aindices=0;
		leng_Aquery=0;
		PE_DATA_UNIT_S pstInUnit;
		//从A_ddr中读取一个结构体到数组pstInUnit中
		pstInUnit.ulIndxValue=A[L][0];
		for (int m = 1; m < DATA_ROW_LEN; ++m)
		{
			  if (m<=INDICE_ARRAY_LEN && A[L][m]!=0)
			  {
					leng_Aindices=leng_Aindices+1;
					pstInUnit.stIndiceUnit[m-1] = A[L][m];
			  }
			  else if(m>INDICE_ARRAY_LEN && A[L][m]!=0)
			  {
					leng_Aquery=leng_Aquery+1;
					pstInUnit.stQueryUnit[m-INDICE_ARRAY_LEN-1] = A[L][m];
			  }
		}
		pstInUnit.iIndiceSize=leng_Aindices;
		pstInUnit.iQuerySize=leng_Aquery;
		//输入A的一个query与输入B的所有indices比较与合并
		int flag_reduce_sum=0;
		for (int k=0; k<B_size; ++k)
		{
#pragma HLS PIPELINE
			  leng_Bindices=0;
			  leng_Bquery=0;
			  PE_DATA_UNIT_S pstDataSet;
			  pstDataSet.ulIndxValue=B[k][0];
			  //B从A_ddr中读取一个结构体到数组pstDataSet中
			  for (int m = 1; m < DATA_ROW_LEN; ++m)
			  {
					if (m<=INDICE_ARRAY_LEN && B[k][m]!=0)
					{
						  leng_Bindices=leng_Bindices+1;
						  pstDataSet.stIndiceUnit[m-1]=B[k][m];
					}
					else if(m>INDICE_ARRAY_LEN && B[k][m]!=0)
					{
						  leng_Bquery=leng_Bquery+1;
						  pstDataSet.stQueryUnit[m-INDICE_ARRAY_LEN-1]=B[k][m];
					}
			  }
			  pstDataSet.iIndiceSize=leng_Bindices;
			  pstDataSet.iQuerySize=leng_Bquery;
			  int flag_reduce=0;
			  PE_ComputeUnit_2(&pstInUnit,&pstDataSet,&pstOutUnit,&flag_reduce, leng_Aquery);
			  //将pstOutUnit传递给数组C，并传递给C_ddr
			  if ((flag_reduce == 1)||((k == B_size-1) && flag_reduce_sum == 0))
			  {
				  flag_reduce_sum = flag_reduce_sum+1;
				  C[L][k][0]=pstOutUnit.ulIndxValue;
				  for (int i=1;i<(INDICE_ARRAY_LEN+1);++i)
				  {
						C[L][k][i]=pstOutUnit.stIndiceUnit[i-1];
				  }
				  for (int i=(INDICE_ARRAY_LEN+1);i<DATA_ROW_LEN;++i)
				  {
						C[L][k][i]=pstOutUnit.stQueryUnit[i-INDICE_ARRAY_LEN-1];
				  }
			  }
		}
	}
	for (int n = 0; n < A_size; ++n)
	{
#pragma HLS PIPELINE
		for (int l = 0; l < B_size; ++l)
		{
#pragma HLS PIPELINE
			for (int m = 0; m < DATA_ROW_LEN; ++m)
			{
#pragma HLS PIPELINE
				if (C[n][l][0] == 0)
				{
					break;
				}
				else
				{
					*(C_ddr++) = C[n][l][m];
				}
			}
		}
	}
	return;
}

/*merge*/
void PE_Sort_Descend(int *PE_Array, int PE_ArrayLen)
{
    int i, j;
    int temp;
    myloop:
    for (i = 0; i < PE_ArrayLen - 1; i++)
    {
        for (j = 0; j < PE_ArrayLen - 1 - i; j ++)
        {
            if (PE_Array[j] < PE_Array[j+1])
            {
                temp = PE_Array[j];
                PE_Array[j] = PE_Array[j+1];
                PE_Array[j+1] = temp;
            }
        }
    }
}

bool PE_Array_Equal
(
    int *Array1,
    int *Array2,
    int ArrayLen
)
{
    int i;
    myloop:
    for (i = 0; i < ArrayLen; i++)
    {
        if (Array1[i] != Array2[i])
        {
            return false;
        }
    }

    return true;
}

//indice2是否是indice1的子集
bool PE_IsSubIndice
(
    PE_ARRAY_INDICE_S stIndice1,
    int indiceCnt1,
    PE_ARRAY_INDICE_S stIndice2,
    int indiceCnt2
)
{
    int i;
    int iFind = PE_INVALID_QUERY_INDEX;
    bool bFindFlag = false;

    //按照降序排序
    PE_Sort_Descend(stIndice1, INDICE_ARRAY_LEN);
    PE_Sort_Descend(stIndice2, INDICE_ARRAY_LEN);

    if (indiceCnt1 < indiceCnt2)
    {
        //indice2是否包含indice1
    	myloop:
        for (i = 0; i < indiceCnt1; i++)
        {
            iFind = PE_Find(stIndice1[i], stIndice2, indiceCnt2);
            if (iFind == PE_INVALID_QUERY_INDEX)
            {
                bFindFlag = false;
                break;
            }
            else
            {
                bFindFlag = true;
                continue;
            }
        }
    }
    else
    {
        //indice1是否包含indice2
        for (i = 0; i < indiceCnt2; i++)
        {
            iFind = PE_Find(stIndice2[i], stIndice1, indiceCnt1);
            if (iFind == PE_INVALID_QUERY_INDEX)
            {
                bFindFlag = false;
                break;
            }
            else
            {
                bFindFlag = true;
                continue;
            }
        }
    }

    return bFindFlag;
}

bool PE_IndiceQueryMerged_Equal
(
	PE_DATA_UNIT_S *pstInUnit1,
	PE_DATA_UNIT_S *pstInUnit2
)
{
    int iDataUnit1[INDICE_ARRAY_LEN + QUERY_ARRAY_LEN] = {0};
    int iDataUnit2[INDICE_ARRAY_LEN + QUERY_ARRAY_LEN] = {0};
    int indx = 0;

    indx = PE_Indice_Memcopy(pstInUnit1->stIndiceUnit, pstInUnit1->iIndiceSize, iDataUnit1, indx);
    indx = PE_Query_Memcopy(pstInUnit1->stQueryUnit,  pstInUnit1->iQuerySize,  iDataUnit1, indx);
    PE_Sort_Descend(iDataUnit1, indx);

    indx = 0;
    indx = PE_Indice_Memcopy(pstInUnit2->stIndiceUnit, pstInUnit2->iIndiceSize, iDataUnit2, indx);
    indx = PE_Query_Memcopy(pstInUnit2->stQueryUnit,  pstInUnit2->iQuerySize,  iDataUnit2, indx);
    PE_Sort_Descend(iDataUnit2, indx);
    if (true == PE_Array_Equal(iDataUnit1, iDataUnit2, INDICE_ARRAY_LEN + QUERY_ARRAY_LEN))
    {
        //indice+query相等时，indice1包含indice2的元素
        if (true == PE_IsSubIndice(pstInUnit1->stIndiceUnit, pstInUnit1->iIndiceSize,
                                   pstInUnit2->stIndiceUnit, pstInUnit2->iIndiceSize))
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }
}

bool PE_DataUnit_Equal
(
	PE_DATA_UNIT_S *pstInUnit1,
	PE_DATA_UNIT_S *pstInUnit2
)
{
    if (true == PE_IndiceQueryMerged_Equal(pstInUnit1, pstInUnit2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PE_IsSubSet_DataUnit
(
    PE_DATA_UNIT_S *pstInUnit,
    PE_DATA_UNIT_S *astSetUnit,
    int iSetDataUnitSize
)
{
    if (0 == iSetDataUnitSize)
    {
        return false;
    }

    int i;
    myloop:
    for (i = 0; i < iSetDataUnitSize; i++)
    {
        if (true == PE_DataUnit_Equal(pstInUnit, astSetUnit + i))
        {
            //DataUnit在DataSet中已存在,indice元素个数多的替换少的;
            if (astSetUnit[i].iIndiceSize <
                pstInUnit->iIndiceSize)
            {
                PE_DATA_UNIT_Memcopy(pstInUnit, astSetUnit + i);
            }

            return true;
        }
    }

    return false;
}

void PE_MergeUnit
(
	PE_DATA_UNIT_S *astInDataUnit1,
    int iDataUnitSize1,
	PE_DATA_UNIT_S *astInDataUnit2,
    int iDataUnitSize2,
	PE_DATA_UNIT_S *astOutDataUnit,
    int *piOutDataUnitSize
)
{
    //1.DataUnit1和DataUnit2合并
    int i=0, j=0;
    int iOutDataUnitSize=0;
    int PE_DataUnitLen = iDataUnitSize1 + iDataUnitSize2;
    PE_DATA_UNIT_S astDataUnitTmp[2 * BUFFER_C_D_LEN/DATA_ROW_LEN];
    myloop:
    for (i = 0; i < 2 * BUFFER_C_D_LEN/DATA_ROW_LEN; i++)
    {
        PE_DATA_UNIT_MemZero(astDataUnitTmp + i);
        PE_DATA_UNIT_MemZero(astOutDataUnit + i);
    }

    for (i = 0; i < iDataUnitSize1; i++)
    {
        PE_DATA_UNIT_Memcopy(astInDataUnit1 + i, astDataUnitTmp + i);
    }

    for (i = 0; i < iDataUnitSize2; i++)
    {
        PE_DATA_UNIT_Memcopy(astInDataUnit2 + i, astDataUnitTmp + iDataUnitSize1 + i);
    }

    //2.按照indice元素个数自小到大排序
    PE_DATA_UNIT_S stDataUnitTmp;

    for (i = 0; i < PE_DataUnitLen - 1; i++)
    {
        for (j = 0; j < PE_DataUnitLen - 1 - i; j++)
        {
            if (astDataUnitTmp[j].iIndiceSize > astDataUnitTmp[j+1].iIndiceSize)
            {
                PE_DATA_UNIT_Memcopy(astDataUnitTmp + j, &stDataUnitTmp);
                PE_DATA_UNIT_Memcopy(astDataUnitTmp + j + 1, astDataUnitTmp + j);
                PE_DATA_UNIT_Memcopy(&stDataUnitTmp, astDataUnitTmp + j + 1);
            }
        }
    }

    //3.去重

    for (i = 0; i < PE_DataUnitLen; i++)
    {
        if (false == PE_IsSubSet_DataUnit(astDataUnitTmp + i,astOutDataUnit,iOutDataUnitSize))
        {
            PE_DATA_UNIT_Memcopy(astDataUnitTmp + i, astOutDataUnit + iOutDataUnitSize);
            iOutDataUnitSize++;
        }
    }

    //4.输出结果
    *piOutDataUnitSize = iOutDataUnitSize;

    return;
}

void PE_ComputeUnit_merge(int *C_ddr, int *D_ddr, int *E_ddr, int flag_A, int flag_B)
{
#pragma HLS INTERFACE ap_fifo depth=34  port=C_ddr //depth = BUFFER_C_D_LEN
#pragma HLS INTERFACE ap_fifo depth=34  port=D_ddr //depth = BUFFER_C_D_LEN
#pragma HLS INTERFACE ap_fifo depth=68  port=E_ddr //depth = BUFFER_E_LEN
	int A[BUFFER_C_D_LEN/DATA_ROW_LEN][DATA_ROW_LEN]={0};
	int B[BUFFER_C_D_LEN/DATA_ROW_LEN][DATA_ROW_LEN]={0};
	int C[DATA_ROW_LEN]={0};
	int leng_Aindices=0;
	int leng_Aquery=0;
    int leng_Bindices=0;
	int leng_Bquery=0;
	int A_size=flag_A;
	int B_size=flag_B;
	PE_DATA_UNIT_S pstInUnit[BUFFER_C_D_LEN/DATA_ROW_LEN];
	PE_DATA_UNIT_S pstDataSet[BUFFER_C_D_LEN/DATA_ROW_LEN];
    PE_DATA_UNIT_S pstDataOut[BUFFER_E_LEN/DATA_ROW_LEN];
    int iDataOutLen = 0;
	//将A_ddr赋值给数组A，并传递给pstInUnit
	for (int l=0; l<A_size; ++l)
	{
#pragma HLS PIPELINE
		for (int m = 0; m < DATA_ROW_LEN; ++m)
		{
			A[l][m] = *(C_ddr++);
		}
	}
    for (int L=0; L<A_size; ++L)
    {
#pragma HLS UNROLL
		leng_Aindices=0;
		leng_Aquery=0;
		pstInUnit[L].ulIndxValue=A[L][0];
		for (int m = 1; m < DATA_ROW_LEN; ++m)
		{
			  if (m<=INDICE_ARRAY_LEN && A[L][m]!=0)
			  {
					leng_Aindices=leng_Aindices+1;
					pstInUnit[L].stIndiceUnit[m-1]=A[L][m];
			  }
			  else if(m>INDICE_ARRAY_LEN && A[L][m]!=0)
			  {
					leng_Aquery=leng_Aquery+1;
					pstInUnit[L].stQueryUnit[m-INDICE_ARRAY_LEN-1] = A[L][m];
			  }
		}
		pstInUnit[L].iIndiceSize=leng_Aindices;
		pstInUnit[L].iQuerySize=leng_Aquery;
	}
	//将B_ddr赋值给数组B，并传递给pstDataSet
	for (int l=0; l<B_size; ++l)
	{
#pragma HLS PIPELINE
		for (int m = 0; m < DATA_ROW_LEN; ++m)
		{
			B[l][m] = *(D_ddr++);
		}
	}
	for (int L=0; L<B_size; ++L)
	{
#pragma HLS UNROLL
		leng_Bindices=0;
		leng_Bquery=0;
		pstDataSet[L].ulIndxValue=B[L][0];
		for (int m = 1; m < DATA_ROW_LEN; ++m)
		{
			  if (m<=INDICE_ARRAY_LEN && B[L][m]!=0)
			  {
				  	leng_Bindices=leng_Bindices+1;
					pstDataSet[L].stIndiceUnit[m-1]=B[L][m];
			  }
			  else if(m>INDICE_ARRAY_LEN && B[L][m]!=0)
			  {
				    leng_Bquery=leng_Bquery+1;
					pstDataSet[L].stQueryUnit[m-INDICE_ARRAY_LEN-1] = B[L][m];
			  }
		}
		pstDataSet[L].iIndiceSize=leng_Bindices;
		pstDataSet[L].iQuerySize=leng_Bquery;
	}
      //合并
     PE_MergeUnit(pstInUnit, A_size, pstDataSet, B_size, pstDataOut, &iDataOutLen);
      //输出
      for (int i = 0; i < iDataOutLen; i++)
      {
          C[0]=(pstDataOut+i)->ulIndxValue;
          for (int k = 1;k<=INDICE_ARRAY_LEN; k++)
          {
               C[k] = ((pstDataOut+i)->stIndiceUnit)[k-1];
          }
          for (int k = INDICE_ARRAY_LEN+1; k < DATA_ROW_LEN; k++)
          {
               C[k] = ((pstDataOut+i)->stQueryUnit)[k-INDICE_ARRAY_LEN-1];
          }
          for (int n = 0; n < DATA_ROW_LEN; ++n)
		  {
				*E_ddr = C[n];
				 E_ddr++;
		  }
      }
	  return;
}
