/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: protocol.proto */

#ifndef PROTOBUF_C_protocol_2eproto__INCLUDED
#define PROTOBUF_C_protocol_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _LizardsNroachestypes__Position LizardsNroachestypes__Position;
typedef struct _LizardsNroachestypes__GameMessage LizardsNroachestypes__GameMessage;


/* --- enums --- */

/*
 * Enumerations for message types
 */
typedef enum _LizardsNroachestypes__MessageType {
  LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_CONNECT = 0,
  LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_MOVEMENT = 1,
  LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_CONNECT = 2,
  LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_MOVEMENT = 3,
  LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ACK = 4,
  LIZARDS_NROACHESTYPES__MESSAGE_TYPE__SCORE_UPDATE = 5,
  LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_DISCONNECT = 100,
  LIZARDS_NROACHESTYPES__MESSAGE_TYPE__WASPS_DISCONNECT = 101
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(LIZARDS_NROACHESTYPES__MESSAGE_TYPE)
} LizardsNroachestypes__MessageType;
/*
 * Enumerations for entity types
 */
typedef enum _LizardsNroachestypes__EntityType {
  LIZARDS_NROACHESTYPES__ENTITY_TYPE__NOTHING = 0,
  LIZARDS_NROACHESTYPES__ENTITY_TYPE__ROACH = 1,
  LIZARDS_NROACHESTYPES__ENTITY_TYPE__WASP = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(LIZARDS_NROACHESTYPES__ENTITY_TYPE)
} LizardsNroachestypes__EntityType;
/*
 * Enumerations for direction types
 */
typedef enum _LizardsNroachestypes__DirectionType {
  LIZARDS_NROACHESTYPES__DIRECTION_TYPE__UP = 0,
  LIZARDS_NROACHESTYPES__DIRECTION_TYPE__DOWN = 1,
  LIZARDS_NROACHESTYPES__DIRECTION_TYPE__LEFT = 2,
  LIZARDS_NROACHESTYPES__DIRECTION_TYPE__RIGHT = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(LIZARDS_NROACHESTYPES__DIRECTION_TYPE)
} LizardsNroachestypes__DirectionType;

/* --- messages --- */

/*
 * Position structure
 */
struct  _LizardsNroachestypes__Position
{
  ProtobufCMessage base;
  int32_t position_x;
  int32_t position_y;
};
#define LIZARDS_NROACHESTYPES__POSITION__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&lizards_nroachestypes__position__descriptor) \
    , 0, 0 }


/*
 * Game message structure
 */
struct  _LizardsNroachestypes__GameMessage
{
  ProtobufCMessage base;
  LizardsNroachestypes__MessageType msg_type;
  protobuf_c_boolean has_n_roaches;
  int32_t n_roaches;
  protobuf_c_boolean has_n_wasps;
  int32_t n_wasps;
  protobuf_c_boolean has_index;
  int32_t index;
  protobuf_c_boolean has_roach_index;
  int32_t roach_index;
  protobuf_c_boolean has_wasp_index;
  int32_t wasp_index;
  size_t n_score_roaches;
  int32_t *score_roaches;
  size_t n_wasps_char;
  int32_t *wasps_char;
  protobuf_c_boolean has_direction;
  LizardsNroachestypes__DirectionType direction;
};
#define LIZARDS_NROACHESTYPES__GAME_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&lizards_nroachestypes__game_message__descriptor) \
    , LIZARDS_NROACHESTYPES__MESSAGE_TYPE__ROACHES_CONNECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,NULL, 0,NULL, 0, LIZARDS_NROACHESTYPES__DIRECTION_TYPE__UP }


/* LizardsNroachestypes__Position methods */
void   lizards_nroachestypes__position__init
                     (LizardsNroachestypes__Position         *message);
size_t lizards_nroachestypes__position__get_packed_size
                     (const LizardsNroachestypes__Position   *message);
size_t lizards_nroachestypes__position__pack
                     (const LizardsNroachestypes__Position   *message,
                      uint8_t             *out);
size_t lizards_nroachestypes__position__pack_to_buffer
                     (const LizardsNroachestypes__Position   *message,
                      ProtobufCBuffer     *buffer);
LizardsNroachestypes__Position *
       lizards_nroachestypes__position__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   lizards_nroachestypes__position__free_unpacked
                     (LizardsNroachestypes__Position *message,
                      ProtobufCAllocator *allocator);
/* LizardsNroachestypes__GameMessage methods */
void   lizards_nroachestypes__game_message__init
                     (LizardsNroachestypes__GameMessage         *message);
size_t lizards_nroachestypes__game_message__get_packed_size
                     (const LizardsNroachestypes__GameMessage   *message);
size_t lizards_nroachestypes__game_message__pack
                     (const LizardsNroachestypes__GameMessage   *message,
                      uint8_t             *out);
size_t lizards_nroachestypes__game_message__pack_to_buffer
                     (const LizardsNroachestypes__GameMessage   *message,
                      ProtobufCBuffer     *buffer);
LizardsNroachestypes__GameMessage *
       lizards_nroachestypes__game_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   lizards_nroachestypes__game_message__free_unpacked
                     (LizardsNroachestypes__GameMessage *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*LizardsNroachestypes__Position_Closure)
                 (const LizardsNroachestypes__Position *message,
                  void *closure_data);
typedef void (*LizardsNroachestypes__GameMessage_Closure)
                 (const LizardsNroachestypes__GameMessage *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    lizards_nroachestypes__message_type__descriptor;
extern const ProtobufCEnumDescriptor    lizards_nroachestypes__entity_type__descriptor;
extern const ProtobufCEnumDescriptor    lizards_nroachestypes__direction_type__descriptor;
extern const ProtobufCMessageDescriptor lizards_nroachestypes__position__descriptor;
extern const ProtobufCMessageDescriptor lizards_nroachestypes__game_message__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_protocol_2eproto__INCLUDED */