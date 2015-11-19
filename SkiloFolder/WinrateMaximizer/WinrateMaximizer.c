#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

char *names[] = {"abaddon", "alchemist", "ancient apparition", "anti mage", "axe", "bane", "batrider", "beastmaster", "bloodseeker", "bounty hunter", "brewmaster", "bristleback", "broodmother", "centaur warrunner", "chaos knight", "chen", "clinkz", "clockwerk", "crystal maiden", "dark seer", "dazzle", "death prophet", "disruptor", "doom", "dragon knight", "drow ranger", "earth spirit", "earthshaker", "elder titan", "ember spirit", "enchantress", "enigma", "faceless void", "gyrocopter", "huskar", "invoker", "io", "jakiro", "juggernaut", "keeper of the light", "kunkka", "legion commander", "leshrac", "lich", "lifestealer", "lina", "lion", "lone druid", "luna", "lycan", "magnus", "medusa", "meepo", "mirana", "morphling", "naga siren", "natures prophet", "necrophos", "night stalker", "nyx assassin", "ogre magi", "omniknight", "oracle", "outworld devourer", "phantom assassin", "phantom lancer", "phoenix", "puck", "pudge", "pugna", "queen of pain", "razor", "riki", "rubick", "sand king", "shadow demon", "shadow fiend", "shadow shaman", "silencer", "skywrath mage", "slardar", "slark", "sniper", "spectre", "spirit breaker", "storm spirit", "sven", "techies", "templar assassin", "terrorblade", "tidehunter", "timbersaw", "tinker", "tiny", "treant protector", "troll warlord", "tusk", "undying", "ursa", "vengeful spirit", "venomancer", "viper", "visage", "warlock", "weaver", "windranger", "winter wyvern", "witch doctor", "wraith king", "zeus"};

typedef struct pair
{
	int id1;
	int id2;
	int cluster;
	double winrate;
} pair;

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

int cmp_cluster(const void *a, const void *b)
{
	const pair *ia = a;
	const pair *ib = b;

	if(ia->cluster > ib->cluster)
		return -1;
	else if(ia->cluster < ib->cluster)
		return 1;
	else
		return 0;
}

int cmp_winrate(const void *a, const void *b)
{
	const pair *ia = a;
	const pair *ib = b;

	if(ia->winrate > ib->winrate)
		return -1;
	else if(ia->winrate < ib->winrate)
		return 1;
	else
		return 0;
}

int main()
{
	FILE *fp_hero, *fp_out;
	char file_name[40];
	char line[40];
	int cluster;
	pair pairs[12100];
	int i, j;
	int id;
	double winrate;

	int start;

	for(i = 0; i < 110; i++)
	{
		strcpy(file_name, "k_");
		strcat(file_name, names[i]);
		strcat(file_name, ".txt");

		fp_hero = fopen(file_name, "r");

		j = 0;
		readLine(line, fp_hero);
		while(strcmp(line, ""))
		{
			cluster = atoi(strtok(line, ","));
			id = atoi(strtok(NULL, ","));
			winrate = atof(strtok(NULL, ","));

			pairs[i*110 + j].id1 = i;
			pairs[i*110 + j].id2 = id;
			pairs[i*110 + j].cluster = cluster;
			pairs[i*110 + j].winrate = winrate;

			readLine(line, fp_hero);
			j++;
		}
	}

	qsort(pairs, 12100, sizeof(pair), cmp_cluster);

	start = -1;
	j = 0;
	cluster = 1000;
	start = 0;
	cluster = pairs[0].cluster;
	for(i = 0; i < 12100; i++)
	{
		if(pairs[i].cluster < cluster)
		{
		    cluster = pairs[i].cluster;
			qsort(&pairs[start], i - start, sizeof(pair), cmp_winrate);
			start = i;
		}
	}
	qsort(&pairs[start], i - start, sizeof(pair), cmp_winrate);

	fp_out = fopen("final.txt", "w");

	for(i = 0; i < 12100; i++)
	{
	    if(pairs[i].id1 != pairs[i].id2 && pairs[i].id1 > pairs[i].id2)
            fprintf(fp_out, "%d,%d,%d,%lf -----> 2 herois: %s e %s com winrate = %.0lf%c\n", pairs[i].cluster, pairs[i].id1, pairs[i].id2, pairs[i].winrate, names[pairs[i].id1], names[pairs[i].id2], pairs[i].winrate * 100, '%');
	}

	fclose(fp_out);

	return 0;
}
