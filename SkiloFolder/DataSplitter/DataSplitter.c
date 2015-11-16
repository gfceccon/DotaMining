#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "util.h"

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

void getArg(char *aux, char *arg, char *line, int arg_id)
{
	int i = 0;

	strcpy(aux, line);

	aux = strtok(aux, ",");
	while(i != arg_id)
	{
		aux = strtok(NULL, ",");
		i++;
	}

	strcpy(arg, aux);
}

int main()
{
	FILE *fp_database;
	FILE *fp_hero;
	char match[300];
	char valid_match;
	char aux[300];
	char arg[30];
	char file_name[40];
	char win;
	int i, j;
	int heroes[5];

	fp_database = fopen("data.csv", "r");


	readLine(match, fp_database);

	while(strcmp(match, ""))
	{
		valid_match = 1;
		for(i = 0, j = 0; i < 116; i++)
		{
			if(!valid_match)
			{
				break;
			}

			getArg(aux, arg, match, i);
			switch (i)
			{
				case 0:
					printf("\nMatch ID: %s", arg);
				break;

				case 1:
				{
					if(strcmp(arg, "OK"))
						valid_match = 0;
					//printf("\nMatch Status: %s", arg);
				}
				break;

				case 2:
					//printf("\nMatch Bracket: %s", arg);
				break;

				case 3:
					//printf("\nMatch Type: %s", arg);
				break;

				case 4:
				{
					if(!strcmp(arg, "Ability Draft"))
					{
						valid_match = 0;
					}
					//printf("\nGame Mode: %s", arg);
				}
				break;

				case 115:
					if(strcmp(arg, "0"))
					{
						win = 1;
						//printf("\nVICTORY!");
					}
					else
					{
						win = 0;
						//printf("\nDEFEAT!");
					}
				break;

				default:
					if(strcmp(arg, "0"))
					{
						heroes[j++] = i - 5;
						//printf("\n%s", names[i - 5]);
					}
				break;
			}
		}

		if(!valid_match)
		{
			readLine(match, fp_database);
			continue;
		}

		for(i = 0; i < 5; i++)
		{
			strcpy(file_name, names[heroes[i]]);
			strcat(file_name, ".txt");
			fp_hero = fopen(file_name, "a");

			for(j = 0; j < 5; j++)
			{
				if(win)
					fprintf(fp_hero, "%d win\n", heroes[j]);
				else
					fprintf(fp_hero, "%d loss\n", heroes[j]);
			}
			fclose(fp_hero);
		}

		readLine(match, fp_database);
	}

	return 0;
}
