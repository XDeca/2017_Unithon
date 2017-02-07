EXE_NAME := Smart_alarm_exe
ARTIK_SDK_LIBS := \
									libartik-sdk-base \
									libartik-sdk-systemio

OBJS := \
				Smart_alarm.o

CC := ${CROSS_COMPILE}gcc

ifdef SYSROOT
PKG_CONFIG := PKG_CONFIG_PATH=${SYSROOT}/usr/lib/pkgconfig/
PKG_CONFIG_SYSROOT_DIR=${SYSROOT} pkg-config
CFLAGS := ${CFLAGS}# --sysroot=${SYSROOT}
LDFLAGS := ${LDFLAGS} --sysroot=${SYSROOT} -L${SYSROOT}/usr/lib

else
PKG_CONFIG := PKG_CONFIG_PATH=${SYSROOT}/usr/lib/pkgconfig/ pkg-config

endif
CFLAGS := ${CFLAGS} $(shell ${PKG_CONFIG} --cflags --libs ${ARTIK_SDK_LIBS})
LDFLAGS := ${LDFLAGS} $(shell ${PKG_CONFIG} --libs ${ARTIK_SDK_LIBS})


%.o: %.c
			$(CC) -c -o $@ $< $(CFLAGS) -I/usr/include/mysql 

$(EXE_NAME): $(OBJS)
							 $(CC) -L/usr/lib/mysql -lmysqlclient -o $@ $^ $(LDFLAGS)

clean:
		rm -f *.o $(EXE_NAME)
