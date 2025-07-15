#pragma once

#include "Asio.h"
#include <memory>
#include <vector>
#include <print>

#include "Game.h"

class Client
{
public:
	Client();
	Client(std::shared_ptr<websocket::stream<tcp::socket>> ws);

	void HandleSession();

	//std::optional<json::object> ParseJson(const std::string& message);

	bool IsInGame() const;
	std::string GetGameID() const;
	//std::string GetName() const;

private:
	//void Login(boost::json::object& obj); // Gives player a username
	//void CreateGame(boost::json::object& obj);
	//void JoinGame(boost::json::object& obj);
	//void LeaveGame();

	std::shared_ptr<websocket::stream<tcp::socket>> mWs{};
	std::shared_ptr<Game> mGame{};
	std::string mName = "anonymous";
};
