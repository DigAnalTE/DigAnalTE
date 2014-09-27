
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
void GetBPAName(char*Name, char*BPAName, float baseKV);//反向操作
void GenerateBPABranchName(char*Name, char*Name1, float baseKV1, char*Name2, float baseKV2, char ID);

const int  AREA_PARA = 14;
//                                    0     1    2      3      4    5    6    7    8    9    10   11   12   13
const char AREA_Para[AREA_PARA][10] = { "A10", "A8", "F4.0", "F8.0", "A2", "A2", "A2", "A2", "A2", "A2", "A2", "A2", "A2", "A2" };
const int  AREA_Loca[AREA_PARA] = { 3, 13, 21, 26, 35, 38, 41, 44, 47, 50, 53, 56, 59, 62 };

const int  BUS_PARA = 14;
//                                  0    1    2      3    4      5      6      7      8      9      10     11     12     13
const char BUS_Para[BUS_PARA][10] = { "A3", "A8", "F4.0", "A2", "F5.0", "F5.0", "F4.0", "F4.0", "F4.0", "F5.0", "F5.0", "F5.0", "F4.3", "F4.3" };
const int  BUS_Loca[BUS_PARA] = { 3, 6, 14, 18, 20, 25, 30, 34, 38, 42, 47, 52, 57, 61 };

const int  LBRANCH_PARA = 19;
//                                          0    1    2      3    4    5      6    7      8    9      10     11     12     13     14     15   16   17   18
const char LBRANCH_Para[LBRANCH_PARA][10] = { "A3", "A8", "F4.0", "I1", "A8", "F4.0", "A1", "F4.0", "I1", "F6.5", "F6.5", "F6.5", "F6.5", "F4.1", "A8", "A1", "I2", "A1", "I2" };
const int  LBRANCH_Loca[LBRANCH_PARA] = { 3, 6, 14, 18, 19, 27, 31, 33, 37, 38, 44, 50, 56, 62, 66, 74, 75, 77, 78 };

const int  LPLUS_PARA = 9;
//                                      0    1    2      3    4    5      6    7      8
const char LPLUS_Para[LPLUS_PARA][10] = { "A3", "A8", "F4.0", "I1", "A8", "F4.0", "A1", "F5.0", "F5.0" };
const int  LPLUS_Loca[LPLUS_PARA] = { 3, 6, 14, 18, 19, 27, 31, 33, 43 };

const int  EBRANCH_PARA = 19;
//                                          0    1    2      3    4    5      6    7      8    9      10     11     12     13     14     15   16   17   18
const char EBRANCH_Para[EBRANCH_PARA][10] = { "A3", "A8", "F4.0", "I1", "A8", "F4.0", "A1", "F4.0", "I1", "F6.5", "F6.5", "F6.5", "F6.5", "F6.5", "F6.5", "A1", "I2", "A1", "I2" };
const int  EBRANCH_Loca[EBRANCH_PARA] = { 3, 6, 14, 18, 19, 27, 31, 33, 37, 38, 44, 50, 56, 62, 68, 74, 75, 77, 78 };

const int  TBRANCH_PARA = 19;
//                                          0    1    2      3    4    5      6    7      8    9      10     11     12     13     14     15   16   17   18
const char TBRANCH_Para[TBRANCH_PARA][10] = { "A3", "A8", "F4.0", "I1", "A8", "F4.0", "A1", "F4.0", "I1", "F6.5", "F6.5", "F6.5", "F6.5", "F5.2", "F5.2", "A1", "I2", "A1", "I2" };
const int  TBRANCH_Loca[TBRANCH_PARA] = { 3, 6, 14, 18, 19, 27, 31, 33, 37, 38, 44, 50, 56, 62, 67, 74, 75, 77, 78 };

const int  PZ_PARA = 5;
//                                0    1      2      3      4
const char PZ_Para[PZ_PARA][10] = { "A3", "F5.0", "F5.0", "F5.0", "F5.0" };
const int  PZ_Loca[PZ_PARA] = { 3, 9, 15, 21, 27 };

#endif
