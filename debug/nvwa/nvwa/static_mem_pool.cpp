// -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim:tabstop=4:shiftwidth=4:expandtab:

/*
 * Copyright (C) 2004-2017 Wu Yongwei <wuyongwei at gmail dot com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 *    not claim that you wrote the original software.  If you use this
 *    software in a product, an acknowledgement in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must
 *    not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 * This file is part of Stones of Nvwa:
 *      https://github.com/adah1972/nvwa
 *
 */

/**
 * @file  static_mem_pool.cpp
 *
 * Non-template and non-inline code for the `static' memory pool.
 *
 * @date  2017-09-09
 */

#include "static_mem_pool.h"    // nvwa::static_mem_pool_set
#include <algorithm>            // std::for_each
#include "_nvwa.h"              // NVWA_NAMESPACE_*
#include "cont_ptr_utils.h"     // nvwa::delete_object

NVWA_NAMESPACE_BEGIN

static_mem_pool_set::static_mem_pool_set()
{
    _STATIC_MEM_POOL_TRACE(false, "The static_mem_pool_set is created");
}

static_mem_pool_set::~static_mem_pool_set()
{
    std::for_each(_M_memory_pool_set.rbegin(),
                  _M_memory_pool_set.rend(),
                  delete_object());
    _STATIC_MEM_POOL_TRACE(false, "The static_mem_pool_set is destroyed");
}

/**
 * Gets the singleton instance of nvwa#static_mem_pool_set.  The
 * instance will be created on the first invocation.
 *
 * @return  reference to the instance of nvwa#static_mem_pool_set
 */
static_mem_pool_set& static_mem_pool_set::instance()
{
    lock guard;
    static static_mem_pool_set _S_instance;
    return _S_instance;
}

/**
 * Asks all static memory pools to recycle unused memory blocks back to
 * the system.  The caller should get the lock to prevent other
 * operations to nvwa#static_mem_pool_set during its execution.
 */
void static_mem_pool_set::recycle()
{
    _STATIC_MEM_POOL_TRACE(false, "Memory pools are being recycled");
    container_type::iterator end = _M_memory_pool_set.end();
    for (container_type::iterator
            i  = _M_memory_pool_set.begin();
            i != end; ++i)
    {
        (*i)->recycle();
    }
}

/**
 * Adds a new memory pool to nvwa#static_mem_pool_set.
 *
 * @param memory_pool_p  pointer to the memory pool to add
 */
void static_mem_pool_set::add(mem_pool_base* memory_pool_p)
{
    lock guard;
    _M_memory_pool_set.push_back(memory_pool_p);
}

NVWA_NAMESPACE_END
