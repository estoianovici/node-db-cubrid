#ifndef CUBRID_EXCEPTION_H_
#define CUBRID_EXCEPTION_H_
#include <cas_cci.h>
#include <cas_error.h>
#include "./node-db/exception.h"
namespace node_db_cubrid {
	class Exception : public node_db::Exception {
	public:
		explicit Exception(const T_CAS_ERROR_CODE& code) throw();
	protected:
		inline void setMessage(const T_CAS_ERROR_CODE& code);
	};
}
#endif
