#ifndef SRC_RESULT_H_
#define SRC_RESULT_H_

#include <string>
#include <stdexcept>
#include <cas_cci.h>

#include "./node-db/exception.h"
#include "./node-db/result.h"

namespace node_db_cubrid {
	class Result : public node_db::Result {
		class Column : public node_db::Result::Column {
		public:
			explicit Column(const T_CCI_COL_INFO& column);
			~Column();
			bool isBinary() const;
			std::string getName() const;
			node_db::Result::Column::type_t getType() const;

		protected:
			std::string name;
			type_t type;
			bool binary;
			friend class Result;
		};

	public:
		Result(int conn_handle, int stmt_handle, T_CCI_COL_INFO* col_info, int column_count, T_CCI_CUBRID_STMT stmt_type) throw(node_db::Exception&);
		~Result();
		void release() throw();
		bool hasNext() const throw();
		char** next() throw(node_db::Exception&);
		unsigned long* columnLengths() throw(node_db::Exception&);
		uint64_t index() const throw(std::out_of_range&);
		Column* column(uint16_t i) const throw(std::out_of_range&);
		uint64_t insertId() const throw();
		uint16_t columnCount() const throw();
		uint64_t affectedCount() const throw();
		uint16_t warningCount() const throw();
		uint64_t count() const throw(node_db::Exception&);
		bool isBuffered() const throw();
		bool isEmpty() const throw();
	private:
		int stmt_handle;
		int conn_handle;
		uint16_t column_count;
		T_CCI_COL_INFO* column_info;
		T_CCI_CUBRID_STMT statement_type;
		char **thisRow;
		uint64_t rowNumber;
		Column** columns;
	};
}

#endif  // SRC_RESULT_H_
