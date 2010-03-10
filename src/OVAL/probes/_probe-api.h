
/*
 * Copyright 2008 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *      "Daniel Kopecek" <dkopecek@redhat.com>
 */

#pragma once
#ifndef _PROBE_API_H
#define _PROBE_API_H

#include "public/probe-api.h"
#include "probe-cache.h"
#include "encache.h"

/**
 * @struct id_desc_t
 * Holds information for item ids generation.
 */
struct id_desc_t {
#ifndef HAVE_ATOMIC_FUNCTIONS
	pthread_mutex_t item_id_ctr_lock;	///< id counter lock
#endif
	int item_id_ctr;	///< id counter
};

/// Probe's global runtime information
typedef struct {
	/* Protocol stuff */
	SEAP_CTX_t *ctx;	///< protocol context for communication with the library
	int sd;			///< file descriptor used for communication

	/* Object cache */
	pcache_t *pcache;	///< probed objects cache
	pthread_rwlock_t pcache_lock;	///< cache lock

	struct id_desc_t id_desc;	///< id generation information

	/* probe main */
	void *probe_arg;	///< optional argument to probe_main()

        /*
         * Element name cache. Elements are in our case a generalization of
         * the basic structure of OVAL objects, entities, etc. Such elements
         * have names and each probe uses just a small set of names that
         * repeats in each generated item. For some probes, the number of
         * items generated is very large and therefore we cache the repeating
         * names utilizing the reference count mechanism in the S-exp object
         * system.
         */
        encache_t *encache;
} globals_t;

/// Probe's global runtime information
extern globals_t global;

#if defined(HAVE_ATOMIC_FUNCTIONS)
#define GLOBALS_INITIALIZER { NULL, -1, NULL, PTHREAD_RWLOCK_INITIALIZER, {1}, NULL, NULL }
#else
#define GLOBALS_INITIALIZER { NULL, -1, NULL, PTHREAD_RWLOCK_INITIALIZER, {PTHREAD_MUTEX_INITIALIZER, 1}, NULL, NULL }
#endif

#define READER_LOCK_CACHE pthread_rwlock_rdlock (&globals.pcache_lock)
#define WRITER_LOCK_CACHE pthread_rwlock_wrlock (&globals.pcache_lock)
#define READER_UNLOCK_CACHE pthread_rwlock_unlock (&globals.pcache_lock)
#define WRITER_UNLOCK_CACHE pthread_rwlock_unlock (&globals.pcache_lock)

#define SEAP_LOCK pthread_mutex_lock (&globals.seap_lock)
#define SEAP_UNLOCK pthread_mutex_unlock (&globals.seap_lock)

#endif				/* _PROBE_API_H */
