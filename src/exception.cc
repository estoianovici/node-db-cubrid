#include "exception.h"

node_db_cubrid::Exception::Exception(const T_CAS_ERROR_CODE& code) throw() :node_db::Exception() {
	setMessage(code);
}

void node_db_cubrid::Exception::setMessage(const T_CAS_ERROR_CODE& code) {
	switch (code) {
	case CAS_ER_DBMS:
		setMessage("DBMS Error");
		break;
	case CAS_ER_INTERNAL:
		setMessage("Internal error");
		break;
	case CAS_ER_NO_MORE_MEMORY:
		setMessage("Out of memory");
		break;
	case CAS_ER_COMMUNICATION:
		setMessage("Communication error");
		break;
	case CAS_ER_ARGS:
		setMessage("Invalid arguments");
		break;
	case CAS_ER_TRAN_TYPE:
		setMessage("Invalid transaction type");
		break;
	case CAS_ER_SRV_HANDLE:
		setMessage("Invalid request");
		break;
	case CAS_ER_NUM_BIND:
		setMessage("Incorrect number of parameters");
		break;
	case CAS_ER_UNKNOWN_U_TYPE:
		setMessage("Unknown type");
		break;
	case CAS_ER_DB_VALUE:
		setMessage("Invalid value");
		break;
	case CAS_ER_TYPE_CONVERSION:
		setMessage("Conversion error");
		break;
	case CAS_ER_PARAM_NAME:
		setMessage("Invalid parameter name");
		break;
	case CAS_ER_NO_MORE_DATA:
		setMessage("No more data");
		break;
	case CAS_ER_OBJECT:
		setMessage("Invalid object");
		break;
	case CAS_ER_OPEN_FILE:
		setMessage("Cannot open file");
		break;
	case CAS_ER_SCHEMA_TYPE:
		setMessage("Invalid schema type");
		break;
	case CAS_ER_VERSION:
		setMessage("Wrong version");
		break;
	case CAS_ER_FREE_SERVER:
		setMessage("Free server");
		break;
	case CAS_ER_NOT_AUTHORIZED_CLIENT:
		setMessage("Client not authorized");
		break;
	case CAS_ER_QUERY_CANCEL:
		setMessage("Query canceled");
		break;
	case CAS_ER_NOT_COLLECTION:
		setMessage("Value is not acollection");
		break;
	case CAS_ER_COLLECTION_DOMAIN:
		setMessage("Invalid collection domain");
		break;
	case CAS_ER_NO_MORE_RESULT_SET:
		setMessage("No more result sets");
		break;
	case CAS_ER_INVALID_CALL_STMT:
		setMessage("Invalid call statement");
		break;
	case CAS_ER_STMT_POOLING:
		setMessage("Statement pooling");
		break;
	case CAS_ER_DBSERVER_DISCONNECTED:
		setMessage("Server disconnected");
		break;
	case CAS_ER_NOT_IMPLEMENTED:
		setMessage("Not implemented");
		break;
	default:
		setMessage("Unknown error");
		break;
	}
}