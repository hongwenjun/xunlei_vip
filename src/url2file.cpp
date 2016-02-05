/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Download a given URL into a local file named page.out.
 * </DESC>
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * 许多在Linux下开发的C程序都需要头文件unistd.h，但VC中没有个头文件，
 * 所以用VC编译总是报错。把下面的内容保存为unistd.h，可以解决这个问题。
 */
#ifdef _WIN32
#include <io.h>
#include <process.h>
#else
#include<unistd.h>
#endif

#include <curl/curl.h>


static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
    return written;
}

// 保存URL文件内容到文件
bool url2file(const char* web_url, const char* savefile)
{
    CURL* curl_handle;
    const char* pagefilename = savefile;
    FILE* pagefile;


    curl_global_init(CURL_GLOBAL_ALL);

    /// init the curl session  初始化cURL协议
    curl_handle = curl_easy_init();

    /// set URL to get here   设置URL到这里
    curl_easy_setopt(curl_handle, CURLOPT_URL, web_url);

    /// Switch on full protocol/debug output while testing  测试时开启完整的协议/调试输出
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);

    /// disable progress meter, set to 0L to enable and disable debug output   禁用进度表，设置为0L启用和禁用调试输出
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

    /// send all data to this function  所有数据发送给这个函数
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    /// open the file   写文件到结果文件
    pagefile = fopen(pagefilename, "wb");
    if (pagefile) {

        /// write the page body to this file handle  写页面文件到保存文件句柄
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

        /* get it! */
        curl_easy_perform(curl_handle);

        /* close the header file */
        fclose(pagefile);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    return true;
}
