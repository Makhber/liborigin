TARGET       = origin2
TEMPLATE     = lib
CONFIG      += warn_on release thread
#CONFIG      += staticlib 
MOC_DIR      = ./tmp
OBJECTS_DIR  = ./tmp

DESTDIR      = ./

# Path to the folder where the header files of the BOOST C++ libraries are installed
INCLUDEPATH += ../boost

# link statically against a copy of BOOST C++ libraries
#LIBS        += ../boost/lib/libboost_date_time.a
#LIBS        += ../boost/lib/libboost_thread.a
# or dynamically against a system-wide installation
LIBS        += -lboost_date_time
LIBS        += -lboost_thread

HEADERS += endianfstream.hh \
	   OriginObj.h \
	   OriginFile.h \
	   OriginParser.h \
           OriginDefaultParser.h \
	   Origin600Parser.h \
	   Origin610Parser.h \
	   Origin700Parser.h \
	   Origin750Parser.h \
	   Origin800Parser.h \
	   Origin810Parser.h

SOURCES += OriginFile.cpp \
	   OriginParser.cpp \
	   OriginDefaultParser.cpp \
	   Origin600Parser.cpp \
	   Origin610Parser.cpp \
	   Origin700Parser.cpp \
	   Origin750Parser.cpp \
	   Origin800Parser.cpp \
	   Origin810Parser.cpp

