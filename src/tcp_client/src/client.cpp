#include "network_socket/tcp/client.hpp"

namespace network_socket
{
	namespace tcp
	{
		Client::Client(void)
		{
			// 
			checkDeadline();
		}

		Client::~Client(void)
		{
			disconnect();
		}

		OperationStatus Client::connect(const std::string &t_address, const unsigned int &t_port, const uint16_t &t_timeoutLimit)
		{
			try
			{
				if (!m_isSocketConnected)
				{
					// 
					boost::asio::ip::tcp::resolver::query query(t_address, std::to_string(t_port));
					boost::asio::ip::tcp::resolver::iterator iter = boost::asio::ip::tcp::resolver(m_ioContext).resolve(query);

					// 
					m_deadlineTimer.expires_from_now(boost::posix_time::seconds(t_timeoutLimit));

					// 
					boost::system::error_code errorCode = boost::asio::error::would_block;

					// 
					boost::asio::async_connect(m_socket, iter, boost::lambda::var(errorCode) = boost::lambda::_1);

					// 
					do
					{
						m_ioContext.run_one();
					} while (errorCode == boost::asio::error::would_block);

					// 
					if (errorCode || !m_socket.is_open())
					{
						throw boost::system::system_error(errorCode ? errorCode : boost::asio::error::operation_aborted);
					}

					// 
					m_isSocketConnected = true;
					return OperationStatus{true, ""};
				}
				else
				{
					return OperationStatus{false, "Socket already connected"};
				}
			}
			catch (std::exception &e)
			{
                if (!m_socket.is_open())
                {
                    m_isSocketConnected = false;
                }

                return OperationStatus{false, e.what()};
			}
		}

		void Client::checkDeadline(void)
		{
			// 
			if (m_deadlineTimer.expires_at() <= boost::asio::deadline_timer::traits_type::now())
			{
				// 
				boost::system::error_code ignoredErrorCode;
				m_socket.close(ignoredErrorCode);

				// 
				m_deadlineTimer.expires_at(boost::posix_time::pos_infin);
			}

			// 
			m_deadlineTimer.async_wait(boost::lambda::bind(&Client::checkDeadline, this));
		}
	}
}
