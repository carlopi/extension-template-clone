#define DUCKDB_EXTENSION_MAIN

#include "some_function_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"


#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

namespace duckdb {

inline void Some_functionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) { 
			return StringVector::AddString(result, "Some_function "+name.GetString()+" 🐥");;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
	Connection con(instance);
    con.BeginTransaction();

    auto &catalog = Catalog::GetSystemCatalog(*con.context);

    CreateScalarFunctionInfo some_function_fun_info(
            ScalarFunction("some_function", {LogicalType::VARCHAR}, LogicalType::VARCHAR, Some_functionScalarFun));
    some_function_fun_info.on_conflict = OnCreateConflict::ALTER_ON_CONFLICT;
    catalog.CreateFunction(*con.context, &some_function_fun_info);
    con.Commit();
}

void Some_functionExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string Some_functionExtension::Name() {
	return "some_function";
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void some_function_init(duckdb::DatabaseInstance &db) {
	LoadInternal(db);
}

DUCKDB_EXTENSION_API const char *some_function_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
