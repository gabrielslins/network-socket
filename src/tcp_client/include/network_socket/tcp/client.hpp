#ifndef NETSOCK_TCP_CLIENT_HPP
#define NETSOCK_TCP_CLIENT_HPP

// 
#include "network_socket/tcp/socket.hpp"

namespace network_socket
{
	namespace tcp
	{
		class Client : public Socket
		{
		public:
			Client(void);
			virtual ~Client(void);

			TcpOperationStatus connect(const std::string &t_host, const std::string &t_port, const uint16_t &t_timeoutLimit = DEFAULT_TIMEOUT_LIMIT);

		private:
			void checkDeadline(void) override;
		};
	}
}

#endif // NETSOCK_TCP_CLIENT_HPP
