/*!The Tiny Box Library
 * 
 * TBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * TBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with TBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2011, ruki All rights reserved.
 *
 * \author		ruki
 * \file		hash.h
 *
 */
#ifndef TB_CONTAINER_HASH_H
#define TB_CONTAINER_HASH_H

// c plus plus
#ifdef __cplusplus
extern "C" {
#endif

/* /////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "pool.h"

/* /////////////////////////////////////////////////////////
 * macros
 */
#define TB_HASH_SIZE_MICRO 				(64)
#define TB_HASH_SIZE_SMALL 				(256)
#define TB_HASH_SIZE_LARGE 				(65535)

#ifdef TB_CONFIG_MEMORY_MODE_SMALL
# 	define TB_HASH_SIZE_DEFAULT 		TB_HASH_SIZE_SMALL
#else
# 	define TB_HASH_SIZE_DEFAULT 		TB_HASH_SIZE_LARGE
#endif

/* /////////////////////////////////////////////////////////
 * types
 */

// the hash iterator type
typedef tb_size_t 		tb_hash_itor_t;

// the hash item type
typedef struct __tb_hash_item_t
{
	// the item name
	tb_void_t* 			name;

	// the prev & next item
	tb_size_t 			prev;
	tb_size_t 			next;

}tb_hash_item_t;

// the hash bucket type
typedef struct __tb_hash_bucket_t
{
	// the bucket size
	tb_size_t 			size;

	// the prev & next item
	tb_size_t 			prev;
	tb_size_t 			next;

}tb_hash_bucket_t;

// the callback type
typedef tb_void_t 	(*tb_hash_name_free_func_t)(tb_void_t* name, tb_void_t* priv);
typedef tb_void_t* 	(*tb_hash_name_dupl_func_t)(tb_void_t const* name, tb_void_t* priv);
typedef tb_size_t 	(*tb_hash_name_hash_func_t)(tb_void_t const* name, tb_size_t size, tb_void_t* priv);
typedef tb_bool_t 	(*tb_hash_name_comp_func_t)(tb_void_t const* lname, tb_void_t const* rname, tb_void_t* priv);

// the item func
typedef tb_void_t 	(*tb_hash_item_free_func_t)(tb_void_t* item, tb_void_t* priv);	

// the hash name func type
typedef struct __tb_hash_name_func_t
{
	// the name func
	tb_hash_name_hash_func_t 	hash;
	tb_hash_name_comp_func_t 	comp;
	tb_hash_name_dupl_func_t 	dupl;
	tb_hash_name_free_func_t 	free;

	// the priv data
	tb_void_t* 					priv;

}tb_hash_name_func_t;

// the hash item func type
typedef struct __tb_hash_item_func_t
{
	// the item func
	tb_hash_item_free_func_t 	free;

	// the priv data
	tb_void_t* 					priv;

}tb_hash_item_func_t;

// the hash pair type
typedef struct __tb_hash_pair_t
{
	tb_void_t const* 			name;
	tb_void_t* 					item;

}tb_hash_pair_t;

// the hash type
typedef struct __tb_hash_t
{
	// the item pool
	tb_pool_t* 					pool;

	// the item step
	tb_size_t 					step;
	
	// the bucket list & size
	tb_hash_bucket_t* 			list;
	tb_size_t 					size;

	// the hash func
	tb_hash_name_func_t 		name_func;
	tb_hash_item_func_t 		item_func;

}tb_hash_t;

/* /////////////////////////////////////////////////////////
 * interfaces
 */

// init & exit
tb_hash_t* 			tb_hash_init(tb_size_t step, tb_size_t size, tb_hash_name_func_t name_func, tb_hash_item_func_t const* item_func);
tb_void_t 			tb_hash_exit(tb_hash_t* hash);

// accessors & modifiors
tb_void_t 			tb_hash_clear(tb_hash_t* hash);

tb_void_t* 			tb_hash_at(tb_hash_t* hash, tb_void_t const* name);
tb_void_t const* 	tb_hash_const_at(tb_hash_t const* hash, tb_void_t const* name);

tb_void_t 	 		tb_hash_del(tb_hash_t* hash, tb_void_t const* name);
tb_void_t 	 		tb_hash_set(tb_hash_t* hash, tb_void_t const* name, tb_void_t const* item);
tb_void_t 	 		tb_hash_get(tb_hash_t* hash, tb_void_t const* name, tb_void_t* item);

// attributes
tb_size_t 			tb_hash_size(tb_hash_t const* hash);
tb_size_t 			tb_hash_maxn(tb_hash_t const* hash);

// hash name func
tb_hash_name_func_t tb_hash_name_func_str(tb_void_t* priv); 	//!< cstring
tb_hash_name_func_t tb_hash_name_func_int(tb_void_t* priv); 	//!< integer
tb_hash_name_func_t tb_hash_name_func_ptr(tb_void_t* priv); 	//!< pointer
tb_hash_name_func_t tb_hash_name_func_mem(tb_void_t* priv); 	//!< memory

/* iterator
 * 
 * tb_hash_itor_t itor = tb_hash_itor_head(hash);
 * tb_hash_itor_t tail = tb_hash_itor_tail(hash);
 * for (; itor != tail; itor = tb_hash_itor_next(hash, itor))
 * {
 * 		tb_hash_pair_t pair = tb_hash_itor_at(hash, itor);
 * 		if (pair.name && pair.item)
 * 		{
 * 			// ...
 * 		}
 * }
 *
 * head: => the first item
 * tail: => behind the last item, no item
 *
 * \note the index of the same item is mutable, only for iterator
 */
tb_hash_pair_t 		tb_hash_itor_at(tb_hash_t* hash, tb_hash_itor_t itor);
tb_hash_itor_t 		tb_hash_itor_head(tb_hash_t const* hash);
tb_hash_itor_t 		tb_hash_itor_tail(tb_hash_t const* hash);
tb_hash_itor_t 		tb_hash_itor_next(tb_hash_t const* hash, tb_hash_itor_t itor);
tb_hash_itor_t 		tb_hash_itor_prev(tb_hash_t const* hash, tb_hash_itor_t itor);

// c plus plus
#ifdef __cplusplus
}
#endif

#endif

