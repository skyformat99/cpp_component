#namespace sessionmgr_db

#create tables
/*
create table if not exists t_session_info (
	id integer primary key autoincrement,
 	sessionid varchar(64),
 	timeouttime bigint,
 	losevaildtime bigint
);
*/
#end

/*
	@bref 创建session
	@is_brace true
	@in_isarr false
	@out_isarr false
	@in sessionId: string
	@in timeoutTime: uint64
	@in losevaildtime: uint64
*/
#define createSession
insert into t_session_info values(null, {0}, {1}, {2});
#end

/*
	@bref 销毁session
	@is_brace true
	@in_isarr false
	@out_isarr false
	@in sessionId: string
*/
#define destroySession
delete from t_session_info
where sessionid = {0};
#end

/*
	@bref 获取sessionId的个数
	@is_brace true
	@in_isarr false
	@out_isarr false
	@in sessionId: string
	@out count: int
*/
#define getSessionIdCount
select count(*) from t_session_info
where sessionid = {0}
limit 1;
#end

/*
	@bref 更新失效时间
	@is_brace true
	@in_isarr false
	@out_isarr false
	@in sessionId: string
	@in losevaildtime: uint64
*/
#define updateLoseVaildTime
update t_session_info set losevaildtime = {1}
where sessionid = {0};
#end

/*
	@bref 获取session信息
	@is_brace true
	@in_isarr false
	@out_isarr false
	@in sessionId: string
	@out id: int
	@out sessionId: string
	@out timeoutTime: uint64
	@out losevaildtime: uint64
*/
#define getSessionInfo
select * from t_session_info
where sessionid = {0};
#end

/*
	@bref 获取全部的session信息
	@is_brace true
	@in_isarr false
	@out_isarr true
	@out id: int
	@out sessionId: string
	@out timeoutTime: uint64
	@out losevaildtime: uint64
*/
#define getAllSessionInfo
select * from t_session_info;
#end
