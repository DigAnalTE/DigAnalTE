
#ifndef __BPA_Format_Read_Write_H__
#define __BPA_Format_Read_Write_H__

#include "../../DigAnalTE/DigAnalTE/CommonFunction/constdef.h"

//本类开放几个读写BPA固定格式数据的数据端口
void GetItemFromLine(char* Line, void *value, const char f_para[10], int f_loca);
void PutItemToLine(char* Line, void *value, const char f_para[10], int f_loca);
bool IsBlank(char* Line, int len);
bool inline IsZero(real v)
{
	return (fabs(v) < 0.00001);
}
void ResetBPAName(char*Name, float baseKV);//由于BPA中是使用名称和电压等级同时决定一个名称，所以这里需要转化成独特的名称
void GenerateBPABranchName(char*Name, char*Name1, float baseKV1, char*Name2, float baseKV2, char ID);

const int  AREA_PARA = 14;
//                                    0     1    2      3      4    5    6    7    8    9    10   11   12   13
const char AREA_Para[AREA_PARA][10]={"A10","A8","F4.0","F8.0","A2","A2","A2","A2","A2","A2","A2","A2","A2","A2"};
const int  AREA_Loca[AREA_PARA]    ={ 3,    13,  21,    26,    35,  38,  41,  44,  47,  50,  53,  56,  59,  62};

const int  BUS_PARA = 14;
//                                  0    1    2      3    4      5      6      7      8      9      10     11     12     13
const char BUS_Para[BUS_PARA][10]={"A3","A8","F4.0","A2","F5.0","F5.0","F4.0","F4.0","F4.0","F5.0","F5.0","F5.0","F4.3","F4.3"};
const int  BUS_Loca[BUS_PARA]    ={ 3,   6,   14,    18,  20,    25,    30,    34,    38,    42,    47,    52,    57,    61};

const int  PLUS_PARA = 10;
//                                    0    1    2      3    4      5      6      7      8      9
const char PLUS_Para[PLUS_PARA][10]={"A3","A8","F4.0","A2","F5.0","F5.0","F4.0","F4.0","F5.0","F5.0"};
const int  PLUS_Loca[PLUS_PARA]    ={ 3,   6,   14,    18,  20,    25,    30,    34,    42,    47};

const int  LBRANCH_PARA = 19;
//                                          0    1    2      3    4    5      6    7      8    9      10     11     12     13     14     15   16   17   18
const char LBRANCH_Para[LBRANCH_PARA][10]={"A3","A8","F4.0","I1","A8","F4.0","A1","F4.0","I1","F6.5","F6.5","F6.5","F6.5","F4.1","A8",  "A1","I2","A1","I2"};
const int  LBRANCH_Loca[LBRANCH_PARA]    ={ 3,   6,   14,    18,  19,  27,    31,  33,    37,  38,    44,    50,    56,    62,    66,    74,  75,  77,  78};

const int  LPLUS_PARA = 9;
//                                      0    1    2      3    4    5      6    7      8
const char LPLUS_Para[LPLUS_PARA][10]={"A3","A8","F4.0","I1","A8","F4.0","A1","F5.0","F5.0"};
const int  LPLUS_Loca[LPLUS_PARA]    ={ 3,   6,   14,    18,  19,  27,    31,  33,    43};

const int  EBRANCH_PARA = 19;
//                                          0    1    2      3    4    5      6    7      8    9      10     11     12     13     14     15   16   17   18
const char EBRANCH_Para[EBRANCH_PARA][10]={"A3","A8","F4.0","I1","A8","F4.0","A1","F4.0","I1","F6.5","F6.5","F6.5","F6.5","F6.5","F6.5","A1","I2","A1","I2"};
const int  EBRANCH_Loca[EBRANCH_PARA]    ={ 3,   6,   14,    18,  19,  27,    31,  33,    37,  38,    44,    50,    56,    62,    68,    74,  75,  77,  78};

const int  TBRANCH_PARA = 19;
//                                          0    1    2      3    4    5      6    7      8    9      10     11     12     13     14     15   16   17   18
const char TBRANCH_Para[TBRANCH_PARA][10]={"A3","A8","F4.0","I1","A8","F4.0","A1","F4.0","I1","F6.5","F6.5","F6.5","F6.5","F5.2","F5.2","A1","I2","A1","I2"};
const int  TBRANCH_Loca[TBRANCH_PARA]    ={ 3,   6,   14,    18,  19,  27,    31,  33,    37,  38,    44,    50,    56,    62,    67,    74,  75,  77,  78};

const int  PZ_PARA = 5;
//                                0    1      2      3      4
const char PZ_Para[PZ_PARA][10]={"A3","F5.0","F5.0","F5.0","F5.0"};
const int  PZ_Loca[PZ_PARA]    ={ 3,   9,     15,    21,    27};

const int  SWI_MC_PARA = 18;
//                                        0    1      2    3      4      5      6      7      8
const char SWI_MC_Para[SWI_MC_PARA][10]={"A8","F4.0","A1","F6.0","F3.2","F3.2","F4.0","F4.4","F5.4"};
const int  SWI_MC_Loca[SWI_MC_PARA]    ={ 3,   11,    15,  16,    22,    25,    28,    32,    36};

const int  SWI_LN_PARA = 18;
//                                        0    1      2    3      4    5      6    7      8    9
const char SWI_LN_Para[SWI_LN_PARA][10]={"A8","F4.0","A8","F4.0","A8","F4.0","A8","F4.0","A8","F4.0"};
const int  SWI_LN_Loca[SWI_LN_PARA]    ={ 3,   11,    18,  26,    33,  41,    48,  56,    63,  71};

const int  SWI_FF_PARA = 2;
//                                        0      1
const char SWI_FF_Para[SWI_FF_PARA][10]={"F3.0","F5.0"};
const int  SWI_FF_Loca[SWI_FF_PARA]    ={ 8,     12};

///////////////////输出卡片//////////////////////////////
const int  OUT_GEN_PARA = 6;
//                                          0    1      2    3    4      5
const char OUT_GEN_Para[OUT_GEN_PARA][10]={"A8","F4.0","A1","A8","F4.0","A1"};
const int  OUT_GEN_Loca[OUT_GEN_PARA]    ={ 3,   11,    16,  62,  70,    74};

const int  OUT_GH_PARA = 3;
//                                        0    1      2
const char OUT_GH_Para[OUT_GH_PARA][10]={"A8","F4.0","A1"};
const int  OUT_GH_Loca[OUT_GH_PARA]    ={ 6,   14,    19};

const int  SWI_LS_LINE_PARA = 10;
//                                                  0    1      2    3      4    5    6      7      8      9
const char SWI_LS_LINE_Para[SWI_LS_LINE_PARA][10]={"A8","F4.0","A8","F4.0","A1","I2","F6.0","F6.0","F6.0","F6.0"};
const int  SWI_LS_LINE_Loca[SWI_LS_LINE_PARA]    ={ 4,   12,    18,  26,    31,  35,  39,    45,    51,    57};

#endif
