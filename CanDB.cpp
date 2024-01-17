#include "CanDB.h"

// Initialize static members
std::unique_ptr<CanDB> CanDB::instance;
std::mutex CanDB::mutex;

CanDB& CanDB::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!instance)
    {
        instance.reset(new CanDB);
    }
    return *instance;
}

void CanDB::setCanMsgRx(canid_t id, const std::shared_ptr<ICAN_MSG>& msg)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->dbRx[id] = msg;
}

void CanDB::setCanMsgTx(canid_t id, const std::shared_ptr<ICAN_MSG>& msg)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->dbTx[id] = msg;
}

std::shared_ptr<ICAN_MSG> CanDB::getCanMsgRx(canid_t id)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = dbRx.find(id);
    if (it != dbRx.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<ICAN_MSG> CanDB::getCanMsgTx(canid_t id)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = dbTx.find(id);
    if (it != dbTx.end()) {
        return it->second;
    }
    return nullptr;
}

uint16_t CanDB::getNumberOfMsgRx() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return static_cast<uint16_t>(dbRx.size());
}

uint16_t CanDB::getNumberOfMsgTx() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return static_cast<uint16_t>(dbTx.size());
}