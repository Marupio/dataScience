##ifndef pqxxInterface_H
#define pqxxInterface_H

#include <pqxx/pqxx>

class pqxxInterface {

public:

    // Constructors

        pqxxInterface():
            C_("dbname = dsdata user = dsuser password = 123 hostaddr = 127.0.0.1 port = 5432")
        {
            checkConnection();
        }

        pqxxInterface(const std::string& opt):
            C_(opt)
        {
            checkConnection();
        }

        pqxxInterface(const char opt[]):
            C_(opt)
        {
            checkConnection();
        }


    // Public Member Functions

        // Query

            //- Returns true if schema exists
            bool foundSchema(std::string schemaName) {
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

            //- Returns true if schemaName.tableName exists
            bool foundSchemaTable(std::string schemaName, std::string tableName)
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


        // Edit - work (i.e. void returning)

            void createSchema(std::string schemaName) {
                try {
                    std::stringstream sql;
                    sql << "CREATE SCHEMA " << schemaName << ";";
                    pqxx::work W(C_);
                    W.exec(sql.str().c_str());
                    W.commit();
                } catch (const std::exception &e) {
                    FatalError << e.what() << std::endl;
                    abort();
                }
            }

            void createTable(std::string tableDetails) {
                try {
                    pqxx::work W(C);
                    W.exec(tableDetails.c_str());
                    W.commit();
                } catch (const std::exception &e) {
                    FatalError << e.what() << std::endl;
                    abort();
                }
            }

private:

    // Private Member Functions

        //- Confirm connection is open, or throw error
        void checkConnection() {
            if (C_.isOpen()) {
                return;
            }
            FatalError << "Can't open database" << std::endl;
            abort();
        }

    // Private member data

        //- Connection to database
        pqxx::connection C_;
};


#endif // pqxxInterface_H
