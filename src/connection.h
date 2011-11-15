#include <cas_cci.h>
#include "./node-db/connection.h"
#include "./node-db/exception.h"

namespace node_db_cubrid {
	class Connection : public node_db::Connection {
	private:
		int conn_handle;
	public:
		Connection();
		virtual ~Connection();
		virtual void open() throw(node_db::Exception&);
		virtual void close();
		virtual std::string escape(const std::string& string) const throw(node_db::Exception&);
		virtual std::string version() const;
		virtual node_db::Result* query(const std::string& query) const throw(node_db::Exception&);
	};
}
