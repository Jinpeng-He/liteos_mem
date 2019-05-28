#include<los_memory.h>
#include <stdio.h>
#include<string.h>


UINT8 *m_aucSysMem0;
extern UINT32 osMemSystemInit();
#define LENGTH_TEST		128
#define HELLO_STR		"hello world"
int main()
{
	osMemSystemInit();
	printf("liteos mem init\n");

	char *test=LOS_MemAlloc(m_aucSysMem0,LENGTH_TEST);
	memset(test,0,LENGTH_TEST);
	memcpy(test,HELLO_STR,strlen(HELLO_STR));
	printf("test=%s\n",test);
	LOS_MemFree(m_aucSysMem0, test);
	return 0;
}
