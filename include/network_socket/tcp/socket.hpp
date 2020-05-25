#ifndef NETSOCK_TCP_SOCKET_HPP
#define NETSOCK_TCP_SOCKET_HPP

// Include pre-compiled headers
#include "network_socket/tcp/pch.hpp"

// Macros
#define DEFAULT_TIMEOUT_LIMIT		30
#define DEFAULT_REC_MSG_MIN_SIZE	1
#define DEFAULT_REC_MSG_MAX_SIZE	2000

namespace network_socket
{
	namespace tcp
	{
		struct OperationStatus
		{
			bool success;
			std::string message;
		};

		struct Endpoint
        {
		    std::string address;
		    unsigned int port;
        };

		class Socket
		{
		public:
			Socket(void);
			virtual ~Socket(void);

			void disconnect(void);
            OperationStatus getLocalEndpoint(Endpoint &t_endpoint) const;
            OperationStatus getLocalEndpointAddress(std::string &t_address) const;
            OperationStatus getLocalEndpointPort(unsigned int &t_port) const;
            OperationStatus getRemoteEndpoint(Endpoint &t_endpoint) const;
            OperationStatus getRemoteEndpointAddress(std::string &t_address) const;
            OperationStatus getRemoteEndpointPort(unsigned int &t_port) const;
            bool isSocketConnected(void) const;
            bool isSocketOpen(void) const;
			OperationStatus read(std::string &t_message, const uint16_t &t_timeoutLimit = DEFAULT_TIMEOUT_LIMIT, const size_t &t_maxSize = DEFAULT_REC_MSG_MAX_SIZE, const size_t &t_minSize = DEFAULT_REC_MSG_MIN_SIZE);
			OperationStatus readLine(std::string &t_message, const uint16_t &t_timeoutLimit = DEFAULT_TIMEOUT_LIMIT);
			OperationStatus write(const std::string &t_message, const uint16_t &t_timeoutLimit = DEFAULT_TIMEOUT_LIMIT);
			OperationStatus writeLine(const std::string &t_message, const uint16_t &t_timeoutLimit = DEFAULT_TIMEOUT_LIMIT);

		protected:
			virtual void checkDeadline(void) = 0;
			void readHandler(const boost::system::error_code &t_errorCode, boost::asio::streambuf *t_buffer, const std::size_t &t_bytesTransferred);

            bool m_ongoingRead;
            bool m_isSocketConnected;
            boost::asio::io_context m_ioContext;
            boost::asio::ip::tcp::socket m_socket;
            boost::asio::deadline_timer m_deadlineTimer;
		};
	}
}

#endif // NETSOCK_TCP_SOCKET_HPP
