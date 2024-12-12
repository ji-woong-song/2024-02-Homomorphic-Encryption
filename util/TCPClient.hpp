//
// Created by jiwoong_dev on 2024-12-08.
//

#ifndef SEAL_REAL_TCPCLIENT_H
#define SEAL_REAL_TCPCLIENT_H

#include <boost/asio.hpp>
#include <iostream>

class TCPClient {
private:
    boost::asio::io_context& context;
    std::string pid;
public:
    TCPClient(boost::asio::io_context& context, const std::string pid):context(context) {}

    void send(const std::string& data, std::string& response) {
        try {
            boost::asio::ip::tcp::resolver resolver(context);
            auto endpoints = resolver.resolve("127.0.0.1", pid);
            boost::asio::ip::tcp::socket socket(context);
            boost::asio::connect(socket, endpoints);

            boost::asio::write(socket, boost::asio::buffer(data));

            // 업로드 응답 수신
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(response), error);
            std::cout << "서버 응답: " << std::string(response, length) << std::endl;
        } catch  (std::exception &e) {
            std::cerr << "클라이언트 오류: " << e.what() << "\n";
        }
    };

};

#endif //SEAL_REAL_TCPCLIENT_H
