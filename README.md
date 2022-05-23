### KMP算法的具体实现流程：
* 首先需要找到模式串的前缀数组，前缀数组中的元素对应某一个字符串的最长前后缀的长度。
* 通过前缀数组，我们能够在模式串中找到某个失配位的最长前缀，最长前缀和最长后缀进行一一匹配，能够确保100%准确率。
[![OzdT41.jpg](https://s1.ax1x.com/2022/05/22/OzdT41.jpg)](https://imgtu.com/i/OzdT41)
##### 1）我们来观察一下每个元素对应的最长前后缀的特点：____
* 模式串中的第零位字符对应的元素是0。
* 对于这个prefix数组，第二位字符对应的是A，对应的字符串是ABA，可以发现prefix的元素1表示的是含第二位在内的字符串ABA的最长前后缀的长度。
* 如果某一个字符对应的prefix是n，那么上一位字符对应的prefix一定是n-1，(n-1>=0)
* 比如第四位字符对应的字符串最长前后缀大小是0，因此对于第五位字符对应的字符串，我只需要比较第零个字符和第五个字符是否相等，如果相等，则最长前后缀只能是1。
* 同理，对于第六位字符对应的字符串，我只需要比较第一位字符和第六位字符是否相等，如果相等，则最长前后缀比上一位加1。
##### 2）问题的重点来到了如何通过代码实现prefix数组：____
* 根据上面分析的后三点，我们要想得到这个数组，需要从头向后分析。
* 首先设置一个变量len，这个变量同来表示每一位字符对应的字符串的最长前后缀的长度，这个len从0开始。由于第零位对应的prefix一定是0，所以设置一个变量i  = 1，从模式串的第一位字符开始。
* 如果上一位字符对应的prefix为0，那么当前字符(第n个字符)对应的字符串一定是比较第零位和最后一位(第n位)是否相等。
* 第零位prefix为0，重点，此时的len也为0(稍后会发现只要当前位对应的prefix为0，len一定为0)所以第一位字符对应的字符串需要比较第零位字符和最后一位的字符。不相等，所以len继续为0，prefix为len的值：0。
* 第一位prefix为0，此时len也为0，所以第二位字符对应的字符串需要比较第零位字符和最后一位字符，相等，所以len为1，prefix为len的值：1。
* 第二位prefix为1，此时len为1，所以第三位字符对应的字符串需要比较第一位（这里恰好是第len位）字符和最后一位字符，相等，所以len为2，prefix为len的值：2。
* （重点）第三位prefix为2，此时len为2，所以第四位字符对应的字符串需要比较第二位（这里恰好是第len位）字符和最后一位字符，不相等，所以现在需要往回倒，也就是比较第一位（len--）字符和最后一位字符，不相等，继续往回倒，现在需要比较第零位（len--）字符和最后一位字符，不相等，现在就可以说明这个字符串最长前后缀为0：len为0，prefix为0。我们需要记住一个规律，如果出现匹配不上的情况，那么len的值应该更新为prefiix[len - 1]，然后接着比较第len位和最后一位的字符。
* （重点）我们现在跳到第八位对应的字符串，第七位对应的字符串判断完毕之后，len = 3 ，prefix = 3，接下来需要比较第三位字符和最后一位字符（B和A），不相等，接下来，我们使用上面的结论，len = prefix[len - 1] = 1，现在比较第一位和最后一位的字符串，相等，结束。
##### 3）通过上面的分析我们得出代码流程：
* 首先，第零位prefix为0。
* 接下来会比较第一位对应的字符串。
* 如果匹配不上，有两种情况值得我们讨论，len的值是否为0，如果len的值为0，接下来的字符串我们比较第零位和最后一位的字符；如果len不为0，我们就需要使用结论len = prefix[len - 1]。
* 还需要注意，prefix的值是对应len的值。
```C
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

int* Prefix_table(char* pattern , int length);

int main(void)
{
	char pattern[] = "ABABCABAA";
	int* prefix = Prefix_table(pattern , strlen(pattern));
	
	for (int i = 0 ; i < strlen(pattern) ; i++)
		printf("%d  ",prefix[i]);
	printf("\n");
		
	return 0;	
}
int* Prefix_table(char* pattern , int length)
{
	int* prefix = (int*)malloc(sizeof(int) * length);
	if (!prefix)
	{
		fprintf(stdout,"malloc apply failed!\n");
		exit(1);
	}
	
	prefix[0] = 0;
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
```
##### 4）现在我们通过前缀数组进行KMP模式匹配：
[![XSx100.jpg](https://s1.ax1x.com/2022/05/23/XSx100.jpg)](https://imgtu.com/i/XSx100)
* 此时的失配位是第四位字符，失配位之前的都已经匹配上，并且失配位之前的字符串的最长前后缀为2，因此现在应该把第二位字符挪到失配位上，即j = prefix[j - 1]，然后接着比较string[i]和pattern[j]。
* 这里有一点需要注意，我们首先应该进行字符匹配，当失配的时候，要分两种情况进行讨论：
* 第一种情况是j = 0，此时意味着pattern的第零位字符没有和string的第i位字符匹配上，因此需要让j++，然后接着进行匹配。
* 第二种情况是j不等于0，那么我们需要把最长前后缀的相应位置挪到失配位置，也就是j = prefix[j - 1]，然后接着进行匹配。
[![XSxNp4.jpg](https://s1.ax1x.com/2022/05/23/XSxNp4.jpg)](https://imgtu.com/i/XSxNp4)
* 整个过程需要在一个while循环里面进行，我们发现，如果j（pattern的驱动）等于strlen(pattern)，说明此时模式串已经完全匹配上了，那么我们已经找到一个模式串，此时模式串的起始位置是i - j。
* 如果把循环条件设置成while(i < strlen(string) && j < strlen(pattern))，那么此时遇到了第一个匹配上的模式串就会退出循环，也就是只能找到第一个模式串。
* 如果循环条件为while(i < strlen(string))，那么会找到所有匹配的模式串。
```C
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
```