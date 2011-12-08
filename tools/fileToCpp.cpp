/**
 *******************************************************************************
 * @file pngToCpp.cpp
 * @brief convert a binary file into a C source vector
 * @author Sandro Sigala
 * @date 26/01/2011
 * @par Project
 * ---
 *
 * @version $Id$
 * 
 * @par Copyright
 * THE "BEER-WARE LICENSE" (Revision 3.1415):
 * sandro AT sigala DOT it wrote this file. As long as you retain this notice you can do
 * whatever you want with this stuff.  If we meet some day, and you think this stuff is
 * worth it, you can buy me a beer in return.  Sandro Sigala
 * 
 * @compilation g++ pngToCpp -o pngToCpp
 * 
 *******************************************************************************
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

bool zeroTerminated = false;

int myfgetc(FILE *f)
{
	int c = fgetc(f);
	if (c == EOF && zeroTerminated)
	{
		zeroTerminated = 0;
		return 0;
	}
	return c;
}

void generateHeader(FILE *file)
{
	fprintf(file, "/**\n");
	fprintf(file, " ******************************************************************************* \n");
	fprintf(file, " * \n");
	fprintf(file, " * @par Project\n");
	fprintf(file, " * Edn\n");
	fprintf(file, " * \n");
	fprintf(file, " * Automatic generated file for Edn Software\n");
	fprintf(file, " * Please do not save this file on File configuration server\n");
	fprintf(file, " * \n");
	fprintf(file, " * @par Copyright\n");
	fprintf(file, " * The copyright of the data depend on the source file but generated file is on GPL v3\n");
	fprintf(file, " * \n");
	fprintf(file, " * Copyright 2010 Edouard DUPIN, all right reserved\n");
	fprintf(file, " * \n");
	fprintf(file, " * This is free software; you can redistribute it and/or modify it under the\n");
	fprintf(file, " * terms of the GNU General Public License as published by the Free Software\n");
	fprintf(file, " * Foundation; either version 3 of the License, or (at your option) any later\n");
	fprintf(file, " * version.\n");
	fprintf(file, " * \n");
	fprintf(file, " * This software is distributed in the hope that it will be useful, but WITHOUT\n");
	fprintf(file, " * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or\n");
	fprintf(file, " * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License\n");
	fprintf(file, " * for more details.\n");
	fprintf(file, " * \n");
	fprintf(file, " ******************************************************************************* \n");
	fprintf(file, " */\n\n");
}

char endTable[256000] = "";

FILE *ofile=NULL;
FILE *ofileH=NULL;

void process(const char *ifname)
{
	printf("                Import File : \"%s\"\n", ifname);
	FILE *ifile=NULL;
	ifile = fopen(ifname, "rb");
	if (ifile == NULL)
	{
		fprintf(stderr, "cannot open %s for reading\n", ifname);
		exit(1);
	}
	char buf[PATH_MAX], *p;
	const char *cp;
	if ((cp = strrchr(ifname, '/')) != NULL)
	{
		++cp;
	} else {
		if ((cp = strrchr(ifname, '\\')) != NULL)
			++cp;
		else
			cp = ifname;
	}
	strcpy(buf, cp);
	for (p = buf; *p != '\0'; ++p)
	{
		if (!isalnum(*p))
			*p = '_';
	}
	fprintf(ofile, "unsigned char %s[] = {", buf);
	int n = 0;
	unsigned char c = 0;
	while(fread(&c, 1, 1, ifile) == 1)
	{
		if(n%100 == 0) {
			fprintf(ofile, "\n\t\t");
		}
		fprintf(ofile, "0x%02x,", c);
		n++;
	}
	
	fprintf(ofile, "\n\t\t0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00");
	fprintf(ofile, "\n};\n");
	
	//fprintf(ofileH, "extern unsigned char %s[];\n", buf);
	//fprintf(ofileH, "extern unsigned long int %s_size;\n", buf);
	char tmpVar[4096];
	sprintf(tmpVar, "	{\"%s\", %d , %s},\n", ifname, n, buf);
	strcat (endTable, tmpVar);
	
	fclose(ifile);
}
 
void usage(void)
{
	fprintf(stderr, "usage: pngToCpp <output_file>.xxx <input_file>\n");
	exit(1);
}
 
int main(int argc, char **argv)
{
	//zeroTerminated = true;
	if (argc < 3) {
		usage();
	}
	// dynamic output name : 
	char tmpOutput[PATH_MAX*2] = "";
	strcpy(tmpOutput, argv[1]);
	int32_t sizeName = strlen(tmpOutput);
	if (6>=sizeName) {
		usage();
	}
	if (tmpOutput[sizeName-1] == '.') {
		tmpOutput[sizeName-1] = '\0';
	} else if (tmpOutput[sizeName-2] == '.') {
		tmpOutput[sizeName-2] = '\0';
	} else if (tmpOutput[sizeName-3] == '.') {
		tmpOutput[sizeName-3] = '\0';
	} else if (tmpOutput[sizeName-4] == '.') {
		tmpOutput[sizeName-4] = '\0';
	} else if (tmpOutput[sizeName-5] == '.') {
		tmpOutput[sizeName-5] = '\0';
	} else if (tmpOutput[sizeName-6] == '.') {
		tmpOutput[sizeName-6] = '\0';
	}
	//fprintf(stderr, "writing to %s\n", tmpOutput);
	
	char tmpOutputCPP[PATH_MAX*2] = "";
	char tmpOutputH[PATH_MAX*2] = "";
	// Generate the output filename
	sprintf(tmpOutputCPP,"%s.cpp", tmpOutput);
	sprintf(tmpOutputH,"%s.h", tmpOutput);
	// open destination name : 
	ofile = fopen(tmpOutputCPP, "wb");
	if (ofile == NULL)
	{
		fprintf(stderr, "cannot open %s for writing\n", tmpOutputCPP);
		exit(1);
	}
	generateHeader(ofile);
	ofileH = fopen(tmpOutputH, "wb");
	if (ofileH == NULL)
	{
		fprintf(stderr, "cannot open %s for writing\n", tmpOutputH);
		exit(1);
	}
	generateHeader(ofileH);
	fprintf(ofileH, "#ifndef __INPUT_PNG_FILE_GENERATED_H__\n");
	fprintf(ofileH, "#define __INPUT_PNG_FILE_GENERATED_H__\n\n");
	fprintf(ofileH, "extern \"C\" {\n");
	fprintf(ofileH, "	typedef struct {\n");
	fprintf(ofileH, "		char filename[1024];\n");
	fprintf(ofileH, "		long int fileLenght;\n");
	fprintf(ofileH, "		unsigned char * data;\n");
	fprintf(ofileH, "	} internalData_ts;\n\n\n");
	
	
	// Generate the output
	int32_t i;
	for(i=2; i<argc; i++) {
		process(argv[i]);
	}
	// set the pointer on the table
	fprintf(ofileH, "	extern internalData_ts internalDataFiles[];\n");
	// set the number of files
	fprintf(ofileH, "	extern long int internalDataFilesSize;\n");
	fprintf(ofileH, "}\n");
	fprintf(ofileH, "#endif\n\n");
	
	
	fprintf(ofile, "#include \"GeneratedData.h\"\n");
	fprintf(ofile, "internalData_ts internalDataFiles[] = {\n");
	fprintf(ofile, "%s", endTable);
	fprintf(ofile, "};\n");
	fprintf(ofile, "long int internalDataFilesSize = sizeof(internalDataFiles) / sizeof(internalData_ts);\n");
	
	
	// Close destination files
	fclose(ofile);
	fclose(ofileH);
	
	return 0;
}
