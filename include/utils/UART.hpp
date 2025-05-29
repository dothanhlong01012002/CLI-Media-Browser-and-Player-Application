#ifndef UART_HPP
#define UART_HPP

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <atomic>
#include <mutex>
#include <string>
#include <functional> 



class UART {
private:
    boost::asio::io_context io_context;      
    boost::asio::serial_port serial_port;     
    std::atomic<bool> running;               
    std::mutex data_mutex;                   
    std::string received_data;                
    std::function<void(const std::string&)> dataCallback;

public:
    UART(const std::string& port, unsigned int baud_rate); 
    ~UART();                                             

    void writeData(const std::string& data);    
    void readData();                           
    void startReadLoop();                      
    void stop();                              
    void setDataCallback(const std::function<void(const std::string&)>& callback);
};

#endif // UART_HPP
