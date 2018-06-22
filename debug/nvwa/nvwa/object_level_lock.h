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
 * @file  object_level_lock.h
 *
 * In essence Loki ObjectLevelLockable re-engineered to use a fast_mutex
 * class.  Check also Andrei Alexandrescu's article <a
 * href="http://www.awprofessional.com/articles/article.asp?p=25298">
 * "Multithreading and the C++ Type System"</a> for the ideas behind.
 *
 * @date  2013-03-01
 */

#ifndef NVWA_OBJECT_LEVEL_LOCK_H
#define NVWA_OBJECT_LEVEL_LOCK_H

#include "fast_mutex.h"         // nvwa::fast_mutex/_NOTHREADS
#include "_nvwa.h"              // NVWA_NAMESPACE_*

NVWA_NAMESPACE_BEGIN

# ifdef _NOTHREADS
    /**
     * Helper class for object-level locking.  This is the
     * single-threaded implementation.
     */
    template <class _Host>
    class object_level_lock
    {
    public:
        /** Type that provides locking/unlocking semantics. */
        class lock
        {
#   ifndef NDEBUG
            const object_level_lock& _M_host;
#   endif

            lock(const lock&);
            lock& operator=(const lock&);
        public:
            explicit lock(const object_level_lock& host)
#   ifndef NDEBUG
                : _M_host(host)
#   endif
            {}
#   ifndef NDEBUG
            // The purpose of this method is allow one to write code
            // like "assert(guard.get_locked_object() == this)" to
            // ensure that the locked object is exactly the object being
            // accessed.
            const object_level_lock* get_locked_object() const
            {
                return &_M_host;
            }
#   endif
        };

        typedef _Host volatile_type;
    };
# else
    /**
     * Helper class for object-level locking.  This is the
     * multi-threaded implementation.
     */
    template <class _Host>
    class object_level_lock
    {
        mutable fast_mutex _M_mtx;

    public:
        // The C++ 1998 Standard required the use of `friend' here, but
        // this requirement was considered a defect and subsequently
        // changed.  It is still used here for compatibility with older
        // compilers.
        class lock;
        friend class lock;

        /** Type that provides locking/unlocking semantics. */
        class lock
        {
            const object_level_lock& _M_host;

            lock(const lock&);
            lock& operator=(const lock&);
        public:
            explicit lock(const object_level_lock& host) : _M_host(host)
            {
                _M_host._M_mtx.lock();
            }
            ~lock()
            {
                _M_host._M_mtx.unlock();
            }
#   ifndef NDEBUG
            // The purpose of this method is allow one to write code
            // like "assert(guard.get_locked_object() == this)" to
            // ensure that the locked object is exactly the object being
            // accessed.
            const object_level_lock* get_locked_object() const
            {
                return &_M_host;
            }
#   endif
        };

        typedef volatile _Host volatile_type;
    };
# endif // _NOTHREADS

NVWA_NAMESPACE_END

#endif // NVWA_OBJECT_LEVEL_LOCK_H
