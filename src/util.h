#ifndef ATEM_UTILS_H
#define ATEM_UTILS_H




inline unsigned char count_bits( unsigned int fields )
{
	unsigned int c = 0;
	for (c = 0; fields; fields >>= 1) {
		c += fields & 1;
	}
	return c;
}

/**
 * copy src to dst string without trailing spaces,
 * src does not need to be zero terminated, dst will be zero terminated so size of
 * dst must be len + 1
 */
inline int trim_end( char *dst, const char *src, int len )
{
	int last = -1;
	for( int i=0; i<len && src[i]!='\0'; i++  ) {
		dst[i] = src[i];
		if( src[i] != ' ' ) {
			last = i;
		}
	}
	dst[last+1] = '\0';
	return last+1;
}


/**
 * ltoa:  convert n to characters in s
 */
extern int ltoa( char *s, long n );

extern int itodatestr( char *s, unsigned int n );

extern int itotimestr( char *s, unsigned int n );

extern int itoa( char *s, long n );

extern int ftoa( char *s, float f );



#ifdef __cplusplus
extern "C" {
#endif

/**
 * taken from <fprintf.h> and modified:
 *    - removed flags: wide, extra, group, i18n, alt (treat them as unset)
 *                     is_short, is_long, is_char, __pad, user (not needed)
 *                     is_long_double (to be handled separately if needed)
 *    - spec must be lowercase char
 *    - spec and pad have type char instead of wchar_t
 */
struct rudi_printf_info
{
	int prec; /* Precision.  */
	int width; /* Width.  */
	char spec; /* Format letter.  */
	unsigned int space:1; /* Space flag.  */
	unsigned int left:1; /* - flag.  */
	unsigned int showsign:1;	/* + flag.  */
	char pad; /* Padding character.  */
};


extern int
#ifdef COMPILE_FOR_LONG_DOUBLE
rudi_printf_fp_long ( char *ccc, const struct rudi_printf_info *info,
	const long double fpnum_ldbl );
#else
rudi_printf_fp ( char *ccc, const struct rudi_printf_info *info,
	const double fpnum_dbl );
#endif




#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
