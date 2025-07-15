#include <iostream>

#include "Server.h"
#include "GameManager.h"
#include "Asio.h"

#include <print>

#include "Client.h"

std::unordered_map<std::shared_ptr<websocket::stream<tcp::socket>>, std::shared_ptr<Client>> clients;
std::vector<Game> games;
std::mutex clients_mutex;

void handle_session(tcp::socket socket)
{
	auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));
	try
	{
		ws->accept();

		{
			std::scoped_lock<std::mutex> lock(clients_mutex);
			clients[ws] = std::make_shared<Client>(ws);
		}

		clients[ws]->HandleSession();
	}
	catch (const std::exception& e)
	{
		std::cerr << "WebSocket error: " << e.what() << std::endl;
	}

	{
		std::scoped_lock<std::mutex> lock(clients_mutex);

		auto& gameManager = GameManager::GetInstance();

		if (clients[ws]->IsInGame())
		{
			std::string gameID = clients[ws]->GetGameID();
			gameManager.QuitGame(gameID, ws);
		}

		clients.erase(ws);
	}
}

int main()
{
	try
	{
		asio::io_context ioc;
		tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 3000));

		std::println("WebSocket server listening on port 3000...");
		for (;;)
		{
			tcp::socket socket(ioc);
			acceptor.accept(socket);
			std::println("New websocket accepted");
			std::thread(handle_session, std::move(socket)).detach();
		}
	}
	catch (const std::exception& e)
	{
		std::print(std::cerr, "Server error: {}", e.what());
	}

	return 0;
}
