#include <stdio.h>
#include <io.h>
#include <malloc.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void main() {
	FILE *archive = fopen("input", "rb");
	if (archive == NULL) {
		printf("Failed to open file 'input', aborting\n");
		Sleep(5000);
		return;
	}
	fseek(archive, 0, SEEK_END);
	int filesize = ftell(archive);
	fseek(archive, 0, SEEK_SET);
	char *file = (char*)malloc(filesize);
	fread(file, filesize, 1, archive);
	fclose(archive);
	unsigned int *ifile = (unsigned int*)file;
	unsigned int i = 0;
	bool infile = false;
	unsigned int startpos = 0;
	int numfiles = 0;
	while (i < filesize/4) {
		// free pvr extractor lol just uncomment this part
		//check GBIX header
		/*if (ifile[i] == 0x58494247) {
			int fsize = ifile[i + 5]+0x18;
			char *outpvr = (char*)malloc(fsize);
			memcpy(outpvr, file + (i * 4), fsize);
			char *filenum = (char*)malloc(16);
			sprintf(filenum, "%d", numfiles);
			char *filename = (char*)malloc(20);
			strcpy(filename, filenum);
			strcat(filename, ".pvr");
			FILE *outfile = fopen(filename, "wb");
			fwrite(outpvr, fsize, 1, outfile);
			fclose(outfile);
			free(outpvr);
			free(filenum);
			free(filename);
			numfiles += 1;
			i += (fsize-4) / 4;
		}*/
		//check nxif header
		if (ifile[i] == 0x4649584E) {
			infile = true;
			startpos = i * 4;
		}
		//check nend header
		if (ifile[i] == 0x444E454E && infile == true) {
			infile = false;
			int fsize = ((i+4) * 4) - startpos;
			char *filenum = (char*)malloc(16);
			sprintf(filenum, "%d", numfiles);
			char *filename = (char*)malloc(20);
			strcpy(filename, filenum);
			strcat(filename, ".xno");
			char *xnodat = (char*)malloc(fsize);
			memcpy(xnodat, file + startpos, fsize);
			FILE *outxno = fopen(filename, "wb");
			fwrite(xnodat, fsize, 1, outxno);
			fclose(outxno);
			fclose(outxno);
			free(xnodat);
			free(filenum);
			free(filename);
			numfiles += 1;
		}
		++i;
	}
	printf("Done!\n");
	Sleep(5000);
}