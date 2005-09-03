#SOURCES = AUTO

bin_PROGRAMS = opj2dat
lib_LTLIBRARIES = liborigin.la

liborigin_la_SOURCES =  OPJFile.cc
liborigin_la_LDFLAGS = -version-info 0:1:0

opj2dat_SOURCES = opj2dat.cc
opj2dat_LDFLAGS = -lorigin

CLEANFILES = opjfile.log *.dat ORIGIN *~

