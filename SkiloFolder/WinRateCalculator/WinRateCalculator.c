#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *names[] = {"abaddon", "alchemist", "ancient apparition", "anti mage", "axe", "bane", "batrider", "beastmaster", "bloodseeker", "bounty hunter", "brewmaster", "bristleback", "broodmother", "centaur warrunner", "chaos knight", "chen", "clinkz", "clockwerk", "crystal maiden", "dark seer", "dazzle", "death prophet", "disruptor", "doom", "dragon knight", "drow ranger", "earth spirit", "earthshaker", "elder titan", "ember spirit", "enchantress", "enigma", "faceless void", "gyrocopter", "huskar", "invoker", "io", "jakiro", "juggernaut", "keeper of the light", "kunkka", "legion commander", "leshrac", "lich", "lifestealer", "lina", "lion", "lone druid", "luna", "lycan", "magnus", "medusa", "meepo", "mirana", "morphling", "naga siren", "natures prophet", "necrophos", "night stalker", "nyx assassin", "ogre magi", "omniknight", "oracle", "outworld devourer", "phantom assassin", "phantom lancer", "phoenix", "puck", "pudge", "pugna", "queen of pain", "razor", "riki", "rubick", "sand king", "shadow demon", "shadow fiend", "shadow shaman", "silencer", "skywrath mage", "slardar", "slark", "sniper", "spectre", "spirit breaker", "storm spirit", "sven", "techies", "templar assassin", "terrorblade", "tidehunter", "timbersaw", "tinker", "tiny", "treant protector", "troll warlord", "tusk", "undying", "ursa", "vengeful spirit", "venomancer", "viper", "visage", "warlock", "weaver", "windranger", "winter wyvern", "witch doctor", "wraith king", "zeus"};

typedef struct ally
{
	int id;
	int wins;
	int losses;
	double winrate;
} ally;

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

int cmp(const void *a, const void *b)
{
	const ally *ia = a;
	const ally *ib = b;

	if(ia->winrate > ib->winrate)
		return -1;
	else if(ia->winrate < ib->winrate)
		return 1;
	else
		return 0;
}

int main()
{
	FILE *fp_hero;
	char file_name[40];
	char line[40];
	ally allies[110];
	int i, j;
	int id;
	double games, maxgames = 0;



	for(i = 0; i < 110; i++)
	{
		// clear allies array
		for(j = 0; j < 110; j++)
		{
			allies[j].id = j;
			allies[j].wins = 0;
			allies[j].losses = 0;
			allies[j].winrate = 0.0;
		}

		strcpy(file_name, names[i]);
		strcat(file_name, ".txt");

		fp_hero = fopen(file_name, "r");

		readLine(line, fp_hero);
		while(strcmp(line, ""))
		{
			id = atoi(strtok(line, " "));
			if(!strcmp(strtok(NULL, " "), "win"))
			{
				allies[id].wins++;
			}
			else
			{
				allies[id].losses++;
			}
			readLine(line, fp_hero);
		}

		for(j = 0; j < 110; j++)
		{
			games = allies[j].wins + allies[j].losses;
			if(games > 0)
				allies[j].winrate = allies[j].wins / games;
			else
				allies[j].winrate = 0.5;

			if(games > maxgames && allies[j].id != i)
			{
				maxgames = games;
			}
		}

		qsort(allies, 110, sizeof(ally), cmp	);

		remove(file_name);
		fp_hero = fopen(file_name, "w");

		for(j = 0; j < 110; j++)
		{
			fprintf(fp_hero, "%d,%d,%lf\n", allies[j].wins + allies[j].losses, allies[j].id, allies[j].winrate);
		}
	}

	printf("maxgames = %lf\n", maxgames);
	getch();

	return 0;
}
