#pragma once

// CorePch : GameServer, DummyClient���� �������� ������ �ֵ�
// pch : ServerCore�� ��� ���Ͽ��� �������� ������ �ֵ�

#include "types.h"	

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include <iostream>
using namespace std;

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

#include <thread>
#include <mutex>
#include <memory>
#include <functional>