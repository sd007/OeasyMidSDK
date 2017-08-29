#ifndef OEASTMID_DEFINES_H
#define OEASTMID_DEFINES_H

#define  RESERVED_LEN 128
#define  DEVICE_ADDRESS_LEN 129
#define  USERNAME_LEN 64
#define  PASSWORD_LEN 64
#define  SERIALNO_LEN 48


#pragma pack(push,1)

typedef enum {
	OEASY_HANDLE_INIT,
	OEASY_HANDLE_CONNECTTING,
	OEASY_HANDLE_ONLINE,
	OEASY_HANDLE_OFFLINE,
	OEASY_HANDLE_ONLINE_LOGINERR,
	OEASY_HANDLE_INVALIDE,
	OEASY_HANDLE_LOGOUT,
}OEASY_HANDLE_STATE;

//码流类型
typedef enum{
	MAIN_STREAM,
	SUB_STREAM
}STREAMTYPE;

//摄像头厂商
typedef enum{
	NO_IPC = -1,
	HIKVISION_IPC, //海康威视
	DAHUA_IPC,  //大华
	TIANDY_IPC, //天地伟业
	OTHER_IPC
}CAMERATYPE;

//可视对讲
typedef enum{
	OEASY_INTERCOM, 
	OTHER_INERCOM
}INTERCOMTYPE;

//报警主机
typedef enum{
	HIKVISION_ALARMHOST, //海康威视
	OTHER_ALARMHOST
}ALARMHOSTTYPE;

//设备类型
typedef enum{
	NO_DEVICE =-1,
	CAMERA,
	INTERCOM,
	ALARMHOST,
	OTHERDEVICE
}DEVICETYPE;

typedef enum{
	NO_ALARM,
	MOVEDETECT_ALARM,
	FACEDETECT_ALARM,
	ALL_ALARM
}ALARMTYPE;

typedef struct{
	ALARMTYPE type; 
	union{

	};
	union{

	};

}ALARMSETTING,*PALARMSETTING;

typedef struct tagEventMsg
{
	DEVICETYPE  deviceType;
	unsigned int	message;
	unsigned int    dataSize;
	char Data[0];
}OEASYLOG_EVENTMSG, *OEASY_PEVENTMSG;

typedef enum
{
	//CAMERA
	IPC_RECONNECT = 0x100,
	
	
	//INTERCOM
	INTERCON_RECONNECT = 0x300,


	//ALARMHOST
	ALARMHOST_RECONNECT = 0x500

}OEASY_MEASSGE;

//登录摄像机传入结构
typedef struct{
	char  deviceAddress[DEVICE_ADDRESS_LEN];
	unsigned short devicePort;
	char  username[USERNAME_LEN];
	char  password[PASSWORD_LEN];
	char    reserved[RESERVED_LEN]; //保留字段
}OEASY_LOGINFO,*LPOEASY_LOGINFO;


//登录摄像机成功返回参数结构
typedef struct
{
	unsigned char serialNumber[SERIALNO_LEN];
}OEASY_DEVICEINFO;

typedef struct{
	OEASY_DEVICEINFO Deviceinfo;
	//unsigned char bySupportLock;
	//unsigned char byRetryLoginTime;
	//unsigned char byPasswordLevel;
	//unsigned char byRes1;
	//DWORD dwSurplusLockTime;
	//unsigned char byCharEncodeType;
	//unsigned char byRes2[255];
}OEASY_LOGIN_RESULTINFO,*LPOEASY_LOGIN_RESULTINFO;


#pragma pack(pop) 

#endif //OEASTMID_DEFINES_H