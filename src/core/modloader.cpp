/*
 * modloader.cpp
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

Date:2022-2-5

Description:Mod Loader for ARAS

**************************************************/

#include "modloader.hpp"
#include "io.hpp"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <dirent.h>
#include <vector>
#include "io.hpp"

/*所需的Lua文件信息*/
static const char *modset[] = {"name", "version", "type", "date", "author"};
/*日志格式*/
static const char *l_modset[] = {
    "{} name is {}",
    "{} version is {}",
    "{} type is {}",
    "{} last update is on {}",
    "{} author is {}"};

namespace ARAS
{
	/*
     * name: WSSERVER()
     * describe: Constructor for initializing mod loader
     * 描述：构造函数，初始化模组加载器
     */
    ModLoader::ModLoader(bool debug = false)
    {
        spdlog::info("ModLoader verison is {}", ModLoader_VERSION);
        if(debug)
            spdlog::set_level(spdlog::level::debug);
        else
            spdlog::set_level(spdlog::level::info);
        spdlog::info("LuaCore version is {}",LUA_VERSION);
    }

	/*
     * name: ~WSSERVER()
     * describe: Destructor
     * 描述：析构函数
     */
    ModLoader::~ModLoader()
    {
    }

	/*
     * name: SearchAllMods(const char *folder)
     * @param folder: 文件夹
     * describe: Search all mods in folder
     * 描述：搜索指定文件夹下的模组
     */
    configor::json ModLoader::SearchAllMods(const char *folder)
    {
        /*检查输入是否合理*/
        if (folder[strlen(folder) - 1] != '/')
#ifdef WIN32
            folder = folder + '\\';
#else
            folder = folder + '/';
#endif
        auto start = std::chrono::high_resolution_clock::now();
        //搜索指定文件夹中所有文件
        struct dirent *ptr;
        DIR *dir;
        dir = opendir(folder);
        std::vector<std::string> files;
        while ((ptr = readdir(dir)) != NULL)
        {
            if (ptr->d_name[0] == '.' || strcmp(ptr->d_name, "..") == 0)
                continue;
            int size = strlen(ptr->d_name);
            if (strcmp((ptr->d_name + (size - 4)), ".lua") != 0)
                continue;
            files.push_back(ptr->d_name);
            spdlog::debug("Found {}", ptr->d_name);
        }
        closedir(dir);
        configor::json j;
        /*如果没有发现模组则返回错误提示*/
        if (files.begin() == files.end())
        {
            spdlog::error("Counldn't found any mod,Do you choose a right folder?");
            j["count"] = 0;
            return j;
        }
        spdlog::info("Found {} mods and loading", files.size());
        j["count"] = files.size();
        spdlog::debug("整理已发现的文件，并统计信息");
        /*获取所有模组的配置信息*/
        for (int i = 0; i < files.size(); i++)
        {
            spdlog::debug("Initializing lua server");
            lua_State *L = luaL_newstate();
            if (L == nullptr)
            {
                spdlog::error("Couldn't initializing lua server");
                j["error"]["time"] = timestamp();
                j["error"]["info"] = "Couldn't initializing lua server";
                j["error"]["place"] = "SearchAllMods";
                return j;
            }
            std::vector<std::string> tm = files;
            tm[i] = folder + files[i];
            if (luaL_loadfile(L, tm[i].c_str()))
            {
                spdlog::error("Couldn't load {}", files[i]);
                goto error;
            }
            if (lua_pcall(L, 0, 0, 0))
            {
                spdlog::error("Couldn't run {}", files[i]);
                goto error;
            }
            for (int jj = 0; jj < 5; jj++)
            {
                lua_getglobal(L, modset[jj]);
                j[files[i]][modset[jj]] = lua_tostring(L, -1);
                spdlog::debug(l_modset[jj], files[i], j[files[i]][modset[jj]]);
            }
            spdlog::debug("Finished reading information");
            lua_close(L);
        }
        error:{}
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        spdlog::info("It took {} seconds to load mods", diff.count());
        return j;
    }

    bool ModLoader::ModsManager(ModsChangeCmd cmd,const char* name ,const char * target_pos = nullptr)
    {
        bool status = false;
        switch(cmd)
        {
            case DEL:status = IO::Delete(name);break;
            case MOVE:status = IO::Move(name,target_pos);break;
            case COPY:status = IO::Copy(name,target_pos);break;
            case CUT:status = IO::Cut(name,target_pos);break;
        };
        return status;
    }

    configor::json SearchAllScripts(const char *folder)
    {
        /*检查输入是否合理*/
        if (folder[strlen(folder) - 1] != '/')
#ifdef WIN32
            folder = folder + '\\';
#else
            folder = folder + '/';
#endif
        auto start = std::chrono::high_resolution_clock::now();
        //搜索指定文件夹中所有脚本文件
        struct dirent *ptr;
        DIR *dir;
        dir = opendir(folder);
        std::vector<std::string> files;
        while ((ptr = readdir(dir)) != NULL)
        {
            if (ptr->d_name[0] == '.' || strcmp(ptr->d_name, "..") == 0)
                continue;
            int size = strlen(ptr->d_name);
            if (strcmp((ptr->d_name + (size - 4)), ".lua") != 0 || strcmp((ptr->d_name + (size - 3)), ".sh") != 0 || strcmp((ptr->d_name + (size - 6)), ".shell") != 0)
                continue;
            files.push_back(ptr->d_name);
            spdlog::debug("Found {}", ptr->d_name);
        }
        closedir(dir);
        configor::json j;
        /*如果没有发现脚本则返回错误提示*/
        if (files.begin() == files.end())
        {
            spdlog::error("Counldn't found any mod,Do you choose a right folder?");
            j["count"] = 0;
            return j;
        }
        spdlog::info("Found {} scripts", files.size());
        j["count"] = files.size();
        for(int i = 0 ;i<files.size();i++)
            j[i]["name"] = files[i];
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        spdlog::info("It took {} seconds to load scripts", diff.count());
        return j;
    }

    bool ModLoader::RunScript(const char * name)
    {
        //运行Lua脚本
        if (strcmp((name + (strlen(name) - 4)), ".lua") == 0)
        {
            lua_State *L = luaL_newstate();
            if(L == nullptr)
            {
                spdlog::error("Couldn't initializing Lua server");
                return false;
            }
            std::string tmp = "scripts/";
            tmp.append(name);
            if(access(tmp.c_str(),F_OK) != 0)
            {
                spdlog::error("{} not found",tmp);
                lua_close(L);
                return false;
            }
            luaL_loadfile(L,tmp.c_str());
            lua_pcall(L,0,0,0);
            lua_getglobal(L,"type");
            if(lua_tostring(L,-1) != "script")
            {
                spdlog::error("{} is not a script",name);
                return false;
            }
            spdlog::info("I'm sorry.You can't use this");
            return true;
        }
        //运行shell
        if (strcmp((name + (strlen(name) - 3)), ".sh") == 0 || strcmp((name + (strlen(name) - 6)), ".shell") == 0)
        {
            std::string tmp = "scripts/",t = "sh ";
            tmp.append(name);
            if(access(tmp.c_str(),F_OK) != 0)
            {
                spdlog::error("{} not found",tmp);
                return false;
            }
            system(t.append(tmp).c_str());
            spdlog::info("Run {}",name);
            return true;
        }
        spdlog::error("Not support!");
        return false;
    }


}
