#include <iostream>
#include <string>

// NOTE: Doesn't support unicode

typedef struct 
{
	char c;
	int i;
} CharactersList;

int checkIfAlreadyFound(CharactersList* list, char character, int k);

void main()
{
	std::string inputString;
	CharactersList* list = nullptr;
	std::cout << "Write something:" << std::endl;
	std::getline(std::cin, inputString);
	int j, k = 0;
	for (int i = 0; i < inputString.length(); i++)
	{
		j = checkIfAlreadyFound(list, inputString.at(i), k);
		if (j >= 0)
			list[j].i++;
		else
		{
			list = (CharactersList*) realloc(list, (k + 1) * sizeof(CharactersList));
			list[k].c = inputString.at(i);
			list[k].i = 1;
			k++;
		}
	}
	for (int i = 0; i < k; i++)
		std::cout << list[i].c << ":" << list[i].i << std::endl;
	free(list);
}

int checkIfAlreadyFound(CharactersList* list, char character, int k)
{
	for (int i = 0; i < k; i++)
		if (list[i].c == character)
			return i;
	return -1;
}