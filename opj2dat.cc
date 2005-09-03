#include <stdio.h>
#include <libgen.h>
#include "OPJFile.h"

char version[]="0.1.0";

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage : ./opj2dat <file.opj>\n");
		return -1;
	}

	printf("opj2dat %s, Copyright (C) 2005 Stefan Gerlach\n",version);

	OPJFile opj(argv[1]);
	opj.Parse();
	printf("OPJ PROJECT \"%s\" VERSION = %.1f\n",argv[1],opj.Version());
	
	printf("NUMBER OF SPREADSHEETS = %d\n",opj.numSpreads());
	for (int s=0;s<opj.numSpreads();s++) {
		int nr_cols=opj.numCols(s);
		
		printf("SPREADSHEET %d : %s\n",s+1,opj.spreadName(s));
		printf("	COLUMNS = %d\n",nr_cols);
		for (int j=0;j<nr_cols;j++) {
			printf("	COLUMN %d : %s / TYPE = %s,ROWS = %d\n",
				j+1,opj.colName(s,j),opj.colType(s,j),opj.numRows(s,j));
			
		}
		FILE *out;
		char filename[255];
		sprintf(filename,"%s.%d.dat",basename(argv[1]),s+1);
		printf("saved to %s\n",filename);
		if((out=fopen(filename,"w")) == NULL ) {
			printf("Could not open %s",filename);
			return -1;
		}
		// header
		for (int j=0;j<nr_cols;j++)
			fprintf(out,"%s ",opj.colName(s,j));	
		fprintf(out,"\n");
		// data
		for (int i=0;i<opj.maxRows(s);i++) {
			for (int j=0;j<nr_cols;j++) {
				double v=0;
				if(i<opj.numRows(s,j))
					v=opj.Data(s,j)[i];
				fprintf(out,"%g ",v);
			}
			fprintf(out,"\n");
		}
	}
}
