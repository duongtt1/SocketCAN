#ifndef CANDB_H
#define CANDB_H

#include "Common/Common.h"
#include <map>
#include <string>
#include <memory>
#include <mutex>

class CanDB
{
private:
    std::map<canid_t, std::shared_ptr<ICAN_MSG>> dbRx;
    std::map<canid_t, std::shared_ptr<ICAN_MSG>> dbTx;
    static std::unique_ptr<CanDB> instance;
    static std::mutex mutex;

    CanDB(/* args */) {}

public:
    ~CanDB() {}

    static CanDB& getInstance();

    // Delete the copy constructor and assignment operator
    CanDB(const CanDB&) = delete;
    CanDB& operator=(const CanDB&) = delete;

    // Function to add an element to the database
    void setCanMsgRx(canid_t id, const std::shared_ptr<ICAN_MSG>& msg);
    void setCanMsgTx(canid_t id, const std::shared_ptr<ICAN_MSG>& msg);
    std::shared_ptr<ICAN_MSG> getCanMsgRx(canid_t id);
    std::shared_ptr<ICAN_MSG> getCanMsgTx(canid_t id);

    uint16_t getNumberOfMsgRx() const;
    uint16_t getNumberOfMsgTx() const;



};

#endif