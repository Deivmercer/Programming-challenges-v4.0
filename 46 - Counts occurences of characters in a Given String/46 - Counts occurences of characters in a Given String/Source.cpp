#include <iostream>
#include <string>

int checkIfAlreadyFound(char* chars, char character, int k);

// NOTE: Doesn't support unicode

void main()
{
	std::string inputString;
	char* chars = nullptr;
	int* count = nullptr;
	std::cout << "Write something:" << std::endl;
	std::getline(std::cin, inputString);
	int j, k = 0;
	for (int i = 0; i < inputString.length(); i++)
	{
		j = checkIfAlreadyFound(chars, inputString.at(i), k);
		if (j >= 0)
			count[j]++;
		else
		{
			chars = (char*)realloc(chars, (k + 1) * sizeof(char));
			count = (int*)realloc(count, (k + 1) * sizeof(int));
			chars[k] = inputString.at(i);
			count[k] = 1;
			k++;
		}
	}
	for (int i = 0; i < k; i++)
		std::cout << chars[i] << ":" << count[i] << std::endl;
	free(chars);
	free(count);
}

int checkIfAlreadyFound(char* chars, char character, int k)
{
	for (int i = 0; i < k; i++)
		if (chars[i] == character)
			return i;
	return -1;
}