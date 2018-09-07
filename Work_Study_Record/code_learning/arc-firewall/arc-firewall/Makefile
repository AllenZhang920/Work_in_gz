include $(TOP_DIR)/.config

OBJ = firewalld

COMMON_CFLAGS= $(CFLAGS) $(LDFLAGS) -Wall -rdynamic

ARCFW_SRC = firewall.c
ARCFW_OBJS = firewall.o \
             access_control_mod.o \
             forwarding_mod.o \
             generic_mod.o \
             remote_mng_mod.o \
             vpn_mod.o \
             spi_mod.o \
             upnp_mod.o\
             log_mod.o\
             guest_mod.o \
             simple_mod.o\
             ipfilter_mod.o\
             ifilter_mod.o

all: $(OBJ)

firewalld: $(ARCFW_OBJS)
	$(CC) -lmidcore -larc-shared $(COMMON_CFLAGS) $(ARCFW_OBJS) -o $@

%.o : %.c
	$(CC) -c $(COMMON_CFLAGS) $< -o $@ -Werror

install:
	$(INSTALL_DIR) $(CONFIG_PREFIX)/sbin
	$(INSTALL_BIN) ./firewalld $(CONFIG_PREFIX)/sbin/arc_firewalld

clean:
	rm -rf *.o $(OBJ)

.PHONY : all clean
