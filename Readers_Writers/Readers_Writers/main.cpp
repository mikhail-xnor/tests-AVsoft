#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>

#define RepC 5 // Iterations count
#define WC 5 // Writers count
#define WD 1000 * (1 + rand() % 3) // Writer delay
#define RC 5 // Readers count
#define RD 1000 * (1 + rand() % 3) // Reader delay

int rOrd;
std::mutex wMut, mut1, mut2;

void Writer(const int num)
{
	for (auto i = 0; i != RepC; ++i)
	{
		wMut.lock();
		std::cout << "Writer #" << num << ", iteration #" << i << " working...\n";
		Sleep(WD);
		wMut.unlock();
	}
}

void Reader(const int num) // high priority
{
	for (auto i = 0; i != RepC; ++i)
	{
		mut1.lock();
		if (rOrd == 0)
			wMut.lock();
		++rOrd;
		mut1.unlock();

		mut2.lock(); // For correctly console output
		std::cout << "Reader #" << num << ", iteration #" << i << " working...\n";
		mut2.unlock();

		Sleep(RD);

		mut1.lock();
		--rOrd;
		if (rOrd == 0)
			wMut.unlock();
		mut1.unlock();
	}
}


int main()
{
	for (auto i = 0; i != WC; ++i)
	{
		std::thread{ &Writer, i }.detach();
	}
	for (auto i = 0; i != RC; ++i)
	{
		std::thread{ &Reader, i }.detach();
	}
	return 0;
}