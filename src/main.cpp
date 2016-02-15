#include <iostream>     // std::cout
#include <string>
#include <fstream>      // std::ifstream

#include <stdio.h>
#include <time.h>       /* time_t, struct tm, time, localtime */
#include <string.h> /* time_t, struct tm, time, localtime */

#include <string>
#include <regex>

#include "xlvip_api.h"
#include "url2file.h"

using namespace std;
int main()
{

//     抓取提供VIP网站的网页，获取当天的用户名和密码列表
//     请确保自己机器的时间同当前的日期时间相符（以后可能会更新为网络获取时间）

    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char cur_day [32];
    char yesterday [32];
    strftime(cur_day, 32, "%Y-%m-%d", timeinfo);
    printf("%s\n", cur_day);

    rawtime = rawtime - 24 * 3600;
    strftime(yesterday, 32, "%Y-%m-%d", localtime(&rawtime));
//    printf("%s\n", yesterday);



    string xlvip_ini[3] = { "http://www.521xunlei.com",
                            "<em>%s</em>.*</a></label> <a href=\"(.*\\.html)",
                            "(\\w+:\\d).*\\D+(\\d+)[<br />|</font><br />]"
                          };

    if (IsFileExist("xlvip.ini")) {

        ifstream infile("xlvip.ini");
        string line ;

        int id = 0;

        while (getline(infile, line)) {

            // 排除空行 和 注释行
            if (line.size() <  3   || line[0] == '/')
                continue;
            xlvip_ini[id++] = line;
        }
    }


//  到521xunlei页面去获取vip帐号

    string xlvip_html = xlvip_ini[0] ;
    string vipid_html;


    url2file(xlvip_html.c_str(), "index.html");
    // 检查html文件是否压缩，如果是 gz压缩先解压文件
    if (-1 == html_gzdecode("index.html"))
        return -1;

    char update_html_reg[128];
    sprintf(update_html_reg,  xlvip_ini[1].c_str(), cur_day);
    std::regex e(update_html_reg);
    ifstream infile("index.html");
    string line ;
    std::smatch m;

//    获取今天的vip帐号页面
    bool cur_day_flag = false;
    while (getline(infile, line)) {
        if (std::regex_search(line, m, e)) {
            vipid_html = m.format("$1");
            cur_day_flag = true;
            break;
        }
    }

//  获取昨天的vip帐号页面
    if (!cur_day_flag) {
        sprintf(update_html_reg, xlvip_ini[1].c_str(), yesterday);
        e = update_html_reg;
        infile.clear();
        infile.seekg(ios_base::beg);

        while (getline(infile, line)) {
            if (std::regex_search(line, m, e)) {
                vipid_html = m.format("$1");
                break;
            }
        }

    }

    cout << vipid_html << endl;
    xlvip_html = xlvip_html + "/" + vipid_html;


    url2file(xlvip_html.c_str(), vipid_html.c_str());
// 检查html文件是否压缩，如果是 gz压缩先解压文件
    if (-1 == html_gzdecode(vipid_html.c_str()))
        return -1;


    // 从VIP帐号页面获取 帐号和密码
    ifstream vipid_file(vipid_html.c_str());

    // 2016年公式   (\w+:\d).*\D+(\d+)[<br />|</font><br />]
    // 2015年公式   (\w+:\d)\D+(\d+)<br />

    std::regex vipid_reg(xlvip_ini[2]);

    while (getline(vipid_file, line)) {
        if (std::regex_search(line, m, vipid_reg)) {

            std::cout <<  m.format("$1") << "\t" << m.format("$2") << endl;
        }
    }

    infile.close();
    vipid_file.close();

    return 0;
}











