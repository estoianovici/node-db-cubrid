#include "./node-db/binding.h"
#include "./cubrid.h"
#include "./query.h"

extern "C" {
	void init(v8::Handle<v8::Object> target) {
		node_db::EventEmitter::Init();
		node_db_cubrid::Cubrid::Init(target);
		node_db_cubrid::Query::Init(target);
	}

	NODE_MODULE(cubrid_bindings, init);
}
