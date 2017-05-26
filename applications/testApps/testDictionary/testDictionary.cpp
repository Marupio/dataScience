#include <climits>
#include <fstream>
#include <Dictionary.h>
#include <dsConfig.h>

using namespace ds;
int main()
{
    Dictionary dict("testDict");
    bool quit = false;
    Dictionary* dictPtr(&dict);
    while(!quit) {
        std::cout << "Current dictionary = [\n" << *dictPtr << "\n]" << std::endl;
        std::cout << "\nSelect action:"
            << "name, scopeName, keyName, depth, found, foundType, lookupEntry, isTokens, "
            << "lookupTokens, isDict,\nlookupDict, reset (to top dict level), add, erase, clear, "
            << "debugWrite, quit\n"
            << "What do you want to do? ";
        std::string action;
        std::cin >> action;
        if (action == "name") {
            std::cout << "Result = [" << dictPtr->name() << "]" << std::endl;
        } else if (action == "scopeName") {
            std::cout << "Result = [" << dictPtr->scopeName() << "]" << std::endl;
        } else if (action == "keyName") {
            std::cout << "Result = [" << dictPtr->keyName() << "]" << std::endl;
        } else if (action == "depth") {
            std::cout << "Result = [" << dictPtr->depth() << "]" << std::endl;
        } else if (action == "found") {
            std::cout << "found(key): What is key? ";
            std::string key;
            std::cin >> key;
            std::cout << "Result = [" << dictPtr->found(key) << "]" << std::endl;
        } else if (action == "foundType") {
            std::cout << "foundType(key): What is key? ";
            std::string key;
            std::cin >> key;
            std::cout << "Result = [" << Entry::typeEnumToString(dictPtr->foundType(key)) << "]"
                << std::endl;
        } else if (action == "lookupEntry") {
            std::cout << "lookupEntry(key): What is key? ";
            std::string key;
            std::cin >> key;
            const Entry& e(dictPtr->lookupEntry(key));
            std::cout << "Result = [";
            e.debugWrite(std::cout);
            std::cout << "]" << std::endl;
        } else if (action == "isTokens") {
            std::cout << "isTokens(key): What is key? ";
            std::string key;
            std::cin >> key;
            std::cout << "Result = [" << dictPtr->isTokens(key) << "]" << std::endl;
        } else if (action == "lookupTokens") {
            std::cout << "lookupTokens(key): What is key? ";
            std::string key;
            std::cin >> key;
            const VecToken& vt(dictPtr->lookupTokens(key));
            std::cout << "Result = [" << vt << "]" << std::endl;
        } else if (action == "isDict") {
            std::cout << "isDict(key): What is key? ";
            std::string key;
            std::cin >> key;
            std::cout << "Result = [" << dictPtr->isDict(key) << "]" << std::endl;
        } else if (action == "lookupDict") {
            std::cout << "lookupDict(key): What is key? ";
            std::string key;
            std::cin >> key;
            dictPtr = &(dictPtr->lookupDict(key));
        } else if (action == "reset") {
            dictPtr = &dict;
        } else if (action == "add") {
            std::cout << "add(key, string): What is key? ";
            std::string key;
            std::cin >> key;
            std::cout << "What is string? ";
            std::string str;
            std::cin >> str;
            std::string tmp;
            std::getline(std::cin, tmp);
            str += tmp;
            dictPtr->add(key, str);
        } else if (action == "erase") {
            std::cout << "erase(key): What is key? ";
            std::string key;
            std::cin >> key;
            dictPtr->erase(key);
        } else if (action == "clear") {
            dictPtr->clear();
        } else if (action == "debugWrite") {
            std::cout << "Result is: [\n";
            dictPtr->debugWrite(std::cout);
            std::cout << "\n]";
        } else if (action == "quit") {
            quit = true;
        } else {
            std::cout << "Unrecognized command: [" << action << "]" << std::endl;
        }
    }
    return 0;
}
