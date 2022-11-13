#ifndef DEFINE_H
#define DEFINE_H

#ifndef DISABLE_ESCAPE

#define DISABLE_ESCAPE 0

#endif

#define ENABLE_ESCAPE !DISABLE_ESCAPE
#define ESCAPE_CHAR '\\'

#define bswap_16(x) (			\
		((x & 0x00ff) << 8) |	\
		((x & 0xff00) >> 8)		\
	)

#define bswap_32(x) (				\
		((x & 0x000000ffu) << 24) |	\
		((x & 0x0000ff00u) << 8) |	\
		((x & 0x00ff0000u) >> 8) |	\
		((x & 0xff000000u) >> 24)	\
	)

#endif
