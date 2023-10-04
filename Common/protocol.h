constexpr int PORT_NUM = 4000;
constexpr int BUF_SIZE = 200;
constexpr int STR_SIZE = 20;
constexpr int MAX_USER = 1000;

enum class PacketType : unsigned char
{
	CS_LOGIN = 11,
	CS_MOVE = 12,

	SC_LOGIN_INFO = 51,
};

#pragma pack (push, 1)
struct CS_LOGIN_PACKET {
	unsigned char size;
	PacketType type;
	char	name[STR_SIZE];
};

struct CS_MOVE_PACKET {
	unsigned char size;
	PacketType	type;
	char	direction;
};

struct SC_LOGIN_INFO_PACKET {
	unsigned char size;
	PacketType type;
	short	id;
	short	x, y;
};

#pragma pack (pop)