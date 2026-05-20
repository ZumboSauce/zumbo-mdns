#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <zmdns/mdns.h>
#include <arpa/inet.h>

/*
 * @brief BIG ENDIAN. Generate a uint16 from 2 consecutive uint8.
 * @param[in] c Pointer to the first character.
 * @return The word.
 */
static inline uint16_t
strtow( const uint16_t *c ) { return (uint16_t)(*c << 8) | *(c + 1); }

/*
 * @brief BIG ENDIAN. Generate a uint32 from 4 consecutive uint8.
 * @param[in] c Pointer to the first character.
 * @return The dword.
 */
static inline uint32_t
strtodw( const uint32_t *c )
{
    return  (uint32_t)(*(c + 2) << 8) | (uint32_t)(*c << 24) |
            (uint32_t)(*(c + 1) << 16) | (*(c + 3));
}

/*
 * @brief Resolves DNS compression.
 * @param[in] c The potential compression pointer.
 * @param[in] b The mdns message buffer.
 * @return Pointer to the location pointed to by the compression, otherwise c.
 */
static inline const uint8_t *
rsl_cmp( const uint16_t *c, const uint8_t *b )
{
    if( *c & 0xc000 )   return b + ( strtow( (const uint16_t *)c ) & ~0xc000 );
    else                return (const uint8_t *) c;
}

/*
 * @brief Find the start of the next label.
 * @param[in] c The size indicator.
 * @param[in] b The mdns message.
 * @return Pointer to the start of the next label, or NULL if the size of the label is 0.
 */
static inline const uint8_t *
find_next_label( const uint16_t *c, const uint8_t *b )
{
    const uint8_t *c_sm = rsl_cmp( c, b );
    if( !(*c_sm) )  return NULL;
    else            return c_sm + (*c_sm + 1);
}

#define NAMEMAXLABEL 20
/*
 * @brief Resolve an mdns name.
 * @param[in] c The start of the label.
 * @param[in] b The mdns message.
 * @param[out] n The buffer that will contain the name.
 * @param[out] n_s The size of the name.
 * @return Pointer to the next element after the compression indicator or the natural end of the name, otherwise, NULL.
 */
static const uint8_t *
resolve_name( const uint8_t *c, const uint8_t *b, uint8_t *n, size_t *n_s )
{
    size_t t = 0;
    //To mitigate f_n_l returning NULL once the terminator is reached, we update c inside the loop so it runs n-1 times.
    for( const uint8_t *o = rsl_cmp( (const uint16_t *)c, b ); o != NULL; o = find_next_label( (const uint16_t *)c, b) )
    {
        memcpy( n + t, o + 1, *o );
        t += *o;
        c = o;
    }
    n[t] = '\0';
    return c + 1;
}
