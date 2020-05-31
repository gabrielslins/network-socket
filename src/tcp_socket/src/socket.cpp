#include "network_socket/tcp/socket.hpp"

namespace network_socket
{
	namespace tcp
	{
        const std::string Socket::m_STATUS_CODE_MSG[] =
		{
			"",
			"Boost error",
			"Unable to open socket",
			"Socket not connected",
			"Socket already connected"
		};

		Socket::Socket(void) : m_ongoingRead(false), m_isSocketConnected(false), m_socket(m_ioContext), m_deadlineTimer(m_ioContext)
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

        Endpoint Socket::getLocalEndpoint(void)
        {
            OperationStatus opStatus;

            return getLocalEndpoint(opStatus);
        }

        Endpoint Socket::getLocalEndpoint(OperationStatus &t_opStatus)
        {
		    Endpoint endpoint = {"", 0};

		    try
		    {
                //
                if (!isSocketOpen())
                {
                    if (!openSocket())
                    {
                        t_opStatus = t_opStatus = OperationStatus{false, StatusCode::SOCKET_CLOSED, m_STATUS_CODE_MSG[(uint16_t) StatusCode::SOCKET_CLOSED]};

                        return endpoint;
                    }
                }

                //
                endpoint =
                {
                    .address = m_socket.local_endpoint().address().to_string(),
                    .port = m_socket.local_endpoint().port()
                };

                t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t) StatusCode::NO_ERROR]};
            }
            catch (std::exception &e)
            {
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
            }

            return endpoint;
        }

        std::string Socket::getLocalEndpointAddress(void)
        {
            OperationStatus opStatus;

            return getLocalEndpointAddress(opStatus);
        }

        std::string Socket::getLocalEndpointAddress(OperationStatus &t_opStatus)
        {
            std::string address = "";

            try
            {
                //
                if (!isSocketOpen())
                {
                    if (!openSocket())
                    {
                        t_opStatus = t_opStatus = OperationStatus{false, StatusCode::SOCKET_CLOSED, m_STATUS_CODE_MSG[(uint16_t) StatusCode::SOCKET_CLOSED]};

                        return address;
                    }
                }

                //
                address = m_socket.local_endpoint().address().to_string();

                t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t) StatusCode::NO_ERROR]};
            }
            catch (std::exception &e)
            {
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
            }

            return address;
        }

        unsigned int Socket::getLocalEndpointPort(void)
        {
            OperationStatus opStatus;

            return getLocalEndpointPort(opStatus);
        }

        unsigned int Socket::getLocalEndpointPort(OperationStatus &t_opStatus)
        {
		    unsigned int port = 0;

            try
            {
                //
                if (!isSocketOpen())
                {
                    if (!openSocket())
                    {
                        t_opStatus = t_opStatus = OperationStatus{false, StatusCode::SOCKET_CLOSED, m_STATUS_CODE_MSG[(uint16_t) StatusCode::SOCKET_CLOSED]};

                        return port;
                    }
                }

                //
                port = m_socket.local_endpoint().port();

                t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t) StatusCode::NO_ERROR]};
            }
            catch (std::exception &e)
            {
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
            }

            return port;
        }

        Endpoint Socket::getRemoteEndpoint(void)
        {
            OperationStatus opStatus;

            return getRemoteEndpoint(opStatus);
        }

        Endpoint Socket::getRemoteEndpoint(OperationStatus &t_opStatus)
        {
            Endpoint endpoint = {"", 0};

            try
            {
                if (isSocketConnected())
                {
                    endpoint =
                    {
                        .address = m_socket.remote_endpoint().address().to_string(),
                        .port = m_socket.remote_endpoint().port()
                    };

                    t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};
                }
                else
                {
                    t_opStatus = OperationStatus{false, StatusCode::SOCKET_DISCONNECTED, m_STATUS_CODE_MSG[(uint16_t)StatusCode::SOCKET_DISCONNECTED]};
                }
            }
            catch (std::exception &e)
            {
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
            }

            return endpoint;
        }

        std::string Socket::getRemoteEndpointAddress(void)
        {
            OperationStatus opStatus;

            return getRemoteEndpointAddress(opStatus);
        }

        std::string Socket::getRemoteEndpointAddress(OperationStatus &t_opStatus)
        {
            std::string address = "";

            try
            {
                if (isSocketConnected())
                {
                    address = m_socket.remote_endpoint().address().to_string();

                    t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};
                }
                else
                {
                    t_opStatus = OperationStatus{false, StatusCode::SOCKET_DISCONNECTED, m_STATUS_CODE_MSG[(uint16_t)StatusCode::SOCKET_DISCONNECTED]};
                }
            }
            catch (std::exception &e)
            {
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
            }

            return address;
        }

        unsigned int Socket::getRemoteEndpointPort(void)
        {
            OperationStatus opStatus;

            return getRemoteEndpointPort(opStatus);
        }

        unsigned int Socket::getRemoteEndpointPort(OperationStatus &t_opStatus)
        {
            unsigned int port = 0;

            try
            {
                if (isSocketConnected())
                {
                    port = m_socket.remote_endpoint().port();

                    t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};
                }
                else
                {
                    t_opStatus = OperationStatus{false, StatusCode::SOCKET_DISCONNECTED, m_STATUS_CODE_MSG[(uint16_t)StatusCode::SOCKET_DISCONNECTED]};
                }
            }
            catch (std::exception &e)
            {
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
            }

            return port;
        }

        bool Socket::isSocketConnected(void)
        {
		    m_isSocketConnected &= isSocketOpen();

			return m_isSocketConnected;
        }

        std::string Socket::read(const uint16_t &t_timeoutLimit, const size_t &t_maxSize, const size_t &t_minSize)
        {
            OperationStatus opStatus;

            return read(opStatus, t_timeoutLimit, t_maxSize, t_minSize);
        }

		std::string Socket::read(OperationStatus &t_opStatus, const uint16_t &t_timeoutLimit, const size_t &t_maxSize, const size_t &t_minSize)
		{
			try
			{
				if (isSocketConnected())
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
						m_ioContext.run_one();
					} while (m_ongoingRead);

					//
                    t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};

                    //
                    return std::string((std::istreambuf_iterator<char>(&buffer)), std::istreambuf_iterator<char>());
				}
				else
				{
				    //
                    t_opStatus = OperationStatus{false, StatusCode::SOCKET_DISCONNECTED, m_STATUS_CODE_MSG[(uint16_t)StatusCode::SOCKET_DISCONNECTED]};

                    //
                    return "";
				}
			}
			catch (std::exception &e)
			{
			    disconnect();

                //
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};

                //
                return "";
			}
		}

        std::string Socket::readLine(const uint16_t &t_timeoutLimit)
        {
            OperationStatus opStatus;

            return read(opStatus, t_timeoutLimit);
        }

        std::string Socket::readLine(OperationStatus &t_opStatus, const uint16_t &t_timeoutLimit)
		{
			try
			{
				if (isSocketConnected())
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
						m_ioContext.run_one();
					} while (errorCode == boost::asio::error::would_block);

					if (errorCode)
					{
						throw boost::system::system_error(errorCode);
					}

					std::istream is(&buffer);
                    std::string message;
                    std::getline(is, message);

                    //
                    t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};

                    //
                    return message;
				}
				else
				{
                    //
                    t_opStatus = OperationStatus{false, StatusCode::SOCKET_DISCONNECTED, m_STATUS_CODE_MSG[(uint16_t)StatusCode::SOCKET_DISCONNECTED]};

                    //
                    return "";
				}
			}
			catch (std::exception &e)
			{
                disconnect();

                //
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};

                //
                return "";
			}
		}

        void Socket::write(const std::string &t_message, const uint16_t &t_timeoutLimit)
        {
            OperationStatus opStatus;

            return write(opStatus, t_message, t_timeoutLimit);
        }

        void Socket::write(OperationStatus &t_opStatus, const std::string &t_message, const uint16_t &t_timeoutLimit)
		{
			try
			{
				if (isSocketConnected())
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
						m_ioContext.run_one();
					} while (errorCode == boost::asio::error::would_block);

					if (errorCode)
					{
						throw boost::system::system_error(errorCode);
					}

					// 
                    t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};
				}
				else
				{
                    t_opStatus = OperationStatus{false, StatusCode::SOCKET_DISCONNECTED, m_STATUS_CODE_MSG[(uint16_t)StatusCode::SOCKET_DISCONNECTED]};
				}
			}
			catch (std::exception &e)
			{
                disconnect();

                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
			}
		}

        void Socket::writeLine(const std::string &t_message, const uint16_t &t_timeoutLimit)
        {
            write(t_message + "\n", t_timeoutLimit);
        }

        void Socket::writeLine(OperationStatus &t_opStatus, const std::string &t_message, const uint16_t &t_timeoutLimit)
		{
			write(t_opStatus, t_message + "\n", t_timeoutLimit);
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

        bool Socket::isSocketOpen(void)
        {
            return m_socket.is_open();
        }

        bool Socket::openSocket(void)
        {
            OperationStatus opStatus;

            return openSocket(opStatus);
        }

        bool Socket::openSocket(OperationStatus &t_opStatus)
        {
            try
            {
                if (!isSocketOpen())
                {
                    boost::system::error_code errorCode;

                    m_socket.open(boost::asio::ip::tcp::v4(), errorCode);

                    if (errorCode)
                    {
                        throw boost::system::system_error(errorCode);
                    }

                    t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};
                }
                else
                {
                    t_opStatus = OperationStatus{true, StatusCode::NO_ERROR, m_STATUS_CODE_MSG[(uint16_t)StatusCode::NO_ERROR]};
                }
            }
            catch (std::exception &e)
            {
                t_opStatus = OperationStatus{false, StatusCode::BOOST_ERROR, e.what()};
            }

            return t_opStatus.success;
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
