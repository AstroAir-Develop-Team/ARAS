/*
 * aras.cpp
 * 
 * Copyright (C) 2020-2022 Max Qian
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/************************************************* 
 
Copyright: 2020-2022 Max Qian. All rights reserved
 
Author:Max Qian

E-mail:astro_air@126.com
 
Date:2022-1-30
 
Description:Main program of astroair server
 
**************************************************/

#include "aras.hpp"

#include "core/wsserver.hpp"

#include "getopt.h"
#include <iomanip>

ServerSetting *Server_Setting;
extern ServerSetting *Server_Setting;
BasicInfo Basic_Info;

static const char *options = "hvp:t:";
static const char *helps = "";
static const struct option long_option[] = {
    {"help" , no_argument , nullptr , 'h'},
    {"version" , no_argument , nullptr , 'v'},
    {"port" , required_argument , nullptr , 'p'},
    {"timeout" , required_argument , nullptr , 't'}
};

#define CoreVerion 1.0

void PrintUsage()
{
    /*后续有命令行了再说*/
    fprintf(stderr, "AstroAir Server Core Version is %f", CoreVerion);
    fprintf(stderr, "Usage: aras [%s]",options);
}

void PrintVersion()
{
    fprintf(stderr, "AstroAir Server Core Version: %f",CoreVerion);
    fprintf(stderr, "ARAS ModLoader version: %f",ModLoader_VERSION);
    fprintf(stderr, "ARAS WS Server version: %f",WS_VERSION);
    fprintf(stderr, "ARAS Socket version: %f",SOCKET_VERSION);
}

bool CheckVersion()
{
    std::fstream in("version.txt", std::ios::in);
    if (!in.is_open())
    {
        spdlog::error("未发现版本文件，请检查");
        return false;
    }
    std::string version;
    in >> version;
    spdlog::info("软件版本为{}", version);
    return true;
}

bool CreateNewFile()
{
    configor::json j;
    j["version"] = Basic_Info.version;
    std::ofstream tmp("config/config.json", std::ios::trunc);
    //tmp << std::stew(4) << j << std::endl;
    j["ws"]["Enable"] = true;
    j["ws"]["MaxClientCount"] = 5;
    j["ws"]["MaxThreadCount"] = 5;
    j["ws"]["Timeout"] = 90;
    j["ws"]["Portal"] = 5950;

    j["ui"]["Enable"] = false;
    tmp << std::setw(4) << j << std::endl;
    tmp.close();
    if (!ARAS::CheckFile("config/config.json"))
    {
        spdlog::error("Could not create config.json");
        spdlog::warn("Did you run the programe on root");
        return false;
    }
    return true;
}

bool CommandParser(int argc, char *argv[])
{
    int opt,opt_idx;
    char * pos = nullptr;
    while((opt = getopt_long(argc,argv,options,long_option,&opt_idx) != EOF))
    {
        switch(opt)
        {
            case 'h':PrintUsage();exit(0);
            case 'v':PrintVersion();exit(0);
            case 'p':Server_Setting->Portal = atoi(optarg);break;
            case 't':Server_Setting->Timeout = atoi(optarg);break;
            default: break;
        }
    }
    return true;
}

namespace ARAS
{
	    bool CheckFile(std::string name)
    {
        std::fstream tmp(name.c_str(),std::ios::in);
        if(!tmp.is_open())
        {
            spdlog::error("Could not found {}",name);
            return false;
        }
        spdlog::info("Found {}",name);
        return true;
    }

    PlatformInfo GetSystemInfo()
    {
        PlatformInfo Info;
        struct utsname buf;
		if(uname(&buf))
		{
			spdlog::error("Couldn't get system infomation");
			exit(1);
		}
        Info.system = static_cast<std::string>(buf.sysname);
        Info.nodename = static_cast<std::string>(buf.nodename);
        Info.release = static_cast<std::string>(buf.release);
        Info.version = static_cast<std::string>(buf.version);
        Info.machine = static_cast<std::string>(buf.machine);
        Info.c_count = std::thread::hardware_concurrency();
        spdlog::debug("Get system info");
        return Info;
    }

    /*
	 * name: timestamp()
	 * @return ts:系统当前时间
	 * describe: Get server time
	 * 描述：获取服务器时间
	 * note: The time is in the UTF-8 time zone
	 */
	const char *timestamp()			//获取当前时间
	{
		static char ts[32];
		struct tm *tp;
		time_t t;
		time(&t);
		tp = localtime(&t);
		strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tp);
		return (ts);
	}	
    
    bool CrashReport(std::string msg)       //To be honest,I hope this will not be used!
    {
        spdlog::info("Make crash report...");
        /*Prepare*/
        std::string time = timestamp();
        std::string name = "crash-reports/Crash-Report_" + time + ".txt";
        std::ofstream tmp(name,std::ios::trunc);
        tmp << "---- AstroAir Server Crash Report ----" << std::endl << std::endl;
        /*Just for fun*/
        tmp << "/******" << std::endl;
        if(rand() > 5000)
            tmp << "What a pity!" << std::endl;
        else
            tmp << "Oh yeah~~~" << std::endl;
        tmp << "******/" << std::endl;
        /*Explain reason*/
        tmp << "Time: " << time << std::endl;
        tmp << "Sorry, there are some problems here that caused the server to quit suddenly." << std::endl;
        tmp << "Possible reasons are as follows:" << std::endl << std::endl;
        tmp << "    " << msg << std::endl << std::endl;
        /*System info*/
        PlatformInfo Info = GetSystemInfo();
        tmp << "-- System Details --" << std::endl << "Details:" << std::endl;
        tmp << "        AIRVersion:" << Basic_Info.version << std::endl;
        tmp << "        AIRCoreVersion:" << Basic_Info.core_version << std::endl;
        tmp << "        Operating System:" << Info.system << std::endl;
        tmp << "        Machine:" << Info.machine << std::endl;
        tmp << "        CpuCount:" << Info.c_count << std::endl;
        tmp.close();
        spdlog::info(" [OK] Save crash report ");
        return true;
    }
}

int main(int argc, char *argv[])
{
    /*解析命令行*/
    CommandParser(argc,argv);
    spdlog::info("启动服务器");
    spdlog::info("检查配置文件");
    /*检查服务器版本*/
    if (!CheckVersion())
        exit(2);
    /*检查配置文件是否参在，若不存在则创建一个*/
    if (!ARAS::CheckFile("config/config.json"))
    {
        spdlog::warn("未找到配置文件，尝试创建新的配置文件");
        if (!CreateNewFile())
        {
            spdlog::info("一些未曾与料的事情发生了");
            ARAS::CrashReport("无法创建配置文件");
            spdlog::info("请查看配置文件，谢谢！");
            exit(2);
        }
    }
    /*读取配置文件*/
    spdlog::info("正在读取配置文件");
    std::ifstream tmp("config/config.json", std::ios::binary);
    configor::json j;
    std::string js, line;
    while (getline(tmp, line))
        js.append(line);
    tmp.close();
    j = configor::json::parse(js);
    Server_Setting->Portal = (int)j["ws"]["Portal"];
    Server_Setting->MaxClient = (int)j["ws"]["MaxClientCount"];
    Server_Setting->MaxThread = (int)j["ws"]["MaxThreadCount"];
    Server_Setting->Timeout = (int)j["ws"]["Timeout"];
    Server_Setting->RunWsServer = (bool)j["ws"]["Enable"];

    Server_Setting->RunUIServer = (bool)j["ui"]["Enable"];
    spdlog::info("读取并加载完成");
    /*这里我的逻辑是这样的：先选择一种方式打开，后续仍可以开启另一种模式*/
    spdlog::info("初始化服务器");

    if (Server_Setting->RunWsServer)
    {
        spdlog::info("初始化Websocket服务器");
#include "core/wsserver.hpp"
        ARAS::ws->run(Server_Setting->Portal,j);
    }
    /*准备加载图形界面*/
    if (Server_Setting->RunUIServer)
    {
        spdlog::info("初始化图形界面");
    }
    /*我相信没有人会这样配置*/
    if (Server_Setting->RunUIServer && Server_Setting->RunWsServer)
    {
        spdlog::info("Neither running websocket server nor running gui");
        spdlog::info("I hope you can create a new way of using ARAS!");
        spdlog::info("Stopping Server ...");
    }
    return 0;
}
