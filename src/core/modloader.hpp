/*
 * modloader.hpp
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

#ifndef _MODLOADER_HPP_
#define _MODLOADER_HPP_

#include "../aras.hpp"
#include "config.h"

#include <assert.h>
#include <stdio.h>

#define MODLOADER_VERSION 1.0

typedef enum
{
    DEL,
    MOVE,
    COPY,
    CUT
} ModsChangeCmd;

namespace ARAS
{
    class ModLoader
    {
    public:
        explicit ModLoader(bool debug);
        ~ModLoader();

        virtual configor::json SearchAllScripts(const char * folder);
        virtual bool RunScript(const char *name);

        virtual configor::json SearchAllMods(const char *folder);
        virtual bool ModsManager(ModsChangeCmd cmd, const char *name, const char *target_pos);

    protected:

    private:
    };

    extern ModLoader *loader;
}

#endif
