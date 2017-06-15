#include <pqxxInterface.h>

// ****** Constructors ****** //

// ds::pqxxInterface::pqxxInterface():
//     C_("dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432")
// {
//     checkConnection();
// }


ds::pqxxInterface::pqxxInterface(const std::string& opt):
    C_(opt)
{
    checkConnection();
}


ds::pqxxInterface::pqxxInterface(const char opt[]):
    C_(opt)
{
    checkConnection();
}


// ****** Public Member Functions ****** //

bool ds::pqxxInterface::foundSchema(std::string schemaName) {
    try {
        std::stringstream sql;
        sql << "SELECT schema_name FROM information_schema.schemata "
            << "WHERE schema_name = '" << schemaName << "';";
        pqxx::nontransaction N(C_);
        pqxx::result R(N.exec(sql));
        if (R.size()) {
            return true;
        }
        return false;
    } catch (const std::exception &e) {
        FatalError << e.what() << std::endl;
        abort();
    }
}


bool ds::pqxxInterface::foundSchemaTable(std::string schemaName, std::string tableName)
{
    try {
        std::stringstream sql;
        sql << "SELECT c.oid, "
            << "  n.nspname, "
            << "  c.relname "
            << "FROM pg_catalog.pg_class c "
            << "     LEFT JOIN pg_catalog.pg_namespace n "
            << "        ON n.oid = c.relnamespace "
            << "WHERE c.relname ~ '^(" << tableName << ")$' "
            << "  AND n.nspname ~ '^(" << schemaName << ")$';";

        pqxx::nontransaction N(C_);
        pqxx::result R(N.exec(sql));
        if (R.size()) {
            return true;
        }
        return false;
    } catch (const std::exception &e) {
        FatalError << e.what() << std::endl;
        abort();
    }
}


void ds::pqxxInterface::createSchema(std::string schemaName) {
    std::stringstream sql;
    sql << "CREATE SCHEMA " << schemaName << ";";
    try {
        pqxx::work W(C_);
        W.exec(sql.str().c_str());
        W.commit();
    } catch (const std::exception &e) {
        FatalError << e.what() << std::endl;
        abort();
    }
}

void ds::pqxxInterface::createTable
(
    std::string schemaName,
    std::string tableName,
    std::vector<std::string> headings,
    std::vector<std::string> dataTypes
) {
    if (headings.size() != dataTypes.size()) {
        FatalError << "Size mismatch, headings (" << headings.size() << ") and "
            << "datatypes (" << datatypes.size() << ").  They are:\n headings:\n"
            << headings << "\ndataTypes:\n" << dataTypes << std::endl;
        abort();
    }
    std::stringstream sql;
    sql << "CREATE TABLE " << schemaName << "." << tableName << " (";
    std::pair<std::vector<std::string>, std::vector<std::string>> itPair(
        headings.cbegin(),
        dataTypes.cbegin()
    );
    sql << *itPair.first() << " " << *itPair.second();
    for (
        ++itPair.first(), ++itPair.second();
        itPair.first() != headings.cend();
        ++itPair.first(), ++itPair.second()
    ) {
         sql << ", " << *itPair.first() << " " << *itPair.second();
    }
    sql << ");";
    try {
        pqxx::work W(C_);
        W.exec(tableDetails.c_str());
        W.commit();
    } catch (const std::exception &e) {
        FatalError << e.what() << std::endl;
        abort();
    }
}


void ds::pqxxInterface::createTable
(
    std::string schemaName,
    std::string tableName,
    std::vector<std::pair<std::string>> headingsAndTypes
) {
    std::stringstream sql;
    sql << "CREATE TABLE " << schemaName << "." << tableName << " (";
    auto it(headingsAndTypes.cbegin());
    sql << *itPair.first() << " " << itPair.second();
    for (
        ++itPair.first(), ++itPair.second();
        itPair.first() != headings.cend();
        ++itPair.first(), ++itPair.second()
    ) {
         sql << ", " << *itPair.first() << " " << itPair.second();
    }
    sql << ");";
    try {
        pqxx::work W(C_);
        W.exec(tableDetails.c_str());
        W.commit();
    } catch (const std::exception &e) {
        FatalError << e.what() << std::endl;
        abort();
    }
}


void ds::pqxxInterface::dropTable(std::string schemaName, std::string tableName) {
    std::stringstream sql;
    sql << "DROP TABLE " << schemaName << "." << tableName << ";";
    try {
        pqxx::work W(C_);
        W.exec(tableDetails.c_str());
        W.commit();
    } catch (const std::exception &e) {
        FatalError << e.what() << std::endl;
        abort();
    }
}


void ds::pqxxInterface::work(std::string sqlCommand) {
    try {
        pqxx::work W(C_);
        W.exec(sqlCommand.c_str());
        W.commit();
    } catch (const std::exception &e) {
        FatalError << e.what() << std::endl;
        abort();
    }
}


void ds::pqxxInterface::work(std::stringstream sqlCommand) {
    work(sqlCommand.str());
}


pqxx::result ds::pqxxInterface::result(std::string sqlQuery) {
    pqxx::nontransaction N(C_);
    return pqxx::result(N.exec(sql));
}


pqxx::result ds::pqxxInterface::result(std::stringstream sqlQuery) {
    return result(sqlQuery.str());
}


// ****** Private Member Functions ****** //

void ds::pqxxInterface::checkConnection() {
    if (C_.isOpen()) {
        return;
    }
    FatalError << "Can't open database" << std::endl;
    abort();
}


// ****** Template Specialisations ****** //

template <>
static std::vector<bool> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetBool,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<std::string> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetString,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<short> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetShort,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<int> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetInt,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<long> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetLong,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<long long> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetLongLong,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<unsigned short> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetUShort,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<unsigned int> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetUInt,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<unsigned long> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetULong,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<unsigned long long> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetULongLong,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<float> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetFloat,
        openChar,
        closeChar,
        delim
    );
}


template <>
static std::vector<double> ds::pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
) {
    std::stringstream is(arrayStr);
    VecToken vt(is);
    return readArrayFromVecTokens(
        vt,
        &VecToken::assertGetDouble,
        openChar,
        closeChar,
        delim
    );
}


// ****** END ****** //