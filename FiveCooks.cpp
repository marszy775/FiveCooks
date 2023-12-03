#include <iostream>
#include <cstdlib>
#include <semaphore>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <numeric>
#include <random>

static constexpr unsigned TABLE_CAPACITY = 4;
static constexpr int TABLE_WEIGHT_LIMIT = 20;

std::random_device r;
std::default_random_engine e1(r());
std::uniform_int_distribution<int> uniform_dist(1, TABLE_WEIGHT_LIMIT);

class Food {
	int weight;
public:
	Food() {
		weight = uniform_dist(e1);
	}
	int get_food() {
		return weight;
	}
};

class Table {
	std::vector<int> meals;
	std::condition_variable check;
	std::mutex meal_mutex;
public:
	Table() = default;
	Table(const Table&) = delete;
	const Table& operator = (const Table&) = delete;

	void put(int weight) {
		std::unique_lock<std::mutex> lock(meal_mutex);
		auto table_weight = std::reduce(std::begin(meals), std::end(meals));
		while ((table_weight + weight) > TABLE_WEIGHT_LIMIT || meals.size() >= TABLE_CAPACITY) {
			check.wait(lock);
			table_weight = std::reduce(std::begin(meals), std::end(meals));
		}
		meals.push_back(weight);
		check.notify_all();
	}

	int get() {
		std::unique_lock<std::mutex> lock(meal_mutex);
		while (meals.size() == 0) {
			check.wait(lock);
		}
		int result = meals[0];
		meals.erase(meals.begin());
		return result;
	}
};


void message(std::string str, int chef_id) {
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	std::cout << "Chef " << chef_id << str << std::endl;
}

void FiveChef(std::binary_semaphore& left, std::binary_semaphore& right, int chef_id) {
	static Table table;
	for (;;) {
		Food food;
		int food_weight = food.get_food();

		message(" is cooking", chef_id);
		std::this_thread::sleep_for(std::chrono::milliseconds(food_weight * 300));

		message(" is waiting to put his food on the table", chef_id);
		table.put(food_weight);

		left.acquire();
		message(" took left fork", chef_id);

		right.acquire();
		message(" took right fork", chef_id);

		message(" is waiting for food", chef_id);
		int eaten = table.get();

		message(" is eating", chef_id);
		std::this_thread::sleep_for(std::chrono::milliseconds(eaten * 200));

		left.release();
		message(" put down left fork", chef_id);

		right.release();
		message(" put down right fork", chef_id);

		std::this_thread::yield();
	}

}

int main()
{
	std::binary_semaphore Fork1{ 1 };
	std::binary_semaphore Fork2{ 1 };
	std::binary_semaphore Fork3{ 1 };
	std::binary_semaphore Fork4{ 1 };
	std::binary_semaphore Fork5{ 1 };

	std::thread chef1(FiveChef, std::ref(Fork1), std::ref(Fork2), 1);
	std::thread chef2(FiveChef, std::ref(Fork2), std::ref(Fork3), 2);
	std::thread chef3(FiveChef, std::ref(Fork3), std::ref(Fork4), 3);
	std::thread chef4(FiveChef, std::ref(Fork4), std::ref(Fork5), 4);
	std::thread chef5(FiveChef, std::ref(Fork5), std::ref(Fork1), 5);

	chef1.join();
	chef2.join();
	chef3.join();
	chef4.join();
	chef5.join();
}
