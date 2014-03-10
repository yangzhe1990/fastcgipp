#include <random>
#include <iostream>

#include "add_remove_list.hpp"

struct A {
	typedef int index_T;
	index_T pos;
	struct handle_saver {
		typedef typename A::index_T index_T;
		static void save(A &x, index_T pos) {
			x.pos = pos;
		}
	};
};

int main() {
	int n = 1000000;
	yz::utils::add_remove_list<int> list;
	std::vector<yz::utils::add_remove_list<int>::handle_t> handles;
	for (int i = 0; i < n; ++i)
		handles.push_back(list.push_back(i));

	std::default_random_engine e1((std::random_device())());
	list.erase(handles[n - 1]);
	handles[n - 1] = NULL;
	for (int i = 0; i < n - 1; ++i)
		if (std::generate_canonical<double, 10>(e1) < 0.5) {
			list.erase(handles[i]);
			handles[i] = NULL;
		}

	bool correct = true;
	for (int i = 0; i < n; ++i) {
		if (handles[i] == NULL)
			continue;
		size_t pos = *handles[i];
		if (list.container[pos] != i) {
			std::cout << "bug: pos of " << i << " is wrong\n";
			correct = false;
		}
		if (*list.get_handle_at(pos) != pos) {
			std::cout << "bug: handle at pos " << pos << " is wrong\n";
			correct = false;
		}
	}

	if (correct)
		std::cout << "test of add_remove_list (no handle member) passed\n";
	else {
		std::cout << "test of add_remove_list (no handle member) failed\n";
		return 1;
	}

	yz::utils::add_remove_list<A, A::handle_saver, true> list2;
	for (int i = 0; i < n; ++i)
		list2.emplace_back();
	list2.erase(&list2.container[n - 1].pos);
	for (int i = 0; i < list2.container.size(); ++i)
		if (std::generate_canonical<double, 10>(e1) < 0.5)
			list2.erase(&list2.container[i].pos);
	int number_remaining = list2.container.size();
	for (int i = 0; i < number_remaining; ++i)
		if (list2.container[i].pos != i) {
			std::cout << "bug: handle at pos " << i << " is wrong\n";
			correct = false;
		}

	if (correct)
		std::cout << "test of add_remove_list (with handle member) passed\n";
	else {
		std::cout << "test of add_remove_list (with handle member) failed\n";
		return 1;
	}

	std::vector<bool> id_exist(n, false);
	yz::utils::unbounded_id_generator<int> id_gen;

	auto check_and_insert_id = [&](int id) {
		if (id_exist[id]) {
			std::cout << "duplicated id allocated\n";
			correct = false;
		}
		id_exist[id] = true;
	};

	for (int i = 0; i < n; ++i)
		check_and_insert_id(id_gen.allocate_id());

	for (int i = n; i > 0;) {
		int new_i = i / 2;
		for (int j = 0; j < i; ++j)
			if (std::generate_canonical<double, 10>(e1) < 0.5) {
				id_gen.return_id(j);
				id_exist[j] = false;
			}
		for (int j = new_i; j < n; ++j)
			if (id_exist[j]) {
				id_gen.return_id(j);
				id_exist[j] = false;
			}

		if (id_gen.get_n() > new_i) {
			std::cout << "id set auto shrink failed\n";
			correct = false;
		}

		int not_exist = 0;
		for (int j = 0; j < new_i; ++j)
			if (!id_exist[j])
				++not_exist;
		for (int j = 0; j < not_exist; ++j) {
			int new_id = id_gen.allocate_id();
			check_and_insert_id(new_id);
			id_exist[new_id] = true;
		}

		if (id_gen.get_n() != new_i) {
			std::cout << "id set auto expand failed\n";
			correct = false;
		}

		i = new_i;
	}

	if (correct)
		std::cout << "test of unbounded_id_generator passed\n";
	else {
		std::cout << "test of unbounded_id_generator failed\n";
		return 1;
	}

	return 0;
}
