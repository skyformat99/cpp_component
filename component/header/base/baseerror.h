#ifndef __BASE_ERROR_H__
#define __BASE_ERROR_H__

#define COMERR_OK (0)
#define COMERR_BASE (0x0000)

#define COMERR_UNKOWN               (-1) //未知错误
#define COMERR_NOT_INIT             (COMERR_BASE + 1) // 没初始化
#define COMERR_ALREADY_INIT         (COMERR_BASE + 2) // 已经初始化
#define COMERR_INVALID_PARAM        (COMERR_BASE + 3) // 参数无效
#define COMERR_NULL_POINTER         (COMERR_BASE + 4) // 指针为空
#define COMERR_TIMEOUT              (COMERR_BASE + 5) // 超时
#define COMERR_INST_LIMIT           (COMERR_BASE + 6) // 无可用实例，能力限制
#define COMERR_INST_INVALID         (COMERR_BASE + 7) // 无效实例
#define COMERR_VERSION              (COMERR_BASE + 8) // 版本不匹配
#define COMERR_NOT_SUPPORT          (COMERR_BASE + 9) // 不支持的功能，暂未实现或指定环境下不支持
#define COMERR_ABILITY_LIMIT        (COMERR_BASE + 10) // 能力限制
#define COMERR_NOT_FOUND            (COMERR_BASE + 11) // 指定数据或指定项不存在
#define COMERR_SEM_FAILED           (COMERR_BASE + 12) // 信号量错误
#define COMERR_LOCK_FAILED          (COMERR_BASE + 13) // 锁错误
#define COMERR_PORT_USED            (COMERR_BASE + 14) // 端口被占用
#define COMERR_DB                   (COMERR_BASE + 15) // 通用数据库错误（包含数据库连接失败）
#define COMERR_PARSE             	(COMERR_BASE + 16) // 解析失败
#define COMERR_PASSWORD             (COMERR_BASE + 17) // 密码错误
#define COMERR_ISLOGIN              (COMERR_BASE + 18) // 多次登录
#define COMERR_THIRDPARTIES         (COMERR_BASE + 19) // 第三方库失败
#define COMERR_CYCLE                (COMERR_BASE + 20) // 循环次数错
#define COMERR_SYSCALL              (COMERR_BASE + 21) // 系统调用错
#define COMERR_MEMERR               (COMERR_BASE + 22) // 内存错
#define COMERR_MEMLOW               (COMERR_BASE + 23) // 内存不足
#define COMERR_TASKERR              (COMERR_BASE + 24) // 任务出错
#define COMERR_STACKERR             (COMERR_BASE + 25) // 堆栈出错
#define COMERR_AUTH                 (COMERR_BASE + 26) // 权限错误
#define COMERR_PRIORITY             (COMERR_BASE + 27) // 等级低(优先级低)     
#define COMERR_USER_FREEZE          (COMERR_BASE + 28) // 账户被冻结
#define COMERR_LICENCES             (COMERR_BASE + 29) // 非法证书
#define COMERR_FILE					(COMERR_BASE + 30) // 文件错误
#define COMERR_NOT_LOGIN			(COMERR_BASE + 31) // 未登陆

#define COMERR_OPEN_FAILED			(COMERR_BASE + 32)	// 打开失败
#define COMERR_INIT_FAILED			(COMERR_BASE + 33)	// 初始化失败
#define COMERR_UNINIT_FAILED		(COMERR_BASE + 34)	// 反初始化失败
#define COMERR_ALREADY_EXIST		(COMERR_BASE + 35)	// 已经存在
#define COMERR_DATAERR				(COMERR_BASE + 36)	// 数据错误
//Socket
#define COMERR_SOCKET_WAIT_CONNECT		(COMERR_BASE + 37)		//等待连接
#define COMERR_SOCKET_CONNECT_FAILED	(COMERR_BASE + 38)		//连接失败
#define COMERR_SOCKET_SEND_FAILED		(COMERR_BASE + 39)		//发送失败
#define COMERR_SOCKET_RECV_FAILED		(COMERR_BASE + 40)		//接收失败
#define COMERR_SOCKET_DISCONNECT		(COMERR_BASE + 41)		//断开连接
#define COMERR_PROTOCOL_UMMATCH           (COMERR_BASE + 42)		//协议不匹配
#define COMERR_ALREADY_RUNNING			(COMERR_BASE + 43)	// 已经运行

#define COMERR_SUBSCRIP_FAILED			(COMERR_BASE + 44)	// 订阅失败
#define COMERR_UNSUBSCRIP_FAILED			(COMERR_BASE + 45)	// 取消订阅失败
#define COMERR_PUBLISH_FAILED			(COMERR_BASE + 46)	// 发布失败


#endif //__BASE_ERROR_H__
