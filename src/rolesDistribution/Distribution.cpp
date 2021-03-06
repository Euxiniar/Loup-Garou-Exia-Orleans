#include "Distribution.h"

Distribution::Distribution(Narrator &nar)
	: m_nar(nar)
{
	std::unique_ptr<Werewolf> werewolf(new Werewolf);
	std::unique_ptr<Villager> villager(new Villager);
	m_roles.push_back(std::move(werewolf));
	m_roles.push_back(std::move(villager));
	/***As to change if there's more roles***/
}

void Distribution::distribPlayers(std::vector<Player>& players)
{
	m_nar.enterPlayer(players);
}

void Distribution::distribRoles(std::vector<Player>& players, std::vector<std::vector<Player*>>& rolesArray)
{
	uint16_t choice = m_nar.selectTypeOfRolesDistrib();
	std::vector<uint16_t> nbPlayersPerRoles;
	switch (choice)
	{
	case 1:
		autoDefineRolesNb(players.size(), nbPlayersPerRoles);
		break;
	case 2:
		m_nar.defineRolesNb(players.size(), nbPlayersPerRoles, m_roles);
		break;
	default:
		break;
	}
	defineRoles(players, rolesArray, nbPlayersPerRoles);
	m_nar.distribRolesFinish();
}

void Distribution::autoDefineRolesNb(const size_t & nbPlayers, std::vector<uint16_t> &nbPlayersPerRoles)
{	
	nbPlayersPerRoles.push_back((uint16_t)(nbPlayers*m_roles.at(0)->getCompositionPorcent()));
	nbPlayersPerRoles.push_back((uint16_t)nbPlayers-(uint16_t)(nbPlayers*m_roles.at(0)->getCompositionPorcent()));
	m_nar.showNbPlayersPerRoles(nbPlayersPerRoles, m_roles);
}

void Distribution::defineRoles(std::vector<Player>& players, std::vector<std::vector<Player*>>& rolesArray, const std::vector<uint16_t> &nbPlayersPerRoles)
{
	std::vector<Player*> shuffledPlayers;

	for (std::vector<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		shuffledPlayers.push_back(&*it);
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(std::begin(shuffledPlayers), std::end(shuffledPlayers), g);

	std::vector<Player*> selectedRole;
	for (int y = 0; y < nbPlayersPerRoles.size(); y++)
	{
		for (int x = 0; x < nbPlayersPerRoles.at(y); x++)
		{
			shuffledPlayers.back()->changeRole(m_roles.at(y)->getId());
			selectedRole.push_back(shuffledPlayers.back());
			shuffledPlayers.pop_back();
		}
		rolesArray.push_back(selectedRole);
		selectedRole.clear();
	}
}
