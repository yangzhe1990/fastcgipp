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

#ifndef _YZ_FASTCGIPP_ADD_REMOVE_LIST_HPP
#define _YZ_FASTCGIPP_ADD_REMOVE_LIST_HPP

#include <memory>
#include <vector>
#include <stdexcept>

namespace yz {
	namespace utils {
		/* add_remove_list for T with/without a handle member
		 * to save where T is stored in this list.
		 *
		 * Default for T without a handle member with size_t
		 * represent the index of T inside this
		 * add_remove_list.
		 */
		template <class T, class index_T = size_t, bool if_has_handle_in_T = false>
		class add_remove_list;

		/* T with a handle member */
		template <class T, class handle_saver>
		class add_remove_list<T, handle_saver, true> {
		public:
			std::vector<T> container;
			typedef typename handle_saver::index_T index_T;
			typedef const index_T *handle_t;
		private:
			static const index_T npos;
		public:
			T &back();

			const T &back() const;

			template <class T2>
			void push_back(T2 &&x);

			template <class... Args>
			void emplace_back(Args&&... args);

			// no throw from inside this class
			void erase(handle_t handle);
		};

		/* T without a handle member */
		template <class T, typename index_T>
		class add_remove_list<T, index_T, false> {
		public:
			std::vector<T> container;
		private:
			static const index_T npos;
			std::vector<std::unique_ptr<index_T> > handles;
		public:
			typedef const index_T *handle_t;
		private:
			handle_t allocate_new_handle();

			void destroy_last_handle() noexcept;
		public:
			T &back();

			const T &back() const;

			template <class T2>
			handle_t push_back(T2 &&x);

			template <class... Args>
			handle_t emplace_back(Args&&... args);

			// no throw from inside this class
			void erase(handle_t handle);

			// only for unit test
			handle_t get_handle_at(size_t pos) const;
		};
	}
}

#include <add_remove_list.tcc>

#endif /* _YZ_FASTCGIPP_ADD_REMOVE_LIST_HPP */
