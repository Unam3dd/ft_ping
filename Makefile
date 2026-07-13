DIST = dist/
NAME = ft_ping
INCLUDE_DIR = inc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR)

ifeq ($(RELEASE), 1)
	CFLAGS += -O2
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

vpath %.c src
vpath %.h inc

CORE = src/core/version.c \
	   src/core/usage.c   \
	   src/core/program.c \
	   src/core/loop.c    \
	   src/core/checksum.c \
	   src/core/stats.c   \
	   src/core/rtt.c     \
	   src/core/verbose.c

UTILS = src/utils/time.c \

NETWORK = src/network/host.c \
		  src/network/socket.c \
		  src/network/icmp.c

PARSING = src/parsing/options.c \
		  src/parsing/arguments.c \
		  src/parsing/number.c

SRCS = $(PARSING) \
	   $(NETWORK) \
	   $(UTILS)   \
	   $(CORE)    \
	   src/main.c

OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

all: $(DIST)$(NAME)

$(DIST):
	mkdir -p $(DIST)

$(DIST)$(NAME): $(DIST) $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(DIST)$(NAME) -lm

$(OBJDIR):
	mkdir -p $(sort $(addprefix $(OBJDIR)/, $(dir $(SRCS))))

$(OBJDIR)/%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(DIST)

re: fclean $(DIST)$(NAME)

.PHONY: all clean fclean re
