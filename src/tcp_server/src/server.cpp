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

		OperationStatus Server::acceptConnection(const uint16_t &t_timeoutLimit)
		{
			try
			{
                //
                if (!isSocketOpen())
                {
                    OperationStatus opStatus;

                    if (!openSocket(opStatus))
                    {
                        return opStatus;
                    }
                }

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
					return OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};
				}
				else
				{
					return OperationStatus{false, StatusCode::SOCKET_ALREADY_CONNECTED, m_STATUS_CODE_MSG[(uint16_t)StatusCode::SOCKET_ALREADY_CONNECTED]};
				}
			}
			catch (std::exception &e)
			{
                disconnect();

                return OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
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
			if (t_errorCode || !isSocketOpen())
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
