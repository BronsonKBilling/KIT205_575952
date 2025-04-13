#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef char* String;

int main() {
	String* strings;
	String buffer = malloc(sizeof(char) * 100);
	int n;

	printf_s("Please enter the number of strings you want: ");
	scanf_s("%d", &n);

	strings = malloc(sizeof(String) * n);

	for (int i = 0; i < n; i++)
	{
		scanf_s("%s", buffer, 100);
		strings[i] = malloc(strlen(buffer) + 1);
		strcpy_s(strings[i], strlen(buffer) + 1, buffer);
	}

	for (int i = 0; i < n; i++) {
		printf_s(strings[i]);
		printf_s("\n");
	}

	for (int i = 0; i < n; i++) {
		free(strings[i]);
	}
	free(strings);

	return 0;
}