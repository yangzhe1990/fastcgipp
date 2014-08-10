/*
 * Copyright (c) 2013, Yang Zhe <yangzhe1990@gmail.com>, All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3.0 of
 * the License, or (at your option) any later version.	This library
 * is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details. You should have received a copy of the
 * GNU Lesser General Public License along with this library.
 */

#ifdef _YZ_FASTCGIPP_UNBOUNDED_ID_GENERATOR_HPP
#ifndef _YZ_FASTCGIPP_UNBOUNDED_ID_GENERATOR_TCC
#define _YZ_FASTCGIPP_UNBOUNDED_ID_GENERATOR_TCC

namespace yz {
	  namespace utils {
		template <class id_T>
		id_T unbounded_id_generator<id_T>::allocate_id() {
			id_t ret;
			size_t size = free_id_pool.size();
			if (size == 0) {
				ret = n;
				++n;
				free_id_pos.push_back(npos);
			}
			else {
				ret = free_id_pool.back();
				free_id_pool.pop_back();
				free_id_pos[ret] = npos;
			}
			return ret;
		}

		template <class id_T>
		void unbounded_id_generator<id_T>::return_id(id_T id) {
			if (id >= n)
				throw std::range_error("larger than upper bound");
			if (id != n - 1) {
				if (free_id_pos[id] != npos)
					throw std::logic_error("already existing");
				size_t pos = free_id_pool.size();
				free_id_pool.push_back(id);
				free_id_pos[id] = pos;
			}
			else {
				int last = id - 1;
				while (last >= 0 && free_id_pos[last] != npos) {
					size_t pos = free_id_pos[last];
					free_id_pos[
						free_id_pool[pos]
						= free_id_pool.back()] = pos;
					free_id_pool.pop_back();
					--last;
				}
				n = ++last;
				free_id_pos.resize(n);

				try {
					// well, assuming 4 is more aggressive than the reallocation factor
					if (last < free_id_pos.capacity() / 4)
						free_id_pos.shrink_to_fit();
					if (last < free_id_pool.capacity() / 4)
						free_id_pool.shrink_to_fit();
				}
				catch (...) {
				}
			}
		}

		template <class id_T>
		id_T unbounded_id_generator<id_T>::get_n() {
			return n;
		}

		template <class id_T>
		const size_t unbounded_id_generator<id_T>::npos = -1;
	  }
}

#endif /* _YZ_FASTCGIPP_UNBOUNDED_ID_GENERATOR_TCC */
#endif /* _YZ_FASTCGIPP_UNBOUNDED_ID_GENERATOR_HPP */
