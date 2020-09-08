#include <condition_variable>
#include <queue>
#include <mutex>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <chrono>

using namespace std;

class Message
{
public:
    Message(const std::string& text)
        : text_(text)
    {  }

    void display(void)
    {
        cout << "message: " << text_ << endl;
    }

private:
    string text_;
};

queue<Message>      mqueue;
condition_variable  mcond;
mutex               mmutex;

void Consumer(void)
{
    while(true)
    {
        unique_lock<mutex> lck {mmutex};
        mcond.wait(lck);

        auto m = mqueue.front();
        mqueue.pop();
        lck.unlock();

        m.display();
    }
}

void Producer(void)
{
    while(true)
    {
        int i = rand() % 100;
        string s("This is a random integer: " + to_string(i));
        Message m(s);
        unique_lock<mutex> lck {mmutex};
        mqueue.push(m);
        mcond.notify_one();
    }
}




int main(void)
{
    thread tC(Consumer);
    thread tP(Producer);

    this_thread::sleep_for(2s);

    tP.join();
    tC.join();
}
