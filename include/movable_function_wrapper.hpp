#ifndef _YZ_FASTCGIPP_MOVABLE_FUNCTION_WRAPPER_HPP
#define _YZ_FASTCGIPP_MOVABLE_FUNCTION_WRAPPER_HPP

#include <functional>
#include <memory>

namespace yz {
	namespace utils {
		template <class F, class>
		struct movable_function_wrapper;

		template <class R, class... Args>
		struct movable_function_wrapper<std::nullptr_t, R(Args...)> {
			movable_function_wrapper(std::nullptr_t) {}
			R operator() (Args... args) {
				throw std::bad_function_call();
			}
		};

		template <class F, class R, class... Args>
		struct movable_function_wrapper<F, R(Args...)> {
			typedef movable_function_wrapper<F, R(Args...)> this_type;
			std::shared_ptr<F> p_f;

			movable_function_wrapper(F &&f) : p_f(std::make_shared<F>(std::move(f))) {}

			R operator() (Args... args) {
				return (*p_f)(args...);
			}
		};

		template <class InvokeT, class FunctorT>
		movable_function_wrapper<FunctorT, InvokeT> make_movable_function(FunctorT &&f) {
			return movable_function_wrapper<FunctorT, InvokeT>(std::move(f));
		}
	}
}

#endif /* _YZ_FASTCGIPP_MOVABLE_FUNCTION_WRAPPER_HPP */
