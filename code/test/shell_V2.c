#include "syscall.h"

int main()
{
	SpaceId newProc;
	char prompt[2], buffer[60];

	prompt[0] = '-';
	prompt[1] = '-';

	while (1)
	{
		PutString(prompt);
		GetString(buffer, 60);
		PutString(buffer);
		newProc = ForkExec(buffer);
		UserThreadJoin(newProc);
	}
}
