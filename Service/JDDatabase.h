////////////////////////////////////////////////////////////
// JDArchive Database Definition
//
// Abin, 2011-9-30
////////////////////////////////////////////////////////////

#ifndef __JDDATABASE_H__
#define __JDDATABASE_H__

////////////////////////////////////////////////////////////
// General
////////////////////////////////////////////////////////////
#define JDB_CONN_STR				_T("Provider=SQLOLEDB.1;Persist Security Info=False;Data Source=%s;Initial Catalog=%s;User ID=%s;Password=%s")
#define JDB_QUERY_ALL				_T("SELECT * FROM %s")
#define JDB_QUERY_BY_ID				_T("SELECT * FROM %s WHERE FID=%d")

////////////////////////////////////////////////////////////
// Backup
////////////////////////////////////////////////////////////
#define JDB_GET_RECORDS_TO_BACKUP	_T("SELECT * FROM v_BackupVersion WHERE FID>%d")
#define JDB_GET_BACKUP_RECORD		_T("SELECT TOP 1 * FROM v_Backup")
#define JDB_GET_BACKUP_FULL			_T("BACKUP DATABASE %s TO DISK='%s'")
#define JDB_GET_BACKUP_DIFF			_T("BACKUP DATABASE %s TO DISK='%s' WITH DIFFERENTIAL")
#define JDB_UPDATE_BACKUP_RECORD	_T("INSERT INTO t_Backup (FBackupID) VALUES (%d)")

////////////////////////////////////////////////////////////
// Table/View Names
////////////////////////////////////////////////////////////
#define JDB_TABLE_ACTION			_T("t_ActionTrack")
#define JDB_TABLE_USER				_T("t_User")
#define JDB_TABLE_ROLE				_T("t_Role")
#define JDB_TABLE_CAT				_T("t_Category")
#define JDB_TABLE_FILE				_T("t_File")
#define JDB_TABLE_VER				_T("t_Version")
#define JDB_VIEW_VER				_T("v_Version")
#define JDB_VIEW_PATCH				_T("v_Patch")

////////////////////////////////////////////////////////////
// App Patch Access
////////////////////////////////////////////////////////////
#define JDB_PATCH_ADD				_T("INSERT INTO t_Patch (FID, FName, FSize, FMD5, FUserID) VALUES (%d, '%s', %d, '%s', %d)")
#define JDB_PATCH_VERIFY_VER		_T("SELECT * FROM t_Patch WHERE FID>=%d")
#define JDB_PATCH_GET_NEWEST		_T("SELECT * FROM v_Patch WHERE FID>%d")

////////////////////////////////////////////////////////////
// User Access
////////////////////////////////////////////////////////////
#define JDB_REQUEST_USER			_T("SELECT * FROM t_User WHERE FID=%d")
#define JDB_LIST_USER				_T("SELECT * FROM t_User ORDER BY FRoleID, FName")
#define JDB_LOGIN					_T("SELECT * FROM v_User WHERE FPrivID>0 AND FName='%s' AND FPassword='%s'")
#define JDB_VERYFY_PASSWORD			_T("SELECT * FROM t_User WHERE FID=%d AND FPassword='%s'")
#define JDB_CHANGE_PASSWORD			_T("UPDATE t_User SET FPassword='%s' WHERE FID=%d")
#define JDB_RESET_PASSWORD			_T("UPDATE t_User SET FPassword='d41d8cd98f00b204e9800998ecf8427e' WHERE FID=%d")
#define JDB_CHECK_USER_NAME			_T("SELECT * FROM t_User WHERE FName='%s'")

////////////////////////////////////////////////////////////
// Role Access
////////////////////////////////////////////////////////////
#define JDB_REQUEST_ROLE			_T("SELECT * FROM t_Role WHERE FID=%d")
#define JDB_LIST_ROLE				_T("SELECT * FROM t_Role ORDER BY FName")
#define JDB_CHECK_ROLE_NAME			_T("SELECT * FROM t_Role WHERE FName='%s'")
#define JDB_UPDATE_ROLE				_T("UPDATE t_Role SET FName='%s' WHERE FID=%d")

////////////////////////////////////////////////////////////
// Category Access
////////////////////////////////////////////////////////////
#define JDB_REQUEST_SUB_CATEGORY	_T("SELECT * FROM t_Category WHERE FParentID>0 AND FID=%d")
#define JDB_REQUEST_ROOT_CATEGORY	_T("SELECT * FROM t_Category WHERE FParentID=0 AND FRoleID=%d")
#define JDB_UPDATE_ROOT_CATEGORY	_T("UPDATE t_Category SET FName='%s' WHERE FParentID=0 AND FRoleID=%d")
#define JDB_REQUEST_CATEGORY		_T("SELECT * FROM t_Category WHERE FID=%d AND (FParentID=0 OR %d>=%d OR FRoleID=%d OR FPrivilege LIKE '%%|%d|%%')")
#define JDB_LIST_CATEGORY			_T("SELECT * FROM t_Category WHERE FParentID=%d AND (FParentID=0 OR %d>=%d OR FRoleID=%d OR FPrivilege LIKE '%%|%d|%%') ORDER BY FName")
#define JDB_DELETE_CATEGORY			_T("DELETE FROM t_Category WHERE FID=%d")
#define JDB_CHECK_CATEGORY_NAME		_T("SELECT * FROM t_Category WHERE FParentID=%d AND FName='%s'")
#define JDB_CHECK_CATEGORY_EMPTY_C	_T("SELECT * FROM t_Category WHERE FParentID=%d")
#define JDB_CHECK_CATEGORY_EMPTY_F	_T("SELECT * FROM t_File WHERE FParentID=%d")
#define JDB_CATEGORY_BATCH_ADD		_T("INSERT INTO t_Category (FParentID, FName, FRoleID, FUserID) VALUES (%d, '%s', %d, %d)")
#define JDB_CATEGORY_PASTE			_T("UPDATE t_Category SET FParentID=%d WHERE FID=%d")
#define JDB_VATEGORY_PRIVILEGE		_T("UPDATE t_Category SET FPrivilege='%s' WHERE FID=%d")
#define JDB_VATEGORY_PRIVILEGE_SUB	_T("UPDATE t_Category SET FPrivilege='%s' WHERE FParentID=%d")
#define JDB_VATEGORY_GETID			_T("SELECT FID FROM t_Category WHERE FParentID=%d")
#define JDB_DELETE_SUB_CATEGORIES	_T("DELETE FROM t_Category WHERE FParentID=%d")

////////////////////////////////////////////////////////////
// File Access
////////////////////////////////////////////////////////////
#define JDB_REQUEST_FILE			_T("SELECT * FROM t_File WHERE FID=%d")
#define JDB_LIST_FILE				_T("SELECT * FROM v_File WHERE FParentID=%d")
#define JDB_CHECK_FILE_NAME			_T("SELECT * FROM t_File WHERE FParentID=%d AND FName='%s'")
#define JDB_CHECK_FILE_EMPTY		_T("SELECT * FROM t_Version WHERE FFileID=%d AND FDeleterID=0")
#define JDB_DELETE_FILE				_T("DELETE FROM t_File WHERE FID=%d")
#define JDB_FILE_BATCH_ADD			_T("INSERT INTO t_File (FParentID, FName, FRoleID) VALUES (%d, '%s', %d)")
#define JDB_FILE_PASTE				_T("UPDATE t_File SET FParentID=%d WHERE FID=%d")
#define JDB_FILE_UNLOCK				_T("UPDATE t_File SET FLockerID=NULL WHERE FLockerID=%d")
#define JDB_ADMIN_DELETE_FILES		_T("DELETE FROM t_File WHERE FParentID=%d")

////////////////////////////////////////////////////////////
// Version Access
////////////////////////////////////////////////////////////
#define JDB_REQUEST_VER				_T("SELECT * FROM v_Version WHERE FID=%d AND (FCheckerID>0 OR FCreatorID=%d OR (FRoleID=%d AND %d>=%d))")
#define JDB_LIST_VERSION			_T("SELECT * FROM v_Version WHERE FFileID=%d AND (FCheckerID>0 OR FCreatorID=%d OR (FRoleID=%d AND %d>=%d)) ORDER BY FID")
#define JDB_CHECK_VER				_T("UPDATE t_Version SET FCheckerID=%d, FCheckDate=GETDATE() WHERE FID=%d")
#define JDB_UNCHECK_VER				_T("UPDATE t_Version SET FCheckerID=0, FCheckDate=NULL WHERE FID=%d")
#define JDB_DELETE_VER				_T("UPDATE t_Version SET FDeleterID=%d, FDeleteDate=GETDATE(), FOrigFileName='%s' WHERE FID=%d")
#define JDB_LIST_VER_ADMIN			_T("SELECT FDiskName FROM t_Version WHERE FFileID=%d")
#define JDB_DELETE_VERS				_T("DELETE FROM t_Version WHERE FFileID=%d")

////////////////////////////////////////////////////////////
// Database Cleanup
////////////////////////////////////////////////////////////
#define JDB_LIST_CLEANUP			_T("SELECT * FROM v_VersionDeleted WHERE FDeleteDays>365")
#define JDB_RUN_CLEANUP				_T("DELETE FROM t_Version WHERE (FDeleteDate IS NOT NULL) AND (DATEDIFF(DAY, FDeleteDate, GETDATE()) > 365)")

////////////////////////////////////////////////////////////
// Data Recovery
////////////////////////////////////////////////////////////
#define JDB_LIST_RECOVABLE			_T("SELECT * FROM t_Version WHERE FRoleID=%d AND FDeleterID>0")
#define JDB_LIST_RECOVABLE_MATCH_NAME	_T("SELECT * FROM t_Version WHERE FRoleID=%d AND FDeleterID>0 AND FOrigFileName LIKE '%%%s%%'")
#define JDB_RECOVER_DATA			_T("UPDATE t_Version SET FDeleterID=0, FDeleteDate=NULL, FOrigFileName=NULL, FFileID=%d WHERE FID=%d")

#endif // __JDDATABASE_H__