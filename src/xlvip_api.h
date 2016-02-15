#ifndef XLVIP_API_H_INCLUDED
#define XLVIP_API_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>
#include <zlib.h>


#define GZ_HEADER_3BYTE 0x088B1F



// 获取gz文件的数据大小
size_t get_gzbinSize(const char* filename);

// 检查一个文件是否存在
BOOL IsFileExist(LPCTSTR lpFileName);

// 获得文件大小
size_t get_fileSize(const char* filename);

// 功能 获得当前路径
char* GetAppDir(char* szPath);

// 得到全路径文件的文件名
const char* GetFileBaseName(const char* szPath);

// 检查html文件是否压缩，如果是 gz压缩先解压文件
int html_gzdecode(const char* filename);




#endif // XLVIP_API_H_INCLUDED
