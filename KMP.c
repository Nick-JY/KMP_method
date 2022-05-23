#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

int* Prefix_table(char* pattern , int* prefix , int length);
int KMP(char* string , char* pattern , int length);

int main(void)
{
	char string[] = "ABABABABCABAAB";
	char pattern[] = "ABABCABAA";
	
	int relay = KMP(string , pattern , strlen(pattern));
	if (relay == -1)
		printf("Can't find %s in %s\n",pattern , string);
	else
		printf("The %s first pos is:%d\n",pattern , relay);
	return 0;
}
int* Prefix_table(char* pattern , int* prefix , int length)
{
	int len = 0;
	int i = 1;
	while (i < length)
	{
		if (pattern[i] == pattern[len])
		{
			len++;
			prefix[i] = len;
			i++;
		}
		else
		{
			if (!len)
				i++;
			else
				len = prefix[len - 1];
		}
	}
	
	return prefix;
}
int KMP(char* string , char* pattern , int length)
{
	int* prefix = (int*)malloc(sizeof(int) * length);
	if (!prefix)
	{
		fprintf(stdout,"malloc apply failed!\n");
		exit(1);
	}
	prefix[0] = 0;
	prefix = Prefix_table(pattern , prefix , length);
	
	int i = 0;
	int j = 0;
	while (i < strlen(string) && j < strlen(pattern))
	{
		if (string[i] == pattern[j])
		{
			i++;
			j++;
		}
		else
		{
			if (!j)
				j++;
			else
				j = prefix[j - 1];
		}
	}
	if (j == strlen(pattern))
		return i - j;
	return -1;
}
