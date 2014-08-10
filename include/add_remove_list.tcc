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

#ifdef _YZ_FASTCGIPP_ADD_REMOVE_LIST_HPP
#ifndef _YZ_FASTCGIPP_ADD_REMOVE_LIST_TCC
#define _YZ_FASTCGIPP_ADD_REMOVE_LIST_TCC

namespace yz {
	namespace utils {
		template <class T, class handle_saver>
		T &add_remove_list<T, handle_saver, true>::back() {
			return container.back();
		}

		template <class T, class handle_saver>
		const T &add_remove_list<T, handle_saver, true>::back() const {
			return container.back();
		}

		template <class T, class handle_saver>
		template <class T2>
		void add_remove_list<T, handle_saver, true>::push_back(T2 &&x) {
			index_T pos = (index_T)container.size();
			container.push_back(std::forward<T2>(x));
			handle_saver::save(container.back(), pos);
		}

		template <class T, class handle_saver>
		template <class... Args>
		void add_remove_list<T, handle_saver, true>::emplace_back(Args&&... args) {
			index_T pos = (index_T)container.size();
			container.emplace_back(args...);
			handle_saver::save(container.back(), pos);
		}

		template <class T, class handle_saver>
		void add_remove_list<T, handle_saver, true>::erase(typename add_remove_list<T, handle_saver, true>::handle_t handle) {
			index_T pos = *handle;
			if (pos == npos)
				throw std::logic_error("erasing already deleted object");
			size_t last_pos = container.size() - 1;
			// exception may be thrown from this section if the implementation of T's dtor or swap may throw
			handle_saver::save(container[pos], npos);
			if (pos < last_pos) {
				using namespace std;
				swap(container[pos], container.back());
				handle_saver::save(container[pos], pos);
			}
			container.pop_back();
		}

		template <class T, class handle_saver>
		const typename add_remove_list<T, handle_saver, true>::index_T add_remove_list<T, handle_saver, true>::npos = -1;

		template <class T, typename index_T>
		T &add_remove_list<T, index_T, false>::back() {
			return container.back();
		}

		template <class T, typename index_T>
		const T &add_remove_list<T, index_T, false>::back() const {
			return container.back();
		}

		template <class T, typename index_T>
		typename add_remove_list<T, index_T, false>::handle_t add_remove_list<T, index_T, false>::allocate_new_handle() {
			handles.emplace_back(new index_T);
		}

		template <class T, typename index_T>
		void add_remove_list<T, index_T, false>::destroy_last_handle() noexcept {
			handles.pop_back();
		};

		template <class T, typename index_T>
		template <class T2>
		typename add_remove_list<T, index_T, false>::handle_t add_remove_list<T, index_T, false>::push_back(T2 &&x) {
			index_T pos = (index_T)handles.size();
			allocate_new_handle();
			try {
				container.push_back(std::forward<T2>(x));
				*handles.back() = pos;
			}
			catch (...) {
				destroy_last_handle();
				throw;
			}
			return handles.back().get();
		}

		template <class T, typename index_T>
		template <class... Args>
		typename add_remove_list<T, index_T, false>::handle_t add_remove_list<T, index_T, false>::emplace_back(Args&&... args) {
			index_T pos = (index_T)handles.size();
			allocate_new_handle();
			try {
				container.emplace_back(args...);
				*handles.back() = pos;
			}
			catch (...) {
				destroy_last_handle();
				throw;
			}
			return handles.back().get();
		}

		template <class T, typename index_T>
		void add_remove_list<T, index_T, false>::erase(typename add_remove_list<T, index_T, false>::handle_t handle) {
			index_T pos = *handle;
			if (pos == npos)
				throw std::logic_error("erasing already deleted object");
			size_t last_pos = container.size() - 1;
			{
				// exception may be thrown from this section if the implementation of T's dtor or swap may throw
				if (pos < last_pos) {
					using namespace std;
					swap(container[pos], container.back());
				}
				*handles[pos] = npos;
				container.pop_back();
			}
			{
				// no exceptions are possible in this section
				if (pos < last_pos) {
					handles[pos] = std::move(handles.back());
					*handles[pos] = pos;
				}
				handles.pop_back();
			}
		}

		template <class T, typename index_T>
		typename add_remove_list<T, index_T, false>::handle_t add_remove_list<T, index_T, false>::get_handle_at(size_t pos) const {
			return handles[pos].get();
		}

		template <class T, typename index_T>
		const index_T add_remove_list<T, index_T, false>::npos = -1;
	}
}

#endif /* _YZ_FASTCGIPP_ADD_REMOVE_LIST_TCC */
#endif /* _YZ_FASTCGIPP_ADD_REMOVE_LIST_HPP */
