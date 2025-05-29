#include "ControllerManager.hpp"
#include "UART.hpp"
#include <iostream>
#include <thread>
#include <boost/system/system_error.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>

// Constructor
UART::UART(const std::string& port, unsigned int baud_rate)
    : serial_port(io_context), running(true) {
    try {
        serial_port.open(port);

        serial_port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        serial_port.set_option(boost::asio::serial_port_base::character_size(8));
        serial_port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        serial_port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        serial_port.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

    } catch (const boost::system::system_error& e) {
        std::cerr << "Error opening serial port: " << e.what() << std::endl;
        throw;
    }
}

// Destructor
UART::~UART() {
    stop();
}

void UART::setDataCallback(const std::function<void(const std::string&)>& callback) {
    dataCallback = callback;
}

// Send data to UART
void UART::writeData(const std::string& data) {
    try {
        boost::asio::write(serial_port, boost::asio::buffer(data));
    } catch (const boost::system::system_error& e) {
        std::cerr << "Error sending data: " << e.what() << std::endl;
    }
}

// Read data from UART
void UART::readData() {
    try {
        char buffer[256];
        std::string receivedData;
        while (running) {
            size_t bytes_read = serial_port.read_some(boost::asio::buffer(buffer));
            if (bytes_read > 0) {
                std::lock_guard<std::mutex> lock(data_mutex);
                receivedData.append(buffer, bytes_read);

                size_t pos;
                while ((pos = receivedData.find('\n')) != std::string::npos) {
                    std::string data = receivedData.substr(0, pos);
                    receivedData.erase(0, pos + 1);

                    // Callback handler data
                    if (dataCallback) {
                        dataCallback(data);
                    } else {
                        std::cout << "No callback registered to handle data: " << data << std::endl;
                    }
                }
            }
        }
    } catch (const boost::system::system_error& e) {
        if (running) {
            std::cerr << "Error reading data: " << e.what() << std::endl;
        }
    }
}

// Start reading loop
void UART::startReadLoop() {
    std::thread([this]() {
        readData();
    }).detach();
}

// Stop UART communication
void UART::stop() {
    running = false;
    if (serial_port.is_open()) {
        serial_port.close();
    }
    std::cout << "Serial communication stopped.\n";
}
