// Include standard C++ libraries
#include <cstdlib>
#include <string>

// Include third-party libraries
#include <boost/asio/connect.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/system/system_error.hpp>
