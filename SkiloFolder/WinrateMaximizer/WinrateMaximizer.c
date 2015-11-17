#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

char *names[] = {"abaddon", "alchemist", "ancient apparition", "anti mage", "axe", "bane", "batrider", "beastmaster", "bloodseeker", "bounty hunter", "brewmaster", "bristleback", "broodmother", "centaur warrunner", "chaos knight", "chen", "clinkz", "clockwerk", "crystal maiden", "dark seer", "dazzle", "death prophet", "disruptor", "doom", "dragon knight", "drow ranger", "earth spirit", "earthshaker", "elder titan", "ember spirit", "enchantress", "enigma", "faceless void", "gyrocopter", "huskar", "invoker", "io", "jakiro", "juggernaut", "keeper of the light", "kunkka", "legion commander", "leshrac", "lich", "lifestealer", "lina", "lion", "lone druid", "luna", "lycan", "magnus", "medusa", "meepo", "mirana", "morphling", "naga siren", "natures prophet", "necrophos", "night stalker", "nyx assassin", "ogre magi", "omniknight", "oracle", "outworld devourer", "phantom assassin", "phantom lancer", "phoenix", "puck", "pudge", "pugna", "queen of pain", "razor", "riki", "rubick", "sand king", "shadow demon", "shadow fiend", "shadow shaman", "silencer", "skywrath mage", "slardar", "slark", "sniper", "spectre", "spirit breaker", "storm spirit", "sven", "techies", "templar assassin", "terrorblade", "tidehunter", "timbersaw", "tinker", "tiny", "treant protector", "troll warlord", "tusk", "undying", "ursa", "vengeful spirit", "venomancer", "viper", "visage", "warlock", "weaver", "windranger", "winter wyvern", "witch doctor", "wraith king", "zeus"};

void readLine(char *line, FILE *stream)
{
	char character;
	int i = 0;

	while(fscanf(stream, "%c", &character) != EOF)
	{
		if(character == '\n' || character == '\r')
			break;

		line[i++] = character;
	}
	line[i] = '\0';
}

int main()
{
	FILE *fp_hero1, *fp_hero2, *fp_hero3, *fp_hero4, *fp_hero5;
	char file_name[40];
	char line[40];
	ally allies[110];
	int i, j;
	int id;
	double games;


	for(i = 0; i < 110; i++)
	{
		for(j = 0; j < 110; j++)
		{
			if(j == i)
				continue;
			for(k = 0; k < 110; k++)
			{
				if(k == i || k == j)
					continue;
				for(l = 0; l < 110; l++)
				{
					if(l == i || l == j || l == k)
						continue;
					for(m = 0; m < 110; m++)
					{
						if(m == i || m == j || m == k || m == l)
							continue;

					}
				}
			}
		}
	}

	return 0;
}
