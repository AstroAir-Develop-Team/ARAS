/*
 * aras.hpp
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

#ifndef _ARAS_HPP_
#define _ARAS_HPP_

#include "spdlog/spdlog.h"
#include "configor/json.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <atomic>

#include <sys/utsname.h>

#include "core/modloader.hpp"

struct BasicInfo
{
    std::string version = "1.0.0";
    std::string core_version = "0.0.1-beta";
};
extern BasicInfo Basic_Info;

struct PlatformInfo
{
    std::string system;
    std::string nodename;
    std::string release;
    std::string version;
    std::string machine;
    int c_count;
};

struct ServerSetting
{
    std::atomic_int MaxThread;
    std::atomic_int MaxClient;
    std::atomic_int Timeout;

    std::atomic_bool RunWsServer;
    std::atomic_int Portal;
    std::atomic_bool SSLEnable;

    std::atomic_bool RunUIServer;
};

namespace ARAS
{
	bool CheckFile(std::string name);
	PlatformInfo GetSystemInfo();
	const char *timestamp();
	bool CrashReport(std::string msg);

    extern ServerSetting *Server_Setting;
}
#endif
