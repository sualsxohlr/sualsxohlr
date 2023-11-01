#pragma once
constexpr int PORT_NUM = 4000;
constexpr int BUF_SIZE = 200;
constexpr int STR_SIZE = 20;
constexpr int MAX_USER = 1000;

enum class PacketType : unsigned char
{
	CS_LOGIN = 11,
	CS_PLAYER_INFO = 12,
	CS_DISCONNECT_PLAYER = 13,

	SC_LOGIN_INFO = 51,
	SC_MOVE_PLAYER = 52,
	SC_REMOVE_PLAYER = 53,
};

#pragma pack (push, 1)
struct CS_LOGIN_PACKET {
	unsigned char	size;
	PacketType		type;
	char			name[STR_SIZE];
};

struct CS_PLAYER_INFO_PACKET {
	unsigned char	size;
	PacketType		type;
	float			x, y, z;
	float			yaw, pitch, roll;
};

struct CS_DISCONNECT_PLAYER_PACKET {
	unsigned char	size;
	PacketType		type;
};

struct SC_LOGIN_INFO_PACKET {
	unsigned char	size;
	PacketType		type;
	short			id;
	float			x, y, z;
	float			yaw, pitch, roll;
};

struct SC_MOVE_PLAYER_PACKET {
	unsigned char	size;
	PacketType		type;
	short			id;
	float			x, y, z;
	float			yaw, pitch, roll;
};

struct SC_REMOVE_PLAYER_PACKET {
	unsigned char	size;
	PacketType		type;
	short			id;
};


#pragma pack (pop)