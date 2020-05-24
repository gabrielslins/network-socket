#include "network_socket/tcp/server.hpp"

namespace network_socket
{
	namespace tcp
	{
		Server::Server(const unsigned int &t_port) : m_acceptor(m_ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), t_port)), m_ongoingAccept(false)
		{
			// 
			checkDeadline();
		}

		Server::~Server(void)
		{
			disconnect();
		}

		TcpOperationStatus Server::acceptConnection(const uint16_t &t_timeoutLimit)
		{
			try
			{
				if (!m_isSocketConnected)
				{
					// 
					m_deadlineTimer.expires_from_now(boost::posix_time::seconds(t_timeoutLimit));

					// 
					m_ongoingAccept = true;
					m_acceptor.async_accept(m_socket, boost::bind(&Server::acceptHandler, this, boost::asio::placeholders::error));

					// 
					do
					{
						m_ioContext.run_one();
					} while (m_ongoingAccept);

					// 
					m_isSocketConnected = true;
					return TcpOperationStatus{true, ""};
				}
				else
				{
					return TcpOperationStatus{false, "Socket already connected"};
				}
			}
			catch (std::exception &e)
			{
				return TcpOperationStatus{false, e.what()};
			}
		}

		void Server::setPort(const unsigned int &t_port)
		{
			disconnect();
			m_acceptor.close();
			m_acceptor = boost::asio::ip::tcp::acceptor(m_ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), t_port));
		}

		void Server::acceptHandler(const boost::system::error_code &t_errorCode)
		{
			// 
			if (t_errorCode || !m_socket.is_open())
			{
				throw boost::system::system_error(t_errorCode ? t_errorCode : boost::asio::error::operation_aborted);
			}

			m_ongoingAccept = false;
		}

		void Server::checkDeadline(void)
		{
			// 
			if (m_deadlineTimer.expires_at() <= boost::asio::deadline_timer::traits_type::now())
			{
				// 
				boost::system::error_code ignoredErrorCode;
				m_socket.close(ignoredErrorCode);
				m_acceptor.cancel(ignoredErrorCode);

				// 
				m_deadlineTimer.expires_at(boost::posix_time::pos_infin);
			}

			// 
			m_deadlineTimer.async_wait(boost::lambda::bind(&Server::checkDeadline, this));
		}
	}
}
