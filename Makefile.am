SOURCES = AUTO

lib_LTLIBRARIES = liborigin.la

liborigin_la_SOURCES =  OPJFile.cc
liborigin_la_LDFLAGS = -version-info 0:1:0

bin_PROGRAMS = opj2dat
opj2dat_SOURCES = opj2dat.cc OPJFile.cc

CLEANFILES = opjfile.log *.dat ORIGIN *~

