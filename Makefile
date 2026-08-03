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

LIB_SRCS = $(PARSING) \
	   $(NETWORK) \
	   $(UTILS)   \
	   $(CORE)

OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
LIB_OBJS = $(addprefix $(OBJDIR)/, $(LIB_SRCS:.c=.o))

TEST_INC = -I$(INCLUDE_DIR) -Itest/inc
TEST_UNIT_SRCS = test/unit/test_unit_main.c \
	test/unit/test_helpers.c \
	test/unit/test_checksum.c \
	test/unit/test_number.c \
	test/unit/test_rtt.c \
	test/unit/test_big16.c \
	test/unit/test_options.c \
	test/unit/test_resolve.c
TEST_UNIT_BIN = $(DIST)test_unit
TEST_CMP_BIN = $(DIST)test_cmp

all: $(DIST)$(NAME)

$(DIST):
	mkdir -p $(DIST)

$(DIST)$(NAME): $(DIST) $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(DIST)$(NAME) -lm

$(OBJDIR):
	mkdir -p $(sort $(addprefix $(OBJDIR)/, $(dir $(SRCS))))

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_UNIT_BIN): $(DIST) $(OBJDIR) $(LIB_OBJS) $(TEST_UNIT_SRCS)
	$(CC) $(CFLAGS) $(TEST_INC) $(TEST_UNIT_SRCS) $(LIB_OBJS) -o $@ -lm

$(TEST_CMP_BIN): $(DIST) $(DIST)$(NAME) test/cmp/test_cmp_ping.c
	$(CC) $(CFLAGS) -DFT_PING_BIN=\"$(CURDIR)/$(DIST)$(NAME)\" \
		test/cmp/test_cmp_ping.c -o $@

test: $(TEST_UNIT_BIN)
	./$(TEST_UNIT_BIN)

test_cmp: $(TEST_CMP_BIN)
	./$(TEST_CMP_BIN)

test_all: test test_cmp

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(DIST)

re: fclean $(DIST)$(NAME)

.PHONY: all clean fclean re test test_cmp test_all
