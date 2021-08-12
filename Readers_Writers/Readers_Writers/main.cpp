#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <string>
#include <Windows.h>

using namespace std::chrono_literals;

#define RepC 5 // Iterations count
#define WC 5 // Writers count
#define WD 1s * (1 + rand() % 3) // Writer delay
#define RC 5 // Readers count
#define RD 1s * (1 + rand() % 3) // Reader delay

int rOrd;
std::mutex wMut, mut1, mut2;

void Writer(const int num, std::shared_ptr<std::string> data)
{
	for (auto i = 0; i != RepC; ++i)
	{
		wMut.lock();
		*data = "Writer #" + std::to_string(num) + " finished work!";
		std::cout << "Writer #" << num << ", iteration #" << i << " working...\n\n";
		std::this_thread::sleep_for(WD);
		wMut.unlock();
	}
}

void Reader(const int num, std::shared_ptr<std::string> data) // high priority
{
	for (auto i = 0; i != RepC; ++i)
	{
		mut1.lock();
		if (rOrd == 0)
			wMut.lock();
		++rOrd;
		mut1.unlock();

		mut2.lock(); // For correctly console output
		std::cout << "Reader #" << num << ", iteration #" << i << " working...\nRead data: " + *data + "\n\n";
		mut2.unlock();

		std::this_thread::sleep_for(RD);

		mut1.lock();
		--rOrd;
		if (rOrd == 0)
			wMut.unlock();
		mut1.unlock();
	}
}


int main()
{
	auto data{ std::make_shared<std::string>() };
	for (auto i = 0; i != WC; ++i)
	{
		std::thread{ &Writer, i, data }.detach();
	}
	for (auto i = 0; i != RC; ++i)
	{
		std::thread{ &Reader, i, data }.detach();
	}
	system("pause");
	return 0;
}