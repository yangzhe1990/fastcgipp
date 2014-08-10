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

#ifndef _YZ_FASTCGIPP_UNBOUNDED_ID_GENERATOR_HPP
#define _YZ_FASTCGIPP_UNBOUNDED_ID_GENERATOR_HPP

#include <vector>

namespace yz {
	namespace utils {
		template <class id_T = int>
		class unbounded_id_generator {
		private:
			id_T n;
			std::vector<id_T> free_id_pool;
			std::vector<size_t> free_id_pos;
			static const size_t npos;
		public:
			unbounded_id_generator() : n(0) {}

			id_T allocate_id();

			// may throw, strong guarantee
			void return_id(id_T id);

			// only for unit test
			id_T get_n();
		};
	}
}

#include <unbounded_id_generator.tcc>

#endif /* _YZ_FASTCGIPP_UNBOUNDED_ID_GENERATOR_HPP */
