#include "./result.h"
#include <string.h>

node_db_cubrid::Result::Result(int conn_handle, int stmt_handle, T_CCI_COL_INFO* col_info,
							   int column_count, T_CCI_CUBRID_STMT stmt_type) throw(node_db::Exception&) {
	this->rowNumber = 0;
	this->conn_handle = conn_handle;
	this->stmt_handle = stmt_handle;
	this->column_info = col_info;
	this->statement_type = stmt_type;
	this->column_count = (uint16_t)column_count;
	this->columns = NULL;
	if (column_count > 0) {
		this->columns = new Column*[column_count];
		if (this->columns == NULL){
			throw node_db::Exception("cannot allocate memory");
		}
		for (int i = 0; i < column_count; i++) {
			columns[i] = new Column(col_info[i]);
			if (columns[i] == NULL) {
				throw node_db::Exception("cannot allocate memory");
			}
		}
	}
}

node_db_cubrid::Result::~Result() {
	if (column_info != NULL) {
		cci_close_req_handle (stmt_handle);
	}
	if (thisRow != NULL) {
		delete [] thisRow;
	}
	if (columns != NULL) {
		for (int i = 0; i < column_count ; i++){
			delete columns[i];
		}
		delete [] columns;
	}
}

void node_db_cubrid::Result::release() throw() {
	cci_close_req_handle(stmt_handle);
	column_info = NULL;
}

bool node_db_cubrid::Result::hasNext() const throw() {
	T_CCI_ERROR error;
	int res = cci_cursor(stmt_handle, 1, CCI_CURSOR_CURRENT, &error);
	if (res < 0) {
		if (res == CCI_ER_NO_MORE_DATA) {
			/* reached end */
			return false;
		} 

		throw node_db::Exception (error.err_msg);
	}

	return true;
}

char** node_db_cubrid::Result::next() throw(node_db::Exception&) {
	if (thisRow != 0) {
		// clear previous row
		delete [] thisRow;
	}
	T_CCI_ERROR error;
	int res = cci_fetch(stmt_handle, &error);
	if (res < 0) {
		throw node_db::Exception (error.err_msg);
	}
	
	thisRow = new char*[column_count];
	if (thisRow == NULL) {
		throw node_db::Exception("Error allocating memory for result");
	}

	for (int i = 1;i<=column_count;i++){
		int ind;
		res = cci_get_data (stmt_handle, i, CCI_A_TYPE_STR, &thisRow[i], &ind);
		if (res < 0){
			delete [] thisRow;
			throw node_db::Exception("Fetch failed");
		}
	}
	
	// TODO: keep track of thisRow because you have to delete it
	rowNumber++;
	return thisRow;
}

unsigned long* node_db_cubrid::Result::columnLengths() throw(node_db::Exception&) {
	if (thisRow == NULL){
		throw node_db::Exception ("Row not fetched yet");
	}
	unsigned long* lenghts = new unsigned long[column_count];
	for (int i = 0;i<column_count;i++) {
		if (thisRow[i] == NULL) {
			lenghts[i] = 0;
		} else {
			lenghts[i] = strlen (thisRow[i]);
		}
	}
	return lenghts;
}

uint64_t node_db_cubrid::Result::index() const throw(std::out_of_range&) {
	if (this->rowNumber == 0) {
		throw std::out_of_range("Not standing on a row");
	}
	return rowNumber;
}

node_db_cubrid::Result::Column* node_db_cubrid::Result::column(uint16_t i) const throw(std::out_of_range&) {
	if (i > column_count) {
		throw std::out_of_range("Wrong column index");
	}
	return columns[i];
}

uint64_t node_db_cubrid::Result::insertId() const throw() {
	T_CCI_ERROR err_buf;
	char *val = NULL;
	if (cci_last_insert_id (conn_handle, &val, &err_buf) < 0) {
		throw node_db::Exception(err_buf.err_msg);
	}
	long lid = atol (val);
	return (uint64_t)lid;
	return 0;
}

uint16_t node_db_cubrid::Result::columnCount() const throw() {
	return column_count;
}

uint64_t node_db_cubrid::Result::affectedCount() const throw() {
	int rowCount = 0;
	T_CCI_ERROR err_buf;
	if (cci_row_count (conn_handle, &rowCount, &err_buf) < 0) {
		throw node_db::Exception(err_buf.err_msg);
	}
	return (uint64_t)rowCount;
}

uint16_t node_db_cubrid::Result::warningCount() const throw() {
	throw node_db::Exception("Not supported in CUBRID");
	return 0;
}

uint64_t node_db_cubrid::Result::count() const throw(node_db::Exception&) {
	/* same as affected count */
	return affectedCount();
}

bool node_db_cubrid::Result::isBuffered() const throw() {
	return false;
}

bool node_db_cubrid::Result::isEmpty() const throw() {
	return affectedCount()!=0;
}

node_db_cubrid::Result::Column::Column(const T_CCI_COL_INFO& col_info) {
	this->binary = false;
	this->name = col_info.col_name;

	switch (col_info.type) {
		case CCI_U_TYPE_NULL:
			// TODO: type INT is not correct here
			this->type = INT;
			break;
		case CCI_U_TYPE_INT:
		case CCI_U_TYPE_SHORT:
			this->type = INT;
			break;
		case CCI_U_TYPE_FLOAT:
		case CCI_U_TYPE_DOUBLE:
		case CCI_U_TYPE_NUMERIC:
		case CCI_U_TYPE_MONETARY:
			this->type = NUMBER;
			break;
		case CCI_U_TYPE_DATE:
			this->type = DATE;
			break;
		case CCI_U_TYPE_TIME:
			this->type = TIME;
			break;
		case CCI_U_TYPE_TIMESTAMP:
		case CCI_U_TYPE_DATETIME:
			this->type = DATETIME;
			break;
		case CCI_U_TYPE_BLOB:
		case CCI_U_TYPE_CLOB:
			this->type = TEXT;
			break;
		case CCI_U_TYPE_CHAR:
		case CCI_U_TYPE_STRING:
		case CCI_U_TYPE_NCHAR:
		case CCI_U_TYPE_VARNCHAR:
		case CCI_U_TYPE_BIT:
		case CCI_U_TYPE_VARBIT:
			// TODO: do bit and varbit belong here?
			this->type = this->binary ? TEXT : STRING;
			break;
		case CCI_U_TYPE_SET:
		case CCI_U_TYPE_MULTISET:
		case CCI_U_TYPE_SEQUENCE:
			this->type = SET;
			break;
		default:
			this->type = STRING;
			break;
	}
}

std::string node_db_cubrid::Result::Column::getName() const {
	return name;
}

node_db::Result::Column::type_t node_db_cubrid::Result::Column::getType() const {
	return type;
}

bool node_db_cubrid::Result::Column::isBinary() const {
	return binary;
}