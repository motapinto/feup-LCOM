PROG=proj

SRCS = proj.c bitmap.c objects.c vetor.c game.c fila.c extras.c  video_gr.c rtc.c video.c vbe.c mouse.c timer.c state.c keyboard.c highscores.c kbc_assembly.S timer_handler_assembly.S

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
