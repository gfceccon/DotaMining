#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *names[] = {"abaddon", "alchemist", "ancient apparition", "anti mage", "axe", "bane", "batrider", "beastmaster", "bloodseeker", "bounty hunter", "brewmaster", "bristleback", "broodmother", "centaur warrunner", "chaos knight", "chen", "clinkz", "clockwerk", "crystal maiden", "dark seer", "dazzle", "death prophet", "disruptor", "doom", "dragon knight", "drow ranger", "earth spirit", "earthshaker", "elder titan", "ember spirit", "enchantress", "enigma", "faceless void", "gyrocopter", "huskar", "invoker", "io", "jakiro", "juggernaut", "keeper of the light", "kunkka", "legion commander", "leshrac", "lich", "lifestealer", "lina", "lion", "lone druid", "luna", "lycan", "magnus", "medusa", "meepo", "mirana", "morphling", "naga siren", "natures prophet", "necrophos", "night stalker", "nyx assassin", "ogre magi", "omniknight", "oracle", "outworld devourer", "phantom assassin", "phantom lancer", "phoenix", "puck", "pudge", "pugna", "queen of pain", "razor", "riki", "rubick", "sand king", "shadow demon", "shadow fiend", "shadow shaman", "silencer", "skywrath mage", "slardar", "slark", "sniper", "spectre", "spirit breaker", "storm spirit", "sven", "techies", "templar assassin", "terrorblade", "tidehunter", "timbersaw", "tinker", "tiny", "treant protector", "troll warlord", "tusk", "undying", "ursa", "vengeful spirit", "venomancer", "viper", "visage", "warlock", "weaver", "windranger", "winter wyvern", "witch doctor", "wraith king", "zeus"};

typedef struct pairwinrate
{
	int id1;
	int id2;
	double games;
	double winrate;
	int group;
} pairWinrate;

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

void set_groups(pairWinrate *wr_array, double *group_values, int array_size, int k)
{
	double least;
	double temp_module;
	int i, j;

	for(i = 0; i < array_size; i++)
	{
		least = 280.0;
		for(j = 1; j <= k; j++)
		{
			temp_module = (double) abs((int) (group_values[j] - wr_array[i].games));
			if(temp_module < least)
			{
				least = temp_module;
				wr_array[i].group = j;
			}
		}
	}
}

void k_calculate(double t_value, pairWinrate *wr_array, double *group_values, int array_size, int k)
{
	int i, j;
	double *means_array;
	double k_mean;
	double sum;
	int count;

	// Atribui o grupo mais apropriado para cada byte
	set_groups(wr_array, group_values, array_size, k);

	// Vetor auxiliar que armazenara os novos valores (ou medias) de cada um dos k grupos.
	means_array = (double *) malloc((k + 1) * sizeof(double));

	// Preenche o vetor "means_array" com os novos valores dos k grupos.
	for(i = 1; i <= k; i++)
	{
		sum = 0.0;
		count = 0;
		for(j = 0; j < array_size; j++)
		{
			if(wr_array[j].group == i)
			{
				count++;
				sum += (double) wr_array[j].games;
			}
		}
		if(count)
		{
			means_array[i] = sum / (double) count;
		}
		else
		{
			means_array[i] = group_values[i];
		}
	}

	// Calcula a media de todas as medias (media k) entao libera o vetor "means_array".
	sum = 0;
	for(i = 1; i <= k; i++)
	{
		sum += (double) abs((int) (means_array[i] - group_values[i]));
	}
	k_mean = sum / (double) k;


	// Verifica se o algoritmo precisa ser executado novamente, ou o finaliza.
	if(k_mean < t_value)
	{
		set_groups(wr_array, group_values, array_size, k);
		for(i = 0; i < array_size; i++)
		{
			wr_array[i].games = (double) floor(means_array[wr_array[i].group]);
		}
		free(means_array);
		return;
	}
	else
	{
		k_calculate(t_value, wr_array, means_array, array_size, k);
		free(means_array);
		return;
	}
}

void k_means(pairWinrate *wr_array)
{
	double group_values[6];

	int k = 5;
	group_values[1] = 1.0;
	group_values[2] = 70.0;
	group_values[3] = 140.0;
	group_values[4] = 210.0;
	group_values[5] = 280.0;

	k_calculate(1.5, wr_array, group_values, 12100, k);
}

int main()
{
	FILE *fp_hero, *fp_heronew;
	char file_name[40];
	char line[40];
	pairWinrate wr_array[12100];
	int i, j;

	for(i = 0; i < 110; i++)
	{
		for(j = 0; j < 110; j++)
		{
			wr_array[i*110 + j].id1 = i;
			wr_array[i*110 + j].id2 = 0;
			wr_array[i*110 + j].games = 0.0;
			wr_array[i*110 + j].winrate = 0.0;
			wr_array[i*110 + j].group = 0;
		}

		strcpy(file_name, names[i]);
		strcat(file_name, ".txt");

		fp_hero = fopen(file_name, "r");

		readLine(line, fp_hero);
		j = 0;
		while(strcmp(line, ""))
		{
			wr_array[i*110 + j].games = atoi(strtok(line, ","));
			wr_array[i*110 + j].id2 = atoi(strtok(NULL, ","));
			wr_array[i*110 + j].winrate = atof(strtok(NULL, ","));

			if(wr_array[i*110 + j].games >= 280)
				wr_array[i*110 + j].games = 280;

			readLine(line, fp_hero);
			j++;
		}
	}
	k_means(wr_array);

	for(i = 0; i < 110; i++)
	{
		strcpy(file_name, "k_");
		strcat(file_name, names[i]);
		strcat(file_name, ".txt");

		fp_heronew = fopen(file_name, "w");

		for(j = 0; j < 110; j++)
		{
			fprintf(fp_heronew, "%d,%d,%lf\n", (int) wr_array[i*110 + j].games, wr_array[i*110 + j].id2, wr_array[i*110 + j].winrate);
		}
	}

	return 0;
}
