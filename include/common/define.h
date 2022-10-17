#ifndef DEFINE_H
#define DEFINE_H

#ifndef DISABLE_ESCAPE

#define DISABLE_ESCAPE 0

#endif

#define ENABLE_ESCAPE !DISABLE_ESCAPE
#define ESCAPE_CHAR '\\'

#define bswap_16(x) (			\
		((x << 8) & 0x00ff) |	\
		((x >> 8) & 0xff00)		\
	)

#define bswap_32(x) (	\
		((x << 24) &	0x000000ff) |	\
		((x << 8) &		0x0000ff00) |	\
		((x >> 8) &		0x00ff0000) |	\
		((x >> 24) &	0xff000000)		\
	)

#endif
