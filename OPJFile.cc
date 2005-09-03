// OPJFile.cc

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "OPJFile.h"

OPJFile::OPJFile(char *filename) 
	: filename(filename) 
{
	version=0;
	nr_spreads=0;
	for(int i=0;i<MAX_SPREADS;i++) {
		spreadname[i] = new char[25];
		spreadname[i][0]=0;
		nr_cols[i]=0;
		maxrows[i]=0;
		for(int j=0;j<MAX_COLS;j++) {
			nr_rows[i][j]=0;
			colname[i][j] = new char[25];
			colname[i][j][0]=0x41+j;
			colname[i][j][1]=0;
			coltype[i][j] = new char[25];
			if(j==0)
				coltype[i][j][0]='X';
			else
				coltype[i][j][0]='Y';
			coltype[i][j][1]=0;
		}
	}
	
}

/* File Structure :
filepre +
	+ pre + head + data	col A
	+ pre + head + data	col B
*/

/* parse file filename completly and save values */
int OPJFile::Parse() {
	FILE *f, *debug;
	if((f=fopen(filename,"r")) == NULL ) {
		printf("Could not open %s",filename);
		return -1;
	}
	
	if((debug=fopen("opjfile.log","w")) == NULL ) {
		printf("Could not open log file!\n");
		return -1;
	}

////////////////////////////// check version from header ///////////////////////////////
	char header[14];
	header[13]=0;
	fread(&header,13,1,f);
	fprintf(debug,"	[header : %s]\n",header);

	fseek(f,0x8,SEEK_SET);
	fread(&version,2,1,f);
	fprintf(debug,"	[version = %d]\n",version);

	int FILEPRE=0x3e;		// file header
	int PRE=0x62;			// pre column 
	int HEAD;		// column header
	int NEW_COL;		// value for new column
	int NEW_SPREAD;		// value for new spreadsheet
	int COL_SIZE;		// value of col size
	// TODO : valuesize depens also on column type!
	int valuesize=10;
	if(version == 12854) {	// 6.0
		version=60;
		FILEPRE=0x2f;
		HEAD=0x22;
		NEW_COL=0x72;
		NEW_SPREAD=0x11;
		COL_SIZE=0x1b;
		valuesize=11;
	} 
	else if(version == 13110 ) {	// 6.0 SR4
		fprintf(debug,"Found project version 6.0\n");
		version=604;
		FILEPRE=0x2f;
		HEAD=0x22;
		NEW_COL=0x72;
		NEW_SPREAD=0x11;
		COL_SIZE=0x1b;
	}
	else if(version == 13622) {
		fprintf(debug,"Found project version 7.0\n");
		version=70;
		HEAD=0x23;
		NEW_COL=0x73;
		NEW_SPREAD=0x11;
		COL_SIZE=0x1c;
	}
	else if(version == 13879) {
		fprintf(debug,"Found project version 7.5\n");
		version=75;
		HEAD=0x33;
		NEW_COL=0x83;
		NEW_SPREAD=0x51;
		COL_SIZE=0x2c;
	}
	else {
		fprintf(debug,"Found unknown project version %d\n",version);
		return -2;
	}
	
/////////////////// find column ///////////////////////////////////////////////////////////7
	fseek(f,FILEPRE + 0x05,SEEK_SET);
	int col_found;
	fread(&col_found,4,1,f);
	fprintf(debug,"	[column found = 0x%X (0x%X : YES) @ 0x%X]\n",col_found,NEW_COL,FILEPRE + 0x05);
	
	int current_col=0, nr=0, POS=FILEPRE, DATA=0;
	double a;
	char name[25];
	while(col_found == NEW_COL) {
//////////////////////////////// PRE HEADER //////////////////////////////////////////////////
		fseek(f,POS + 0x21,SEEK_SET);
		char new_spread;
		fread(&new_spread,1,1,f);
		fprintf(debug,"	[new spreadsheet = 0x%X (0x%X : YES) @ 0x%X]\n",new_spread,NEW_SPREAD,POS + 0x21);
		fflush(debug);
		if(new_spread == NEW_SPREAD) {
			current_col=1;
			maxrows[nr_spreads]=0;
			nr_spreads++;
			fprintf(debug,"	SPREADSHEET %d\n",nr_spreads);
		}
		else {
			current_col++;
			
			nr_cols[nr_spreads-1]=current_col;
		}
			
//////////////////////////////// COLUMN HEADER /////////////////////////////////////////////
		// TODO : data isn't the same for all spreads !
		fprintf(debug,"	[column header @ 0x%X]\n",POS);
		fflush(debug);
		fseek(f,POS + PRE,SEEK_SET);
		fread(&name,25,1,f);
		fprintf(debug,"		COLUMN %d (%s) @ 0x%X ",current_col, name,POS+PRE);
		fflush(debug);
		sprintf(colname[nr_spreads-1][current_col-1],"%s",name);
		
////////////////////////////// SIZE of column /////////////////////////////////////////////
		fseek(f,POS+PRE+COL_SIZE,SEEK_SET);
		fread(&nr,4,1,f);
		nr /= valuesize;
		fprintf(debug,"	[number of rows = %d @ 0x%X]\n",nr,POS+PRE+COL_SIZE);
		fflush(debug);
		nr_rows[nr_spreads-1][current_col-1]=nr;
		maxrows[nr_spreads-1]<nr?maxrows[nr_spreads-1]=nr:0;

////////////////////////////////////// DATA ////////////////////////////////////////////////
		fseek(f,POS+PRE+HEAD,SEEK_SET);
		fprintf(debug,"	[data @ 0x%X]\n",POS+PRE+HEAD);
		data[nr_spreads-1][current_col-1] = (double *) malloc(nr*sizeof(double));
		for (int i=0;i<nr;i++) {
			fread(&a,valuesize,1,f);
			if(fabs(a)<1.0e-100) a=0;
			fprintf(debug,"%g ",a);
			data[nr_spreads-1][(current_col-1)][i]=a;
		}
		fprintf(debug,"\n");

		DATA = valuesize*nr - 1;
		fseek(f,POS + PRE + DATA + HEAD + 0x05,SEEK_SET);
		fread(&col_found,4,1,f);
		fprintf(debug,"	[column found = 0x%X (0x%X : YES)]\n",col_found,NEW_COL);
		
		POS += (DATA + HEAD + PRE);
	}

	POS-=1;
	fprintf(debug,"		[position @ 0x%X]\n",POS);
	
///////////////////// SPREADSHEET 1 ////////////////////////////////////
	// HEADER
	fseek(f,POS + 0x12,SEEK_SET);
	fread(&name,25,1,f);
	fprintf(debug,"\n	SPREADSHEET 1 NAME : %s	(@ 0x%X) has %d columns\n",name,POS + 0x12,nr_cols[0]);
	sprintf(spreadname[0],"%s",name);

	fseek(f,POS + 0x55,SEEK_SET);
	fread(&name,7,1,f);
	fprintf(debug,"		%s [ORIGIN]	(@ 0x%X)\n",name,POS + 0x55);
	
	/* LAYER A section */
	int LAYER = POS + 0x4AB;
	
 	// seek for "L"ayerInfoStorage to find layer section	: only 7.5 
	LAYER -= 0x99;
	char c;
	int jump=0;
	do {
		LAYER += 0x99;
		fseek(f,LAYER+0x53, SEEK_SET);
		fread(&c,1,1,f);
		jump++;
	} while(c!='L' && jump<10);		// no inf loop

	if(jump==10) {
		fprintf(debug,"		LAYER section not found !\nGiving up.");
		return -3;
	}

	fprintf(debug,"		[LAYER @ 0x%X]\n",LAYER);

////////////////////////// COLUMN Types ///////////////////////////////////////////
	// TODO : 7.0 : A @ 0x680
	int ATYPE = 0xCE, COL_JUMP=0x1F2;
	for (int i=0;i<nr_cols[0];i++) {
		fseek(f,LAYER+ATYPE+i*COL_JUMP+1, SEEK_SET);
		fread(&c,1,1,f);
		if(c==0x41+i) {
			fseek(f,LAYER+ATYPE+i*COL_JUMP, SEEK_SET);
			fread(&c,1,1,f);
			char type[5];
			switch(c) {
			case 3: sprintf(type,"X");break;
			case 0: sprintf(type,"Y");break;
			case 5: sprintf(type,"Z");break;
			case 6: sprintf(type,"DX");break;
			case 2: sprintf(type,"DY");break;
			case 4: sprintf(type,"LABEL");break;
			}
			sprintf(coltype[0][i],"%s",type);
			fprintf(debug,"		COLUMN %c type = %s (@ 0x%X)\n",0x41+i,type,LAYER+ATYPE+i*COL_JUMP);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// SPREADSHEET 2,3,...
	// TODO
	
	if(nr_spreads>=2) {
		POS=0x3474;
		if(version==70)
			POS=0x3008;
		// HEADER
			
		fseek(f,POS+0x11,SEEK_SET);
		fread(&name,25,1,f);
		fprintf(debug,"	SPREADSHEET 2 NAME : %s	(@ 0x%X)\n",name,POS + 0x11);
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// TODO : GRAPHS
/*	int graph = 0x2fc1;
	int pre_graph = 0x12;
	fseek(f,graph + pre_graph,SEEK_SET);
	fread(&name,25,1,f);
	printf("GRAPH : %s\n",name);

	fseek(f,graph + pre_graph + 0x43, SEEK_SET);
	fread(&name,25,1,f);
	printf("TYPE : %s\n",name);

	fseek(f,graph + pre_graph + 0x2b3, SEEK_SET);
	fread(&name,25,1,f);
	printf("Y AXIS TITLE : %s\n",name);
	fseek(f,graph + pre_graph + 0x38d, SEEK_SET);
	fread(&name,25,1,f);
	printf("X AXIS TITLE : %s\n",name);

	fseek(f,graph + pre_graph + 0xadb, SEEK_SET);
	fread(&name,25,1,f);
	printf("LEGEND : %s\n",name);
*/
	fclose(debug);

	return 0;
}
