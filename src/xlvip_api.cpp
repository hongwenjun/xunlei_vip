#include "xlvip_api.h"


// 检查一个文件是否存在
BOOL IsFileExist(LPCTSTR lpFileName)
{
    WIN32_FIND_DATA fd = {0};
    HANDLE hFind = FindFirstFile(lpFileName, &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
    }
    return ((hFind != INVALID_HANDLE_VALUE) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

// 获得文件大小
size_t get_fileSize(const char* filename)
{
    FILE* pfile = fopen(filename, "rb");
    fseek(pfile, 0, SEEK_END);
    size_t size = ftell(pfile);
    fclose(pfile);
    return size;

}

// 获取gz文件的数据大小
size_t get_gzbinSize(const char* filename)
{
    const int BUFSIZE = 1024 * 1024;
    char* buf = new char[BUFSIZE];
    int data_size = 0;  int cnt = 0;
    gzFile gzf = gzopen(filename, "rb");
    while ((cnt = gzread(gzf, buf,   BUFSIZE))  > 0)
        data_size += cnt;

    gzclose(gzf);
    delete[] buf;
    return data_size;

}
// 功能 获得当前路径
char* GetAppDir(char* szPath)
{
    char* ret = szPath;
    GetModuleFileName(NULL, szPath, MAX_PATH); // 得到当前执行文件的文件名（包含路径）
    *(strrchr(szPath, '\\')) = '\0';    // 删除文件名，只留下目录
    return ret;
}

// 得到全路径文件的文件名
const char* GetFileBaseName(const char* szPath)
{
    const char* ret = szPath + strlen(szPath);
    while ((*ret != '\\') && (ret != (szPath - 1))) // 得到文件名
        ret--;
    ret++;
    return ret;
}


// 检查html文件是否压缩，如果是 gz压缩先解压文件
int html_gzdecode(const char* filename)
{
    FILE* pFile = fopen(filename, "rb+"); // 数据输入文件
    long data_size = get_fileSize(filename);
    int32_t gz_header_3byte = GZ_HEADER_3BYTE;
    fread(&gz_header_3byte, 1, sizeof(gz_header_3byte), pFile);
    rewind(pFile);

    // 判断是否是gz文件
    bool is_gzfile = (GZ_HEADER_3BYTE == (gz_header_3byte & 0xFFFFFF));

    char* buffer = NULL;
    if (is_gzfile) {
        // 读取gz文件到内存
        data_size = get_gzbinSize(filename);

        gzFile gzf = gzopen(filename, "rb");
        buffer = new char[data_size + 1];
        buffer[data_size] = 0;

        if (gzread(gzf, buffer,  data_size)  < 0)
            return -1;
        gzclose(gzf);

        fwrite(buffer, sizeof(char), data_size, pFile);

        delete[] buffer;
    }
    fclose(pFile);

    return data_size;
}
