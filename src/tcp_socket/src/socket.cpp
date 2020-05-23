#include "network_socket/tcp/socket.hpp"

namespace network_socket
{
	namespace tcp
	{
		Socket::Socket(void) : m_deadlineTimer(m_ioService), m_isSocketConnected(false), m_ongoingRead(false), m_socket(m_ioService)
		{
			// 
			m_deadlineTimer.expires_at(boost::posix_time::pos_infin);
		}

		Socket::~Socket(void)
		{
			disconnect();
		}

		void Socket::disconnect(void)
		{
			m_socket.close();
			m_isSocketConnected = false;
		}

		TcpOperationStatus Socket::read(std::string &t_message, const uint16_t &t_timeoutLimit, const size_t &t_maxSize, const size_t &t_minSize)
		{
			try
			{
				if (m_isSocketConnected)
				{
					// 
					m_deadlineTimer.expires_from_now(boost::posix_time::seconds(t_timeoutLimit));

					// 
					m_ongoingRead = true;
					
					boost::asio::streambuf buffer;
					boost::asio::streambuf::mutable_buffers_type mutableBuffer = buffer.prepare(t_maxSize);

					boost::asio::async_read(m_socket, mutableBuffer, boost::asio::transfer_at_least(t_minSize),
						boost::bind(&Socket::readHandler, this, boost::asio::placeholders::error, &buffer, boost::asio::placeholders::bytes_transferred));

					// 
					do
					{
						m_ioService.run_one();
					} while (m_ongoingRead);

					// 
					t_message = std::string((std::istreambuf_iterator<char>(&buffer)), std::istreambuf_iterator<char>());

					// 
					return TcpOperationStatus{true, ""};
				}
				else
				{
					return TcpOperationStatus{false, "Socket not connected"};
				}
			}
			catch (std::exception &e)
			{
				return TcpOperationStatus{false, e.what()};
			}
		}

		TcpOperationStatus Socket::readLine(std::string &t_message, const uint16_t &t_timeoutLimit)
		{
			try
			{
				if (m_isSocketConnected)
				{
					// 
					m_deadlineTimer.expires_from_now(boost::posix_time::seconds(t_timeoutLimit));

					// 
					boost::system::error_code errorCode = boost::asio::error::would_block;

					// 
					boost::asio::streambuf buffer;
					boost::asio::async_read_until(m_socket, buffer, '\n', boost::lambda::var(errorCode) = boost::lambda::_1);

					// 
					do
					{
						m_ioService.run_one();
					} while (errorCode == boost::asio::error::would_block);

					if (errorCode)
					{
						throw boost::system::system_error(errorCode);
					}

					std::istream is(&buffer);
					std::getline(is, t_message);

					// 
					return TcpOperationStatus{true, ""};
				}
				else
				{
					return TcpOperationStatus{false, "Socket not connected"};
				}
			}
			catch (std::exception &e)
			{
				return TcpOperationStatus{false, e.what()};
			}
		}

		TcpOperationStatus Socket::write(const std::string &t_message, const uint16_t &t_timeoutLimit)
		{
			try
			{
				if (m_isSocketConnected)
				{
					// 
					m_deadlineTimer.expires_from_now(boost::posix_time::seconds(t_timeoutLimit));

					// 
					boost::system::error_code errorCode = boost::asio::error::would_block;

					// 
					boost::asio::async_write(m_socket, boost::asio::buffer(t_message), boost::lambda::var(errorCode) = boost::lambda::_1);

					// 
					do
					{
						m_ioService.run_one();
					} while (errorCode == boost::asio::error::would_block);

					if (errorCode)
					{
						throw boost::system::system_error(errorCode);
					}

					// 
					return TcpOperationStatus{true, ""};
				}
				else
				{
					return TcpOperationStatus{false, "Socket not connected"};
				}
			}
			catch (std::exception &e)
			{
				return TcpOperationStatus{false, e.what()};
			}
		}

		TcpOperationStatus Socket::writeLine(const std::string &t_message, const uint16_t &t_timeoutLimit)
		{
			return write(t_message + "\n", t_timeoutLimit);
		}

		void Socket::checkDeadline(void)
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
			m_deadlineTimer.async_wait(boost::lambda::bind(&Socket::checkDeadline, this));
		}

		void Socket::readHandler(const boost::system::error_code &t_errorCode, boost::asio::streambuf *buffer, const std::size_t &t_bytesTransferred)
		{
			if (t_errorCode)
			{
				throw boost::system::system_error(t_errorCode ? t_errorCode : boost::asio::error::operation_aborted);
			}
			else
			{
				buffer->commit(t_bytesTransferred);
			}

			m_ongoingRead = false;
		}
	}
}
