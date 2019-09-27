#include <iostream>
#include <string>

void main()
{
	std::string inputString;
	bool upper = false;
	std::cout << "Write something:" << std::endl;
	std::getline(std::cin, inputString);
	for (int i = 0; i < inputString.length(); i++) 
	{
		if (inputString.at(i) == ' ')
			continue;
		else if (isupper(inputString.at(i)))
		{
			upper = true;
			inputString.at(i) = tolower(inputString.at(i));
		}
		inputString.at(i) = inputString.at(i) + 13 > 'z' ? 'a' + (12 - ('z' - inputString.at(i))) : inputString.at(i) + 13;
		if (upper)
			inputString.at(i) = toupper(inputString.at(i));
	}
	std::cout << inputString << std::endl;
}