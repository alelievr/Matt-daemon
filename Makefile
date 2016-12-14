# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alelievr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/07/15 15:13:38 by alelievr          #+#    #+#              #
#    Updated: 2016/12/14 02:37:05 by root             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

#	Sources
SRCSERVDIR	=	src/server
SRCSERV		=	main.c				\
				matt_daemon.cpp		\
				Tintin_reporter.cpp	\
				Server.cpp			\

SRCCLIENTDIR=	src/client
SRCCLIENT	=	main.cpp			\

SRCBOTHDIR	=	src/
SRCBOTH		=	RSAEncrypt.cpp

#	Objects
OBJCLIENTDIR=	obj/client
OBJSERVDIR	=	obj/server
OBJBOTHDIR	=	obj/

#	Variables
LIBFT		=	2	#1 or 0 to include the libft / 2 for autodetct
DEBUGLEVEL	=	0	#can be 0 for no debug 1 for or 2 for harder debug
					#Warrning: non null debuglevel will disable optlevel
OPTLEVEL	=	1	#same than debuglevel
					#Warrning: non null optlevel will disable debuglevel
CPPVERSION	=	c++14
#For simpler and faster use, use commnd line variables DEBUG and OPTI:
#Example $> make DEBUG=2 will set debuglevel to 2

#	Includes
INCDIRS		=	inc/server inc/client inc

#	Libraries
LIBDIRS		=	
LDLIBS		=	-lutil -lbsd

#	Output
SERVNAME	=	Matt_daemon
CLIENTNAME	=	Ben_AFK

#	Compiler
WERROR		=	#-Werror
CFLAGS		=	-Weverything -pedantic -ferror-limit=999
CPROTECTION	=	-z execstack -fno-stack-protector

DEBUGFLAGS1	=	-fsanitize=address -ggdb -O0 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
DEBUGFLAGS2	=	-fsanitize-memory-track-origins=2
OPTFLAGS1	=	-funroll-loops# -O2
OPTFLAGS2	=	-pipe -funroll-loops -Ofast

#	Framework
FRAMEWORK	=	

#################
##  COLORS     ##
#################
CPREFIX		=	"\033[38;5;"
BGPREFIX	=	"\033[48;5;"
CCLEAR		=	"\033[0m"
CLINK_T		=	$(CPREFIX)"129m"
CLINK		=	$(CPREFIX)"93m"
COBJ_T		=	$(CPREFIX)"119m"
COBJ		=	$(CPREFIX)"113m"
CCLEAN_T	=	$(CPREFIX)"9m"
CCLEAN		=	$(CPREFIX)"166m"
CRUN_T		=	$(CPREFIX)"198m"
CRUN		=	$(CPREFIX)"163m"
CNORM_T		=	"226m"
CNORM_ERR	=	"196m"
CNORM_WARN	=	"202m"
CNORM_OK	=	"231m"

#################
##  OS/PROC    ##
#################

OS			:=	$(shell uname -s)
PROC		:=	$(shell uname -p)
DEBUGFLAGS	=	""
LINKDEBUG	=	""
OPTFLAGS	=	""
#COMPILATION	=	""

ifeq "$(OS)" "Windows_NT"
endif
ifeq "$(OS)" "Linux"
	LDLIBS		+= ""
	DEBUGFLAGS	+= #-fsanitize=memory# -fsanitize-memory-use-after-dtor# -fsanitize=thread
endif
ifeq "$(OS)" "Darwin"
endif

#################
##  AUTO       ##
#################

NASM		=	nasm
SERVOBJS	=	$(patsubst %.c,%.o, $(filter %.c, $(SRCSERV))) \
				$(patsubst %.cpp,%.o, $(filter %.cpp, $(SRCSERV))) \
				$(patsubst %.s,%.o, $(filter %.s, $(SRCSERV)))
CLIENTOBJS	=	$(patsubst %.c,%.o, $(filter %.c, $(SRCCLIENT))) \
				$(patsubst %.cpp,%.o, $(filter %.cpp, $(SRCCLIENT))) \
				$(patsubst %.s,%.o, $(filter %.s, $(SRCCLIENT)))
BOTHOBJS	=	$(patsubst %.c,%.o, $(filter %.c, $(SRCBOTH))) \
				$(patsubst %.cpp,%.o, $(filter %.cpp, $(SRCBOTH))) \
				$(patsubst %.s,%.o, $(filter %.s, $(SRCBOTH)))
SERVOBJ		=	$(addprefix $(OBJSERVDIR)/,$(notdir $(SERVOBJS)))
CLIENTOBJ	=	$(addprefix $(OBJCLIENTDIR)/,$(notdir $(CLIENTOBJS)))
BOTHOBJ		=	$(addprefix $(OBJBOTHDIR)/,$(notdir $(BOTHOBJS)))
OBJS		=	$(SERVOBJ) $(CLIENTOBJ) $(BOTHOBJ)
NORME		=	**/*.[ch]
VPATH		+=	$(dir $(addprefix $(SRCDIR)/,$(SRC)))
VFRAME		=	$(addprefix -framework ,$(FRAMEWORK))
INCFILES	=	$(foreach inc, $(INCDIRS), $(wildcard $(inc)/*.h))
CPPFLAGS	=	$(addprefix -I,$(INCDIRS))
LDFLAGS		=	$(addprefix -L,$(LIBDIRS))
CC			=	clang
CXX			=	clang++
LINKER		=	$(CXX)

disp_indent	=	for I in `seq 1 $(MAKELEVEL)`; do \
					test "$(MAKELEVEL)" '!=' '0' && printf "\t"; \
				done
color_exec	=	$(call disp_indent); \
				echo $(1)➤ $(3)$(2)"\n"$(strip $(4))$(CCLEAR);$(4)
color_exec_t=	$(call disp_indent); \
				echo $(1)➤ $(strip $(3))$(2)$(CCLEAR);$(3)

ifneq ($(filter 1,$(strip $(DEBUGLEVEL)) ${DEBUG}),)
	OPTLEVEL = 0
	OPTI = 0
	DEBUGFLAGS += $(DEBUGFLAGS1)
endif
ifneq ($(filter 2,$(strip $(DEBUGLEVEL)) ${DEBUG}),)
	OPTLEVEL = 0
	OPTI = 0
	DEBUGFLAGS += $(DEBUGFLAGS1)
	LINKDEBUG += $(DEBUGFLAGS1) $(DEBUGFLAGS2)
	export ASAN_OPTIONS=check_initialization_order=1
endif

ifneq ($(filter 1,$(strip $(OPTLEVEL)) ${OPTI}),)
	DEBUGFLAGS = ""
	OPTFLAGS = $(OPTFLAGS1)
endif
ifneq ($(filter 2,$(strip $(OPTLEVEL)) ${OPTI}),)
	DEBUGFLAGS = ""
	OPTFLAGS = $(OPTFLAGS1) $(OPTFLAGS2)
endif

ifndef $(CXX)
	CXX = clang++
endif

ifneq ($(filter %.cpp,$(SRC)),)
	LINKER = $(CXX)
endif

ifdef ${NOWERROR}
	WERROR = ""
endif

ifeq "$(strip $(LIBFT))" "2"
ifneq ($(wildcard ./libft),)
	LIBDIRS += "libft"
	LDLIBS += "-lft"
	INCDIRS += "libft"
endif
endif

#################
##  TARGETS    ##
#################

#	First target
all: $(SERVNAME) $(CLIENTNAME)

client: $(CLIENTNAME)

server: $(SERVNAME)

#	Linking
$(SERVNAME): $(SERVOBJ) $(BOTHOBJ)
	@$(call color_exec,$(CLINK_T),$(CLINK),"Link of $(SERVNAME):",\
		$(LINKER) $(WERROR) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(OPTFLAGS) $(DEBUGFLAGS) $(LINKDEBUG) $(VFRAME) -o $@ $^)

$(CLIENTNAME): $(CLIENTOBJ) $(BOTHOBJ)
	@$(call color_exec,$(CLINK_T),$(CLINK),"Link of $(CLIENTNAME):",\
		$(LINKER) $(WERROR) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(OPTFLAGS) $(DEBUGFLAGS) $(LINKDEBUG) $(VFRAME) -o $@ $^)

#	Objects compilation
$(OBJSERVDIR)/%.o:  $(SRCSERVDIR)/%.cpp $(INCFILES)
	@mkdir -p $(OBJSERVDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(CXX) -std=$(CPPVERSION) $(WERROR) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $<)

$(OBJSERVDIR)/%.o: $(SRCSERVDIR)/%.c $(INCFILES)
	@mkdir -p $(OBJSERVDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(CC) $(WERROR) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $<)

$(OBJCLIENTDIR)/%.o:  $(SRCCLIENTDIR)/%.cpp $(INCFILES)
	@mkdir -p $(OBJCLIENTDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(CXX) -std=$(CPPVERSION) $(WERROR) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $<)

$(OBJCLIENTDIR)/%.o: $(SRCCLIENTDIR)/%.c $(INCFILES)
	@mkdir -p $(OBJBOTHDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(CC) $(WERROR) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $<)

$(OBJBOTHDIR)/%.o:  $(SRCBOTHDIR)/%.cpp $(INCFILES)
	@mkdir -p $(OBJBOTHDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(CXX) -std=$(CPPVERSION) $(WERROR) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $<)

#	Removing objects
clean:
	@$(call color_exec,$(CCLEAN_T),$(CCLEAN),"Clean:",\
		$(RM) $(OBJ))
	@rm -rf $(OBJCLIENTDIR) $(OBJSERVDIR) $(OBJBOTHDIR)

#	Removing objects and exe
fclean: clean
	@$(call color_exec,$(CCLEAN_T),$(CCLEAN),"Fclean:",\
		$(RM) $(SERVNAME) $(CLIENTNAME))

#	All removing then compiling
re: fclean all

f:	all run

#	Checking norme
norme:
	@norminette $(NORME) | sed "s/Norme/[38;5;$(CNORM_T)➤ [38;5;$(CNORM_OK)Norme/g;s/Warning/[0;$(CNORM_WARN)Warning/g;s/Error/[0;$(CNORM_ERR)Error/g"

codesize:
	@cat $(NORME) |grep -v '/\*' |wc -l

coffee:
	@clear
	@echo ""
	@echo "                   ("
	@echo "	                     )     ("
	@echo "               ___...(-------)-....___"
	@echo '           .-""       )    (          ""-.'
	@echo "      .-''''|-._             )         _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "                 ("
	@echo "	                  )      ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""       )   (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "               ("
	@echo "	                  )     ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "             (         ) "
	@echo "	              )        ("
	@echo "               ___)...----)----....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'

.PHONY: all clean fclean re norme codesize
