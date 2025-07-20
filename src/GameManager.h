#pragma once

#include "Game.h"
#include "Asio.h"

#include <unordered_map>
#include <string>
#include <print>
#include <mutex>

class GameManager
{
public:
	static GameManager& GetInstance()
	{
		static GameManager instance;
		return instance;
	}

	void CreateGame(const std::string& gameID)
	{
		std::scoped_lock<std::mutex> lock(m_mutex);
		
		if (m_games.contains(gameID))
		{
			std::println("Player requested to create a game with an already taken id.");
			return;
		}

		m_games[gameID] = std::make_shared<Game>(gameID);
	}

	[[nodiscard]] std::shared_ptr<Game> JoinGame(std::shared_ptr<websocket::stream<tcp::socket>> ws,const std::string& gameID)
	{
		std::scoped_lock<std::mutex> lock(m_mutex);

		if (!m_games.contains(gameID))
		{
			std::println("Player requested to join a game that doesn't exist.");
			return nullptr;
		}

		if (m_games[gameID]->GetNumberOfPlayers() >= 2)
		{
			std::println("Cannot join game {}, it is already full.", gameID);
			return nullptr;
		}

		m_games[gameID]->addPlayer(ws);

		return m_games[gameID];
	}

	void QuitGame(const std::string& gameID, std::shared_ptr<websocket::stream<tcp::socket>> ws)
	{
		std::scoped_lock<std::mutex> lock(m_mutex);

		if (!m_games.contains(gameID))
			return;

		m_games[gameID]->disconnectPlayer(ws);
		if (m_games[gameID]->GetNumberOfPlayers() == 0)
		{
			m_games.erase(gameID);
			std::println("Game: {} has been closed", gameID);
		}
	}

	bool DoesGameExist(const std::string& gameID) const
	{
		return m_games.contains(gameID);
	}

	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

private:
	GameManager() = default;

	std::mutex m_mutex{};
	std::unordered_map<std::string, std::shared_ptr<Game>> m_games{};
};
