#include <iostream>
#include <thread>
#include <mutex>

using std::cout;
using std::endl;
using std::thread;
using std::mutex;


class monitor_conta{
    private:
        int cpf_usuario;
        float saldo, result;

        mutex m_saldo;
    
    public:
        monitor_conta(int cpf, float s): cpf_usuario(cpf), saldo(s)
        {
            cout << "Usuario: " << cpf << endl;
            cout << "Saldo Inicial: " << s << endl;
        }

        void credito(float v)
        {
            m_saldo.lock();
            saldo += v;
            cout << "[" << cpf_usuario << "] CREDITO: " << v;
            cout << "\t(" << saldo << ")" << endl;
            m_saldo.unlock();
        }

        void debito(float v)
        {
            m_saldo.lock();
            saldo -= v;
            cout << "[" << cpf_usuario << "] DEBITO: " << v;
            cout << "\t(" << saldo << ")" << endl;
            m_saldo.unlock();
        }

        float get_saldo()
        {
            float result;
            m_saldo.lock();
            result = saldo;
            m_saldo.unlock();
            return result;
        }
};

int main(void)
{
    monitor_conta *moni = new monitor_conta(1010, 1500);

    thread t1(&monitor_conta::credito, moni, 100);
    thread t2(&monitor_conta::debito, moni, 200);
    thread t3(&monitor_conta::credito, moni, 50);
    thread t4(&monitor_conta::debito, moni, 500);

    t1.join();
    t2.join();
    t3.join();   
    t4.join();

    cout << "Saldo Final: " << moni->get_saldo() << endl;


    return 0;
}