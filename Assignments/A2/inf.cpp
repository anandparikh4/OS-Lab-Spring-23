#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
// EACCES
int main() {
	char file[256]; // or whatever, I think there is a #define for this, something like PATH_MAX
	strcat(strcpy(file, getenv("HOME")), "/.history");
	FILE *fp  = fopen(file,"w");
	if(fp==NULL){
		printf("I give up\n");
	}
    return 0;
}