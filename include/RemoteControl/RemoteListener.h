
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

//TODO: Asta e incomplet

using boost::asio::ip::tcp;

namespace Bolts{
	namespace Remote{

		class TCPServer{

		};

	}
}


class TCPConnection
	: public boost::enable_shared_from_this<TCPConnection>
{
public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new TCPConnection(io_service));
	}

	tcp::socket& socket()
	{
		return m_socket;
	}

private:
	TCPConnection(boost::asio::io_service& io_service)
		: m_socket(io_service)
	{
	}

	tcp::socket m_socket;
};

class TCPServer
{
public:
	TCPServer(boost::asio::io_service& io_service, uint16_t listenPort)
		: m_acceptor(io_service, tcp::endpoint(tcp::v4(), listenPort))
	{
	}

	void ListenForNewConnection()
	{
		m_connection = TCPConnection::create(m_acceptor.get_io_service());
		StartListeningForConnection ();
	}

	void ListenForMessages()
	{
		if (!m_connection){
			//TODO LOG: No TCP Connection
		}
	}

	//Return true if you want to continue listening for connections
	virtual bool ConnectionAccepted(const boost::system::error_code& error) = 0;

private:
	void StartListeningForConnection()
	{
		m_acceptor.async_accept(m_connection->socket(),
			boost::bind(&TCPServer::HandleAccept, this,
			boost::asio::placeholders::error));
	}

	void HandleAccept( const boost::system::error_code& error)
	{
		if (!error)
		{
			if ( ConnectionAccepted( error) ){
				StartListeningForConnection();
				return;
			}
		}

		StartListeningForConnection();
	}

	TCPConnection::pointer	m_connection;
	tcp::acceptor			m_acceptor;
};