#include <sstream>
#include "connection.h"
#include "error.h"
#include "result.h"

node_db_cubrid::Connection::Connection() {
	conn_handle = 0;
	this->port = 33000;
}

node_db_cubrid::Connection::~Connection() {
	this->close();
}

void node_db_cubrid::Connection::open() throw (node_db::Exception&) {
	/* cast away const since CUBRID does not provide an API with const corectness */
	int handle =  cci_connect ((char *)hostname.c_str(),
							  port,
							  (char *)database.c_str(),
							  (char *)user.c_str(),
							  (char *)password.c_str());
	if (handle < 0) {
		throw node_db::Exception ("Connect failed");
	} else {
		conn_handle = handle;
	}
}

void node_db_cubrid::Connection::close() {
	if (conn_handle > 0) {
		cci_disconnect(conn_handle, NULL);
	}
}

std::string node_db_cubrid::Connection::escape(const std::string& string) const throw(node_db::Exception&) {
	std::string escaped ="";
	// CUBRID does not provide an API call for escaping strings. 
	// For now, just escape apostrophes
	for(unsigned int i = 0;i<string.length();i++){
		switch (string[i])
		{
		case '\'':
			escaped += "\\'";
			break;
		default:
			escaped+=string[i];
		}
	}
	return escaped;
}

std::string node_db_cubrid::Connection::version() const {
	int major = 0, minor = 0, patch = 0;
	cci_get_version (&major, &minor, &patch);
    std::stringstream strm;
	strm<<major<<"."<<minor<<"."<<patch;
    return strm.str();
}

node_db::Result* node_db_cubrid::Connection::query(const std::string& query) const throw(node_db::Exception&) {
	int stmt_handle = 0, res = 0;
	T_CCI_ERROR err_buf;

	// prepare the query
	stmt_handle = cci_prepare (conn_handle, (char *)query.c_str(), 0, &err_buf);
	if (res < 0) {
		throw node_db::Exception (err_buf.err_msg);
	}
	
	// get column information, column count and statement type
	// TODO: see if it's better to move this code in Result
	T_CCI_CUBRID_STMT stmt_type;
	int column_count = 0;
	T_CCI_COL_INFO* col_info = cci_get_result_info (stmt_handle, &stmt_type, &column_count);
	if (col_info == NULL) {
		throw node_db::Exception("Failed to get results");
	}

	// execute the query
	res = cci_execute (stmt_handle, 0, 0, &err_buf);
	if (res < 0) {
		throw node_db::Exception (err_buf.err_msg);
	}

	return new node_db_cubrid::Result(conn_handle, stmt_handle, col_info, column_count, stmt_type);
}
