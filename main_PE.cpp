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
#include "PE_Compute.h"
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
typedef ap_int<Width>		self_int;
int main()
{
	int Vector_A[BUFFER_A_B_E_LEN]={0};
	int Vector_B[BUFFER_A_B_E_LEN]={0};
	int Vector_C[BUFFER_C_D_LEN]={0};
	int Vector_D[BUFFER_C_D_LEN]={0};
	int Vector_E[BUFFER_E_LEN]={0};
	int flag_B=0;
	int flag_A=0;
	int flag_D=0;
	int flag_C=0;
	int i=0;

    ifstream Adata;
    Adata.open("E:\\test\\HLS\\FTTU\\PE_MERGE\\PE_MERGE\\indice_number_8\\A1.txt");
    for (i=0; i<BUFFER_A_B_E_LEN; ++i)
    {
        Adata>>Vector_A[i];
    }

    ifstream Bdata;
    Bdata.open("E:\\test\\HLS\\FTTU\\PE_MERGE\\PE_MERGE\\indice_number_8\\A9.txt");
    for (i=0; i<BUFFER_A_B_E_LEN; ++i)
    {
        Bdata>>Vector_B[i];
    }
    for (int l=0; l<DATA_ARRAY_LEN; ++l)
      {
            if (Vector_A[l*DATA_ROW_LEN]!=0)
            {
                  flag_A=flag_A+1;
            }
      }
    for (int k=0; k<DATA_ARRAY_LEN; ++k)
      {
            if (Vector_B[k*DATA_ROW_LEN]!=0)
            {
                  flag_B=flag_B+1;
            }
      }
    /*计算Vector_C[72]*/
	PE_ComputeUnit_1(Vector_A, Vector_B, Vector_C, flag_A, flag_B);

    /*计算Vector_D[72]*/
	PE_ComputeUnit_1(Vector_B, Vector_A, Vector_D, flag_B, flag_A);
	cout << " Vector_C的输出结果为" << endl;
    for (int i = 0; i < BUFFER_C_D_LEN; i++)
    {
          printf("%d ", Vector_C[i]);
          if (0 == ((i + 1) % DATA_ROW_LEN))
          {
              printf("\n");
          }
    }
	cout << " Vector_D的输出结果为" << endl;
    for (int i = 0; i < BUFFER_C_D_LEN; i++)
    {
          printf("%d ", Vector_D[i]);
          if (0 == ((i + 1) % DATA_ROW_LEN))
          {
              printf("\n");
          }
    }

    /*计算Vector_E[72]*/
    for (int l=0; l<BUFFER_C_D_LEN/DATA_ROW_LEN; ++l)
    {
        if (Vector_C[l*DATA_ROW_LEN]!=0)
        {
              flag_C=flag_C+1;
        }
    }
    for (int k=0; k<BUFFER_C_D_LEN/DATA_ROW_LEN; ++k)
    {
        if (Vector_D[k*DATA_ROW_LEN]!=0)
        {
              flag_D=flag_D+1;
        }
    }
    printf("flag_C=%d ", flag_C);
    printf("flag_D=%d ", flag_D);
	PE_ComputeUnit_merge(Vector_C, Vector_D, Vector_E, flag_C, flag_D);
	cout << " Vector_E的输出结果为" << endl;
    for (int i = 0; i < BUFFER_E_LEN; i++)
    {
          printf("%d ", Vector_E[i]);
          if (0 == ((i + 1) % DATA_ROW_LEN))
          {
              printf("\n");
          }
    }
    ofstream outfile_E;
    outfile_E.open("E:\\test\\HLS\\FTTU\\PE_MERGE\\PE_MERGE\\indice_number_8\\E8.txt");
    for (int i = 0; i < BUFFER_E_LEN; i++)
    {
       outfile_E << Vector_E[i]<<" ";
       if (0 == ((i + 1) % DATA_ROW_LEN))
       {
           outfile_E <<"\n";
       }
    }
    outfile_E.close();
    return 0;
}
