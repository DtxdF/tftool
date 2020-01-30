COMPILER		=	gcc
PROJECT			=	tftool
MAIN_PROJECT		=	tftool.c
FLAGS			=	-Wall -g
OBJECTS			=	conf_parser.o cronometer.o exists.o get_extension.o \
				ini.o interact.o join_filename.o free_resources.o \
				get_filesize.o free_secure.o exit_secure.o parser_content.o \
				strip.o check_action.o strlen_m.o strtok_m.o download.o list.o \
				upload.o delete.o client.o strtohost.o check_error.o
LINK_FILES		=	-lpthread
OBJECT_COMPILE		=	${COMPILER} ${FLAGS} -c
FILE_COMPILE		=	${COMPILER} ${FLAGS} -o ${PROJECT} ${MAIN_PROJECT} ${OBJECTS} ${LINK_FILES}

main: ${OBJECTS}
	${FILE_COMPILE}

conf_parser.o:
	${OBJECT_COMPILE} utils/conf_parser/conf_parser.c

interact.o:
	${OBJECT_COMPILE} utils/interact/interact.c

free_resources.o:
	${OBJECT_COMPILE} utils/free_resources/free_resources.c

free_secure.o:
	${OBJECT_COMPILE} utils/free_secure/free_secure.c

exit_secure.o:
	${OBJECT_COMPILE} utils/exit_secure/exit_secure.c

parser_content.o:
	${OBJECT_COMPILE} utils/parser_content/parser_content.c

check_action.o:
	${OBJECT_COMPILE} utils/check_action/check_action.c

strtok_m.o:
	${OBJECT_COMPILE} utils/strtok_m/strtok_m.c

download.o:
	${OBJECT_COMPILE} utils/skills/download.c

list.o:
	${OBJECT_COMPILE} utils/skills/list.c

upload.o:
	${OBJECT_COMPILE} utils/skills/upload.c

delete.o:
	${OBJECT_COMPILE} utils/skills/delete.c

client.o:
	${OBJECT_COMPILE} utils/client/client.c

strtohost.o:
	${OBJECT_COMPILE} utils/strtohost/strtohost.c

check_error.o:
	${OBJECT_COMPILE} utils/skills/check_error/check_error.c

strlen_m.o:
	${OBJECT_COMPILE} core/strlen_m/strlen_m.c

strip.o:
	${OBJECT_COMPILE} core/strip/strip.c

ini.o:
	${OBJECT_COMPILE} core/inih/ini.c

exists.o:
	${OBJECT_COMPILE} core/exists/exists.c

get_extension.o:
	${OBJECT_COMPILE} core/get_extension/get_extension.c

cronometer.o:
	${OBJECT_COMPILE} core/cronometer/cronometer.c

join_filename.o:
	${OBJECT_COMPILE} core/join_filename/join_filename.c

get_filesize.o:
	${OBJECT_COMPILE} core/get_filesize/get_filesize.c

.PHONY: clean
clean:
	rm --recursive --force *.o
