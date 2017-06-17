#ifndef pqxxInterface_H
#define pqxxInterface_H

#include <pqxx/pqxx>

#include <VecToken.h>

namespace ds
{

class pqxxInterface {

public:

    // Constructors

        //- Construct null - this fails because no connection will be open, and connection options
        //  cannot be modified
        // pqxxInterface();

        //- Construct with connection parameters
        pqxxInterface(const std::string& opt);

        //- Construct with connection parameters
        pqxxInterface(const char opt[]);

    //- Destructor
    ~pqxxInterface();


    // Public Member Functions

        // Query

            //- Returns true if schema exists
            bool foundSchema(std::string schemaName);

            //- Returns true if schemaName.tableName exists
            bool foundSchemaTable(std::string schemaName, std::string tableName);


        // Edit - work (i.e. void returning)

            //- Create schema, fails if already exists
            void createSchema(std::string schemaName);

            //- Create schema.table given headings and data types
            //  Fails if table already exists
            void createTable
            (
                std::string schemaName,
                std::string tableName,
                std::vector<std::string> headings,
                std::vector<std::string> dataTypes
            );

            //- Create schema.table given headings and data types
            //  Fails if table already exists
            void createTable
            (
                std::string schemaName,
                std::string tableName,
                std::vector<std::pair<std::string, std::string>> headingsAndTypes
            );

            //- Drop schema.table, fails if it does not exist
            void dropTable(std::string schemaName, std::string tableName);

            //- Perform general SQL work
            void work(std::string sqlCommand);
            void work(std::stringstream& sqlCommand);

            //- Perform general SQL query
            pqxx::result result(std::string sqlQuery);
            pqxx::result result(std::stringstream& sqlQuery);

            //- Convert postgresql array into a vector
            template <class T>
            static std::vector<T> readArray(
                std::string arrayStr,
                char openChar = '{',
                char closeChar = '}',
                char delim = ','
            );


private:

    // Private Member Functions

        //- Confirm connection is open, or throw error
        void checkConnection();

        //- Used by readArray
        template <class T>
        static std::vector<T> readArrayFromVecTokens(
            VecToken& vt,
            T (VecToken::*assertGetFn)() const,
            char openChar,
            char closeChar,
            char delim
        );


    // Private member data

        //- Connection to database
        pqxx::connection C_;
};

template <>
std::vector<bool> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

// Assumed to be getString, as postgres requires strings enclosed in ""
template <>
std::vector<std::string> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<short> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<int> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<long> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<long long> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<unsigned short> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<unsigned int> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<unsigned long> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<unsigned long long> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<float> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);

template <>
std::vector<double> pqxxInterface::readArray(
    std::string arrayStr,
    char openChar,
    char closeChar,
    char delim
);


} // end namespace

#include <pqxxInterfaceTemplates.cpp>

#endif // pqxxInterface_H
