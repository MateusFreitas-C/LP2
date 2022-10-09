#include <iostream>
#include <thread>
#include <chrono>

using std::cout;
using std::endl;
using std::thread;

#define QTD 5

char ret[QTD];

int funcao_thread(int id, char b) {
	cout << "[thread " << id << "] start" << endl;
	cout << "[thread " << id << "] ";
	cout << "recebi: " << b << endl;
	std::this_thread::sleep_for (
			std::chrono::seconds(1+id));
	cout << "[thread " << id << "] end" << endl;

	ret[id] = b;

	return 0;
}

int main(void) {
	thread* ths[QTD];

	cout << "[main] start" << endl;

	for (int i = 0; i < QTD; i++)
		ths[i] = new thread(funcao_thread,
			       	i, 'A' + i);

	for (int i = 0; i < QTD; i++)
		ths[i]->join();

	cout << "[main] end" << endl;



}
