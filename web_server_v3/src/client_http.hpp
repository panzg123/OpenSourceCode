#ifndef CLIENT_HTTP_HPP
#define	CLIENT_HTTP_HPP

#include <boost/asio.hpp>

#include <unordered_map>
#include <map>
#include <random>

namespace SimpleWeb {
    template <class socket_type>
    class ClientBase {
    public:
    	//[p]内部类，HTTP响应结构体
        class Response {
            friend class ClientBase<socket_type>;
            
        public:
            std::string http_version, status_code;      //[p]HTTP协议版本，状态码

            std::istream content;                                    //[p]响应报文主体

            std::unordered_map<std::string, std::string> header; //[p]存储响应首部
            
        private:
            boost::asio::streambuf content_buffer;   //[p]缓冲区
            
            Response(): content(&content_buffer) {};//[p]响应内容？
        };
    
        std::shared_ptr<Response> request(const std::string& request_type, const std::string& path="/", 
                const std::map<std::string, std::string>& header=std::map<std::string, std::string>()) {
            std::stringstream empty_ss;
            return request(request_type, path, empty_ss, header);
        }
        //[p]request方法重载，客户端请求方法
        std::shared_ptr<Response> request(const std::string& request_type, const std::string& path, std::ostream& content, 
                const std::map<std::string, std::string>& header=std::map<std::string, std::string>()) {
            std::string corrected_path=path;
            if(corrected_path=="")
                corrected_path="/";
            
            content.seekp(0, std::ios::end);
            size_t content_length=content.tellp();
            content.seekp(0, std::ios::beg);
            //[p]下面用write_buffer来构造一个完整的HTTP请求
            boost::asio::streambuf write_buffer;
            std::ostream write_stream(&write_buffer);
            //[p]构造请求行
            write_stream << request_type << " " << corrected_path << " HTTP/1.1\r\n";
            //[p]构造首部
            write_stream << "Host: " << host << "\r\n";
            for(auto& h: header) {
                write_stream << h.first << ": " << h.second << "\r\n";
            }
            if(content_length>0)
                write_stream << "Content-Length: " << std::to_string(content_length) << "\r\n";
            write_stream << "\r\n";
            //[p]构造请求主体
            if(content_length>0)
                write_stream << content.rdbuf();

            std::shared_ptr<Response> response(new Response());
            
            try {
            	//[p]请求连接
                connect();
                //[p]asio::write写，发送http请求
                boost::asio::write(*socket, write_buffer);
                //[p]接收响应报文内容
                size_t bytes_transferred = boost::asio::read_until(*socket, response->content_buffer, "\r\n\r\n");
                //[p]计算响应报文主体的长度
                size_t num_additional_bytes=response->content_buffer.size()-bytes_transferred;
                //[p]解析响应首部
                parse_response_header(response, response->content);
                //[p]读取响应报文主体
                if(response->header.count("Content-Length")>0) {
                    boost::asio::read(*socket, response->content_buffer, 
                            boost::asio::transfer_exactly(stoull(response->header["Content-Length"])-num_additional_bytes));
                }
                //[p]没看懂
                else if(response->header.count("Transfer-Encoding")>0 && response->header["Transfer-Encoding"]=="chunked") {
                    boost::asio::streambuf streambuf;
                    std::ostream content(&streambuf);
                    
                    size_t length;
                    std::string buffer;
                    do {
                        size_t bytes_transferred = boost::asio::read_until(*socket, response->content_buffer, "\r\n");
                        std::string line;
                        getline(response->content, line);
                        bytes_transferred-=line.size()+1;
                        line.pop_back();
                        length=stoull(line, 0, 16);

                        size_t num_additional_bytes=response->content_buffer.size()-bytes_transferred;
                    
                        if((2+length)>num_additional_bytes) {
                            boost::asio::read(*socket, response->content_buffer, 
                                boost::asio::transfer_exactly(2+length-num_additional_bytes));
                        }
                                                
                        buffer.resize(length);
                        response->content.read(&buffer[0], length);
                        content.write(&buffer[0], length);

                        //Remove "\r\n"
                        response->content.get();
                        response->content.get();
                    } while(length>0);
                    
                    std::ostream response_content_output_stream(&response->content_buffer);
                    response_content_output_stream << content.rdbuf();
                }
            }
            catch(const std::exception& e) {
                socket_error=true;
                throw std::invalid_argument(e.what());
            }
            
            return response;
        }
        
    protected:
        boost::asio::io_service asio_io_service;
        boost::asio::ip::tcp::endpoint asio_endpoint;
        boost::asio::ip::tcp::resolver asio_resolver;
        
        std::shared_ptr<socket_type> socket;     //[p]socket
        bool socket_error;
        
        std::string host;
        unsigned short port;
        //[p]构造函数，指定服务器的地址和端口
        ClientBase(const std::string& host_port, unsigned short default_port) : 
                asio_resolver(asio_io_service), socket_error(false) {
            size_t host_end=host_port.find(':');
            if(host_end==std::string::npos) {
                host=host_port;
                port=default_port;
            }
            else {
                host=host_port.substr(0, host_end);
                port=(unsigned short)stoul(host_port.substr(host_end+1));
            }

            asio_endpoint=boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
        }
        
        virtual void connect()=0;
        //[p]解析响应首部
        void parse_response_header(std::shared_ptr<Response> response, std::istream& stream) const {
            std::string line;
            //[p]读取一行首部内容
            getline(stream, line);
            size_t version_end=line.find(' ');
            if(version_end!=std::string::npos) {
                if(5<line.size())
                    response->http_version=line.substr(5, version_end-5);    //[p]协议版本号
                if((version_end+1)<line.size())
                    response->status_code=line.substr(version_end+1, line.size()-(version_end+1)-1);  //[p]状态码
                //[p]下面循环读取响应首部
                getline(stream, line);
                size_t param_end;
                while((param_end=line.find(':'))!=std::string::npos) {
                    size_t value_start=param_end+1;
                    if((value_start)<line.size()) {
                        if(line[value_start]==' ')
                            value_start++;
                        if(value_start<line.size())
                            response->header.insert(std::make_pair(line.substr(0, param_end), line.substr(value_start, line.size()-value_start-1)));
                    }

                    getline(stream, line);
                }
            }
        }
    };
    
    template<class socket_type>
    class Client : public ClientBase<socket_type> {};
    
    typedef boost::asio::ip::tcp::socket HTTP;
    
    template<>
    class Client<HTTP> : public ClientBase<HTTP> {
    public:
    	//[p]构造函数，创建tcp套接字，在父类中指定服务器的地址和端口
        Client(const std::string& server_port_path) : ClientBase<HTTP>::ClientBase(server_port_path, 80) {
            socket=std::make_shared<HTTP>(asio_io_service);
        }
        
    private:
        //[p]请求连接
        void connect() {
            if(socket_error || !socket->is_open()) {
                boost::asio::ip::tcp::resolver::query query(host, std::to_string(port));
                boost::asio::connect(*socket, asio_resolver.resolve(query));
                
                boost::asio::ip::tcp::no_delay option(true);
                socket->set_option(option);
                
                socket_error=false;
            }
        }
    };
}

#endif	/* CLIENT_HTTP_HPP */
