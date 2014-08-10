#include <functional>
#include <iostream>
#include <memory>
#include <movable_function_wrapper.hpp>

void f(std::unique_ptr<int> &&x) {
	std::cout << x.get();
	if (x.get() != NULL)
		std::cout << ' ' << *x;
	std::cout << std::endl;
}

int main() {
	std::unique_ptr<int> p_int(new int);
	*p_int = 3;

	std::function<void()> h;
	{
		auto g = std::bind(f, std::bind(std::move<std::unique_ptr<int> &>, std::move(p_int)));
		g();
		std::function<void()> gg = yz::utils::make_movable_function<void()>(std::move(g));
		decltype(g) gggg = std::move(g);
		gggg();
		gg();
		g();

		h = std::move(gg);
		h();
	}
	h();

	std::function<void()> gg = yz::utils::make_movable_function<void()>(nullptr);
	gg(); //  throws
}
