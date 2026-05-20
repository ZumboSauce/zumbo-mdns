#ifndef __ZMDNS_H__
#define __ZMDNS_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define MDNS_NAMESTRLEN 100

struct mdns_header
{
    u_char          is_query;
    u_char          tc;
    uint16_t        q_count;
    uint16_t        an_count;
    uint16_t        aa_count;
    uint16_t        ar_count;
};

struct mdns_query
{
    char*       name;
    size_t      size_name;
    uint16_t    type;
    uint16_t    clss;
    uint16_t    unicast_response;
};

enum MDNS_TYPES
{
    A       = 1,
    PTR     = 12,
    TXT     = 16,
    SRV     = 33
};

struct mdns_record_data;

struct mdns_record_data_a
{
    char                *address;
    size_t              size_address;
};

struct mdns_record_data_ptr
{
    char                *domain_name;
    size_t              size_name;
};

struct mdns_record_data_srv
{
    uint16_t            priority;
    uint16_t            weight;
    uint16_t            port;
    char                *domain_name;
    size_t              size_name;
};

struct txt_string
{
    char                    *key;
    char                    *value;
    size_t                  size_key;
    size_t                  size_value;
};

struct mdns_record_data_txt
{
    struct txt_string       *strings;
    size_t                  st_count;
};

struct mdns_record
{
    char                        *name;
    size_t                      size_name;
    uint16_t                    type;
    uint16_t                    clss;
    uint8_t                     flush;
    uint32_t                    ttl;
    uint16_t                    size_rdata;
    struct mdns_record_data     *rdata;
};

struct mdns_msg
{
    struct mdns_header          header;
    struct mdns_query           *q;
    struct mdns_record          *an;
    struct mdns_record          *aa;
    struct mdns_record          *ar;
};

#endif
