//UDP
#include <iostream>
#include <string>
#include <cstdlib> //exit()
#include <cstring> // strpy()
#include <unistd.h> //close()
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void Exception(const string & why, const int exitCode ) // Исключения (возможные ошибки)
{
    cout << "ErrorCode:"<<exitCode <<endl<< why << endl;
    exit(exitCode);
}
int main()
{

    // структура с адресом нашей программы (клиента)
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET; // интернет протокол IPv4
    selfAddr->sin_port = 0;         // любой порт на усмотрение ОС
    selfAddr->sin_addr.s_addr = 0;

    // структура с адресом "на той стороне" (сервера)
    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;     // интернет протокол IPv4
    remoteAddr->sin_port = htons(44214); 
    remoteAddr->sin_addr.s_addr = inet_addr("127.0.0.1"); //  адрес 

    // буфер для передачи и приема данных
    char *buffer = new char[4096];
    strcpy(buffer,"Дата и время:");   //копируем строку
    int msgLen = strlen(buffer);      //вычисляем длину строки

    // создаём сокет
    int mySocket = socket(AF_INET, SOCK_DGRAM, 0); //udp протокол
    if (mySocket == -1) {
        close(mySocket);
        Exception("Ошибка открытия сокета",11);
    }
    //связываем сокет с адрессом
    int rc = bind(mySocket,(const sockaddr *) selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Ошибка привязки сокета с локальным адресом",12);
    }

    //установливаем соединение
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Ошибка подключения сокета к удаленному серверу.", 13);
    }

    // передаём сообщение из буффера
    rc = send(mySocket, buffer, msgLen, 0);
    if (rc == -1) {
        close(mySocket);
        Exception("Сообщение об ошибке отправки", 14);
    }
    cout << "Мы отправляем: " << buffer << endl;

    // принимаем ответ в буффер
    rc = recv(mySocket, buffer, 4096, 0);
    if (rc == -1) {
        close(mySocket);
        Exception("Ошибка получения ответа.", 15);
    }
    buffer[rc] = '\0'; // конец принятой строки
    cout << "Мы получаем: " << buffer << endl; // вывод полученного сообщения от сервера
    // закрыем сокет
    close(mySocket);

    delete selfAddr;
    delete remoteAddr;
    delete[] buffer;
    return 0;
}
