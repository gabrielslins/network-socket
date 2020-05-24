#ifndef NETSOCK_TCP_SERVER_HPP
#define NETSOCK_TCP_SERVER_HPP

// 
#include "network_socket/tcp/socket.hpp"

namespace network_socket
{
	namespace tcp
	{
		class Server : public Socket
		{
		public:
			Server(const unsigned int &t_port);
			virtual ~Server(void);

			TcpOperationStatus acceptConnection(const uint16_t &t_timeoutLimit = DEFAULT_TIMEOUT_LIMIT);
			void setPort(const unsigned int &t_port);

		private:
			void acceptHandler(const boost::system::error_code &t_errorCode);
			void checkDeadline(void) override;

			boost::asio::ip::tcp::acceptor m_acceptor;
			bool m_ongoingAccept;
		};
	}
}

#endif // NETSOCK_TCP_SERVER_HPP
